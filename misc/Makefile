CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(filter-out endianess exparser struct xmas, $(EXES)): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

endianess: endianess.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-sequence-point

exparser: exparser.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-unused-result -Wno-maybe-uninitialized

struct: struct.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-format

xmas: xmas.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-implicit-int -Wno-logical-not-parentheses

clean:
	rm -rf $(TGTS) *.out *.exe
