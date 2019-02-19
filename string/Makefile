CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := strfunc.o
TGTS := strtest

all: $(TGTS) $(OBJS)

strtest: strfunc.o
	$(CC) $(CFLAGS) $@.c $< -o $@

strfunc.o:
	$(CC) $(CFLAGS) -c strfunc.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) a.out
