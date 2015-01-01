#include <sys/time.h>
#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "goochbot.h"

struct goochbot_s {
	uint64_t id;
	char *name;
};

struct goochbot_too_s {
	uint64_t id;
	char *name;
};

const char *name_default = "default";

goochbot_t *
goochbot_new(void)
{
	goochbot_t *gbp = NULL;

	if ((gbp = malloc(sizeof(goochbot_t))) == NULL)
		errx(EX_OSERR, "malloc failed");

	if ((gbp->name = malloc(sizeof(char) * GOOCHBOT_NAME_MAXLEN)) == NULL)
		errx(EX_OSERR, "malloc failed");

	return gbp;
}

int
goochbot_init(goochbot_t *gbp, uint64_t id, char *name)
{
	int error;

	if (gbp == NULL) {
		error = ERR_GB_NULL;
		return error;
	}

	if (gbp->name == NULL) {
		error = ERR_GB_NULL;
		return error;
	}

	if ((error = goochbot_id_set(gbp, id)) != 0)
		return error;

	if ((error = goochbot_name_set(gbp, name)) != 0)
		return error;

	return error;
}

void
goochbot_destroy(goochbot_t *gbp)
{
	if (gbp == NULL)
		return;

	if (gbp->name != NULL)
		free(gbp->name);

	free(gbp);
	gbp = NULL;
}

uint64_t
goochbot_id_get(goochbot_t *gbp)
{
	int error;

	if (gbp == NULL) {
		error = ERR_GB_NULL;
		return error;
	}

	return gbp->id;
}

int
goochbot_id_set(goochbot_t *gbp, uint64_t id)
{
	if (gbp == NULL) {
		return ERR_GB_NULL;
	}

	if (id == 0) {
		srand(time(NULL));
		uint64_t random_id = rand();
		gbp->id = random_id;
	} else
		gbp->id = id;

	return 0;
}

char *
goochbot_name_get(goochbot_t *gbp)
{
	if (gbp == NULL)
		return NULL;

	if (gbp->name != NULL)
		return gbp->name;

	return NULL;
}

int
goochbot_name_set(goochbot_t *gbp, char *name)
{
	int error;

	if (gbp == NULL) {
		error = ERR_GB_NULL;
		return error;
	}

	if (gbp->name != NULL) {
		if (name == NULL)
			strlcpy(gbp->name, name_default, GOOCHBOT_NAME_MAXLEN);
		else
			strlcpy(gbp->name, name, GOOCHBOT_NAME_MAXLEN);
		error = 0;
	} else
		error = ERR_GB_NULL;

	return error;
}

uint64_t
goochbot_too_id_set(goochbot_too_t gbp, uint64_t id)
{
	int error;

	if (gbp == NULL) {
		error = ERR_GB_NULL;
		return error;
	}

	if (id == 0) {
		srand(time(NULL));
		uint64_t random_id = rand();
		gbp->id = random_id;
	} else
		gbp->id = id;

	return gbp->id;
}
