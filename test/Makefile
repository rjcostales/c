CC     = gcc
CFLAGS = -Ofast -w -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS)  *.out *.exe
