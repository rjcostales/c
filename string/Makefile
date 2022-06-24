<<<<<<< HEAD
CC     = gcc-10
=======
CC     = gcc
>>>>>>> 238493a (update)
CFLAGS = -Ofast -Wall -std=gnu17
NOWARN = -Wno-implicit-function-declaration -Wno-format

SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)

OBJS := strfunc.o
EXES := $(shell grep -w main *.c|sed 's/.c:.*//')

TGTS := $(EXES) $(OBJS)

all: $(TGTS)

$(filter-out strtok, $(EXES)): $(SRCS) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS)

strtok: $(SRCS) $(HDRS) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $@.c $(OBJS) $(NOWARN)

$(OBJS): strfunc.c $(HDRS)
	$(CC) $(CFLAGS) -c strfunc.c

clean:
	rm -rf $(TGTS) *.out *.exe

print:
	echo SRCS $(SRCS)
	echo HDRS $(HDRS)
	echo TGTS $(TGTS)
