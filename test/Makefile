CC     = gcc
CFLAGS = -Ofast -w -std=gnu17

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
TGTS := $(shell grep -w main *.c|sed 's/.c:.*//')
#OBJS := $(patsubst %.h,%.o,$(HDRS))

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $@.c -o $@

#$(OBJS):
#	$(CC) $(CFLAGS) -c sorts.c

clean:
	rm -rf $(TGTS)  *.out *.exe
