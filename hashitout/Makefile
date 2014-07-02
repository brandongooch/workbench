CFLAGS=-g -Wall -Wextra -std=c99
LDFLAGS=-v
PROG=hashitout
SRCS=main.c hashtable.c hashfunc.c datafile.c
OBJS=${SRCS:.c=.o}

${PROG}: ${OBJS}
	${CC} ${CFLAGS} -o ${PROG} ${OBJS}

clean:
	rm -f ${PROG} *.o *.core
