/*
 * Hash Functions
 *
 * Various hash algorithms are implemented in this file, and can
 * be passed as the hash function parameter to the hash table
 * implementation's "create" function.
 */

#define HASH_DEF	0
#define HASH_PJW	1
#define HASH_ELF	2
#define HASH_SDBM	3
#define HASH_DJB2	4
#define HASH_XOR	5

size_t	hashfunc_pjw(const char *);
size_t	hashfunc_elf(const char *);
size_t	hashfunc_sdbm(const char *);
size_t	hashfunc_djb2(const char *);
size_t	hashfunc_xor(const char *);
size_t	hashfunc_def(const char *);
