/*
  gnuan.c - Analysis interface for gnuchess.

  Revision: 1991-04-15

  Copyright (C) 1986, 1987, 1988, 1989, 1990 Free Software Foundation, Inc.
  Copyright (c) 1988, 1989, 1990  John Stanback

  This file is part of CHESS.

  CHESS is distributed in the hope that it will be useful, but WITHOUT ANY
  WARRANTY.  No author or distributor accepts responsibility to anyone for
  the consequences of using it or for whether it serves any particular
  purpose or works at all, unless he says so in writing.  Refer to the CHESS
  General Public License for full details.

  Everyone is granted permission to copy, modify and redistribute CHESS, but
  only under the conditions described in the CHESS General Public License.
  A copy of this license is supposed to have been given to you along with
  CHESS so you can know your rights and responsibilities.  It should be in a
  file named COPYING.  Among other things, the copyright notice and this
  notice must be preserved on all copies.
*/

/*
  This is a front end for a chess analysis program.  It takes a file of moves
  in algebraic notation and creates a file containing each move that was
  made, the move it would have made, the score it would give itself after
  making the move it recommended, and the depth it searched for the move.

  This is a modification of nondsp.c.  I have cut out code that was not needed
  for this application such as the help and edit functions.  Most of the
  modifications were done in InputCommand.
*/

/*
  This file will generate two different analysis programs.  One is meant to
  be run as a background process inwhich the in and out files are predefined.
  The other is meant to be run in the foreground where it will prompt you for
  the name of the file with the game in it, the maximum depth the search should
  be conducted to and the time limit per move.  It would be nice to be able to
  give these parameters on the command line, but that would mean changing main
  which is in the gnuchess.c file.

  For each move it will analyse the move until either it has run out of time
  or it has reached the maximum depth.

  To build the version for background processing define BACKGROUND_ANALYSIS
  either at the top of this file, or in compilation.  The files and depth
  used are defined below.  They are MAX_DEPTH, MAX_TIME, OUT_FILE, IN_FILE
  and PROG_FILE.  The PROG_FILE is a file that will be updated as each move is
  analysed so you can check its progress.  This is only updated when running
  the BACKGROUND_ANALYSIS version.  In the version where the filename and
  depth are entered at runtime, the output goes to stdout.
*/

/* #define BACKGROUND_ANALYSIS 1 */

#define MAX_DEPTH  15
#define MAX_TIME   20
#define OUT_FILE   "/pad/gnuan.out"
#define IN_FILE    "/pad/gnuan.in"
#define PROG_FILE  "/pad/gnuan.prog"


#include <ctype.h>
#include <signal.h>
#ifdef MSDOS
#include <dos.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
void TerminateSearch (int), Die (int);
#endif /* MSDOS */

#include "gnuchess.h"

#define pxx " PNBRQK"
#define qxx " pnbrqk"
#define rxx "12345678"
#define cxx "abcdefgh"
#define printz printf
#define scanz scanf

static char mvstr[4][6];
static char* ColorStr[2] = {"White", "Black"};
static long evrate;
static int mycnt1, mycnt2;
static int ahead;

static FILE *fpin;
static FILE *fpout;
char *DRAW;
unsigned short int MV[30];
enum {XBOARD, GNUCHESS, OTHER} InFileType;
char InBuf[256];

#ifdef BACKGROUND_ANALYSIS
static FILE *fpprog;
#endif
static char white_actual_move[20];
static char black_actual_move[20];
static char white_suggest_move[20];
static char black_suggest_move[20];
static int  white_score;
static int  black_score;
static int  white_moving;
static int  current_depth;
static int  current_score;
static int  enable_update_display = 0;


void
Initialize (void)
{
  mycnt1 = mycnt2 = 0;
}

void
ExitChess (void)
{
  ListGame ();
  exit (0);
}

#ifndef MSDOS                           /* never called!!! */
void
Die (int sig)
{
    ExitChess ();
}
#endif /* MSDOS */

void
TerminateSearch (int sig)
{
#ifdef MSDOS
  sig++;                                /* shut up the compiler */
#endif /* MSDOS */
  flag.timeout   = true;
  flag.bothsides = false;
}

void
algbr (short int f, short int t, short int flag)

/*
   Generate move strings in different formats.
*/

{
  int m3p;

  if (f != t)
    {
      /* algebraic notation */
      mvstr[0][0] = cxx[column (f)];
      mvstr[0][1] = rxx[row (f)];
      mvstr[0][2] = cxx[column (t)];
      mvstr[0][3] = rxx[row (t)];
      mvstr[0][4] = mvstr[3][0] = '\0';
      if (((mvstr[1][0] = pxx[board[f]]) == 'P') || (flag & promote))
        {
          if (mvstr[0][0] == mvstr[0][2])       /* pawn did not eat */
            {
              mvstr[2][0] = mvstr[1][0] = mvstr[0][2];  /* to column */
              mvstr[2][1] = mvstr[1][1] = mvstr[0][3];  /* to row */
              m3p = 2;
            }
          else
            /* pawn ate */
            {
              mvstr[2][0] = mvstr[1][0] = mvstr[0][0];  /* from column */
              mvstr[2][1] = mvstr[1][1] = mvstr[0][2];  /* to column */
              mvstr[2][2] = mvstr[0][3];
              m3p = 3;          /* to row */
            }
          mvstr[2][m3p] = mvstr[1][2] = '\0';
          if (flag & promote)
            {
              mvstr[0][4] = mvstr[1][2] = mvstr[2][m3p] = qxx[flag & pmask];
              mvstr[1][3] = mvstr[2][m3p + 1] = mvstr[0][5] = '\0';
            }
        }
      else
        /* not a pawn */
        {
          mvstr[2][0] = mvstr[1][0];
          mvstr[2][1] = mvstr[0][1];
          mvstr[2][2] = mvstr[1][1] = mvstr[0][2];      /* to column */
          mvstr[2][3] = mvstr[1][2] = mvstr[0][3];      /* to row */
          mvstr[2][4] = mvstr[1][3] = '\0';
          strcpy (mvstr[3], mvstr[2]);
          mvstr[3][1] = mvstr[0][0];
          if (flag & cstlmask)
            {
              if (t > f)
                {
                  strcpy (mvstr[1], "o-o");
                  strcpy (mvstr[2], "O-O");
                }
              else
                {
                  strcpy (mvstr[1], "o-o-o");
                  strcpy (mvstr[2], "O-O-O");
                }
            }
        }
    }
  else
    mvstr[0][0] = mvstr[1][0] = mvstr[2][0] = mvstr[3][0] = '\0';
}


int
VerifyMove (char *s, short int iop, unsigned short int *mv)

/*
   Compare the string 's' to the list of legal moves available for the
   opponent. If a match is found, make the move on the board.
*/

{
  static short pnt, tempb, tempc, tempsf, tempst, cnt;
  static struct leaf xnode;
  struct leaf *node;

  *mv = 0;
  if (iop == 2)
    {
      UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
      return (false);
    }
  cnt = 0;
  MoveList (opponent, 2);
  pnt = TrPnt[2];
  while (pnt < TrPnt[3])
    {
      node = &Tree[pnt++];
      algbr (node->f, node->t, (short) node->flags);
      if (strcmp (s, mvstr[0]) == 0 || strcmp (s, mvstr[1]) == 0 ||
          strcmp (s, mvstr[2]) == 0 || strcmp (s, mvstr[3]) == 0)
        {
          cnt++;
          xnode = *node;
        }
    }
  if (cnt == 1)
    {
      MakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst, &INCscore);
      if (SqAtakd (PieceList[opponent][0], computer))
        {
          UnmakeMove (opponent, &xnode, &tempb, &tempc, &tempsf, &tempst);
          printz ("Illegal move\n");
          return (false);
        }
      else
        {
          if (iop == 1)
            return (true);
/*        UpdateDisplay (xnode.f, xnode.t, 0, (short) xnode.flags); */
          if ((board[xnode.t] == pawn)
              || (xnode.flags & capture)
              || (xnode.flags & cstlmask))
            {
              Game50 = GameCnt;
              ZeroRPT ();
            }
          GameList[GameCnt].depth = GameList[GameCnt].score = 0;
          GameList[GameCnt].nodes = 0;
          ElapsedTime (1);
          GameList[GameCnt].time = (short) et;
          TimeControl.clock[opponent] -= et;
          --TimeControl.moves[opponent];
          *mv = (xnode.f << 8) | xnode.t;
          algbr (xnode.f, xnode.t, false);
          return (true);
        }
    }
#ifdef CHESSTOOL
  printz ("Illegal move\n");
#else
  if (cnt > 1)
    ShowMessage ("Ambiguous Move!");
#endif
  return (false);
}

void
help (void)
{
}

void
EditBoard (void)
/*
   Set up a board position. Pieces are entered by typing the piece followed
   by the location. For example, Nf3 will place a knight on square f3.
*/

{
}

void
SetupBoard (void)

/*
   Compatibility with Unix chess and the nchesstool.
   Set up a board position. Eight lines of eight characters are used
   to setup the board. a8-h8 is the first line.
   Black pieces are  represented  by  uppercase characters.
*/

{
}

void
ShowDepth (char ch)
{
#ifdef MSDOS
  ch++;                                 /* shut up the compiler */
#endif /* MSDOS */
}

void
ShowResults (short int score, unsigned short int *bstline, char ch)
{
	int i;
    current_score = score;
    current_depth = Sdepth;
      for (i = 1; bstline[i] > 0; i++) { MV[i] = bstline[i];} MV[i] = 0;
}

void
SearchStartStuff (short int side)
{ 	
  signal (SIGINT, TerminateSearch);
#ifndef MSDOS
  signal (SIGQUIT, TerminateSearch);
#endif /* MSDOS */
  if (flag.post)
    {
      fprintf (stderr, "\nMove# %d    Target= %ld    Clock: %ld\n",
               TCmoves - TimeControl.moves[side] + 1,
               ResponseTime, TimeControl.clock[side]);
    }
}

void
OutputMove (void)
{
    if(white_moving) {
        strcpy(white_suggest_move, mvstr[0]);
        white_score = current_score;
    } else {
        strcpy(black_suggest_move, mvstr[0]);
        black_score = current_score;
    }
}

void
ElapsedTime (short int iop)
/*
   Determine the time that has passed since the search was started. If the
   elapsed time exceeds the target (ResponseTime+ExtraTime) then set timeout
   to true which will terminate the search.
*/
{
  if (ahead)
    {
#ifndef MSDOS
      long nchar;
      ioctl(0, FIONREAD, &nchar);
      if (nchar)
#else
      if (kbhit())
#endif /* MSDOS */
        {
          flag.timeout = true;
          flag.bothsides = false;
        }
    }
  et = time ((long *) 0) - time0;
  if (et < 0)
    et = 0;
  ETnodes += 50;
  if (et > et0 || iop == 1)
    {
      if (et > ResponseTime + ExtraTime && Sdepth > 1)
        flag.timeout = true;
      et0 = et;
      if (iop == 1)
        {
          time0 = time ((long *) 0);
          et0 = 0;
        }
      if (et > 0)
        /* evrate used to be Nodes / cputime I dont` know why */
        evrate = NodeCnt / (et + ft);
      else
        evrate = 0;
      ETnodes = NodeCnt + 50;
    }
}

void
SetTimeControl (void)
{
  if (TCflag)
    {
      TimeControl.moves[white] = TimeControl.moves[black] = TCmoves;
      TimeControl.clock[white] = TimeControl.clock[black] = 60L * TCminutes;
    }
  else
    {
      TimeControl.moves[white] = TimeControl.moves[black] = 0;
      TimeControl.clock[white] = TimeControl.clock[black] = 0;
      Level = 60L * TCminutes;
    }
  et = 0;
  ElapsedTime (1);
}

void
ClrScreen (void)
{
#ifndef CHESSTOOL
  printz ("\n");
#endif
}

void
UpdateDisplay (short int f, short int t, short int redraw, short int isspec)
{
#ifndef CHESSTOOL
  short r, c, l;
  if(!enable_update_display) return;
  if (redraw)
    {
      fprintf(fpout, "\n");
      for (r = 7; r >= 0; r--)
        {
          for (c = 0; c <= 7; c++)
            {
              if (flag.reverse)
                l = locn (7 - r, 7 - c);
              else
                l = locn (r, c);
              if (color[l] == neutral)
                fprintf (fpout, " -");
              else if (color[l] == white)
                fprintf (fpout, " %c", qxx[board[l]]);
              else
                fprintf (fpout, " %c", pxx[board[l]]);
            }
          fprintf(fpout, "\n");
        }
      fprintf(fpout, "\n");
    }
#endif /* CHESSTOOL */
#ifdef MSDOS
  f++; t++; isspec++;                   /* shut up the compiler */
#endif /* MSDOS */
}

void
GetGame (void)
{
}

void
SaveGame (void)
{
}

void
ListGame (void)
{
}



void
Undo (void)
/*
   Undo the most recent half-move.
*/

{
  short f, t;
  f = GameList[GameCnt].gmove >> 8;
  t = GameList[GameCnt].gmove & 0xFF;
  if (board[t] == king && distance (t, f) > 1) (void) castle (GameList[GameCnt].color, f, t, 2);
  else
    {
      /* Check for promotion: */
      if(GameList[GameCnt].flags & promote){board[t] = pawn; }
      board[f] = board[t];
      color[f] = color[t];
      board[t] = GameList[GameCnt].piece;
      color[t] = GameList[GameCnt].color;
      if (color[t] != neutral) Mvboard[t]--;
      Mvboard[f]--;
    }
  if(GameList[GameCnt].flags & epmask)EnPassant(color[t],f,t,2);
  if (TCflag) ++TimeControl.moves[color[f]];
  GameCnt--;
  computer = otherside[computer];
  opponent = otherside[opponent];
  flag.mate = false;
  Sdepth = 0;
  InitializeStats ();
}

void
ShowMessage (char *s)
{
  fprintf (stderr, "%s\n", s);
}

void
ShowSidetoMove (void)
{
}

void
PromptForMove (void)
{
#ifndef CHESSTOOL
  printz ("\nYour move is? ");
#endif /* CHESSTOOL */
}


void
ShowCurrentMove (short int pnt, short int f, short int t)
{
#ifdef MSDOS
  f++; t++; pnt++;                      /* shut up the compiler */
#endif /* MSDOS */
}

void
ChangeAlphaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Awindow);
}

void
ChangeBetaWindow (void)
{
  printz ("window: ");
  scanz ("%hd", &Bwindow);
}

void
GiveHint (void)
{
  algbr ((short) (hint >> 8), (short) (hint & 0xFF), false);
  fprintf (stderr, "Hint: %s\n", mvstr[0]);
}

void
SelectLevel (void)
{
}

void
ChangeSearchDepth (void)
{
  printz ("depth= ");
  scanz ("%hd", &MaxSearchDepth);
}

void
SetContempt (void)
{
  printz ("contempt= ");
  scanz ("%hd", &contempt);
}

void
ChangeXwindow (void)
{
  printz ("xwndw= ");
  scanz ("%hd", &xwndw);
}

void
TestSpeed(void (*f) (short int side, short int ply))
{
  short i;
  long cnt, rate, t1, t2;

  t1 = time (0);
  for (i = 0; i < 10000; i++)
    {
      f (opponent, 2);
    }
  t2 = time (0);
  cnt = 10000L * (TrPnt[3] - TrPnt[2]);
  rate = cnt / (t2 - t1);
  printz ("Nodes= %ld Nodes/sec= %ld\n", cnt, rate);
}


int GetNextMove(char *buffer)
    {
        char current;
        
     while(true){
	while((current = fgetc(fpin) ) == ' ' || current == '\n');
	if ((current != EOF) && (isdigit(current) || current == '-'))
		{ while((current = fgetc(fpin) ) != ' ' && current != EOF); 
			if(current == EOF)return 0; else continue;}
        if(current == EOF) return(0);
        while(current != ' ' && current != '\n' && current != EOF) {
            if(!(current == '-' || current == 'x' || current == 'X')) { *buffer++ = current; }
            current = fgetc(fpin);
        }
        *buffer = '\0';
        return(1);
      }
    }

void InputCommand(void)
/*
    Open the file of moves to analyse.  Go through the file move by move
    and do the following for each move.  See what gnuchess would have done
    in that position.  Record the move it would have made along with the
    score it would have given and the search depth.  Take back its move.
    Force the move that was actually made.  Keep track of statistics such
    as how many moves agreed.
*/

{
    int i;
    short ok,tmp;
    int search_depth, max_minutes;
    long cnt,rate,t1,t2;
    unsigned short mv;
    char s[80];
    char infilename[255];
    char outfilename[255];
    char progfilename[255];
    int move_number = 1;
    long start_time, end_time, elapsed_time;
    int total_black_moves, total_white_moves;
    int same_black_moves, same_white_moves;
    float white_percent, black_percent;

    /* Initialize necessary variables. */

    flag.quit = false;
    flag.beep = false;
    player = opponent;
    enable_update_display = 1;
    ft = 0;
    Book = NULL;
    total_black_moves = 0;
    total_white_moves = 0;
    same_black_moves  = 0;
    same_white_moves  = 0;

#ifdef BACKGROUND_ANALYSIS

    /* Set the in files to standard ones for analysis if background processing selected. */

    strcpy(infilename,   IN_FILE);
    strcpy(outfilename,  OUT_FILE);
    strcpy(progfilename, PROG_FILE);
    fpout = fopen(outfilename, "w");
    if(fpout == NULL) {
        fprintf(fpout,"This file does not exist : %s\n", outfilename);
        flag.quit = 1;
        return;
    }

    MaxSearchDepth = MAX_DEPTH;
    TCminutes = MAX_TIME;

#else

    /* Request information on where the file is and the depth an time. */

    fpout = stderr;
    fprintf(fpout, "Input the file with the algebraic moves of the game.\n");
    scanf("%s", infilename);

    fprintf(fpout, "\n");
    do {
        fprintf(fpout, "Input the search depth you want to use.  (1 to 29)\n");
        scanf("%d", &search_depth);
    } while(search_depth < 1 || search_depth > 29);
    MaxSearchDepth = search_depth;

    fprintf(fpout, "\n");
    do {
        fprintf(fpout, "Input the maximun number of minutes per move you want to use.\n");
        scanf("%d", &max_minutes);
    } while(max_minutes < 1);
    TCminutes = max_minutes;

    fprintf(fpout, "\n\n");

#endif

    fpin  = fopen(infilename, "r");
    if(fpin == NULL) {
        fprintf(fpout,"This file does not exist : %s\n", infilename);
        flag.quit = 1;
        return;
    }
/* lets find out about this file */
	fgets(InBuf,256,fpin);
	InFileType = OTHER;
	if(strncmp(InBuf,"xboard",6)){InFileType = XBOARD; fgets(InBuf,256,fpin); fgets(InBuf,256,fpin);}
	else if(strncmp(InBuf,"gnuchess",6)){InFileType = GNUCHESS; fgets(InBuf,256,fpin); fgets(InBuf,256,fpin);}
	else rewind(fpin);
    TCmoves = 1;
    TCflag = (TCmoves > 1);
    OperatorTime = 0;
    SetTimeControl();

    fpout = stdout;

    fprintf(fpout, "Move White Black      Score Depth     Best Line\n");
    fprintf(fpout, "------------------------------------------------------------------\n");

#ifdef BACKGROUND_ANALYSIS

    /* Update progress in the progress file if this is running in the background. */

    fpprog = fopen(progfilename, "w");
    fprintf(fpprog, "Done with move #%d.\n", move_number-1);
    fclose(fpprog);

#endif

    time(&start_time);
    while(1) {
        opponent = black;
        computer = white;
        player = computer;
        white_moving = 1;
        if(!GetNextMove(s)) {
            flag.quit = 1;
            break;
        }
        flag.force = 0;
        SelectMove(computer,1);
        Undo();
        flag.force = 1;
        opponent = white;
        computer = black;
        player = opponent;
        ok = VerifyMove(s,0,&mv);
        if(!ok) {
            fprintf(fpout, "\nBad move.  %s  Board position is: \n",s);
            UpdateDisplay(0, 0, 1, 0);
            flag.quit = 1;
            break;
        } else {
            strcpy(white_actual_move, s);
            fprintf(fpout, "%3d   %-9s      %5d%5d   ",move_number, white_actual_move, white_score, current_depth);
            for (i = 1; MV[i] > 0; i++)
		{ algbr ((short) (MV[i] >> 8), (short) (MV[i] & 0xFF), false);
	  	printf ("%5s ", mvstr[0]); }
	    printf("\n");
            fflush(fpout);
            move_number++;
            total_white_moves++;
            if(!strcmp(white_actual_move, white_suggest_move)) same_white_moves++;
            Sdepth = 0;
            ft = 0;
        }
        player = computer;
        white_moving = 0;
        if(!GetNextMove(s)) {
            flag.quit = 1;
            break;
        }
        flag.force = 0;
        SelectMove(computer,1);
        Undo();
        flag.force = 1;
        opponent = black;
        computer = white;
        player = opponent;
        ok = VerifyMove(s,0,&mv);
        if(!ok) {
            fprintf(fpout, "\nBad move.  %s  Board position is: \n",s);
            UpdateDisplay(0, 0, 1, 0);
            flag.quit = 1;
            break;
        } else {
            strcpy(black_actual_move, s);
            fprintf(fpout, "            %-9s%5d%5d   ", black_actual_move, black_score, current_depth);
            for (i = 1; MV[i] > 0; i++)
		{ algbr ((short) (MV[i] >> 8), (short) (MV[i] & 0xFF), false);
	  	printf ("%5s ", mvstr[0]); }
	    printf("\n");
            fflush(fpout);

#ifdef BACKGROUND_ANALYSIS

            /* Update progress in the progress file if this is running in the background. */

            fpprog = fopen(progfilename, "w");
            fprintf(fpprog, "Done with move #%d.\n", move_number-1);
            fclose(fpprog);
#else
/*            fprintf(stderr, "Done with move #%d.\n", move_number-1); */

#endif

            total_black_moves++;
            if(!strcmp(black_actual_move, black_suggest_move)) same_black_moves++;
            Sdepth = 0;
            ft = 0;
        }
    }

    white_percent = ((float) same_white_moves) * 100 / (float) total_white_moves;
    black_percent = ((float) same_black_moves) * 100 / (float) total_black_moves;
    fprintf(fpout, "\n           White's percentage was %5.2f%%.     Black's percentage was %5.2f%%.\n",  white_percent, black_percent);
    time(&end_time);
    elapsed_time = end_time - start_time;
    fprintf(fpout, "\n           Elapsed time was %d seconds.\n", elapsed_time);
    fclose(fpin);
    fclose(fpout);
    return;
}

