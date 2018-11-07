CC =		gcc
CFLAGS =	-O
LDFLAGS =	-s
BINDIR =	/usr/local/bin
MANDIR =	/usr/local/man/man1

all:		random

random:		random.c
	$(CC) $(CFLAGS) random.c $(LDFLAGS) -o random

install:	all
	rm -f ${BINDIR}/random
	cp random ${BINDIR}
	rm -f ${MANDIR}/random.1
	cp random.1 ${MANDIR}

clean:
	rm -f random *.o core