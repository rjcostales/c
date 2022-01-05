CC     = gcc-10
CFLAGS = -Ofast -Wall -std=gnu17 -DWRITE

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := fileutils.o sortutils.o
EXES := generate quicksort sorts_int
SORT := bubble merge insertion selection shell

TGTS := $(SORT) $(EXES) $(OBJS)

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)

$(SORT): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -DSORT_NAME=$@_sort -o $@ sort.c $(OBJS)

$(EXES): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c fileutils.c sortutils.c

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
