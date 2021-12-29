CC     = gcc
CFLAGS = -Ofast -Wall -std=c11 -DWRITE

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
TGTS  := $(shell grep -w main *.c|sed 's/.c:.*//')
OBJS  := fileutils.o

all: $(TGTS)

$(TGTS): $(OBJS) *.c
	$(CC) $(CFLAGS) $@.c $(OBJS) -o $@

$(OBJS):
	$(CC) $(CFLAGS) -c fileutils.c

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
