# Universal Makefile ver. 1.0
EXEC	= lc

CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.c, %.o, $(SRCS))

all:$(EXEC)

$(EXEC):$(OBJS)
		$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS):$(SRCS)
		$(CC) $(CFLAGS) -c $(SRCS) $(LIBS)

clean:
		rm -rf $(EXEC) $(OBJS)
