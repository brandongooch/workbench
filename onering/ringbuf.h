#define RINGSIZE 16

struct ringbuf_s {
	uint32_t slot[RINGSIZE];
	size_t head;
	size_t tail;
	ssize_t count;
};

int		ringbuf_init(struct ringbuf_s *);
int		ringbuf_put(struct ringbuf_s *, uint32_t);
uint32_t	ringbuf_get(struct ringbuf_s *);
void		ringbuf_drain(struct ringbuf_s *);
void		ringbuf_print(struct ringbuf_s *);
