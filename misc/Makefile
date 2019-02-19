CC		= gcc
CFLAGS	= -std=c99

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(shell grep -w 'int main' *.c|sed 's/.c:.*//')

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $(LFLAGS) $@.c -o $@

xmas:
	$(CC) -Wno-implicit-int $@.c -o $@

clean:
	rm -rf $(TGTS)
