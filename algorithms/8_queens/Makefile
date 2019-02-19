CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS) a.out
