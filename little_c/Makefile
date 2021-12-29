# Universal Makefile ver. 1.0
CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := $(patsubst %.c,%.o,$(SRCS))
TGTS   = lc

all: $(TGTS) $(OBJS)

$(TGTS): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TGTS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -rf $(TGTS) $(OBJS) *.out *.exe
