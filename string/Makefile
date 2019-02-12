CC     = clang
CFLAGS = -Ofast -Wall -std=c99
LIBS 	= -Lm

TGTS := strfunc.o strtest
SRCS := *.c
HDRS := *.h

all:$(TGTS)

strtest:
	$(CC) $(CFLAGS) -o strtest strtest.c strfunc.o

clean:
	rm -f $(TGTS)
