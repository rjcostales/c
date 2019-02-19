CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS)

$(TGTS): math.o
	$(CC) $(CFLAGS) -o $@ $@.c $< $(LIBS)

math.o:
	$(CC) $(CFLAGS) -o $@ -c math.c

clean:
	rm -rf $(TGTS) $(OBJS) *.o a.out *.txt
