CC = clang
CFLAGS = -Ofast
LFLAGS = -Lm
TARGETS = backtrack bruteforce permutation random
SOURCES = *.c

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
