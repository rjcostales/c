obj = fibonacci fibonacciT math pi primes sizes random
src = *.c

all: fibonacci fibonacciT math pi primes sizes random

fibonacci: fibonacci.c
	gcc fibonacci.c -o fibonacci

fibonacciT: fibonacciT.c
	gcc fibonacciT.c -o fibonacciT

math: math.c
	gcc math.c -o math

pi: pi.c
	gcc pi.c -o pi -lm

primes: primes.c
	gcc primes.c -o primes

sizes: sizes.c
	gcc sizes.c -o sizes

random: random.c
	gcc random.c -o random

clean:
	rm -f $(obj)
