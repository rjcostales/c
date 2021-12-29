CC     = gcc
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=c11
NOWARN = -Wno-unused-variable -Wno-uninitialized

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   := math.o
TGTS   := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS) $(OBJS)

$(TGTS): $(SRCS) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS) $(LIBS) $(NOWARN)

$(OBJS):
	$(CC) $(CFLAGS) -c math.c

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
