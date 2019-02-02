/*
 * util.c - C source for GNU CHESS
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

int
parse (FILE * fd, UTSHORT *mv, SHORT side, CHAR *opening)
{
  register int c, i, r1, r2, c1, c2;
  CHAR s[128];
  CHAR *p;

  while ((c = getc (fd)) == ' ' || c == '\n') ;
  i = 0;
  s[0] = (CHAR) c;
  if (c == '!')
    {
      p = opening;
      do
	{
	  *p++ = c;
	  c = getc (fd);
	  if (c == '\n' || c == EOF)
	    {
	      *p = '\0';
	      return 0;
	    }
      } while (true);
    }
  while (c != '?' && c != ' ' && c != '\t' && c != '\n' && c != EOF)
    s[++i] = (CHAR) (c = getc (fd));
  s[++i] = '\0';
  if (c == EOF)
    return (-1);
  if (s[0] == '!' || s[0] == ';' || i < 3)
    {
      while (c != '\n' && c != EOF)
	c = getc (fd);
      return (0);
    }
  if (s[4] == 'o')
    *mv = ((side == black) ? LONGBLACKCASTLE : LONGWHITECASTLE);
  else if (s[0] == 'o')
    *mv = ((side == black) ? BLACKCASTLE : WHITECASTLE);
  else
    {
      c1 = s[0] - 'a';
      r1 = s[1] - '1';
      c2 = s[2] - 'a';
      r2 = s[3] - '1';
      *mv = (locn (r1, c1) << 8) | locn (r2, c2);
    }
  if (c == '?')
    {				/* Bad move, not for the program to play */
      *mv |= 0x8000;		/* Flag it ! */
      c = getc (fd);
    }
  return (1);
}
