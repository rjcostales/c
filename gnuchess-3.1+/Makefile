#
# Makefile for CHESS
#
# Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.
#
# This file is part of CHESS.
#
# CHESS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.  No author or distributor
# accepts responsibility to anyone for the consequences of using it
# or for whether it serves any particular purpose or works at all,
# unless he says so in writing.  Refer to the CHESS General Public
# License for full details.
#
# Everyone is granted permission to copy, modify and redistribute
# CHESS, but only under the conditions described in the
# CHESS General Public License.   A copy of this license is
# supposed to have been given to you along with CHESS so you
# can know your rights and responsibilities.  It should be in a
# file named COPYING.  Among other things, the copyright notice
# and this notice must be preserved on all copies.
#
# gnuchess  will be the curses-based chess.
# gnuchessn will be the fancy-display-curses-based chess.
# gnuchessr will be the unfancy tty based chess.
# gnuchessc will be the chesstool based chess.
# gnuchessx will be the xchess based chess.
#		(/usr/games/chesstool gnuchessr)
#    for use with SUN's chesstool and X-window's xchess.
#    To use with latter, add -DCHESSTOOL to the nondsp.o
#    compilation lines (e.g. cc -O -DCHESSTOOL ...)
# If you want to use the persistent transpositon table add
# -DHASHFILE=\"$(LIBDIR)/gnuchess.hash\"
# to the gnuchess.o compilation line. You also have to run gnuchess -t
# To actually create the file. -t can also be used to report
# statistics about the file or to resize it.
#
VERS=	3.1
DIST=	README DOCUMENTATION Makefile Makefile.dos version.h\
	gnuchess.h gnuchess.c\
	uxdsp.c nondsp.c nuxdsp.c gnuchess.book Xchess
BINDIR= /usr/local/bin
LIBDIR= /usr/local/lib
LCURSES=-lcurses -ltermcap
HASH=	-DHASHFILE=\"$(LIBDIR)/gnuchess.hash\"
BOOK=	-DBOOK=\"$(LIBDIR)/gnuchess.book\"
#CC= cc
CC=	gcc -W
#CFLAGS= -O -Dinline=""		# for Aviion gcc-1.35
CFLAGS=	-O -finline-functions -fstrength-reduce

all : gnuchess gnuchessr gnuchessn xboard chesstool postprint gnuan

gnuchess: gnuchess.o uxdsp.o
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -o gnuchess gnuchess.o uxdsp.o $(LCURSES)

gnuan: gnuchess.o gnuan.o nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DCHESSTOOL -o gnuan gnuchess.o gnuan.o

chesstool: gnuchess.o nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DCHESSTOOL -c nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DCHESSTOOL -o gnuchessc gnuchess.o nondsp.o

xboard: gnuchess.o nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DXBOARD -c nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DXBOARD -o gnuchessx gnuchess.o nondsp.o

gnuchessr: gnuchess.o nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -c nondsp.c
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -o gnuchessr gnuchess.o nondsp.o

gnuchessn: gnuchess.o nuxdsp.o
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -o gnuchessn gnuchess.o nuxdsp.o $(LCURSES)
	
postprint: postprint.o
	$(CC) $(CFLAGS) -o postprint postprint.o
	
gnuchessd:  nondsp.c gnuchess.c
	$(CC) -Wall -fstrength-reduce -p -g -Dinline="" $(HASH) $(BOOK) -DCHESSTOOL -DDEBUG -o  gnuchessd  nondsp.c gnuchess.c


gnuan.o: gnuan.c gnuchess.h version.h
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -c gnuan.c

gnuchess.o: gnuchess.c gnuchess.h version.h
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -c gnuchess.c

uxdsp.o: uxdsp.c gnuchess.h version.h
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -c uxdsp.c

nuxdsp.o: nuxdsp.c gnuchess.h version.h
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -c nuxdsp.c

nondsp.o: nondsp.c gnuchess.h version.h
	$(CC) $(CFLAGS) $(HASH) $(BOOK) -DCHESSTOOL -c nondsp.c

postprint.o: postprint.c gnuchess.h version.h
	$(CC) $(CFLAGS) -c postprint.c

lint:
	lint gnuchess.c uxdsp.c nondsp.c nuxdsp.c

unprotoize:
	unprotoize -c -DHASHFILE gnuchess.c uxdsp.c nondsp.c nuxdsp.c

protoize:
	protoize -c -DHASHFILE gnuchess.c uxdsp.c nondsp.c nuxdsp.c

distribution:
	tar cf - $(DIST) > gnuchess.tar
	compress gnuchess.tar
	mv gnuchess.tar.Z gnuchess-$(VERS).tar.Z

install:
	cp gnuchessr $(BINDIR)/gnuchess
	cp gnuchess $(BINDIR)/gnuchess
	cp gnuchess.book $(LIBDIR)/gnuchess.book

clean:
	-rm gnuchessx gnuchessc gnuchess gnuchessr gnuchessn gnuchessd postprint gnuan *.o
