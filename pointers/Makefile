CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(filter-out struct insertintoarray, $(EXES)): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

struct: struct.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-format

insertintoarray: insertintoarray.c
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-unused-result

clean:
	rm -rf $(TGTS) *.out *.exe
