CFLAGS=	-g -Wall -Wextra -std=c99 -pedantic
PROG=	outofsorts
SRCS=	main.c \
	bubblesort.c
HDRS=	allsorts.h \
	bubblesort.h
OBJS=	main.o \
	bubblesort.o \

.PHONY: all clean

all: ${OBJS} ${PROG}

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

${OBJS}: ${HDRS}

main.o: main.c
	${CC} ${CFLAGS} -o $@ -c $<

bubblesort.o: bubblesort.c
	${CC} ${CFLAGS} -o $@ -c $<

clean:
	rm -f ${PROG} ${OBJS} *.core
