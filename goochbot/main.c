#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "goochbot.h"

#define NUM_BOTS 9

int
main(void)
{
	goochbot_t *gbp = NULL;
	uint64_t id = 777;
	char *name = "brandon";

	gbp = goochbot_init(0, NULL);
	goochbot_set_id(gbp, 0);
	goochbot_set_name(gbp, NULL);

	printf("goochbot id: %lu\n", gbp->id);
	printf("goochbot name: %s\n", gbp->name);

	goochbot_destroy(gbp);
	gbp = NULL;

	gbp = goochbot_init(id, name);
	printf("goochbot id: %lu\n", gbp->id);
	printf("goochbot name: %s\n", gbp->name);

	printf("\nmodifying goochbot configuration...\n\n");
	goochbot_set_id(gbp, 999);
	goochbot_set_name(gbp, "stephanie");

	printf("goochbot id: %lu\n", gbp->id);
	printf("goochbot name: %s\n", gbp->name);

	goochbot_destroy(gbp);
	gbp = NULL;

	exit(EXIT_SUCCESS);
}
