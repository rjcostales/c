CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
TGTS := $(patsubst %.c,%,$(SRCS))

all: $(TGTS) $(OBJS)

$(filter-out endianess exparser struct xmas, $(TGTS)): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@

endianess:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-sequence-point

exparser:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-unused-result -Wno-maybe-uninitialized

struct:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-format

xmas:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-implicit-int -Wno-logical-not-parentheses

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
