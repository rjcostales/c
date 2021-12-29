CC     = gcc
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=c11
NOWARN = -Wno-unused-result

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   := $(wildcard *.o)
TGTS   := $(shell grep -w main *.c|sed 's/.c:.*//' | sort -u)

all: $(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS) $(NOWARN)

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
