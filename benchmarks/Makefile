CC     = gcc
LIBS   = -lm
CFLAGS = -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := $(wildcard *.o)
TGTS := $(patsubst %.c,%,$(SRCS))

all: $(TGTS) $(OBJS)

$(TGTS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

clean:
	rm -f $(TGTS) $(OBJS) *.out *.exe
