#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

#include "allsorts.h"

#define BUBBLE		'b'

#define DEBUG 0

#ifdef DEBUG
#define DPRINTF(fmt, ...) do {			\
	if (DEBUG)				\
		printf(fmt, __VA_ARGS__);	\
} while (0)
#endif

#define NUMELEM 10

int intlist[NUMELEM] = { 6, 3, 1, 5, 7, 0, 2, 9, 4, 8 };

char strlist_data[NUMELEM][BUFSIZ] = {
	"Stephanie",
	"Brandon",
	"Ava",
	"Taylor",
	"Gabe",
	"Tiffany",
	"Jeff",
	"Latrece",
	"Justis",
	"Ruby"
};

static int
compar_int(const void *i, const void *j)
{
	return (*(int *)i - *(int *)j);
}

static void
printlist_int(int *list, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		printf("%d\n", list[i]);
	}
	printf("\n");
}

static int
compar_str(const void *li1, const void *li2)
{
	DPRINTF("cmfunc: %s %s\n", li1, li2);
	if (li1 == 0 && li2 == 0)
		return 0;
	if (li1 == 0 && li2 != 0)
		return 1;
	if (li2 == 0)
		return -1;

	return (strcmp(li1, li2));
}

static void
printlist_str(char **list, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		printf("%s\n", list[i]);
	}
	printf("\n");
}

static void
usage(void)
{
	printf("usage: outofsorts -s [SORT_TYPE]\n");
	exit(1);
}

int
main(int argc, char *argv[])
{
	int ch;
	char **strlist;
	char *strlistelem;
	size_t nelem = NUMELEM;
	char sort_type = 'b';

	while ((ch = getopt(argc, argv, "s:")) != -1) {
		switch (ch) {
		case 's':
			sort_type = optarg[0];
			break;
		default:
			usage();
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Allocate space for pointers to n elements */
	strlist = calloc(nelem, sizeof(char *));
	if (strlist == NULL)
		errx(EX_OSERR, "calloc failed");

	/* Allocate space for each element in the list */
	for (size_t i = 0; i < nelem; i++) {
		strlistelem = malloc(sizeof(char) * BUFSIZ);
		strlist[i] = strlistelem;
		strlcpy(strlist[i], strlist_data[i],
		    sizeof(char) * BUFSIZ - 1);
	}

	printlist_str(strlist, nelem);
	printlist_int(intlist, nelem);
	switch(sort_type) {
		case BUBBLE:
			DPRINTF("bubblesort: %zu elements\n", nelem);
			bubblesort(*strlist, nelem, sizeof(char) * BUFSIZ,
			    compar_str);

			DPRINTF("bubblesort: %d elements\n", NUMELEM);
			bubblesort(&intlist, NUMELEM,
			    sizeof(int), compar_int);
			break;
		default:
			printf("error: invalid sort\n");
			exit(EXIT_FAILURE);
	}
	printlist_str(strlist, nelem);
	printlist_int(intlist, nelem);

	/* Free the memory allocated for the list and elements */
	for (size_t i = 0; i < nelem; i++) {
		strlistelem = strlist[i];
		free(strlistelem);
	}
	free(strlist);

	exit(EXIT_SUCCESS);
}
