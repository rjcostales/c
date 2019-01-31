CC = clang
CFLAGS = -Ofast
LFLAGS = -lm

OBJECTS = math.o mathTest factorize pi primes randgen sizes a.out
SOURCES = *.c

all: math mathTest factorize pi primes randgen sizes

math: math.c
	$(CC) $(CFLAGS) -c math.c -o math.o

mathTest: mathTest.c math.h
	$(CC) $(CFLAGS) mathTest.c math.o -o mathTest

factorize: math.c
	$(CC) $(CFLAGS) -c factorize.c -o factorize

pi: pi.c
	$(CC) $(CFLAGS) pi.c -o pi $(LFLAGS)

primes: primes.c
	$(CC) $(CFLAGS) primes.c -o primes

randgen: randgen.c
	$(CC) $(CFLAGS) randgen.c -o randgen

sizes: sizes.c
	$(CC) $(CFLAGS) sizes.c -o sizes

clean:
	rm -f $(OBJECTS)
