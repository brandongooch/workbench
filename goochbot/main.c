#include <sys/types.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "goochbot.h"

#define NUM_BOTS 9

#if 0
void
goochbot_too_change_id(goochbot_too_t gbp)
{
	goochbot_too_id_set(gbp, 0);
}

void
dont_change_me_bro_too(const goochbot_too_t gbp)
{
	goochbot_too_change_id(gbp);
}

void
goochbot_change_id(goochbot_t *gbp)
{
	goochbot_id_set(gbp, 0);
}

void
dont_change_me_bro(const goochbot_t *gbp)
{
	goochbot_change_id(gbp);
}
#endif

int
main(void)
{
	goochbot_t *gbp = NULL;
	uint64_t id = 777;
	char *name = "brandon";
	int error;

	printf("creating new goochbot...\n");
	gbp = goochbot_new();

	printf("initializing goochbot...\n");
	if ((error = goochbot_init(gbp, 0, name)) != 0)
		errx(ERR_GB_INIT, "error: could not initialize goochbot");

	printf("  --> goochbot id: %lu\n", goochbot_id_get(gbp));
	printf("  --> goochbot name: %s\n", goochbot_name_get(gbp));

	printf("setting goochbot id to 0...\n");
	goochbot_id_set(gbp, 0);

	printf("setting goochbot name...\n");
	goochbot_name_set(gbp, NULL);

	printf("  --> goochbot id: %lu\n", goochbot_id_get(gbp));
	printf("  --> goochbot name: %s\n", goochbot_name_get(gbp));

	printf("destroying goochbot...\n");
	goochbot_destroy(gbp);

	printf("creating new goochbot...\n");
	gbp = goochbot_new();

	printf("initializing goochbot...\n");
	goochbot_init(gbp, id, name);

	printf("  --> goochbot id: %lu\n", goochbot_id_get(gbp));
	printf("  --> goochbot name: %s\n", goochbot_name_get(gbp));

	printf("setting goochbot id to 999...\n");
	goochbot_id_set(gbp, 999);

	printf("setting goochbot name to stephanie...\n");
	goochbot_name_set(gbp, "stephanie");

	printf("  --> goochbot id: %lu\n", goochbot_id_get(gbp));
	printf("  --> goochbot name: %s\n", goochbot_name_get(gbp));

	goochbot_destroy(gbp);

	exit(EXIT_SUCCESS);
}
