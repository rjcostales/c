CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-unused-result

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(patsubst %.c,%,$(SRCS))

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c $(NOWARN)

clean:
	rm -rf $(TGTS) *.out *.exe
