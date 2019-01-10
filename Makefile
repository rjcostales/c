CC = clang
CFLAGS = -Ofast
LFLAGS = -Lm
TARGETS = analyze length size test
SOURCES = $(wildcard *.c)

all: $(TARGETS)

clean:
	rm -f $(TARGETS)
