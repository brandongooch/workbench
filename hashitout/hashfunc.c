#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "hashfunc.h"

/* Peter Weinberger's */
size_t
hashfunc_pjw(const char *key)
{
	size_t hash, g;

	hash = 0;
	while (*key) {
		hash = (hash << 4) + (*key++);
		if ((g = (hash & 0xF0000000))) {
			hash ^= g >> 24;
			hash ^= g;
		}
	}

	hash = hash % 211;
	return (hash);
}

/*
 * UNIX ELF Hash Function
 * Published hash algorithm used in the UNIX ELF object file format
 */
size_t
hashfunc_elf(const char *key)
{
	size_t hash, g;

	hash = 0;
	while (*key) {
		hash = (hash << 4) + (*key++);
		if ((g = (hash & 0xF0000000)))
			hash ^= g >> 24;
		hash &= ~g;
	}

	return (hash);
}

/*
 * sdbm
 * This algorithm was created for the sdbm (a reimplementation of ndbm)
 * database library and seems to work relatively well in scrambling bits
 */
size_t
hashfunc_sdbm(const char *key)
{
	size_t hash;

	hash = 0;
	while (*key)
		hash = (*key++) + (hash << 6) + (hash << 16) - hash;

	return (hash);
}

/*
 * djb2
 * This algorithm was first reported by Dan Bernstein
 * many years ago in comp.lang.c
 */
size_t
hashfunc_djb2(const char *key)
{
	size_t hash;

	hash = 5381;
	while (*key)
		hash = ((hash << 5) + hash) + (*key++); /* hash * 33 */

	return (hash);
}

size_t
hashfunc_xor(const char *key)
{
	size_t hash;

	hash = 0;
	while (*key)
		hash = hash ^ (*key++);

	hash = hash % 101; /* 101 is a prime number */
	return (hash);
}
