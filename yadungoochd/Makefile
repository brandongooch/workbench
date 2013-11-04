CFLAGS=	 -g -Wall -Wextra -std=c99 -lutil -lpthread
PROG=	goochd
SRCS=	goochd.c

${PROG}: goochd.c
	${CC} ${CFLAGS} $< -o $@

.PHONY: clean

clean:
	rm -f ${PROG} *.o *.core
