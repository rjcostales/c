MAKEFILE= Makefile
ROOTDIR		= $$HOME

CC	= $(CPREFIX)gcc -I/usr/ucbinclude
#	Directory where program is installed

all:	cmpr testgen score

clean:	
	-rm *.o *.textx* testgen cmpr score

test:	cmpr testgen score
	cd  ../../src; ../test/CHESSTEST/testall

cmpr:	cmpr.c
	$(CC) $? -o $@

testgen:	testgen.c
	$(CC) $? -o $@

score:	score.c
	$(CC) $? -o $@
