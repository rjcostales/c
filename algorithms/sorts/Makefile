CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(shell grep -w main *.c|sed 's/.c:.*//')

all: $(TGTS)

$(TGTS): sortutils.o
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJ)

sortutils.o:
	$(CC) $(CFLAGS) -o $@ -c sortutils.c

clean:
	rm -rf $(TGTS) $(OBJS) *.o a.out *.txt
