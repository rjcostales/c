CC     = gcc-10
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(filter-out struct insertintoarray, $(EXES)): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c

struct: struct.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-format

insertintoarray: insertintoarray.c
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-unused-result

clean:
	rm -rf $(TGTS) *.out *.exe
