CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all: sortutils $(TGTS)

$(TGTS): sortutils
	$(CC) $(CFLAGS) $@.c sortutils.o -o $@

sortutils:
	$(CC) $(CFLAGS) -c sortutils.c -o $@.o

clean:
	rm -rf $(TGTS) $(OBJS) a.out *.txt
