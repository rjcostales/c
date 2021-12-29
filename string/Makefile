CC	    = gcc
CFLAGS = -Ofast -Wall -std=c11
NOWARN =  -Wno-implicit-function-declaration -Wno-format

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
TGTS  := $(shell grep -w main *.c|sed 's/.c:.*//')
OBJS  := strfunc.o

all: $(TGTS) $(OBJS)

$(filter-out strtok, $(TGTS)): $(SRCS) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS)

strtok: strfunc.o
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS) $(NOWARN)

$(OBJS):
	$(CC) $(CFLAGS) -c strfunc.c

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
