#include <sys/types.h>

#define NAMESIZE 256

typedef struct goochbot {
	uint64_t id;
	char *name;
} goochbot_s;

goochbot_s *goochbot_init(uint64_t, char *);
int goochbot_destroy(struct goochbot *);
int goochbot_set_id(struct goochbot *, uint64_t);
int goochbot_set_name(struct goochbot *, char *);
