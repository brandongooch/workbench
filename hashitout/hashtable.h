/*
 * Simple Hash Table Implementation
 *
 * References:
 *
 *   http://en.wikipedia.org/wiki/Hash_table
 *   http://en.literateprograms.org/Hash_table_(C)
 *   http://www.cs.cornell.edu/courses/cs312/2008sp/lectures/lec20.html
 */

struct hashentry_t {
	char *key;
	void *data;
	struct hashentry_t *next;
};

struct hashtable_t {
	size_t size;
	int nentries;
	float loadfactor;
	struct hashentry_t **list;
	size_t (*hashfunc)(const char *);
};

struct hashtable_t *ht_create(size_t, size_t (*)(const char *));
int	 ht_insert(struct hashtable_t *, const char *, void *);
int	 ht_delete(struct hashtable_t *, const char *);
int	 ht_resize(struct hashtable_t *, size_t);
void	 ht_destroy(struct hashtable_t *);
void	*ht_getentry(struct hashtable_t *, const char *);
void	 ht_setloadfactor(struct hashtable_t *);
size_t	 ht_defaulthash(const char *);
