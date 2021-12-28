CC	    = gcc
CFLAGS = -Ofast -Wall -std=c11

SRCS  := $(wildcard *.c)
HDRS  := $(wildcard *.h)
OBJS  := strfunc.o
TGTS  := csv

all: $(TGTS) $(OBJS)

strtest: strfunc.o
	$(CC) $(CFLAGS) $@.c $< -o $@

strfunc.o:
	$(CC) $(CFLAGS) -c strfunc.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) *.O *.out *.exe
