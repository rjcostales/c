CC		= gcc
CFLAGS	= -std=c99

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(shell grep -w 'int main' *.c|sed 's/.c:.*//')

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@

xmas:
	$(CC) -Wno-implicit-int -o $@ $@.c

clean:
	rm -rf $(TGTS)
