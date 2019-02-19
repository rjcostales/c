CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS)

$(TGTS): sortutils.o
	$(CC) $(CFLAGS) $@.c $(OBJ) -o $@

sortutils.o:
	$(CC) $(CFLAGS) -c sortutils.c -o $@

clean:
	rm -rf $(TGTS) $(OBJS) *.o a.out *.txt
