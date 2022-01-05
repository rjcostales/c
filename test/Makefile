CC     = gcc-10
CFLAGS = -Ofast -w -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := $(wildcard *.o)
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(EXES): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
