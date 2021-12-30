# Universal Makefile ver. 1.0
CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(patsubst %.c,%.o,$(SRCS))
EXEC = lc

TGTS := $(EXEC) $(OBJS)

all: $(TGTS)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c $*.c

clean:
	rm -rf $(TGTS) *.out *.exe
