CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS): *.c
	$(CC) $(CFLAGS) $@.c -o $@

exparser:
	$(CC) $(CFLAGS) -Wno-unused-result -Wno-maybe-uninitialized -o $@ $@.c

xmas:
	$(CC) $(CFLAGS) -Wno-implicit-int -Wno-logical-not-parentheses -o $@ $@.c

clean:
	rm -rf $(TGTS) *.out *.exe
