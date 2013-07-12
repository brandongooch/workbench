CFLAGS=-g -Wall -Wextra -std=c99
PROG=robotic
SRCS=main.c goochbot.c
OBJS=${SRCS:.c=.o}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} $^

clean:
	rm -f robotic *.o *.core
