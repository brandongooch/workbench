CFLAGS=	-g -Wall -Wextra -std=c99
PROG=	onering
SRCS=	main.c \
	ringbuf.c
OBJS=	main.o \
	ringbuf.o

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

main.o: main.c ringbuf.h
	${CC} ${CFLAGS} -c $< -o $@

ringbuf.o: ringbuf.c ringbuf.h
	${CC} ${CFLAGS} -c $< -o $@

.PHONY: clean

clean:
	rm -f onering *.o *.core
