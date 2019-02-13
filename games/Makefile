CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS):$(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TGTS)
