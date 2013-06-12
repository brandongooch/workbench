#define DELIM "="
#define DFLINEBUFLEN 256
#define DEBUG 1

#define DPRINTF(fmt, ...) do {				\
	if (DEBUG)					\
		printf(fmt, __VA_ARGS__);		\
} while (0)

struct simpledata {
	int id;
	char *text;
};

int df_load(struct hashtable_t *, char *);
