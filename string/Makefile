CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := strfunc.o
TGTS := strtest

all: $(TGTS) $(OBJS)

strtest: strfunc.o
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS) 

strfunc.o:
	$(CC) $(CFLAGS) -o $@ -c strfunc.c

clean:
	rm -rf $(TGTS) $(OBJS) a.out
