CC     = gcc
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-unused-variable -Wno-uninitialized

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := math.o
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS) $(LIBS) $(NOWARN)

$(OBJS): math.c math.h
	$(CC) $(CFLAGS) -c math.c

clean:
	rm -rf $(TGTS) *.out *.exe
