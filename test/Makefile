CC     = gcc
CFLAGS = -Ofast -w -std=c11

SRCS  := $(wildcard *.c)
#HDRS  := $(wildcard *.h)
TGTS  := $(patsubst %.c,%,$(SRCS))
#OBJS  := $(patsubst %.h,%.o,$(HDRS))

all:	$(TGTS)

$(TGTS):	$(OBJS)
	$(CC) $(CFLAGS) $@.c $(OBJS) -o $@

#$(OBJS):
#	$(CC) $(CFLAGS) -c sorts.c

clean:
	rm -rf $(TGTS) *.o *.out *.exe
