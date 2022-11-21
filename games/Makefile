CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-implicit-function-declaration

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c $(NOWARN)

clean:
	rm -rf $(TGTS) *.out *.exe
