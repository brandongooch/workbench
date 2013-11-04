#include <sys/param.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libutil.h>
#include <pthread.h>
#include <pthread_np.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <syslog.h>
#include <unistd.h>

#define DAEMON_NAME "goochd"

/* Flags */
int debug = 0;
int foreground = 0;

/* pidfile handling */
const char *pidfile = "/var/run/goochd.pid";
struct pidfh *pfh;

/* Signal handling */
struct sigaction sa;
sigset_t mask;

static int	 daemonize();
static void	*gd_sighandler(void *);
static void	 gd_shutdown();

static int
daemonize()
{
	pid_t pid;
	struct rlimit rl;

	/* Clear file creation mask. */
	umask(0);

	/* Get maximum number of file descriptors. */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		errx(EX_OSERR, "%s: cannot get rlimit", DAEMON_NAME);

	/* Become a session leader to lose controlling TTY. */
	if ((pid = fork()) < 0)
		errx(EX_OSERR, "%s: cannot fork", DAEMON_NAME);
	else if (pid != 0) /* parent (will be a grandparent soon)*/
		_exit(EXIT_SUCCESS);

	/* Create the session and set process group ID */
	setsid();

	/* Ensure future opens will not allocate controlling TTYs. */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		errx(EX_OSERR, "%s: cannot ignore SIGHUP", DAEMON_NAME);
	if ((pid = fork()) < 0)
		errx(EX_OSERR, "%s: cannot fork", DAEMON_NAME);
	else if (pid != 0) /* parent */
		_exit(EXIT_SUCCESS);

	/*
	 * Change the current working directory to '/'
	 * to prevent file systems from being unmountable.
	 */
	if (chdir("/") < 0)
		errx(EX_OSERR, "%s: can't change directory to /", DAEMON_NAME);

#if 0
	int i;
	int fd0, fd1, fd2;

	/*
	 * Close all open file descriptors.
	 */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);

	/*
	 * Attach file descriptors 0, 1, and 2 to /dev/null.
	 */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/*
	 * Look for oddities or issues with file descriptors and exit if
	 * we find anything unusual. I have no idea what else we need here.
	 */
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		errx(EX_OSERR, "%s: unexpected file descriptors: %d %d %d",
		    DAEMON_NAME, fd0, fd1, fd2);
	}
#endif

	/* Close STDIN, STDOUT, and STDERR */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return 0;
}

static void
gd_shutdown()
{
	pidfile_remove(pfh);
}

static void *
gd_sighandler(void *arg)
{
	int sigcnt = 0;
	int err, signo;

	arg = NULL;

	for (;;) {
		err = sigwait(&mask, &signo);
		if (err != 0) {
			syslog(LOG_ERR, "sigwait failed");
			exit(EXIT_FAILURE);
		}

		switch (signo) {
		case SIGHUP:
			syslog(LOG_NOTICE, "received SIGHUP signal");
			break;
		case SIGINT:
		case SIGTERM:
			syslog(LOG_NOTICE, "terminating");
			gd_shutdown();
			exit(EXIT_SUCCESS);
			break;
		case SIGUSR1:
			syslog(LOG_NOTICE, "received SIGUSR1 signal");
			break;
		default:
			syslog(LOG_NOTICE, "unhandled signal %s",
			    strsignal(signo));
			break;
		}

		sigcnt++;
		syslog(LOG_NOTICE, "gd_sighandler: signal count == %d", sigcnt);
		sleep(2);
	}
}

int
main(int argc, char *argv[])
{
	int ch;
	int error;
	pid_t otherpid;
	pthread_t td;

	while ((ch = getopt(argc, argv, "dF")) != -1){
		switch (ch) {
		case 'd':
			debug = 1;
			break;
		case 'F':
			foreground = 1;
			break;
		default:
			/* Nothing */
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Initialize logging */
	openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_DAEMON);

	if (!foreground) {
		pfh = pidfile_open(pidfile, 0600, &otherpid);
		if (pfh == NULL) {
			if (errno == EEXIST) {
				errx(EXIT_FAILURE, "daemon already running, "
				    "pid: %jd", (intmax_t)otherpid);
			}
			errx(EX_OSERR, "cannot open or create pidfile");
		}

		if (daemonize() != 0) {
			pidfile_remove(pfh);
			errx(EX_OSERR, "%s: could not daemonize", DAEMON_NAME);
		}

		if (pfh != NULL)
			pidfile_write(pfh);
	}

        /*
         * Restore SIGHUP default and block all signals.
         */
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGHUP, &sa, NULL) < 0)
                errx(EX_OSERR, "can't restore SIGHUP default");
        sigfillset(&mask);
        if ((pthread_sigmask(SIG_BLOCK, &mask, NULL)) != 0)
                errx(EX_OSERR, "SIG_BLOCK error");

	/* Signal handling thread */
	error = pthread_create(&td, NULL, gd_sighandler, "test");

	/* Set the thread name, mostly for debugging */
	pthread_set_name_np(td, "gd_sighandler");

	/* Main Loop
	 * XXX  Need to turn the following into:
	 * for (;;)
	 */
	for (int i = 0; i <= 30; i++) {
		syslog(LOG_NOTICE, "main: %d", i);
		sleep(3);
	}

	if (pfh != NULL)
		pidfile_remove(pfh);

	exit(EXIT_SUCCESS);
}
