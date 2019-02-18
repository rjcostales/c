CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS := strfunc.o strtest

<<<<<<< HEAD
all:

strfunc.o:
	$(CC) $(CFLAGS) -o strfunc.o -c strfunc.c
=======
all: $(TGTS)
>>>>>>> d9b64c0b0335c56d2f4705a25ddf7b861f3b1aa1

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -rf $(TGTS) $(OBJS) a.out
