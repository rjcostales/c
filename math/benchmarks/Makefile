CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17
LIBS   = -lm

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
TGTS := $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) -o $@ $@.c $(LIBS)

clean:
	rm -f $(TGTS) *.out *.exe
