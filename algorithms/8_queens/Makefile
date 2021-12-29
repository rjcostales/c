CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
TGTS := $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS) *.out *.exe
