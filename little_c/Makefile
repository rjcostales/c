# Universal Makefile ver. 1.0
CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

TGTS   = lc

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := $(patsubst %.c,%.o,$(SRCS))

all: $(TGTS)

$(TGTS): $(OBJS) $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TGTS)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
