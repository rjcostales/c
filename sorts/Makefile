CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17 -DWRITE

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := fileutils.o
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)

$(EXES): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS)

$(OBJS): fileutils.c $(HDRS)
	$(CC) $(CFLAGS) -c fileutils.c

clean:
	rm -rf $(TGTS) *.out *.exe
