CC     = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   := $(wildcard *.o)
TGTS   := $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS) *.out *.exe
