CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
TGTS := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS) $(OBJS)

$(filter-out struct insertintoarray, $(TGTS)): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-format

insertintoarray:
	$(CC) $(CFLAGS) $@.c -o $@ -Wno-unused-result

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
