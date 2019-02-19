# Universal Makefile ver. 1.0
CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

EXEC	= lc

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.c,%.o,$(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS) $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) $*.c

clean:
	rm -rf $(EXEC) $(OBJS)
