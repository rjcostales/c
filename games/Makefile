CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) a.out
