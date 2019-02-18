CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

TGTS := strfunc.o strtest
SRCS := *.c
HDRS := *.h

all:

strfunc.o:
	$(CC) $(CFLAGS) -o strfunc.o -c strfunc.c

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -rf $(TGTS)
