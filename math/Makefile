CC		= gcc
CFLAGS	= -std=c99 -Wall
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

math:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

math_test:
	$(CC) $(CFLAGS) $@.c math.o -o $@

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)


clean:
	rm -rf $(TGTS) $(OBJS)
