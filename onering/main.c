#include <sys/time.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ringbuf.h"

#define NUMDATA  256

uint32_t
get_random()
{
	uint32_t rn;
	size_t i;

	for (i = 0; i < 1000; ++i) {
		rn = ((float)rand()) / RAND_MAX * 99 + 1;
	}

	return rn;
}

int
main(void)
{
	size_t i;
	uint32_t rv;
	uint32_t sv;
	struct ringbuf_s rb;
	struct ringbuf_s *rbp;

	srand(time(NULL));

	rbp = &rb;
	ringbuf_init(rbp);

	for (i = 0; i < NUMDATA; i++) {
		rv = get_random();
		if ((rv % 2) == 0) {
			printf("info: inserting: %d\n", rv);
			ringbuf_put(rbp, rv);
		} else {
			sv = ringbuf_get(rbp);
			printf("info: removing: %d\n", sv);
		}
		ringbuf_print(rbp);
	}

	printf("info: draining ringbuf\n");
	ringbuf_drain(rbp);
	ringbuf_print(rbp);

	exit(EXIT_SUCCESS);
}
