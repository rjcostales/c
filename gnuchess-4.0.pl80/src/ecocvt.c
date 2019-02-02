/*
 * ecocvt.c - ECO code for GNU Chess
 *
 * Copyright (c) 1985-1996 Stuart Cracraft, John Stanback,
 *                         Daryl Baker, Conor McCarthy,
 *                         Mike McGann, Chua Kong Sian
 * Copyright (c) 1985-1996 Free Software Foundation
 *
 * This file is part of GNU CHESS.
 *
 * GNU Chess is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GNU Chess is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Chess; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "gnuchess.h"
struct gdxdata
{
    unsigned long hashbd;
    unsigned long hashkey;
    unsigned int ecoptr;
    utshort cntr;
} DATA,DATA2;

struct stat buf;
main(int argc, char *argv[])
{
int fd,ofd;
int j=0;
int i;
int first;
if(argc != 3){printf("Usage ecocvt infile outfile\n"); exit(1);}
fd=open(argv[1],O_RDONLY);
ofd=open(argv[2],O_CREAT | O_RDWR, 0644);
stat(argv[1],&buf);

if(fd == 0) {printf("open error\n");exit(10);}
if(ofd == 0) {printf("open error\n");exit(10);}
first=1;
for(i=0;i<buf.st_size; i+= sizeof(struct gdxdata)){
	if(read(fd,&DATA,sizeof(struct gdxdata))<0){perror("ofd");exit(1);}
	if(first){DATA2 = DATA; first=0; } else
	{
	if(DATA2.hashbd == DATA.hashbd && DATA2.hashkey == DATA.hashkey){j++;DATA.cntr = j;}
	else {DATA2 = DATA; j=0;}
	}
if(DATA2.hashbd < DATA.hashbd)printf("erroe\n");
if(	write(ofd,&DATA,sizeof(struct gdxdata)) <0){perror("write");exit(1);};
}
if(close(fd) < 0){perror("fd");exit(1);}
if(close(ofd) < 0){perror("ofd");exit(1);}
exit(0);
}
