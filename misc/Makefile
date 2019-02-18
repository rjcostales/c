CC		= gcc
CFLAGS	= -std=c99

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS 	:= $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $(LFLAGS) $@.c -o $@

xmas:
	$(CC) -Wno-implicit-int $@.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS)
