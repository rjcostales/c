CC		= gcc
CFLAGS	= -std=c99 -Wall
LIBS	= -lm

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS 	:= $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $(LFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -Wno-format $@.c -o $@

xmas:
	gcc -Wno-implicit-int $@.c -o $@

clean:
	rm -f $(TGTS) $(OBJS)
