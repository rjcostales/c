CC     = gcc
CFLAGS = -Ofast -Wall -std=gnu17 -DWRITE

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := fileutils.o sortutils.o
EXES := bubble merge insertion selection shell quicksort generate

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)

$(filter-out quicksort generate, $(EXES)): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -DSORT_NAME=$@_sort -o $@ sort.c $(OBJS)

quicksort generate: fileutils.c sortutils.c $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c fileutils.c sortutils.c

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
