CC     = gcc
LIBS   = -lm
CFLAGS = -Wall -std=c11

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
TGTS  := $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS): *.c
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

clean:
	rm -f $(TGTS) *.out *.exe
