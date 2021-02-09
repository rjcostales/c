# Universal Makefile ver. 1.0
CC		= gcc
CFLAGS	= -Ofast -Wall -std=c11

EXEC	= lc

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.c,%.o,$(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS) $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -rf $(EXEC) $(OBJS) *.o *.out *.exe
