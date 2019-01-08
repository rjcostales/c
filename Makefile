CC = clang
CFLAGS = -O3 -Wall
LFLAGS = -Lm
TARGETS = backtrack bruteforce permutation random
SOURCES = *.c

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
