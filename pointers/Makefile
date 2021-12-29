CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS) $(OBJS)

$(filter-out struct insertintoarray, $(TGTS)): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-format

insertintoarray:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-unused-result

clean:
	rm -rf $(TGTS) *.out *.exe
