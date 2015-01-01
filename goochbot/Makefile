CFLAGS=	-g -Wall -Wextra -std=c99
PROG=	robotic
SRCS=	main.c \
	goochbot.c
OBJS=	main.o \
	goochbot.o

${PROG}: ${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o $@

main.o: main.c goochbot.h goochbot.o
	${CC} ${CFLAGS} -c $< -o $@

goochbot.o: goochbot.c goochbot.h
	${CC} ${CFLAGS} -c $< -o $@

.PHONY: clean

clean:
	rm -f robotic *.o *.core
