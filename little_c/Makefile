# Universal Makefile ver. 1.0
CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

EXEC	= lc

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS):$ (SRCS)
	$(CC) $(CFLAGS) -c $(SRCS) $(LIBS)

clean:
	rm -rf $(EXEC) $(OBJS)
