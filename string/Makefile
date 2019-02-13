CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall
LIBS	= -lm

TGTS := strfunc.o strtest
SRCS := *.c
HDRS := *.h

all:$(TGTS)

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -rf $(TGTS)
