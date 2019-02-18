CC		= gcc
CFLAGS	= -Ofast -Wall -std=c99
LIBS	= -lm

SRCS	:= $(wildcard *.c)
HDRS	:= $(wildcard *.h)
OBJS	:= $(patsubst %.h,%.o,$(HDRS))
TGTS	:= $(patsubst %.c,%,$(SRCS))

all:$(TGTS)

$(TGTS):
	$(CC) $(CFLAGS) $@.c -o $@ $(LIBS)

math_test: math
	$(CC) $(CFLAGS) $@.c $(OBJS) -o $@

math:
	$(CC) $(CFLAGS) -c $@.c -o $@.o

clean:
	rm -rf $(TGTS) $(OBJS)
