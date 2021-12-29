CC     = gcc
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-unused-result

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := $(wildcard *.o)
TGTS := $(shell grep -w main *.c|sed 's/.c:.*//' | sort -u)

all: $(TGTS) $(OBJS)

$(TGTS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS) $(NOWARN)

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
