CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-implicit-function-declaration

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
OBJS   := $(wildcard *.o)
TGTS   := $(patsubst %.c,%,$(SRCS))

all: $(TGTS) $(OBJS)

$(TGTS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@ $(NOWARN)

clean:
	rm -rf $(TGTS) *.out *.exe
