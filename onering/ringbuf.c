#include <sys/time.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "ringbuf.h"

int
ringbuf_init(struct ringbuf_s *rbp)
{
	size_t i;

	rbp->head = 0;
	rbp->tail = 0;
	rbp->count = 0;

	for (i = 0; i < RINGSIZE; i++) {
		rbp->slot[i] = 0;
	}

	return 0;
}

int
ringbuf_put(struct ringbuf_s *rbp, uint32_t i)
{
	if (rbp->count < RINGSIZE) {
		size_t tail = rbp->tail;
		rbp->slot[tail % RINGSIZE] = i;
		tail = (tail + 1) % RINGSIZE;
		rbp->tail = tail;
		rbp->count++;
	} else {
		printf("warning: buffer full\n");
		return -1;
	}

	return 0;
}

uint32_t
ringbuf_get(struct ringbuf_s *rbp)
{
	uint32_t sv;
	size_t head = rbp->head;

	if (rbp->count == 0) {
		printf("warning: buffer empty\n");
		return 0;
	}

	sv = rbp->slot[head % RINGSIZE];
	rbp->slot[head % RINGSIZE] = 0;
	head = (head + 1) % RINGSIZE;
	rbp->head = head;
	rbp->count--;

	return sv;
}

void
ringbuf_drain(struct ringbuf_s *rbp)
{
	while (rbp->count > 0) {
		ringbuf_get(rbp);
	}
}

void
ringbuf_print(struct ringbuf_s *rbp)
{
	size_t i;
	for (i = 0; i < RINGSIZE; i++) {
		printf("[%d]", rbp->slot[i]);
	}
	printf("\n");
}
