CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

clean:
	rm -rf $(TGTS)
