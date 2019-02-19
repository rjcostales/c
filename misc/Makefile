CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

PROJECT := .
SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@

exparser:
	$(CC) $(CFLAGS) -Wno-unused-result -Wno-maybe-uninitialized -o $@ $@.c

struct:
	$(CC) $(CFLAGS) -Wno-format -o $@ $@.c

xmas:
	$(CC) $(CFLAGS) -Wno-implicit-int -Wno-logical-not-parentheses -o $@ $@.c

clean:
	rm -rf $(TGTS)
