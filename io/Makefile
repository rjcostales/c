CC     = gcc
CFLAGS = -std=c11 -Wall -Ofast

SRCS   := $(wildcard *.c)
HDRS   := $(wildcard *.h)
TGTS   := $(patsubst %.c,%,$(SRCS))

all: $(TGTS)

$(TGTS): $(SRCS)
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -rf $(TGTS) *.out *.exe
