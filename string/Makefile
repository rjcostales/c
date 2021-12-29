CC	    = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
TGTS  := $(shell grep -w main *.c|sed 's/.c:.*//')
OBJS  := strfunc.o

all: $(TGTS) $(OBJS)

$(filter-out strtok, $(TGTS)): $(OBJS)
	$(CC) $(CFLAGS) $@.c -o $@ $< $(LIBS) $(NOWARN)

strtok: strfunc.o
	$(CC) $(CFLAGS) $@.c -o $@ $(OBJS) -Wno-implicit-function-declaration -Wno-format

strfunc.o:
	$(CC) $(CFLAGS) -c strfunc.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) *.o *.out *.exe
