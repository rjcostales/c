CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(patsubst %.c,%,$(SRCS))

all:	$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -Wno-format -o $@ $@.c

clean:
	rm -rf $(TGTS) *.out *.exe
