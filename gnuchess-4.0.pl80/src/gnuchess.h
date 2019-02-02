/*
 * gnuchess.h - Header file for GNU CHESS
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

#if SIZEOF_LONG == 8
#define LONG64 1
#endif

#ifdef WIN32
#define HASHFILE "gnuchess.has"
#define BINBOOK "gnuchess.dat"
#define BINECO "gnuchess.eco"
#define PGNECO "eco.pgn"
#define LANGFILE "gnuchess.lan"
#define SRCLANGFILE "..\\misc\\gnuchess.lang"
#define stat _stat
#define close _close
#define lseek _lseek
#define open _open
#define read _read
#define write _write
#endif

#if !defined(__STDC__) && !defined(MSDOS)
#define const
#endif

#ifndef MSDOS
#define huge
#endif
#include <stdio.h>
#ifdef HAVE_MEMSET
#include <memory.h>
#endif
#ifdef SIGNCHAR
typedef signed char CHAR;
#else
typedef char CHAR;
#endif
typedef unsigned char UCHAR;
typedef short tshort;
typedef unsigned short utshort;
#ifdef USEINT
typedef  int SHORT;
typedef  unsigned int UTSHORT;
#else
typedef unsigned short UTSHORT;
typedef short SHORT;
#endif


#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_END 2
#endif

#ifdef DEBUG
void
ShowDBLine (CHAR *, SHORT, SHORT,
	    SHORT, SHORT, SHORT,
	    utshort *);
     extern FILE *debugfd;
     extern SHORT debuglevel;

#endif /* DEBUG */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#if defined(MSDOS) || defined(WIN32)
#define HAVE_TIME_H
#include <malloc.h>
#define RWA_ACC "r+b"
#define WA_ACC "w+b"
#define printz printf
#define scanz scanf
#else
#define RWA_ACC "r+"
#define WA_ACC "w+"
#ifndef Think_C
#include <sys/param.h>
#include <sys/types.h>
#ifndef AMIGADOS
#include <sys/times.h>
#endif
#endif /* Think_C */
#ifdef NONDSP
#define printz printf
#define scanz scanf
#else
#include <curses.h>
#define scanz fflush(stdout),scanw
#define printz printw
#endif
#endif /* MSDOS */
 
#ifdef HAVE_TIME_H
#include <time.h>
#endif

 
/* Magic for those compilers that can't handle "huge" static structures... */
/* Be sure to also invoke Initialize_mem() if you use this */
#ifdef Think_C
#define ALLOCATE(ms) if (!(_##ms=malloc(sizeof(ms)))) \
        {perror(#ms);exit(1);}\
        else printf("Allocate %s @ %ld, %ld bytes.\n", #ms, _##ms, sizeof(ms));
/* #define etab (*_etab) */
#define nextpos (*_nextpos)
#define nextdir (*_nextdir)
#define history (*_history)
#define Tree (*_Tree)
#endif /* Think_C */

/* Piece values */
#define valueP 100
#define valueN 350
#define valueB 355
#define valueR 550
#define valueQ 1100
#define valueK 1200
/* masks into upper 8 bits of ataks array */
#define ctlP 0x4000
#define ctlN 0x2800
#define ctlB 0x1800
#define ctlR 0x0400
#define ctlQ 0x0200
#define ctlK 0x0100
#define ctlBQ 0x1200
#define ctlBN 0x0800
#define ctlRQ 0x0600
#define ctlNN 0x2000
/* attack functions */
#define Patak(c, u) (atak[c][u] > ctlP)
#define Anyatak(c, u) (atak[c][u] > 0)
/* distance function */
#define taxicab(a,b) taxidata[a][b]
/* king positions */
#define wking PieceList[white][0]
#define bking PieceList[black][0]
#define EnemyKing PieceList[c2][0]
/* constants */
/* castle moves */
#define BLACKCASTLE	0x3C3E
#define WHITECASTLE	0x0406
#define LONGBLACKCASTLE	0x3C3A
#define LONGWHITECASTLE	0x0402
/* truth values */
#define false 0
#define true 1
/* colors */
#define white 0
#define black 1
#define neutral 2
/* piece code defines */
#define no_piece 0
#define pawn 1
#define knight 2
#define bishop 3
#define rook 4
#define queen 5
#define king 6
#define bpawn 7
/* node flags */
#define pmask 0x0007
#define promote 0x0008
#define cstlmask 0x0010
#define epmask 0x0020
#define exact 0x0040
#define pwnthrt 0x0080
#define check 0x0100
#define capture 0x0200
#define draw 0x0400
#define book 0x1000
/* move symbols */
#define pxx (CP[2])
#define qxx (CP[1])
#define rxx (CP[4])
#define cxx (CP[3])
/* for everything that can't use the above */
#define Qxx " pnbrqk"
#define Pxx " PNBRQK"
#define Cxx "abcdefgh"
#define Rxx "12345678"
/***************************************************************************/
/***************** Table limits ********************************************/
#define TREE 1500		/* max number of tree entries */
#define MAXDEPTH 35		/* max depth a search can be carried */
#define MINDEPTH 2		/* min search depth =1 (no hint), >1 hint */
#define MAXMOVES 400		/* max number of half moves in a game */
#define NMOVELIMIT 20
#define XCLIMIT 5		/* number of different Time inputs */
#ifndef BOOKSIZE
#if defined MSDOS
#define BOOKSIZE 10000		/* Number of unique position/move combinations allowed */
#else
#define BOOKSIZE 250000		/* Number of unique position/move combinations allowed */
#endif
#endif
#define CPSIZE 235		/* size of lang file max */
#define DEFETABLE 18001		/* static eval cache */
#ifdef ECO
#define ECOCNT  5
#endif
/***************** tuning paramaters **********************************************/
#define NMOVEINIT { 75,75,60,55,55,55,55,58,55,56,60,64,68,72,76,80,84,88,90,94,98,98,98,98,99,99,99}
#define MINGAMEIN 4
#define MINMOVES 15
#define FBEYOND 5
#define SBEYOND 9
#define TBEYOND 11
#define HASHDEPTH 2		/* depth above which to use HashFile */
#define HASHMOVELIMIT 40	/* Use HashFile only for this many moves */
#define PTVALUE 0	        /* material value below which pawn threats at 5 & 3 are used */
#define ZDELTA 40		/* score delta per ply to cause extra time to be given */
#define QBLOCK false		/* if true cache quiescent positions */
#define BESTDELTA 90
/* about 1/2 second worth of nodes for your machine */
#ifdef Think_C
#define ZNODES 500              /* check the time every ZNODES positions */
#else /* Think_C */
#define ZNODES 4000             /* check the time every ZNODES positions */
#endif /* Think_C */
#define MAXTCCOUNTX  5		/* max number of time clicks per search to complete ply*/
#define MAXTCCOUNTR 6		/* max number of time clicks per search extensions*/
#define HISTORYLIM 4096		/* Max value of history killer */
#define WAWNDW 50		/* alpha window when computer white*/
#define WBWNDW 50		/* beta window when computer white*/
#define BAWNDW 50		/* alpha window when computer black*/
#define BBWNDW 50		/* beta window when computer black*/
#define BXWNDW 50		/* window to force position scoring at lower */
#define WXWNDW 50		/* window to force position scoring at lower */
#ifdef SMALLRANDY
#define DITHER 2		/* max amount random can alter a pos value */
#else
#define DITHER 5
#endif
#define BBONUS 2		/* points per stage value of B increases */
#define RBONUS 6		/* points per stage value of R increases */
#define PCRASHS	5
#define PCRASHV 5
#define PCENTERS 5
#define PCENTERV 5
#define KINGPOSLIMIT  -1	/* King positional scoring limit */
#define KINGSAFETY  40
#define NULLMOVELIM valueQ	/* below this total material on board don't use null move */
#define DEPTHMARGIN 2
#define THRSTAGE    6
#define CHECKSTAGE  8

/************************* parameters for Opening Book *********************************/
#define BOOKFAIL 8		/* if no book move found for BOOKFAIL turns stop using book */
#define BOOKMAXPLY 100		/* Max plys to keep in book database */
#define BOOKPOCKET 64
#define BOOKRAND 1000		/* used to select an opening move from a list */
#define BOOKENDPCT 950		/* 5 % chance a BOOKEND will stop the book */
#define DONTUSE -32000		/* flag move as don't use */
/*************************** Book access defines ****************************************/
#define LASTMOVE 0x4000		/* means this is the last move of an opening */
#define BADMOVE 0x8000		/* means this is a bad move in this position */
/****************************************************************************************/

     struct leaf
     {
       	tshort f;
	tshort t;
	tshort  score;
	tshort  reply;
	tshort  width;
       	utshort flags;
     };
     struct GameRec
     {
       utshort gmove;	/* this move */
       tshort score;		/* score after this move */
       tshort depth;		/* search depth this move */
       long time;               /* search time this move */
       tshort piece;		/* piece captured */
       tshort color;		/* color */
       tshort flags;		/* move flags capture, promote, castle */
       tshort Game50;		/* flag for repetition */
       long nodes;		/* nodes searched for this move */
       unsigned long hashkey, hashbd;	/* board key before this move */
       tshort epssq;		/* epssquare before this move */
#ifdef DEBUG40
       UTSHORT d1[7];
#endif
     };
     struct TimeControlRec
     {
       SHORT moves[2];
       long clock[2];
     };

     struct flags
     {
       SHORT mate;		/* the game is over */
       SHORT post;		/* show principle variation */
       SHORT quit;		/* quit/exit */
       SHORT regularstart;	/* did the game start from standard
				 * initial board ? */
       SHORT reverse;		/* reverse board display */
       SHORT bothsides;		/* computer plays both sides */
       SHORT hash;		/* enable/disable transposition table */
       SHORT force;		/* enter moves */
       SHORT easy;		/* disable thinking on opponents time */
       SHORT beep;		/* enable/disable beep */
       SHORT timeout;		/* time to make a move */
       SHORT musttimeout;	/* time to make a move */
       SHORT back;		/* time to make a move */
       SHORT rcptr;		/* enable/disable recapture heuristics */
       SHORT rv;		/* reverse video */
       SHORT stars;		/* add stars to uxdsp screen */
       SHORT coords;		/* add coords to visual screen */
       SHORT shade;
       SHORT material;		/* draw on lack of material */
       SHORT illegal;		/* illegal position */
       SHORT onemove;		/* timing is onemove */
       SHORT gamein;		/* timing is gamein */
       SHORT autolist;		/* list games */
       SHORT threat;            /* Enable threats, tom@izf.tno.nl */
       SHORT nonull;            /* Disable nullmoves, tom@izf.tno.nl */
       SHORT deepnull;          /* Enable deep nullmoves, tom@izf.tno.nl */
       SHORT verydeep;          /* Enable very deep nullmoves, tom@izf.tno.nl */
       SHORT pvs;               /* Enable PVS, tom@izf.tno.nl */
       SHORT neweval;           /* Enable new eval, tom@izf.tno.nl */
       SHORT noscore;           /* Enable no score heuristic, tom@izf.tno.nl */
#ifdef DEBUG
	SHORT nott;
	SHORT noft;
	SHORT nocache;
#endif
     };
#ifdef DEBUG
     extern FILE *debugfile;

#endif /* DEBUG */
#ifdef HISTORY
     extern UTSHORT history[32768];
#endif
     extern CHAR *ColorStr[2];
     extern UTSHORT MV[MAXDEPTH+1];
     extern SHORT PCRASH,PCENTER;
     extern int MSCORE;
     extern int mycnt1, mycnt2;
     extern SHORT ahead;
     extern SHORT chesstool;
     extern struct leaf Tree[TREE], *root,rootnode;
     extern CHAR savefile[], listfile[];
     extern SHORT TrPnt[];
     extern SHORT board[], color[];
     extern SHORT PieceList[2][64], PawnCnt[2][8];
     extern SHORT castld[], Mvboard[];
     extern tshort svalue[64];
     extern struct flags flag;
     extern SHORT opponent, computer, INCscore;
     extern tshort WAwindow, BAwindow, WBwindow, BBwindow;
     extern SHORT dither, player;
     extern tshort xwndw, contempt;
     extern SHORT epsquare;
     extern long ResponseTime, ExtraTime, MaxResponseTime, et, et0, time0, ft;
     extern long reminus, replus;
     extern unsigned long GenCnt, NodeCnt, ETnodes, EvalNodes;
     extern tshort HashDepth, HashMoveLimit;
     extern struct GameRec GameList[];
     extern SHORT GameCnt, Game50;
     extern SHORT Sdepth, MaxSearchDepth;
     extern int Book;
     extern struct TimeControlRec TimeControl;
     extern int TCadd;
     extern SHORT TCflag, TCmoves, TCminutes, TCseconds, OperatorTime;
     extern int timecomp[MINGAMEIN], timeopp[MINGAMEIN];
     extern int compptr,oppptr;
     extern SHORT XCmore,XCadd[], XCmoves[], XCminutes[], XCseconds[], XC;
     extern const SHORT otherside[];
     extern const SHORT Stboard[];
     extern const SHORT Stcolor[];
     extern SHORT hint;
     extern SHORT TOflag;
     extern SHORT stage, stage2, Developed[];
     extern SHORT ChkFlag[], CptrFlag[], PawnThreat[];
     extern SHORT QueenCheck[];  /* tom@izf.tno.nl */
     extern SHORT NMoves[];  /* tom@izf.tno.nl */
     extern SHORT Threat[];  /* tom@izf.tno.nl */
     extern SHORT ThreatSave[];  /* tom@izf.tno.nl */
     extern SHORT Pscore[], Tscore[];
     extern SHORT mtl[], pmtl[], hung[], emtl[];
     extern SHORT Pindex[];
     extern SHORT PieceCnt[];
     extern SHORT FROMsquare, TOsquare;
     extern SHORT HasKnight[], HasBishop[], HasRook[], HasQueen[];
     extern const SHORT qrook[];
     extern const SHORT krook[];
     extern const SHORT kingP[];
     extern const SHORT rank7[];
     extern const SHORT sweep[];
     extern const SHORT epmove1[], epmove2[];
     extern UTSHORT killr0[], killr1[];
     extern UTSHORT killr2[], killr3[];
     extern UTSHORT PV, SwagHt, Swag0, Swag1, Swag2, Swag3, Swag4, sidebit;
     extern tshort killt[0x4000];
     extern SHORT mtl[2], pmtl[2], hung[2];
     extern const SHORT value[];
     extern const SHORT control[];
     extern UCHAR nextpos[8][64][64];
     extern UCHAR nextdir[8][64][64];
     extern const SHORT ptype[2][8];
     extern unsigned int starttime;
     extern SHORT distdata[64][64], taxidata[64][64];
     extern CHAR mvstr[5][7];
#ifndef AMIGADOS
     extern union U admin;
#endif
     extern UTSHORT bookmaxply;
     extern unsigned long bookcount;
     extern unsigned long booksize;
     extern CHAR *CP[];
#ifdef QUIETBACKGROUND
     extern SHORT background;
#endif /* QUIETBACKGROUND */

     extern CHAR *DRAW;

#define distance(a,b) distdata[a][b]
#define row(a) ((a) >> 3)
#define column(a) ((a) & 7)
#define locn(a,b) (((a) << 3) | (b))
     extern SHORT distdata[64][64];

/* init external functions */
     extern void InitConst (CHAR *lang);
     extern void Initialize_dist (void);
     extern void NewGame (void);
     extern int parse (FILE * fd, UTSHORT *mv, SHORT side, CHAR *opening);
     extern void GetOpenings (void);
     extern int OpeningBook (SHORT *hint, SHORT side);
     extern int GOpeningBook (SHORT *hint, SHORT side, CHAR *mv);
     extern void SelectMove (SHORT side, SHORT iop);
     extern int
      search (SHORT side,
	       SHORT ply,
	       SHORT depth,
               SHORT ext,
	       SHORT alpha,
	       SHORT beta,
	       UTSHORT *bstline,
	       SHORT *rpt,
	       SHORT SAVEHT,
	       SHORT didnull );
     extern void Initialize_moves (void);
     extern void MoveList (SHORT side, SHORT ply);
     extern void CaptureList (SHORT side, SHORT ply);
     extern int castle (SHORT side, SHORT kf, SHORT kt, SHORT iop);
     extern void ataks (SHORT side, SHORT *a);
     extern void
      MakeMove (SHORT side,
		 struct leaf * node,
		 SHORT *tempb,
		 SHORT *tempc,
		 SHORT *tempsf,
		 SHORT *tempst);
     extern void
      UnmakeMove (SHORT side,
		   struct leaf * node,
		   SHORT *tempb,
		   SHORT *tempc,
		   SHORT *tempsf,
		   SHORT *tempst);
     extern void InitializeStats (void);
     extern int
      evaluate (SHORT side,
		 SHORT ply,
		 SHORT depth,
		 SHORT ext,
		 SHORT alpha,
		 SHORT beta,
	         SHORT *terminal,
		 SHORT *InChk);
     extern SHORT ScorePosition (SHORT side);
     extern void ExaminePosition (void);
     extern void UpdateWeights (void);
     extern void Initialize (void);
     extern void InputCommand (void);
     extern void ExitChess (void);
     extern void ClrScreen (void);
     extern void SetTimeControl (void);
     extern void SelectLevel (CHAR *sx);
     extern void
      UpdateDisplay (SHORT f,
		      SHORT t,
		      SHORT flag,
		      SHORT iscastle);
     extern void ElapsedTime (SHORT iop);
     extern void ShowSidetoMove (void);
     extern void SearchStartStuff (SHORT side);
     extern void ShowDepth (CHAR ch);
     extern void TerminateSearch (int);
     extern void
      ShowResults (SHORT score,
		    UTSHORT *bstline,
		    CHAR ch);
     extern void PromptForMove (void);
     extern void SetupBoard (void);
     extern void algbr (SHORT f, SHORT t, SHORT flag);
     extern void OutputMove (SHORT score);
     extern void ShowCurrentMove (SHORT pnt, SHORT f, SHORT t);
     extern void ListGame (void);
     extern void ShowMessage (CHAR *s);
     extern void ClrScreen (void);
     extern void gotoXY (SHORT x, SHORT y);
     extern void ClrEoln (void);
     extern void DrawPiece (SHORT sq);
     extern void UpdateClocks (void);
     extern void DoDebug (void);
     extern void DoTable (SHORT table[64]);
     extern void ShowPostnValues (void);
     extern void ChangeXwindow (void);
     extern void SetContempt (void);
     extern void ChangeHashDepth (void);
     extern void ChangeBetaWindow (void);
     extern void ChangeAlphaWindow (void);
     extern void GiveHint (void);
     extern void ShowPrompt (void);
     extern void EditBoard (void);
     extern void help (void);
     extern void ChangeSearchDepth (void);
     extern void skip (void);
     extern void skipb (void);
     extern void EnPassant (SHORT xside, SHORT f, SHORT t, SHORT iop);
     extern void ShowNodeCnt (long int NodeCnt);
     extern void ShowLine (UTSHORT *bstline);
     extern int pick (SHORT p1, SHORT p2);
     extern int VerifyMove (CHAR *s, SHORT inp, UTSHORT *mv);
     extern void VMoveList (SHORT side, register SHORT ply);
#if !defined(AMIGADOS)
     extern struct gdxadmin B;
#endif

#if defined(AMIGADOS)  &&  !defined(XBOARD)
#define exit(retcode) CleanExit(retcode)
#endif
/* Take care of MEMSET once and for all... */
#if (!defined(HAVE_MEMSET) || defined(MSDOS)) && !defined(__GNUC__)
#define memset(s,c,l) {\
  unsigned long j; \
    for (j = 0; j < (l); j++) \
      ((CHAR *)s)[j] = (c);}
#endif /* MEMSET */

