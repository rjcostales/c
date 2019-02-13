CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS := strfunc.o strtest

all: $(TGTS)

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -rf $(TGTS) $(OBJS) a.out
