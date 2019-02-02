/*

  C source for GNU CHESS



  Revision: 1990-12-26



  Modified by Daryl Baker for use in MS WINDOWS environment



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



#define NOATOM 

#define NOCLIPBOARD

#define NOCREATESTRUCT

#define NOFONT

#define NOREGION

#define NOSOUND

#define NOWH

#define NOWINOFFSETS

#define NOCOMM

#define NOKANJI



#include <windows.h>

#include <string.h>

#include <time.h>

#include <stdlib.h>

#include <malloc.h>

#include <stdio.h>



#include "gnuchess.h"

#include "defs.h"



#if ttblsz

extern unsigned long hashkey, hashbd;

/*extern struct hashval hashcode[2][7][64];*/

/*extern struct hashentry huge ttable[2][ttblsz];*/

extern struct hashval far *hashcode;

extern struct hashentry far *ttable;

#endif /* ttblsz */



/*extern unsigned char history[8192];*/

extern unsigned char far * history;



extern short rpthash[2][256];

/*extern unsigned char nextpos[8][64][64];*/

/*extern unsigned char nextdir[8][64][64];*/

extern unsigned char far *nextpos;

extern unsigned char far *nextdir;



extern short FROMsquare, TOsquare, Zscore, zwndw;

extern unsigned short PV, Swag0, Swag1, Swag2, Swag3, Swag4;

extern unsigned short killr0[maxdepth], killr1[maxdepth];

extern unsigned short killr2[maxdepth], killr3[maxdepth];

extern short Pscore[maxdepth], Tscore[maxdepth];

extern unsigned long hashkey, hashbd;

extern short ChkFlag[maxdepth], CptrFlag[maxdepth], PawnThreat[maxdepth];

extern short mtl[2], pmtl[2], emtl[2], hung[2];

extern short player, xwndw, rehash;

extern short PieceCnt[2];

extern long NodeCnt, ETnodes, EvalNodes, HashCnt, FHashCnt, HashCol;

extern short HasKnight[2], HasBishop[2], HasRook[2], HasQueen[2];

extern short Pindex[64];



static short _based(_segname("_CODE")) rank7[3] = {6, 1, 0};

static short _based(_segname("_CODE")) kingP[3] = {4, 60, 0};

static short _based(_segname("_CODE")) value[7] =

{0, valueP, valueN, valueB, valueR, valueQ, valueK};



static short _based(_segname("_CODE")) sweep[8] =

{false, false, false, true, true, true, false, false};



static short _based(_segname("_CODE")) ptype[2][8] = {

  no_piece, pawn, knight, bishop, rook, queen, king, no_piece,

  no_piece, bpawn, knight, bishop, rook, queen, king, no_piece};



static short _based(_segname("_CODE")) control[7] =

{0, ctlP, ctlN, ctlB, ctlR, ctlQ, ctlK};



/* ............    MOVE GENERATION & SEARCH ROUTINES    .............. */



void

pick (short int p1, short int p2)



/*

  Find the best move in the tree between indexes p1 and p2. Swap the best

  move into the p1 element.

*/



{

  register short p, s;

  short p0, s0;

  struct leaf temp;



  s0 = Tree[p1].score;

  p0 = p1;

  for (p = p1 + 1; p <= p2; p++)

    if ((s = Tree[p].score) > s0)

      {

        s0 = s;

        p0 = p;

      }

  if (p0 != p1)

    {

      temp = Tree[p1];

      Tree[p1] = Tree[p0];

      Tree[p0] = temp;

    }

}



void

SelectMove (HWND hWnd, short int side, short int iop)



/*

  Select a move by calling function search() at progressively deeper ply

  until time is up or a mate or draw is reached. An alpha-beta window of -90

  to +90 points is set around the score returned from the previous

  iteration. If Sdepth != 0 then the program has correctly predicted the

  opponents move and the search will start at a depth of Sdepth+1 rather

  than a depth of 1.

*/



{

  static short i, tempb, tempc, tempsf, tempst, xside, rpt;

  static short alpha, beta, score;



  flag.timeout = false;

  xside = otherside[side];

  if (iop != 2)

    player = side;

  if (TCflag)

    {

      if ((TimeControl.moves[side] + 3) != 0)

        ResponseTime = (TimeControl.clock[side]) /

          (TimeControl.moves[side] + 3) -

          OperatorTime;

      else

        ResponseTime = 0;

      ResponseTime += (ResponseTime * TimeControl.moves[side]) / (2 * TCmoves + 1);

    }

  else

    ResponseTime = Level;

  if (iop == 2)

    ResponseTime = 99999;

  if (Sdepth > 0 && root->score > Zscore - zwndw)

    ResponseTime -= ft;

  else if (ResponseTime < 1)

    ResponseTime = 1;

  ExtraTime = 0;

  ExaminePosition ();

  ScorePosition (side, &score);

  /* Pscore[0] = -score; */

  ShowSidetoMove ();



  if (Sdepth == 0)

    {

#if ttblsz

      /* ZeroTTable (); */

#endif /* ttblsz */

      SearchStartStuff (side);

#ifdef NOMEMSET

      for (i = 0; i < 8192; i++)

/*        history[i] = 0; */

          *(history+i) = 0;



#else

      _fmemset ( history, 0, 8192 * sizeof (char));

#endif /* NOMEMSET */

      FROMsquare = TOsquare = -1;

      PV = 0;

      if (iop != 2)

        hint = 0;

      for (i = 0; i < maxdepth; i++)

        PrVar[i] = killr0[i] = killr1[i] = killr2[i] = killr3[i] = 0;

      alpha = score - 90;

      beta = score + 90;

      rpt = 0;

      TrPnt[1] = 0;

      root = &Tree[0];

      MoveList (side, 1);

      for (i = TrPnt[1]; i < TrPnt[2]; i++)

        pick (i, TrPnt[2] - 1);

      if (Book != NULL)

        OpeningBook (&hint);

      if (Book != NULL)

        flag.timeout = true;

      NodeCnt = ETnodes = EvalNodes = HashCnt = FHashCnt = HashCol = 0;

      Zscore = 0;

      zwndw = 20;

    }

  while (!flag.timeout && Sdepth < MaxSearchDepth)

    {

      Sdepth++;

      ShowDepth (' ');

      score = search (hWnd, side, 1, Sdepth, alpha, beta, PrVar, &rpt);

      for (i = 1; i <= Sdepth; i++)

        killr0[i] = PrVar[i];

      if (score < alpha)

        {

          ShowDepth ('\xbb' /*'-'*/);

          ExtraTime = 10 * ResponseTime;

          /* ZeroTTable (); */

          score = search (hWnd, side, 1, Sdepth, -9000, score, PrVar, &rpt);

        }

      if (score > beta && !(root->flags & exact))

        {

          ShowDepth ('\xab' /*'+'*/);

          ExtraTime = 0;

          /* ZeroTTable (); */

          score = search (hWnd, side, 1, Sdepth, score, 9000, PrVar, &rpt);

        }

      score = root->score;

      if (!flag.timeout)

        for (i = TrPnt[1] + 1; i < TrPnt[2]; i++)

          pick (i, TrPnt[2] - 1);

      ShowResults (score, PrVar, '\xb7' /*'.'*/);

      for (i = 1; i <= Sdepth; i++)

        killr0[i] = PrVar[i];

      if (score > Zscore - zwndw && score > Tree[1].score + 250)

        ExtraTime = 0;

      else if (score > Zscore - 3 * zwndw)

        ExtraTime = ResponseTime;

      else

        ExtraTime = 3 * ResponseTime;

      if (root->flags & exact)

        flag.timeout = true;

      if (Tree[1].score < -9000)

        flag.timeout = true;

      if (4 * et > 2 * ResponseTime + ExtraTime)

        flag.timeout = true;

      if (!flag.timeout)

        {

          Tscore[0] = score;

          if (Zscore == 0)

            Zscore = score;

          else

            Zscore = (Zscore + score) / 2;

        }

      zwndw = 20 + abs (Zscore / 12);

      beta = score + Bwindow;

      if (Zscore < score)

        alpha = Zscore - Awindow - zwndw;

      else

        alpha = score - Awindow - zwndw;

    }



  score = root->score;

  if (rpt >= 2 || score < -12000)

    root->flags |= draw;

  if (iop == 2)

    return;

  if (Book == NULL)

    hint = PrVar[2];

  ElapsedTime (1);



  if (score > -9999 && rpt <= 2)

    {

      MakeMove (side, root, &tempb, &tempc, &tempsf, &tempst, &INCscore);

      algbr (root->f, root->t, (short) root->flags);

    }

  else

    algbr (0, 0, 0);

  OutputMove (hWnd);

  if (score == -9999 || score == 9998)

    flag.mate = true;

  if (flag.mate)

    hint = 0;

  if ((board[root->t] == pawn)

      || (root->flags & capture)

      || (root->flags & cstlmask))

    {

      Game50 = GameCnt;

      ZeroRPT ();

    }

  GameList[GameCnt].score = score;

  GameList[GameCnt].nodes = NodeCnt;

  GameList[GameCnt].time = (short) et;

  GameList[GameCnt].depth = Sdepth;

  if (TCflag)

    {

      TimeControl.clock[side] -= (et + OperatorTime);

      if (--TimeControl.moves[side] == 0)

        SetTimeControl ();

    }

  if ((root->flags & draw) && flag.bothsides)

    flag.mate = true;

  if (GameCnt > 470)

    flag.mate = true; /* out of move store, you loose */

  player = xside;

  Sdepth = 0;

}



int

parse (FILE *fd, short unsigned int *mv, short int side)

{

  int c, i, r1, r2, c1, c2;

  char s[100];

  while ((c = getc (fd)) == ' ') ;

  i = 0;

  s[0] = (char) c;

  while (c != ' ' && c != '\n' && c != EOF)

    s[++i] = (char) (c = getc (fd));

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

    if (side == black)

      *mv = 0x3C3A;

    else

      *mv = 0x0402;

  else if (s[0] == 'o')

    if (side == black)

      *mv = 0x3C3E;

    else

      *mv = 0x0406;

  else

    {

      c1 = s[0] - 'a';

      r1 = s[1] - '1';

      c2 = s[2] - 'a';

      r2 = s[3] - '1';

      *mv = (locn (r1, c1) << 8) | locn (r2, c2);

    }

  return (1);

}



inline void

repetition (short int *cnt)



/*

  Check for draw by threefold repetition.

*/



{

  short i, c, f, t;

  short b[64];

  unsigned short m;



  *cnt = c = 0;

  if (GameCnt > Game50 + 3)

    {

#ifdef NOMEMSET

      for (i = 0; i < 64; b[i++] = 0) ;

#else

      memset ((char *) b, 0, sizeof (b));

#endif /* NOMEMSET */

      for (i = GameCnt; i > Game50; i--)

        {

          m = GameList[i].gmove;

          f = m >> 8;

          t = m & 0xFF;

          if (++b[f] == 0)

            c--;

          else

            c++;

          if (--b[t] == 0)

            c--;

          else

            c++;

          if (c == 0)

            (*cnt)++;

        }

    }

}



int

search (HWND hWnd,

        short int side,

        short int ply,

        short int depth,

        short int alpha,

        short int beta,

        short unsigned int *bstline,

        short int *rpt)



/*

  Perform an alpha-beta search to determine the score for the current board

  position. If depth <= 0 only capturing moves, pawn promotions and

  responses to check are generated and searched, otherwise all moves are

  processed. The search depth is modified for check evasions, certain

  re-captures and threats. Extensions may continue for up to 11 ply beyond

  the nominal search depth.

*/



#define UpdateSearchStatus \

{\

   if (flag.post) ShowCurrentMove(pnt,node->f,node->t);\

     if (pnt > TrPnt[1])\

       {\

          d = best-Zscore; e = best-node->score;\

            if (best < alpha) ExtraTime = 10*ResponseTime;\

            else if (d > -zwndw && e > 4*zwndw) ExtraTime = -ResponseTime/3;\

            else if (d > -zwndw) ExtraTime = 0;\

            else if (d > -3*zwndw) ExtraTime = ResponseTime;\

            else if (d > -9*zwndw) ExtraTime = 3*ResponseTime;\

            else ExtraTime = 5*ResponseTime;\

            }\

            }

#define prune (cf && score+node->score < alpha)

#define ReCapture (flag.rcptr && score > alpha && score < beta &&\

                   ply > 2 && CptrFlag[ply-1] && CptrFlag[ply-2])

/* && depth == Sdepth-ply+1 */

#define Parry (hung[side] > 1 && ply == Sdepth+1)

#define MateThreat (ply < Sdepth+4 && ply > 4 &&\

                    ChkFlag[ply-2] && ChkFlag[ply-4] &&\

                    ChkFlag[ply-2] != ChkFlag[ply-4])



{

  register short j, pnt;

  short best, tempb, tempc, tempsf, tempst;

  short xside, pbst, d, e, cf, score, rcnt, slk, InChk;

  unsigned short mv, nxtline[maxdepth];

  struct leaf far *node, tmp;



  NodeCnt++;

  xside = otherside[side];



  if ((ply <= Sdepth + 3) && rpthash[side][hashkey & 0xFF] > 0)

    repetition (rpt);

  else

    *rpt = 0;

  /* Detect repetitions a bit earlier. SMC. 12/89 */

  if (*rpt == 1 && ply > 1)

    return (0);

  /* if (*rpt >= 2) return(0); */



  score = evaluate (side, ply, alpha, beta, INCscore, &slk, &InChk);

  if (score > 9000)

    {

      bstline[ply] = 0;

      return (score);

    }



      /* This test has been modified in 3.1 from 1.55 code. I think the mods

         have introduced an error in the search which causes the programme

         to run away from checking moves - hence the failure to find mates

         and to play through mate situations.

         The fixes introduced solve the problem reported by:

         cambell@rnd.GBA.NYU.EDU

         in the mate example.

                                 J.Birmingham.

      */

  if (depth > 0)

    {

      /* Allow opponent a chance to check again */

      if (InChk)

        depth = (depth < 2) ? 2 : depth;

      else if (PawnThreat[ply - 1] || ReCapture)

        ++depth;

    }

  else

    {

      if (score >= alpha &&

          (InChk || PawnThreat[ply - 1] || Parry))

        ++depth;           /* this was depth=1 in original ?bug? */

      else if (score <= beta && MateThreat)

        ++depth;           /* this was also set to depth=1  ?bug? */

    }



   /* end of changed section      J.Birmingham.          */



#if ttblsz

  if (depth > 0 && flag.hash && ply > 1)

    {

      if (ProbeTTable (side, depth, &alpha, &beta, &score) == false)

#ifdef HASHFILE 

        if (hashfile && (depth > 5) && (GameCnt < 12))

          ProbeFTable (side, depth, &alpha, &beta, &score);

#else

      /* do nothing */;

#endif /* HASHFILE */      

      bstline[ply] = PV;

      bstline[ply + 1] = 0;

      if (beta == -20000)

        return (score);

      if (alpha > beta)

        return (alpha);

    }

#endif /* ttblsz */

  if (Sdepth == 1)

    d = 7;

  else

    d = 11;

  if (ply > Sdepth + d || (depth < 1 && score > beta))

    /* score >= beta ?? */

    return (score);



  if (ply > 1)

    if (depth > 0)

      MoveList (side, ply);

    else

      CaptureList (side, ply);



  if (TrPnt[ply] == TrPnt[ply + 1])

    return (score);



  cf = (depth < 1 && ply > Sdepth + 1 && !ChkFlag[ply - 2] && !slk);



  if (depth > 0)

    best = -12000;

  else

    best = score;

  if (best > alpha)

    alpha = best;



  for (pnt = pbst = TrPnt[ply];

       pnt < TrPnt[ply + 1] && best <= beta;    /* best < beta ?? */

       pnt++)

    {



      /* Little code segment to allow cooperative multitasking */

      {

         MSG msg;

         extern HANDLE hAccel;

         if ( !flag.timeout && PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)){

            if ( !TranslateAccelerator (hWnd, hAccel, &msg) ) {

               TranslateMessage(&msg);

               DispatchMessage(&msg);

            }

         }

      }

      /* End of segment */



      if (ply > 1)

        pick (pnt, TrPnt[ply + 1] - 1);

      node = &Tree[pnt];

      mv = (node->f << 8) | node->t;

      nxtline[ply + 1] = 0;



      if (prune)

        break;

      if (ply == 1)

        UpdateSearchStatus;



      if (!(node->flags & exact))

        {

          MakeMove (side, node, &tempb, &tempc, &tempsf, &tempst, &INCscore);

          CptrFlag[ply] = (node->flags & capture);

          PawnThreat[ply] = (node->flags & pwnthrt);

          Tscore[ply] = node->score;

          PV = node->reply;

          node->score = -search (hWnd, xside, ply + 1,

                                 (depth > 0) ? depth - 1 : 0,

                                 -beta, -alpha,

                                 nxtline, &rcnt);

          if (abs (node->score) > 9000)

            node->flags |= exact;

          else if (rcnt == 1)

            node->score /= 2;

          if (rcnt >= 2 || GameCnt - Game50 > 99 ||

              (node->score == 9999 - ply && !ChkFlag[ply]))

            {

              node->flags |= draw;

              node->flags |= exact;

              if (side == computer)

                node->score = contempt;

              else

                node->score = -contempt;

            }

          node->reply = nxtline[ply + 1];

          UnmakeMove (side, node, &tempb, &tempc, &tempsf, &tempst);

        }

      if (node->score > best && !flag.timeout)

        {

          if (depth > 0)

            if (node->score > alpha && !(node->flags & exact))

              node->score += depth;

          best = node->score;

          pbst = pnt;

          if (best > alpha)

            alpha = best;

          for (j = ply + 1; nxtline[j] > 0; j++)

            bstline[j] = nxtline[j];

          bstline[j] = 0;

          bstline[ply] = mv;

          if (ply == 1)

            {

              if (best > root->score)

                {

                  tmp = Tree[pnt];

                  for (j = pnt - 1; j >= 0; j--)

                    Tree[j + 1] = Tree[j];

                  Tree[0] = tmp;

                  pbst = 0;

                }

              if (Sdepth > 2)

                if (best > beta)

                  ShowResults (best, bstline, '\xab' /*'+'*/);

                else if (best < alpha)

                  ShowResults (best, bstline, '\xbb' /* '-'*/);

                else

                  ShowResults (best, bstline, '\xb1' /*'&'*/);

            }

        }

      if (NodeCnt > ETnodes)

        ElapsedTime (0);

      if (flag.timeout)

        return (-Tscore[ply - 1]);

    }



  node = &Tree[pbst];

  mv = (node->f << 8) | node->t;

#if ttblsz

  if (flag.hash && ply <= Sdepth && *rpt == 0 && best == alpha)

    {

      PutInTTable (side, best, depth, alpha, beta, mv);

#ifdef HASHFILE      

      if (hashfile && (depth > 5) && (GameCnt < 12))

        PutInFTable (side, best, depth, alpha, beta, node->f, node->t);

#endif /* HASHFILE */      

    }

#endif /* ttblsz */

  if (depth > 0)

    {

      j = (node->f << 6) | node->t;

      if (side == black)

        j |= 0x1000;

/*      if (history[j] < 150)

          history[j] += (unsigned char) 2 * depth; */

      if (*(history+j) < 150)

        *(history+j) += (unsigned char) 2 * depth;

      if (node->t != (GameList[GameCnt].gmove & 0xFF))

        if (best <= beta)

          killr3[ply] = mv;

        else if (mv != killr1[ply])

          {

            killr2[ply] = killr1[ply];

            killr1[ply] = mv;

          }

      if (best > 9000)

        killr0[ply] = mv;

      else

        killr0[ply] = 0;

    }

  return (best);

}



#if ttblsz

/*

  hashbd contains a 32 bit "signature" of the board position. hashkey

  contains a 16 bit code used to address the hash table. When a move is

  made, XOR'ing the hashcode of moved piece on the from and to squares with

  the hashbd and hashkey values keeps things current.

*/

#define UpdateHashbd(side, piece, f, t) \

{\

  if ((f) >= 0)\

    {\

      hashbd ^= (hashcode+(side)*7*64+(piece)*64+f)->bd;\

      hashkey ^= (hashcode+(side)*7*64+(piece)*64+f)->key;\

    }\

  if ((t) >= 0)\

    {\

      hashbd ^= (hashcode+(side)*7*64+(piece)*64+t)->bd;\

      hashkey ^= (hashcode+(side)*7*64+(piece)*64+t)->key;\

    }\

}



#define CB(i) (unsigned char) ((color[2 * (i)] ? 0x80 : 0)\

               | (board[2 * (i)] << 4)\

               | (color[2 * (i) + 1] ? 0x8 : 0)\

               | (board[2 * (i) + 1]))



int

ProbeTTable (short int side,

             short int depth,

             short int *alpha,

             short int *beta,

             short int *score)



/*

  Look for the current board position in the transposition table.

*/



{

  struct hashentry far *ptbl;

  register unsigned short i;



/*  ptbl = &ttable[side][hashkey & (ttblsz - 1)]; */

   ptbl = ttable+side*2+(hashkey & (ttblsz - 1));



  /* rehash max rehash times */

  for (i = 1; ptbl->hashbd != hashbd && i <= rehash; i++)

/*    ptbl = &ttable[side][(hashkey + i) & (ttblsz - 1)]; */

    ptbl = ttable+side*2+((hashkey + i) & (ttblsz - 1));

  if ((short) ptbl->depth >= depth && ptbl->hashbd == hashbd)

    {

      HashCnt++;

#ifdef HASHTEST

      for (i = 0; i < 32; i++)

        {

          if (ptbl->bd[i] != CB(i))

            {

              HashCol++;

              ShowMessage("ttable collision detected");

              break;

            }

        }

#endif /* HASHTEST */



      PV = ptbl->mv;

      if (ptbl->flags & truescore)

        {

          *score = ptbl->score;

          *beta = -20000;

        }

#if 0 /* commented out, why? */

      else if (ptbl->flags & upperbound)

        {

          if (ptbl->score < *beta) *beta = ptbl->score+1;

        }

#endif

      else if (ptbl->flags & lowerbound)

        {

          if (ptbl->score > *alpha)

            *alpha = ptbl->score - 1;

        }

      return(true);

    }

  return(false);

}



void

PutInTTable (short int side,

             short int score,

             short int depth,

             short int alpha,

             short int beta,

             short unsigned int mv)



/*

  Store the current board position in the transposition table.

*/



{

  struct hashentry far *ptbl;

  register unsigned short i;



/*  ptbl = &ttable[side][hashkey & (ttblsz - 1)]; */

  ptbl = ttable+side*2+(hashkey & (ttblsz - 1));



  /* rehash max rehash times */

  for (i = 1; depth < ptbl->depth && ptbl->hashbd != hashbd && i <= rehash; i++)

/*    ptbl = &ttable[side][(hashkey + i) & (ttblsz - 1)]; */

    ptbl = ttable+side*2+((hashkey + i) & (ttblsz - 1));

  if (depth >= ptbl->depth || ptbl->hashbd != hashbd)

    {

      ptbl->hashbd = hashbd;

      ptbl->depth = (unsigned char) depth;

      ptbl->score = score;

      ptbl->mv = mv;

      ptbl->flags = 0;

      if (score < alpha)

        ptbl->flags |= upperbound;

      else if (score > beta)

        ptbl->flags |= lowerbound;

      else

        ptbl->flags |= truescore;

#ifdef HASHTEST

      for (i = 0; i < 32; i++)

        {

          ptbl->bd[i] = CB(i);

        }

#endif /* HASHTEST */

    }

}



void

ZeroTTable (void)

{

  register int side, i;



  if (flag.hash)

    for (side = white; side <= black; side++)

      for (i = 0; i < ttblsz; i++)

/*        ttable[side][i].depth = 0; */

        (ttable+side*2+i)->depth = 0;

}



#ifdef HASHFILE

int

ProbeFTable(short int side,

            short int depth,

            short int *alpha,

            short int *beta,

            short int *score)



/*

  Look for the current board position in the persistent transposition table.

*/



{

  register unsigned short i, j;

  unsigned long hashix;

  short s;

  struct fileentry new, t;



  if (side == white)

    hashix = hashkey & 0xFFFFFFFE & (filesz - 1);

  else

    hashix = hashkey | 1 & (filesz - 1);



  for (i = 0; i < 32; i++)

    new.bd[i] = CB(i);

  new.flags = 0;

  if ((Mvboard[kingP[side]] == 0) && (Mvboard[qrook[side]] == 0))

    new.flags |= queencastle;

  if ((Mvboard[kingP[side]] == 0) && (Mvboard[krook[side]] == 0))

    new.flags |= kingcastle;



  for (i = 0; i < frehash; i++)

    {

      fseek(hashfile,

            sizeof(struct fileentry) * ((hashix + 2 * i) & (filesz - 1)),

            SEEK_SET);

      fread(&t, sizeof(struct fileentry), 1, hashfile);

      for (j = 0; j < 32; j++)

        if (t.bd[j] != new.bd[j])

          break;

      if (((short) t.depth >= depth) && (j >= 32)

          && (new.flags == (t.flags & (kingcastle | queencastle))))

        {

          FHashCnt++;

          PV = (t.f << 8) | t.t;

          s = (t.sh << 8) | t.sl;

          if (t.flags & truescore)

            {

              *score = s;

              *beta = -20000;

            }

          else if (t.flags & lowerbound)

            {

              if (s > *alpha)

                *alpha = s - 1;

            }

          return(true);

        }

    }

  return(false);

}



void

PutInFTable (short int side,

             short int score,

             short int depth,

             short int alpha,

             short int beta,

             short unsigned int f,

             short unsigned int t)



/*

  Store the current board position in the persistent transposition table.

*/



{

  register unsigned short i;

  unsigned long hashix;

  struct fileentry new, tmp;



  if (side == white)

    hashix = hashkey & 0xFFFFFFFE & (filesz - 1);

  else

    hashix = hashkey | 1 & (filesz - 1);



  for (i = 0; i < 32; i++)

    new.bd[i] = CB(i);

  new.f = (unsigned char) f;

  new.t = (unsigned char) t;

  new.flags = 0;

  if (score < alpha)

    new.flags |= upperbound;

  else if (score > beta)

    new.flags |= lowerbound;

  else

    new.flags |= truescore;

  if ((Mvboard[kingP[side]] == 0) && (Mvboard[qrook[side]] == 0))

    new.flags |= queencastle;

  if ((Mvboard[kingP[side]] == 0) && (Mvboard[krook[side]] == 0))

    new.flags |= kingcastle;

  new.depth = (unsigned char) depth;

  new.sh = (unsigned char) (score >> 8);

  new.sl = (unsigned char) (score & 0xFF);



  for (i = 0; i < frehash; i++)

    {

      fseek(hashfile,

            sizeof(struct fileentry) * ((hashix + 2 * i) & (filesz - 1)),

            SEEK_SET);

      fread(&tmp, sizeof(struct fileentry), 1, hashfile);

      if ((short) tmp.depth <= depth)

        {

          fseek(hashfile,

                sizeof(struct fileentry) * ((hashix + 2 * i) & (filesz - 1)),

                SEEK_SET);

          fwrite (&new, sizeof(struct fileentry), 1, hashfile);

          break;

        }

    }

}

#endif /* HASHFILE */

#endif /* ttblsz */



void

ZeroRPT (void)

{

  register int side, i;



  for (side = white; side <= black; side++)

    for (i = 0; i < 256; i++)

      rpthash[side][i] = 0;

}



#define Link(from,to,flag,s) \

{\

   node->f = from; node->t = to;\

     node->reply = 0;\

       node->flags = flag;\

         node->score = s;\

           ++node;\

             ++TrPnt[ply+1];\

             }



static inline void

LinkMove (short int ply,

          short int f,

          short int t,

          short int flag,

          short int xside)



/*

  Add a move to the tree.  Assign a bonus to order the moves

  as follows:

  1. Principle variation

  2. Capture of last moved piece

  3. Other captures (major pieces first)

  4. Killer moves

  5. "history" killers

*/



{

  register short s, z;

  unsigned short mv;

  struct leaf far *node;



  node = &Tree[TrPnt[ply + 1]];

  mv = (f << 8) | t;

  s = 0;

  if (mv == Swag0)

    s = 2000;

  else if (mv == Swag1)

    s = 60;

  else if (mv == Swag2)

    s = 50;

  else if (mv == Swag3)

    s = 40;

  else if (mv == Swag4)

    s = 30;

  z = (f << 6) | t;

  if (xside == white)

    z |= 0x1000;

/*  s += history[z]; */

  s += *(history+z);

  if (color[t] != neutral)

    {

      if (t == TOsquare)

        s += 500;

      s += value[board[t]] - board[f];

    }

  if (board[f] == pawn)

    if (row (t) == 0 || row (t) == 7)

      {

        flag |= promote;

        s += 800;

        Link (f, t, flag | queen, s - 20000);

        s -= 200;

        Link (f, t, flag | knight, s - 20000);

        s -= 50;

        Link (f, t, flag | rook, s - 20000);

        flag |= bishop;

        s -= 50;

      }

    else if (row (t) == 1 || row (t) == 6)

      {

        flag |= pwnthrt;

        s += 600;

      }

  Link (f, t, flag, s - 20000);

}





static inline void

GenMoves (short int ply, short int sq, short int side, short int xside)



/*

  Generate moves for a piece. The moves are taken from the precalulated

  array nextpos/nextdir. If the board is free, next move is choosen from

  nextpos else from nextdir.

*/



{

  register short u, piece;

  unsigned char far *ppos, far *pdir;



  piece = board[sq];

  ppos = nextpos+ptype[side][piece]*64*64+sq*64;

  pdir = nextdir+ptype[side][piece]*64*64+sq*64;

  if (piece == pawn)

    {

      u = ppos[sq];     /* follow no captures thread */

      if (color[u] == neutral)

        {

          LinkMove (ply, sq, u, 0, xside);

          u = ppos[u];

          if (color[u] == neutral)

            LinkMove (ply, sq, u, 0, xside);

        }

      u = pdir[sq];     /* follow captures thread */

      if (color[u] == xside)

        LinkMove (ply, sq, u, capture, xside);

      else

        if (u == epsquare)

          LinkMove (ply, sq, u, capture | epmask, xside);

      u = pdir[u];

      if (color[u] == xside)

        LinkMove (ply, sq, u, capture, xside);

      else

        if (u == epsquare)

          LinkMove (ply, sq, u, capture | epmask, xside);

    }

  else

    {

      u = ppos[sq];

      do

        {

          if (color[u] == neutral)

            {

              LinkMove (ply, sq, u, 0, xside);

              u = ppos[u];

            }

          else

            {

              if (color[u] == xside)

                LinkMove (ply, sq, u, capture, xside);

              u = pdir[u];

            }

      } while (u != sq);

    }

}



void

MoveList (short int side, short int ply)



/*

  Fill the array Tree[] with all available moves for side to play. Array

  TrPnt[ply] contains the index into Tree[] of the first move at a ply.

*/



{

  short i, xside, f;



  xside = otherside[side];

  TrPnt[ply + 1] = TrPnt[ply];

  if (PV == 0)

    Swag0 = killr0[ply];

  else

    Swag0 = PV;

  Swag1 = killr1[ply];

  Swag2 = killr2[ply];

  Swag3 = killr3[ply];

  if (ply > 2)

    Swag4 = killr1[ply - 2];

  else

    Swag4 = 0;

  for (i = PieceCnt[side]; i >= 0; i--)

    GenMoves (ply, PieceList[side][i], side, xside);

  if (!castld[side])

    {

      f = PieceList[side][0];

      if (castle (side, f, f + 2, 0))

        {

          LinkMove (ply, f, f + 2, cstlmask, xside);

        }

      if (castle (side, f, f - 2, 0))

        {

          LinkMove (ply, f, f - 2, cstlmask, xside);

        }

    }

}



void

CaptureList (short int side, short int ply)



/*

  Fill the array Tree[] with all available cature and promote moves for

  side to play. Array TrPnt[ply] contains the index into Tree[]

  of the first move at a ply.

*/



{

  short u, sq, xside;

  struct leaf far *node;

  unsigned char far *ppos, far *pdir;

  short i, piece, *PL, r7;



  xside = otherside[side];

  TrPnt[ply + 1] = TrPnt[ply];

  node = &Tree[TrPnt[ply]];

  r7 = rank7[side];

  PL = PieceList[side];

  for (i = 0; i <= PieceCnt[side]; i++)

    {

      sq = PL[i];

      piece = board[sq];

      if (sweep[piece])

        {

          ppos = nextpos+piece*64*64+sq*64;

          pdir = nextdir+piece*64*64+sq*64;

          u = ppos[sq];

          do

            {

              if (color[u] == neutral)

                u = ppos[u];

              else

                {

                  if (color[u] == xside)

                    Link (sq, u, capture,

                          value[board[u]] + svalue[board[u]] - piece);

                  u = pdir[u];

                }

          } while (u != sq);

        }

      else

        {

          pdir = nextdir+ptype[side][piece]*64*64+sq*64;

          if (piece == pawn && row (sq) == r7)

            {

              u = pdir[sq];

              if (color[u] == xside)

                Link (sq, u, capture | promote | queen, valueQ);

              u = pdir[u];

              if (color[u] == xside)

                Link (sq, u, capture | promote | queen, valueQ);

              ppos = nextpos+ptype[side][piece]*64*64+sq*64;

              u = ppos[sq]; /* also generate non capture promote */

              if (color[u] == neutral)

                Link (sq, u, promote | queen, valueQ);

            }

          else

            {

              u = pdir[sq];

              do

                {

                  if (color[u] == xside)

                    Link (sq, u, capture,

                          value[board[u]] + svalue[board[u]] - piece);

                  u = pdir[u];

              } while (u != sq);

            }

        }

    }

}





int

castle (short int side, short int kf, short int kt, short int iop)



/* Make or Unmake a castling move. */



{

  short rf, rt, t0, xside;



  xside = otherside[side];

  if (kt > kf)

    {

      rf = kf + 3;

      rt = kt - 1;

    }

  else

    {

      rf = kf - 4;

      rt = kt + 1;

    }

  if (iop == 0)

    {

      if (kf != kingP[side] ||

          board[kf] != king ||

          board[rf] != rook ||

          Mvboard[kf] != 0 ||

          Mvboard[rf] != 0 ||

          color[kt] != neutral ||

          color[rt] != neutral ||

          color[kt - 1] != neutral ||

          SqAtakd (kf, xside) ||

          SqAtakd (kt, xside) ||

          SqAtakd (rt, xside))

        return (false);

    }

  else

    {

      if (iop == 1)

        {

          castld[side] = true;

          Mvboard[kf]++;

          Mvboard[rf]++;

        }

      else

        {

          castld[side] = false;

          Mvboard[kf]--;

          Mvboard[rf]--;

          t0 = kt;

          kt = kf;

          kf = t0;

          t0 = rt;

          rt = rf;

          rf = t0;

        }

      board[kt] = king;

      color[kt] = side;

      Pindex[kt] = 0;

      board[kf] = no_piece;

      color[kf] = neutral;

      board[rt] = rook;

      color[rt] = side;

      Pindex[rt] = Pindex[rf];

      board[rf] = no_piece;

      color[rf] = neutral;

      PieceList[side][Pindex[kt]] = kt;

      PieceList[side][Pindex[rt]] = rt;

#if ttblsz

      UpdateHashbd (side, king, kf, kt);

      UpdateHashbd (side, rook, rf, rt);

#endif /* ttblsz */

    }

  return (true);

}





static inline void

EnPassant (short int xside, short int f, short int t, short int iop)



/*

  Make or unmake an en passant move.

*/



{

  register short l;



  if (t > f)

    l = t - 8;

  else

    l = t + 8;

  if (iop == 1)

    {

      board[l] = no_piece;

      color[l] = neutral;

    }

  else

    {

      board[l] = pawn;

      color[l] = xside;

    }

  InitializeStats ();

}





static inline void

UpdatePieceList (short int side, short int sq, short int iop)



/*

  Update the PieceList and Pindex arrays when a piece is captured or when a

  capture is unmade.

*/



{

  register short i;

  if (iop == 1)

    {

      PieceCnt[side]--;

      for (i = Pindex[sq]; i <= PieceCnt[side]; i++)

        {

          PieceList[side][i] = PieceList[side][i + 1];

          Pindex[PieceList[side][i]] = i;

        }

    }

  else

    {

      PieceCnt[side]++;

      PieceList[side][PieceCnt[side]] = sq;

      Pindex[sq] = PieceCnt[side];

    }

}



void

MakeMove (short int side,

          struct leaf far * node,

          short int *tempb,

          short int *tempc,

          short int *tempsf,

          short int *tempst,

          short int *INCscore)



/*

  Update Arrays board[], color[], and Pindex[] to reflect the new board

  position obtained after making the move pointed to by node. Also update

  miscellaneous stuff that changes when a move is made.

*/



{

  short f, t, xside, ct, cf;



  xside = otherside[side];

  GameCnt++;

  f = node->f;

  t = node->t;

  epsquare = -1;

  FROMsquare = f;

  TOsquare = t;

  *INCscore = 0;

  GameList[GameCnt].gmove = (f << 8) | t;

  if (node->flags & cstlmask)

    {

      GameList[GameCnt].piece = no_piece;

      GameList[GameCnt].color = side;

      (void) castle (side, f, t, 1);

    }

  else

    {

      if (!(node->flags & capture) && (board[f] != pawn))

        rpthash[side][hashkey & 0xFF]++;

      *tempc = color[t];

      *tempb = board[t];

      *tempsf = svalue[f];

      *tempst = svalue[t];

      GameList[GameCnt].piece = *tempb;

      GameList[GameCnt].color = *tempc;

      if (*tempc != neutral)

        {

          UpdatePieceList (*tempc, t, 1);

          if (*tempb == pawn)

            --PawnCnt[*tempc][column (t)];

          if (board[f] == pawn)

            {

              --PawnCnt[side][column (f)];

              ++PawnCnt[side][column (t)];

              cf = column (f);

              ct = column (t);

              if (PawnCnt[side][ct] > 1 + PawnCnt[side][cf])

                *INCscore -= 15;

              else if (PawnCnt[side][ct] < 1 + PawnCnt[side][cf])

                *INCscore += 15;

              else if (ct == 0 || ct == 7 || PawnCnt[side][ct + ct - cf] == 0)

                *INCscore -= 15;

            }

          mtl[xside] -= value[*tempb];

          if (*tempb == pawn)

            pmtl[xside] -= valueP;

#if ttblsz

          UpdateHashbd (xside, *tempb, -1, t);

#endif /* ttblsz */

          *INCscore += *tempst;

          Mvboard[t]++;

        }

      color[t] = color[f];

      board[t] = board[f];

      svalue[t] = svalue[f];

      Pindex[t] = Pindex[f];

      PieceList[side][Pindex[t]] = t;

      color[f] = neutral;

      board[f] = no_piece;

      if (board[t] == pawn)

        if (t - f == 16)

          epsquare = f + 8;

        else if (f - t == 16)

          epsquare = f - 8;

      if (node->flags & promote)

        {

          board[t] = node->flags & pmask;

          if (board[t] == queen)

            HasQueen[side]++;

          else if (board[t] == rook)

            HasRook[side]++;

          else if (board[t] == bishop)

            HasBishop[side]++;

          else if (board[t] == knight)

            HasKnight[side]++;

          --PawnCnt[side][column (t)];

          mtl[side] += value[board[t]] - valueP;

          pmtl[side] -= valueP;

#if ttblsz

          UpdateHashbd (side, pawn, f, -1);

          UpdateHashbd (side, board[t], f, -1);

#endif /* ttblsz */

          *INCscore -= *tempsf;

        }

      if (node->flags & epmask)

        EnPassant (xside, f, t, 1);

      else

#if ttblsz

        UpdateHashbd (side, board[t], f, t);

#else

        /* NOOP */;     

#endif /* ttblsz */

      Mvboard[f]++;

    }

}



void

UnmakeMove (short int side,

            struct leaf far * node,

            short int *tempb,

            short int *tempc,

            short int *tempsf,

            short int *tempst)



/*

  Take back a move.

*/



{

  short f, t, xside;



  xside = otherside[side];

  f = node->f;

  t = node->t;

  epsquare = -1;

  GameCnt--;

  if (node->flags & cstlmask)

    (void) castle (side, f, t, 2);

  else

    {

      color[f] = color[t];

      board[f] = board[t];

      svalue[f] = *tempsf;

      Pindex[f] = Pindex[t];

      PieceList[side][Pindex[f]] = f;

      color[t] = *tempc;

      board[t] = *tempb;

      svalue[t] = *tempst;

      if (node->flags & promote)

        {

          board[f] = pawn;

          ++PawnCnt[side][column (t)];

          mtl[side] += valueP - value[node->flags & pmask];

          pmtl[side] += valueP;

#if ttblsz

          UpdateHashbd (side, (short) node->flags & pmask, -1, t);

          UpdateHashbd (side, pawn, -1, t);

#endif /* ttblsz */

        }

      if (*tempc != neutral)

        {

          UpdatePieceList (*tempc, t, 2);

          if (*tempb == pawn)

            ++PawnCnt[*tempc][column (t)];

          if (board[f] == pawn)

            {

              --PawnCnt[side][column (t)];

              ++PawnCnt[side][column (f)];

            }

          mtl[xside] += value[*tempb];

          if (*tempb == pawn)

            pmtl[xside] += valueP;

#if ttblsz

          UpdateHashbd (xside, *tempb, -1, t);

#endif /* ttblsz */

          Mvboard[t]--;

        }

      if (node->flags & epmask)

        EnPassant (xside, f, t, 2);

      else

#if ttblsz

        UpdateHashbd (side, board[f], f, t);

#else

      /* NOOP */;

#endif /* ttblsz */

      Mvboard[f]--;

      if (!(node->flags & capture) && (board[f] != pawn))

        rpthash[side][hashkey & 0xFF]--;

    }

}





void

InitializeStats (void)



/*

  Scan thru the board seeing what's on each square. If a piece is found,

  update the variables PieceCnt, PawnCnt, Pindex and PieceList. Also

  determine the material for each side and set the hashkey and hashbd

  variables to represent the current board position. Array

  PieceList[side][indx] contains the location of all the pieces of either

  side. Array Pindex[sq] contains the indx into PieceList for a given

  square.

*/



{

  short i, sq;

  

  epsquare = -1;

  for (i = 0; i < 8; i++)

    PawnCnt[white][i] = PawnCnt[black][i] = 0;

  mtl[white] = mtl[black] = pmtl[white] = pmtl[black] = 0;

  PieceCnt[white] = PieceCnt[black] = 0;

#if ttblsz

  hashbd = hashkey = 0;

#endif /* ttblsz */

  for (sq = 0; sq < 64; sq++)

    if (color[sq] != neutral)

      {

        mtl[color[sq]] += value[board[sq]];

        if (board[sq] == pawn)

          {

            pmtl[color[sq]] += valueP;

            ++PawnCnt[color[sq]][column (sq)];

          }

        if (board[sq] == king)

          Pindex[sq] = 0;

        else

          Pindex[sq] = ++PieceCnt[color[sq]];

        PieceList[color[sq]][Pindex[sq]] = sq;

#if ttblsz

        hashbd ^= (hashcode+color[sq]*7*64+board[sq]*64+sq)->bd;

        hashkey ^= (hashcode+color[sq]*7*64+board[sq]*64+sq)->key;

#endif /* ttblsz */

      }

}





int

SqAtakd (short int sq, short int side)



/*

  See if any piece with color 'side' ataks sq.  First check pawns then Queen,

  Bishop, Rook and King and last Knight.

*/



{

  register short u;

  unsigned char far *ppos, far *pdir;

  short xside;



  xside = otherside[side];

  pdir = nextdir+ptype[xside][pawn]*64*64+sq*64;

  u = pdir[sq];         /* follow captures thread */

  if (u != sq)

    {

      if (board[u] == pawn && color[u] == side)

        return (true);

      u = pdir[u];

      if (u != sq && board[u] == pawn && color[u] == side)

        return (true);

    }

  /* king capture */

  if (distance (sq, PieceList[side][0]) == 1)

    return (true);

  /* try a queen bishop capture */

  ppos = nextpos+bishop*64*64+sq*64;

  pdir = nextdir+bishop*64*64+sq*64;

  u = ppos[sq];

  do

    {

      if (color[u] == neutral)

        u = ppos[u];

      else

        {

          if (color[u] == side &&

              (board[u] == queen || board[u] == bishop))

            return (true);

          u = pdir[u];

        }

  } while (u != sq);

  /* try a queen rook capture */

  ppos = nextpos+rook*64*64+sq*64;

  pdir = nextdir+rook*64*64+sq*64;

  u = ppos[sq];

  do

    {

      if (color[u] == neutral)

        u = ppos[u];

      else

        {

          if (color[u] == side &&

              (board[u] == queen || board[u] == rook))

            return (true);

          u = pdir[u];

        }

  } while (u != sq);

  /* try a knight capture */

  pdir = nextdir+knight*64*64+sq*64;

  u = pdir[sq];

  do

    {

      if (color[u] == side && board[u] == knight)

        return (true);

      u = pdir[u];

  } while (u != sq);

  return (false);

}



void

ataks (short int side, short int *a)



/*

  Fill array atak[][] with info about ataks to a square.  Bits 8-15 are set

  if the piece (king..pawn) ataks the square.  Bits 0-7 contain a count of

  total ataks to the square.

*/



{

  short u, c, sq;

  unsigned char far *ppos, far *pdir;

  short i, piece, *PL;



#ifdef NOMEMSET

  for (u = 64; u; a[--u] = 0) ;

#else

  memset ((char *) a, 0, 64 * sizeof (a[0]));

#endif /* NOMEMSET */

  PL = PieceList[side];

  for (i = PieceCnt[side]; i >= 0; i--)

    {

      sq = PL[i];

      piece = board[sq];

      c = control[piece];

      if (sweep[piece])

        {

          ppos = nextpos+piece*64*64+sq*64;

          pdir = nextdir+piece*64*64+sq*64;

          u = ppos[sq];

          do

            {

              a[u] = ++a[u] | c;

              u = (color[u] == neutral) ? ppos[u] : pdir[u];

          } while (u != sq);

        }

      else

        {

          pdir = nextdir+ptype[side][piece]*64*64+sq*64;

          u = pdir[sq]; /* follow captures thread for pawns */

          do

            {

              a[u] = ++a[u] | c;

              u = pdir[u];

          } while (u != sq);

        }

    }

}



