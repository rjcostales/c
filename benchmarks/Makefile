CC     = gcc
LIBS   = -lm
CFLAGS = -Wall -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(patsubst %.c,%,$(SRCS))

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

clean:
	rm -rf $(TGTS) *.out *.exe
