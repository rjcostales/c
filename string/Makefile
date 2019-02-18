CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS := strfunc.o strtest

all: $(TGTS)

strfunc.o:
	$(CC) $(CFLAGS) -o strfunc.o -c strfunc.c

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -rf $(TGTS) $(OBJS) a.out
