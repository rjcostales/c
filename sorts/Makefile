CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17 -DWRITE

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := fileutils.o
TGTS := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS) $(OBJS)

$(TGTS): $(SRCS) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS)

$(OBJS): $(HDRS)
	$(CC) $(CFLAGS) -c fileutils.c

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
