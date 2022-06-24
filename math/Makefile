<<<<<<< HEAD
CC     = gcc-10
=======
CC     = gcc
>>>>>>> 238493a (update)
LIBS   = -lm
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-unused-variable -Wno-uninitialized

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := math.o
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS) $(LIBS) $(NOWARN)

$(OBJS): math.c $(HDRS)
	$(CC) $(CFLAGS) -c math.c

clean:
	rm -rf $(TGTS) *.out *.exe
