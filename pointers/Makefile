CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-format

insertintoarray:
	$(CC) $(CFLAGS) -o $@ $@.c -Wno-unused-result

clean:
	rm -rf $(TGTS) *.out *.exe
