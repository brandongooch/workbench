#ifndef _GOOCHBOT_H_
#define _GOOCHBOT_H_

#include <sys/types.h>

#define GOOCHBOT_NAME_MAXLEN		256

#define ERR_GB_ID			1
#define ERR_GB_INIT			2
#define ERR_GB_NULL			3

typedef struct goochbot_s goochbot_t;
typedef struct goochbot_too_s *goochbot_too_t;

void		 goochbot_destroy(goochbot_t *);
uint64_t	 goochbot_id_get(goochbot_t *);
int		 goochbot_id_set(goochbot_t *, uint64_t);
int		 goochbot_init(goochbot_t *, uint64_t, char *);
char		*goochbot_name_get(goochbot_t *);
int		 goochbot_name_set(goochbot_t *, char *);
goochbot_t	*goochbot_new();

uint64_t	 goochbot_too_id_set(goochbot_too_t, uint64_t);

#endif /* !_GOOCHBOT_H_ */
