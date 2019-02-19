CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(shell grep -w 'main' *.c| grep -v 'xmas\|struct'| sed 's/.c:.*//')

all:	$(TGTS)

$(TGTS): struct
	$(CC) $(CFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -Wno-format -o $@ $@.c

xmas:

clean:
	rm -rf $(TGTS) xmas struct
