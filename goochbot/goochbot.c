#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "goochbot.h"

const char *name_default = "default";

struct goochbot *
goochbot_init(uint64_t id, char *name)
{
	struct goochbot *gbp = NULL;

	/* Allocate memory for the robot */
	if ((gbp = malloc(sizeof(struct goochbot))) == NULL)
		errx(EX_OSERR, "malloc failed");

	if ((gbp->name = malloc(sizeof(char) * NAMESIZE)) == NULL)
		errx(EX_OSERR, "malloc failed");

	/* Set the id */
	goochbot_set_id(gbp, id);

	/* Set the name */
	goochbot_set_name(gbp, name);

	return gbp;
}

int
goochbot_destroy(struct goochbot *gbp)
{
	int error = 0;

	/* Free the memory used by name */
	if (gbp->name != NULL)
		free(gbp->name);

	/* Free the memory used by the goochbot object itself */
	if (gbp != NULL)
		free(gbp);

	return error;
}

int
goochbot_set_id(struct goochbot *gbp, uint64_t id)
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
goochbot_set_name(struct goochbot *gbp, char *name)
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

