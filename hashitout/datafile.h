#define DELIM "="
#define DFLINEBUFLEN 256

struct simpledata {
	int id;
	char *text;
};

int df_load(struct hashtable_s *, char *);
