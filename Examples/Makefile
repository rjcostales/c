obj = a.out array math pi primes randgen random sizes
src = *.c

all: array math pi primes randgen random sizes

array: array.c

math: math.c
	gcc math.c -o math

pi: pi.c
	gcc pi.c -o pi -lm

primes: primes.c
	gcc primes.c -o primes

sizes: sizes.c
	gcc sizes.c -o sizes

randgen: randgen.c
	gcc randgen.c -o randgen

random: random.c
	gcc random.c -o random

clean:
	rm -f $(obj)
