/*
 * ecosort.c - ECO Code for GNU Chess
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
#include "gnuchess.h"    
struct gdxecodata
      {
          unsigned long hashbd;
          unsigned long hashkey;
          unsigned int ecoptr;
          utshort cntr;
      };
char strcmd[256];
main()
{
sprintf(strcmd,"./binsort -r %d -k L:a:0:2<step1.eco>step2.eco\n",sizeof(struct gdxecodata));
printf("%s",strcmd);
system(strcmd);
exit(0);
}
