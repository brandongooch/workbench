#include <err.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include "hashtable.h"
#include "hashfunc.h"
#include "datafile.h"

#define DEBUG 1

#define DPRINTF(fmt, ...) do {				\
	if (DEBUG)					\
		printf(fmt, __VA_ARGS__);		\
} while (0)

#define DEFAULT_DATFILE "example.dat"

void print_hashtable(struct hashtable_s *);
void free_hashdata(struct hashtable_s *);

int
main(int argc, char *argv[])
{
	struct hashtable_s *ht;
	size_t (*hashfunc)(const char *);
	char *filename;
	int hashalg;
	int ch, fd;

	/* Default hash algorithm and function */
	hashalg = 0;
	hashfunc = NULL;

	/* Allocate some space for the datafile filename */
	filename = malloc(sizeof(char) * BUFSIZ);

	/* Set the default filename */
	strlcpy(filename, DEFAULT_DATFILE, BUFSIZ);

	while ((ch = getopt(argc, argv, "f:a:")) != -1) {
		switch (ch) {
		case 'a': {
			char *endptr;

			hashalg = strtol(optarg, &endptr, 10);
			break;
		}
		case 'f':
			if ((fd = access(optarg, R_OK)) < 0) {
				err(EX_NOINPUT, "%s", optarg);
			} else {
				strlcpy(filename, optarg, BUFSIZ);
			}
		default:
			break;
		}
	}
	argc -= optind;
	argv += optind;

	/* Determine the user-passed hash algorithm option */
	switch (hashalg) {
	case HASH_PJW:
		hashfunc = &hashfunc_pjw;
		DPRINTF("hashfunc: %s\n", "pjw");
		break;
	case HASH_ELF:
		hashfunc = &hashfunc_elf;
		DPRINTF("hashfunc: %s\n", "elf");
		break;
	case HASH_SDBM:
		hashfunc = &hashfunc_sdbm;
		DPRINTF("hashfunc: %s\n", "sdbm");
		break;
	case HASH_DJB2:
		hashfunc = &hashfunc_djb2;
		DPRINTF("hashfunc: %s\n", "djb2");
		break;
	case HASH_XOR:
		hashfunc = &hashfunc_xor;
		DPRINTF("hashfunc: %s\n", "xor");
		break;
	default:
		hashfunc = &ht_defaulthash;
		DPRINTF("hashfunc: %s\n", "default");
		break;
	}

	/* Create a hashtable */
	ht = ht_create(16, hashfunc);
	if (ht == NULL) {
		err(EX_SOFTWARE, "could not create hashtable");
		exit(EXIT_FAILURE);
	}

	DPRINTF("datafile: %s\n", filename);

	/* Open a datafile from which to fill the hashtable */
	int error;
	error = df_load(ht, filename);
	if (error < 0)
		err(EX_SOFTWARE, "could not load datafile: %s", filename);

	/* Take a look at the hashtable after loading a datafile */
	print_hashtable(ht);

	/* Check the load on the hashtable, resizing it if it is too high */
	while (ht->loadfactor > 1.0) {
		ht_resize(ht, ht->size * 2);
	}

	/* Take a look at the hashtable after resizing */
	print_hashtable(ht);

	/* Try searching for some data */
	struct simpledata *search;
	printf("Searching for: \"FreeBSD\"\n");
	search = (struct simpledata *)ht_getentry(ht, "FreeBSD");
	if (search != NULL)
		printf("FreeBSD: %s\n", search->text);
	else
		printf("Could not find and entry for \"FreeBSD\"\n");

	/* Delete data above, then inspect the hashtable output */
	printf("Deleting \"FreeBSD\"...\n");
	ht_delete(ht, "FreeBSD");

	/* Try searching for some data */
	printf("Searching for: \"FreeBSD\"\n");
	search = (struct simpledata *)ht_getentry(ht, "FreeBSD");
	if (search != NULL)
		printf("FreeBSD: %s\n", search->text);
	else
		printf("Could not find and entry for \"FreeBSD\"\n");

	/* View the hashtable */
	print_hashtable(ht);

	/* Resize the hashtable, then view the changes */
	ht_resize(ht, 8);

	/* View the hashtable */
	print_hashtable(ht);

	/* Free the memory used by the hashtable data */
	free_hashdata(ht);

	/* Destroy the hashtable, freeing valuable memory */
	ht_destroy(ht);

	/* Free the memory allocated in main */
	free(filename);

	exit(0);
}

void
print_hashtable(struct hashtable_s *ht)
{
	size_t n;
	struct hashentry_s *entry, *next;
	struct simpledata *m;

	printf("\n");
	printf("==================================================\n");
	printf("%-6s", "NODE");
	printf("%-32s", "KEY");
	printf("%-32s", "DATA");
	printf("\n");

	for (n = 0; n < ht->size; ++n) {
		for (entry = ht->list[n]; entry; entry = next) {
			next = entry->next;
			printf("%-6ld", n);
			printf("%-32s", entry->key);
			m = (struct simpledata *)(entry->data);
			printf("%-3d %-27s\n", (int)m->id, (char *)m->text);
		}
	}

	/* Print out hashtable stats */
	printf("\n");
	printf("  STATS:\n");
	printf("\n");
	printf("    ht->nentries: %zu\n", ht->nentries);
	printf("    ht->loadfactor: %f\n", ht->loadfactor);
	printf("\n\n");
}

void
free_hashdata(struct hashtable_s *ht)
{
	size_t n;
	struct hashentry_s *list, *oldlist;
	struct simpledata *m;

	for (n = 0; n < ht->size; ++n) {
		list = ht->list[n];
		while (list) {
			if (list->data != NULL) {
				m = (struct simpledata *)(list->data);
				free(m->text);
			}
			oldlist = list;
			list = list->next;
		}
	}
}

