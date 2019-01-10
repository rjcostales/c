CC = clang
CCFLAGS = -Ofast

TARGETS = fib fib0 fib1 fib2 fib3 fib4
SOURCES = *.c

all: $(TARGETS)

fib: fibonacci.c
	$(CC) $(CCFLAGS) fibonacci.c -o fib

fib0: fibonacci.c
	$(CC) $(CCFLAGS) fibonacci0.c -o fib0

clean:
	rm -f $(TARGETS)
