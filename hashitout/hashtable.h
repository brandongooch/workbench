/*
 * Simple Hash Table Implementation
 *
 * References:
 *
 *   http://en.wikipedia.org/wiki/Hash_table
 *   http://en.literateprograms.org/Hash_table_(C)
 *   http://www.cs.cornell.edu/courses/cs312/2008sp/lectures/lec20.html
 */
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

struct hashentry_s {
	char *key;
	void *data;
	struct hashentry_s *next;
};

struct hashtable_s {
	size_t size;
	size_t nentries;
	float loadfactor;
	struct hashentry_s **list;
	size_t (*hashfunc)(const char *);
};

struct hashtable_s *ht_create(size_t, size_t (*)(const char *));
int	 ht_insert(struct hashtable_s *, const char *, void *);
int	 ht_delete(struct hashtable_s *, const char *);
int	 ht_resize(struct hashtable_s *, size_t);
void	 ht_destroy(struct hashtable_s *);
void	*ht_getentry(struct hashtable_s *, const char *);
void	 ht_setloadfactor(struct hashtable_s *);
size_t	 ht_defaulthash(const char *);

#endif /* !_HASHTABLE_H_ */
