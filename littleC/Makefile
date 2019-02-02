CC      = clang
CFLAGS  = -Ofast

all:	lc

lc:		littlec.o lclib.o parser.o
		$(CC) $(CFLAGS) -o lc littlec.o lclib.o parser.o

littlec.o:	LittleC.c LittleC.h
		$(CC) $(CFLAGS) -c -o littlec.o LittleC.c

lclib.o:	LCLib.c
		$(CC) $(CFLAGS) -c -o lclib.o LCLib.c

parser.o:	Parser.c
		$(CC) $(CFLAGS) -c -o parser.o Parser.c

clean:
		rm *.o
		rm lc
