CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(patsubst %.c,%,$(SRCS))

all: $(TGTS) $(OBJS)

$(filter-out endianess exparser struct xmas, $(TGTS)): $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@

endianess:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-sequence-point

exparser:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-unused-result -Wno-maybe-uninitialized

struct:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-format

xmas:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-implicit-int -Wno-logical-not-parentheses

clean:
	rm -rf $(TGTS) *.out *.exe
