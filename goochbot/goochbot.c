#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "goochbot.h"

const char *name_default = "default";

goochbot_s *
goochbot_init(uint64_t id, char *name)
{
	goochbot_s *gbp = NULL;

	if ((gbp = malloc(sizeof(goochbot_s))) == NULL)
		errx(EX_OSERR, "malloc failed");

	if ((gbp->name = malloc(sizeof(char) * NAMESIZE)) == NULL)
		errx(EX_OSERR, "malloc failed");

	goochbot_set_id(gbp, id);
	goochbot_set_name(gbp, name);

	return gbp;
}

int
goochbot_destroy(goochbot_s *gbp)
{
	int error = 0;

	if (gbp->name != NULL)
		free(gbp->name);

	if (gbp != NULL)
		free(gbp);

	return error;
}

int
goochbot_set_id(goochbot_s *gbp, uint64_t id)
{
	int error = 0;

	if (id == 0) {
		srand(time(NULL));
		uint64_t random_id = rand();
		gbp->id = random_id;
	} else
		gbp->id = id;

	return error;
}

int
goochbot_set_name(goochbot_s *gbp, char *name)
{
	int error = 0;

	if (gbp->name != NULL) {
		if (name == NULL)
			strlcpy(gbp->name, name_default, NAMESIZE);
		else
			strlcpy(gbp->name, name, NAMESIZE);
	} else
		error = 1;

	return error;
}

