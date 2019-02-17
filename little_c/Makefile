# Universal Makefile ver. 1.0
CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall -Wextra

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
	rm -rf $(EXEC) $(OBJS)
