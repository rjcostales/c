CC     = gcc
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=c11
NOWARN = -Wno-unused-variable -Wno-uninitialized

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   :=  math.o
TGTS   := $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS) $(OBJS)

$(TGTS): $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $< $(LIBS) $(NOWARN)

math.o:
	$(CC) $(CFLAGS) -c math.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
