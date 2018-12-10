obj = write1 write2 write3
src = *.c

all: write1 write2 write3


write1: write1.c
	gcc write1.c -o write1

write2: write2.c
	gcc write2.c -o write2

write3: write3.c
	gcc write3.c -o write3

clean:
	rm -f $(obj)
