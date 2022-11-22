CC   = gcc -Ofast -Wall -std=gnu17

SRCS = *.c
EXES = fib0 fib1 fib2 fib3 fib4 fibT

all: $(EXES)

fib0: fibonacci0.c
	$(CC) -o fib0 fibonacci0.c

fib1: fibonacci1.c
	$(CC) -o fib1 fibonacci1.c

fib2: fibonacci2.c
	$(CC) -o fib2 fibonacci2.c

fib3: fibonacci3.c
	$(CC) -o fib3 fibonacci3.c

fib4: fibonacci4.c
	$(CC) -o fib4 fibonacci4.c

fibT: fibonacciT.c
	$(CC) -o fibT fibonacciT.c

clean:
	rm -f $(EXES) *.out *.exe
