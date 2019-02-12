CC     = clang
CFLAGS = -std=c99
LFLAGS = -lm

PROJECT := .
SOURCES := $(shell ls *.c)
HEADERS := $(shell ls *.h)
OBJECTS := $(patsubst %.c,%.o,$(SOURCES))
TARGETS := $(patsubst %.c,%,$(SOURCES))

all: $(TARGETS)

$(TARGETS):
	$(CC) $(CFLAGS) $(LFLAGS) $@.c -o $@

struct:
	$(CC) $(CFLAGS) -Wno-format $@.c -o $@

xmas:
	gcc -Wno-implicit-int $@.c -o $@

clean:
	rm -f $(TARGETS) $(OBJECTS)
