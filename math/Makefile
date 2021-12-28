CC     = gcc
CFLAGS = -Ofast -Wall -std=c11
LIBS   = -lm
NOWARN = -Wno-unused-variable -Wno-uninitialized

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   := $(wildcard *.o)
TGTS   := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS)

$(TGTS): math.o
	$(CC) $(CFLAGS) $@.c $< -o $@ $(LIBS) $(NOWARN)

math.o:
	$(CC) $(CFLAGS) -c math.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
