<<<<<<< HEAD
CC     = gcc-10
=======
CC     = gcc
>>>>>>> 238493a (update)
CFLAGS = -Ofast -Wall -std=gnu17 -DWRITE

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := fileutils.o sortutils.o
<<<<<<< HEAD
EXES := generate quicksort sorts_int
SORT := bubble merge insertion selection shell

TGTS := $(SORT) $(EXES) $(OBJS)
=======
EXES := bubble merge insertion selection shell quicksort generate

TGTS := $(EXES) $(OBJS)
>>>>>>> 238493a (update)

all: $(TGTS)

$(TGTS): $(SRCS) $(HDRS)

<<<<<<< HEAD
$(SORT): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -DSORT_NAME=$@_sort -o $@ sort.c $(OBJS)

$(EXES): $(SRCS) $(OBJS)
=======
$(filter-out quicksort generate, $(EXES)): $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -DSORT_NAME=$@_sort -o $@ sort.c $(OBJS)

quicksort generate: fileutils.c sortutils.c $(OBJS)
>>>>>>> 238493a (update)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS)

$(OBJS): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -c fileutils.c sortutils.c

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
