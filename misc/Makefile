<<<<<<< HEAD
CC     = gcc-10
=======
CC     = gcc
>>>>>>> 238493a (update)
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//' | sort -u)

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(filter-out endianess exparser struct xmas, $(EXES)): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c

endianess: endianess.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-sequence-point

exparser: exparser.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-unused-result -Wno-maybe-uninitialized

struct: struct.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-format

xmas: xmas.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-implicit-int -Wno-logical-not-parentheses

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
