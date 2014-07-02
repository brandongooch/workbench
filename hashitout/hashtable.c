/*
 * Simple Hash Table Implementation
 *
 * References:
 *
 *   http://en.wikipedia.org/wiki/Hash_table
 *   http://en.literateprograms.org/Hash_table_(C)
 *   http://www.cs.cornell.edu/courses/cs312/2008sp/lectures/lec20.html
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

struct hashtable_s *
ht_create(size_t size, size_t (*hashfunc)(const char *))
{
	struct hashtable_s *ht;

	/* Allocate space for the hashtable */
	ht = malloc(sizeof(struct hashtable_s));
	if (ht == NULL)
		return (NULL);

	/* Allocate space for the hash buckets, which are linked lists */
	ht->list = calloc(size, sizeof(struct hashentry_s *));
	if (ht->list == NULL) {
		free(ht);
		return (NULL);
	}

	/* Specify the number of buckets that the hashtable will use */
	ht->size = size;

	/* Initialize the count of the total number of entries */
	ht->nentries = 0;

	/* Initialize the calculated load factor */
	ht->loadfactor = 0.0;

	/* If a hash function wasn't specified, use the default */
	if (hashfunc == NULL)
		ht->hashfunc = ht_defaulthash;
	else
		ht->hashfunc = hashfunc;

	return (ht);
}

void
ht_destroy(struct hashtable_s *ht)
{
	size_t i;
	struct hashentry_s *entry, *oldentry;

	/* Free each element in each bucket */
	for (i = 0; i < ht->size; i++) {
		entry = ht->list[i];
		while (entry != NULL) {
			if (entry->key != NULL)
				free(entry->key);
			oldentry = entry;
			entry = entry->next;
			free(oldentry);
		}
	}

	/* Free the hashtable buckets (linked lists) */
	free(ht->list);

	/* Free the memory used by the hashtable itself */
	free(ht);
}

int
ht_insert(struct hashtable_s *ht, const char *key, void *data)
{
	struct hashentry_s *entry;
	size_t hash;
	size_t len;

	/* Calculate the hash */
	hash = ht->hashfunc(key) % ht->size;

	/* Point to the correct bucket */
	entry = ht->list[hash];

	/* Get the length of the key, and add 1 to it */
	len = strlen(key) + 1;

	/*
	 * Search through list looking for an identical key.
	 * If one exists, simply replace the data with our new data.
	 */
	while (entry != NULL) {
		if (!strncmp(entry->key, key, len)) {
			if (data == NULL)
				return (-1);
			entry->data = data;
			return (0);
		}
		entry = entry->next;
	}

	/* No existing entry, so allocate space for a new one */
	entry = malloc(sizeof(struct hashentry_s));
	if (entry == NULL)
		return (-1);

	/* Assign a key to our new entry */
	entry->key = strndup(key, len);
	if (entry->key == NULL) {
		free(entry);
		return (-1);
	}

	/* Assign some data for our new entry */
	entry->data = data;

	/* Modify the pointers to add the link to our new entry */
	entry->next = ht->list[hash];
	ht->list[hash] = entry;

	/* Increment the number of nentries count */
	ht->nentries++;

	/* Recalculate load factor */
	ht_setloadfactor(ht);

	return (0);
}

int
ht_delete(struct hashtable_s *ht, const char *key)
{
	struct hashentry_s *entry, *preventry = NULL;
	size_t hash;
	size_t len;

	/* Calculate the hash */
	hash = ht->hashfunc(key) % ht->size;

	/* Point to the correct bucket */
	entry = ht->list[hash];

	/* Get the length of the key, and add 1 to it */
	len = strlen(key) + 1;

	/*
	 * Search through list looking for the specified key.
	 * If we find it, delete it.
	 */
	while (entry != NULL) {
		if (strncmp(entry->key, key, len) == 0) {

			/* Free the found entry key */
			free(entry->key);

			/* Fix-up the linked list to remove the entry */
			if (preventry != NULL)
				preventry->next = entry->next;
			else
				ht->list[hash] = entry->next;

			/* Delete the entry */
			free(entry);

			/* Decrement the number of nentries count */
			ht->nentries--;

			/* Recalculate load factor */
			ht_setloadfactor(ht);

			return (0);
		}
		/* Entry not found yet, move to the next... */
		preventry = entry;
		entry = entry->next;
	}

	return (-1);
}

void *
ht_getentry(struct hashtable_s *ht, const char *key)
{
	struct hashentry_s *entry;
	size_t hash;
	size_t len;

	hash = ht->hashfunc(key) % ht->size;
	entry = ht->list[hash];
	len = strlen(key) + 1;

	while (entry != NULL) {
		if (strncmp(entry->key, key, len) == 0)
			return (entry->data);
		entry = entry->next;
	}

	return (NULL);
}

int
ht_resize(struct hashtable_s *ht, size_t size)
{
	struct hashtable_s htmp;
	size_t i;
	struct hashentry_s *entry, *next;

	htmp.size = size;
	htmp.nentries = 0;
	htmp.hashfunc = ht->hashfunc;

	htmp.list = calloc(size, sizeof(struct hashentry_s*));
	if (htmp.list == NULL)
		return (-1);

	for (i = 0; i < ht->size; i++) {
		for (entry = ht->list[i]; entry; entry = next) {
			next = entry->next;
			ht_insert(&htmp, entry->key, entry->data);
			ht_delete(ht, entry->key);
		}
	}

	free(ht->list);
	ht->size = htmp.size;
	ht->list = htmp.list;
	ht->nentries = htmp.nentries;
	ht_setloadfactor(ht);

	return (0);
}

void
ht_setloadfactor(struct hashtable_s *ht)
{
	float lf;

	lf = (ht->nentries / ht->size);
	ht->loadfactor = lf;
}

size_t
ht_defaulthash(const char *key)
{
	size_t hash;

	hash = 0;

	while (*key)
		hash += (unsigned char)*key++;

	return (hash);
}

