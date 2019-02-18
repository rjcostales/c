CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

math_test:
	$(CC) $(CFLAGS) $@.c math.o -o $@

math:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

clean:
	rm -rf $(TGTS) $(OBJS)
