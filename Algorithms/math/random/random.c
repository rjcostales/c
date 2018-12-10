/* random - print a random number
**
** Copyright (C) 1992 by Jef Poskanzer <jef@acme.com>.  All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions
** are met:
** 1. Redistributions of source code must retain the above copyright
**    notice, this list of conditions and the following disclaimer.
** 2. Redistributions in binary form must reproduce the above copyright
**    notice, this list of conditions and the following disclaimer in the
**    documentation and/or other materials provided with the distribution.
** 
** THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
** OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
** HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
** OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
*/

#include <stdio.h>
#include <sys/times.h>

extern long random();
extern void srandom();
extern long time();
extern int atoi();
extern int getpid();
extern int getppid();

int
main( int argc, char** argv )
    {
    int argn;
    unsigned long seed, val;
    struct tms t;
    int n;
    char* usage = "usage:  %s [-s seed] <number> ...\n";

    /* This stuff is pretty non-portable.  Modify as necessary for
    ** your system.
    */
    seed = time( (long*) 0 );
    seed ^= getpid();
    seed ^= getppid();
    seed ^= times( &t );

    argn = 1;
    while ( argn < argc && argv[argn][0] == '-' )
	{
	if ( strcmp( argv[argn], "-s" ) == 0 )
	    {
	    ++argn;
	    seed = atoi( argv[argn] );
	    }
	else
	    {
	    (void) fprintf( stderr, usage, argv[0] );
	    exit( 1 );
	    }
	++argn;
	}
    if ( argc - argn < 1 )
	{
	(void) fprintf( stderr, usage, argv[0] );
	exit( 1 );
	}

    srandom( seed );
    for ( ; argn < argc; ++argn )
	{
	n = atoi( argv[argn] );
	if ( n <= 0 )
	    {
	    (void) fprintf( stderr, usage, argv[0] );
	    exit( 1 );
	    }
	val = random();
	val = val ^ ( random() >> 6 );
	val = ( val % n ) + 1;
	(void) printf( "%ld\n", val );
	}

    exit( 0 );
    }
