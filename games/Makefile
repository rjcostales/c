CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.c,%.o,$(HDRS))
TGTS	:= $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TGTS) $(OBJS) a.out
