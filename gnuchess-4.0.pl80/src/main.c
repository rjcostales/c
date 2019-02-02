/*
 * main.c - C source for GNU CHESS
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
#include "version.h"
#include "ttable.h"
#include <signal.h>
#if defined Think_C && defined Window_Events
#include <console.h>
#endif
CHAR *ColorStr[2];
CHAR *CP[CPSIZE];

#ifdef BINBOOK
extern CHAR *binbookfile;
#endif
extern CHAR *bookfile;
UTSHORT ETABLE = DEFETABLE;
CHAR savefile[128] = "";
CHAR listfile[128] = "";
#ifdef HISTORY
UTSHORT history[32768];
#endif
tshort svalue[64];
struct leaf Tree[TREE], *root;
SHORT TrPnt[MAXDEPTH+1];
SHORT PieceList[2][64], PawnCnt[2][8];
SHORT castld[2], Mvboard[64];
struct flags flag;
SHORT opponent, computer, dither;
tshort  WAwindow, WBwindow, BAwindow, BBwindow;
SHORT  INCscore;
long ResponseTime, ExtraTime, MaxResponseTime, et, et0, time0, ft;
unsigned long GenCnt, NodeCnt, ETnodes, EvalNodes;
long replus, reminus;
tshort HashDepth = HASHDEPTH, HashMoveLimit = HASHMOVELIMIT;
SHORT player;
struct GameRec GameList[MAXMOVES + MAXDEPTH];
SHORT Sdepth, Game50, MaxSearchDepth;
SHORT GameCnt = 0;
SHORT epsquare;
tshort  contempt, xwndw;
int Book;
struct TimeControlRec TimeControl;
int TCadd = 0;
SHORT TCflag, TCmoves, TCminutes, TCseconds, OperatorTime;
SHORT XCmoves[3], XCminutes[3], XCseconds[3],XCadd[3], XC, XCmore;
const SHORT otherside[3] =
{black, white, neutral};
SHORT hint;
SHORT TOflag;		/* force search re-init if we backup search */

SHORT mtl[2], pmtl[2], hung[2];
SHORT Pindex[64];
SHORT PieceCnt[2];
SHORT FROMsquare, TOsquare;
SHORT HasKnight[2], HasBishop[2], HasRook[2], HasQueen[2];
SHORT ChkFlag[MAXDEPTH], CptrFlag[MAXDEPTH], PawnThreat[MAXDEPTH];
SHORT QueenCheck[MAXDEPTH]; /* tom@izf.tno.nl */
SHORT NMoves[MAXDEPTH]; /* tom@izf.tno.nl */
SHORT Threat[MAXDEPTH]; /* tom@izf.tno.nl */
SHORT ThreatSave[MAXDEPTH]; /* tom@izf.tno.nl */
SHORT Pscore[MAXDEPTH], Tscore[MAXDEPTH];
const SHORT qrook[3] =
{0, 56, 0};
const SHORT krook[3] =
{7, 63, 0};
const SHORT kingP[3] =
{4, 60, 0};
const SHORT rank7[3] =
{6, 1, 0};
const SHORT sweep[8] =
{false, false, false, true, true, true, false, false};
UTSHORT killr0[MAXDEPTH+1], killr1[MAXDEPTH+1], killr2[MAXDEPTH+1];
UTSHORT PV, SwagHt, Swag0, Swag1, Swag2, Swag4, sidebit;
#ifdef KILLT
tshort killt[0x4000];
#endif
const SHORT value[7] =
{0, valueP, valueN, valueB, valueR, valueQ, valueK};
const SHORT control[7] =
{0, ctlP, ctlN, ctlB, ctlR, ctlQ, ctlK};
SHORT stage, stage2, Developed[2];
unsigned int starttime;
SHORT ahead = true, hash = true;
SHORT PCRASH, PCENTER;

#if defined CHESSTOOL || defined XBOARD
void
TerminateChess (int sig)
{
  ExitChess ();
}

#endif
int timeopp[MINGAMEIN], timecomp[MINGAMEIN];
int compptr, oppptr;
inline void
TimeCalc ()
{
/* adjust number of moves remaining in gamein games */
  int increment = 0;
  int topsum = 0;
  int tcompsum = 0;
  int me,him;
  int i;
/* dont do anything til you have enough numbers */
  if (GameCnt < (MINGAMEIN * 2)) return;
/* calculate average time in sec for last MINGAMEIN moves */
  for (i = 0; i < MINGAMEIN; i++)
    {
      tcompsum += timecomp[i];
      topsum += timeopp[i];
    }
  topsum /= (100 * MINGAMEIN);
  tcompsum /= (100 * MINGAMEIN);
/* if I have less time than opponent add another move */
	me = TimeControl.clock[computer]/100; 
	him = TimeControl.clock[opponent]/100;
	if(me < him) increment += 2;
/* if I am > 60 sec behind increment or if I am less and in last 2 min */
	if((him - me) > 60 || (me<him && me < 120))increment++;
/* if I am losing more time with each move add another */
  if ( ((me - him) > 60) && tcompsum < topsum) increment--;
  if ( tcompsum > topsum) increment +=2;
/* if I am doing really well use more time per move */
  else if (me > him && tcompsum < topsum) increment = -1;
  if (me > (him+60) ) increment = -1;
  TimeControl.moves[computer] += increment;
/* but dont let moves go below MINMOVES */
  if (TimeControl.moves[computer] < MINMOVES )TimeControl.moves[computer] = MINMOVES;
}

/* hmm.... shouldn`t main be moved to the interface routines */
int
main (int argc, CHAR **argv)
{
  CHAR *xwin = 0;
  CHAR *Lang = NULL;
#if defined Think_C && defined Window_Events
	if (StillDown()) /* fake command line by holding mouse btn */
  		argc = ccommand(&argv);
#endif
  gsrand (starttime = ((unsigned int) time ((time_t *) 0)));	/* init urand */
#ifdef ttblsz
  ttblsize = ttblsz;
  rehash = -1;
#endif /* ttblsz */
  if (argc > 2)
    {
      if (argv[1][0] == '-' && argv[1][1] == 'L')
	{
	  Lang = argv[2];
	  argv += 2;
	  argc -= 2;
	}
    }
  InitConst (Lang);
  ColorStr[0] = CP[118];
  ColorStr[1] = CP[119];

  while (argc > 1 && ((argv[1][0] == '-') || (argv[1][0] == '+')))
    {
      switch (argv[1][1])
	{
	case 'a':
	  ahead = ((argv[1][0] == '-') ? false : true);
	  break;
	case 'b':
	  argv++;
	  argc--;
	  if (argc > 1)
	    {
	      bookfile = argv[1];
#ifdef notdef
#ifdef BINBOOK
	      binbookfile = NULL;
#endif
#endif
	    }
	  break;
#ifdef BINBOOK
	case 'B':
	  argv++;
	  argc--;
	  if (argc > 1)
	    binbookfile = argv[1];
	  break;
#endif
#ifdef CACHE
	case 'C':
		argc--;
                argv++;
                if(argc > 1)ETABLE = atoi(argv[1]);
		break;
#endif
        case 'N': /* tom@izf.tno.nl */
          flag.nonull = ((argv[1][0] == '-') ? true : false);
          break;
        case 'V': /* tom@izf.tno.nl */
          flag.verydeep = ((argv[1][0] == '-') ? false : true);
          break;
        case 'D': /* tom@izf.tno.nl */
          flag.deepnull = ((argv[1][0] == '-') ? false : true);
          break;
        case 'p': /* tom@izf.tno.nl */
          flag.pvs = ((argv[1][0] == '-') ? false : true);
          break;
        case 'F': /* tom@izf.tno.nl */
          flag.noscore = ((argv[1][0] == '-') ? false : true);
          break;
        case 'e': /* tom@izf.tno.nl */
          flag.neweval = ((argv[1][0] == '-') ? false : true);
          break;
        case 'E': /* tom@izf.tno.nl */
          flag.threat = ((argv[1][0] == '-') ? false : true);
          break;
	case 'h':
	  hash = ((argv[1][0] == '-') ? false : true);
	  break;
	case 's':
	  argc--;
	  argv++;
	  if (argc > 1)
	    strcpy (savefile, argv[1]);
	  break;
	case 'l':
	  argc--;
	  argv++;
	  if (argc > 1)
	    strcpy (listfile, argv[1]);
	  break;
#ifndef GDBM
	case 'S':
	  argc--;
	  argv++;
	  if(argc > 1)booksize = atoi(argv[1]);
	  break;
#endif
	case 'P':
	  argc--;
	  argv++;
	  if(argc > 1)bookmaxply = atoi(argv[1]);
	  break;

#if ttblsz
	case 'r':
	  if (argc > 2)
	    rehash = atoi (argv[2]);
	  argc--;
	  argv++;
	  if (rehash > MAXrehash)
	    rehash = MAXrehash;
	  break;
	case 'T':
	  if (argc > 2)
	    ttblsize = atoi (argv[2]);
	  argc--;
	  argv++;
	  if ((ttblsize <= MINTTABLE)) ttblsize = (MINTTABLE)+1;
	  break;
	case 'H':
	  if (argc > 2)
	    HashDepth = atoi (argv[2]);
	  argc--;
	  argv++;
	  break;
#ifdef NEWAGE
	case 'A':
	  if (argc > 2)
	    newage = atoi (argv[2]);
	  argc--;
	  argv++;
	  break;
#endif
#ifdef HASHFILE
	case 't':		/* create or test persistent transposition */
	  TestHashFile();
	  return 0;
	case 'c':		/* create or test persistent transposition table */
		{
                long filesz = 0L;
                        filesz = atol (argv[2]);
                if (filesz < 0)
                        filesz = 0;
                if (filesz < 24)
                        filesz = (1 << filesz);
                CreateHashFile(filesz);
                return (0);
                }
#endif /* HASHFILE */
#endif /* ttblsz */
	case 'x':
	  xwin = &argv[1][2];
	  break;
	case 'v':
	  fprintf (stderr, CP[102], version, patchlevel);
	  exit (1);
	default:
	  fprintf (stderr, CP[113]);
	  exit (1);
	}
      argv++;
      argc--;
    }
  XC = 0;
  MaxResponseTime = 0;
#if defined CHESSTOOL || defined XBOARD
  signal (SIGTERM, TerminateChess);
  XCmoves[0] = 40;
  XCminutes[0] = 5;
  XCseconds[0] = 0;
  XCadd[0] = 0;
  XC = 1;
  TCflag = true;
#else
  TCflag = false;
#endif
  if (argc == 2)
    {
      CHAR *p;

      MaxResponseTime = 100L * strtol (argv[1], &p, 10);
      if (*p == ':')
	MaxResponseTime = 60L * MaxResponseTime +
	  100L * strtol (++p, (CHAR **) NULL, 10);
      XCadd[0] = 0;
      XCmoves[0] = 0;
      XCminutes[0] = 0;
      XCseconds[0] = 0;
    }
  if (argc >= 3)
    {
      CHAR *p;
      if (argc > 9)
	{
	  printf ("%s\n", CP[220]);
	  exit (1);
	}
      XCmoves[0] = atoi (argv[1]);
      XCminutes[0] = (SHORT)strtol (argv[2], &p, 10);
      if (*p == ':')
	XCseconds[0] = (SHORT)strtol (p + 1, (CHAR **) NULL, 10);
      else
	XCseconds[0] = 0;
      XC = 1;
      argc -= 3;
      argv += 3;
      while (argc > 1)
	{
	  XCmoves[XC] = atoi (argv[0]);
	  XCminutes[XC] = (SHORT)strtol (argv[1], &p, 10);
	  if (*p == ':')
	    XCseconds[XC] = (SHORT)strtol (p + 1, (CHAR **) NULL, 10);
	  else
	    XCseconds[XC] = 0;
	  if (XCmoves[XC] && (XCminutes[XC] || XCseconds[XC]))
	    XC++;
	  else
	    {
	      printf (CP[220]);
	      exit (1);
	    }
	  argc -= 2;
	  argv += 2;
	}
      if (argc)
	{
	  printf ("%s\n", CP[220]);
	  exit (1);
	}
    }
#ifdef Think_C	/* Allocate memory for "huge" static arrays */
  Initialize_mem();
#endif
  Initialize ();
  Initialize_dist ();
  Initialize_moves ();
  NewGame ();

  flag.easy = ahead;
  flag.hash = hash;
  if (xwin)
    xwndw = atoi (xwin);

  OpenHashFile(); /* Open persistent transposition table */
  while (!(flag.quit))
    {
      oppptr = (oppptr + 1) % MINGAMEIN;
      if (flag.bothsides && !flag.mate)
	SelectMove (opponent, 1);
      else
	InputCommand ();
      if (opponent == black)
	if (flag.gamein || TCadd)
	  {
	    TimeCalc ();
	  }
	else if (TimeControl.moves[opponent] == 0 && TCflag)
	  {
	    SetTimeControl ();
	  }

      compptr = (compptr + 1) % MINGAMEIN;
      if (!(flag.quit || flag.mate || flag.force))
	{
	  SelectMove (computer, 1);
	  if (computer == black)
	    if (flag.gamein)
	      {
		TimeCalc ();
	      }
	    else if (TimeControl.moves[computer] == 0 && TCflag)
	      {
		SetTimeControl ();
	      }
	}
    }
        CloseHashFile();

  ExitChess ();
  return (0);
}
