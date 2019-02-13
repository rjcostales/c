CC		= gcc
CFLAGS	= -Ofast -std=c99 -Wall

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(wildcard *.o)
TGTS	:= $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TGTS) $(OBJS) a.out
