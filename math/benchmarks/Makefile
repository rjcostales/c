CC     = gcc
CFLAGS = -Ofast -Wall -std=c11
LIBS   = -lm

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
TGTS  := $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

clean:
	rm -f $(TGTS) *.out *.exe
