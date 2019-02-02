

  GNU Chess 3.21 for  MicroSoft Windows.

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



Version 3.21 fixes/changes since 3.2

        1) Corrected operation of force command and bug in undo.

        2) Added manual move entry mode.  Hit F2 to bring up a dialog

           box to enter a move in algerbraic notation.

        3) Control C aborts the computer's turn.

        4) Revised compiler switches for maximum optimazation.

        5) Added accelerator keys. (f1=help, bksp=undo, alt-bksp=remove)

        6) disabled save and list if no moves have been entered.

        7) corrected accelerator key for Hint



Version 3.2 fixes/changes since 1.01

        1) Added help system, file chess.hlp must be in the same directory

           as the executable.

        2) Added modifications to bring chess engine upto GNU Chess 3.2



release 1.01 notes:

        1) I have only tested this under windows 3.0 386 enhanced mode.

           Seems to work ok in real mode though slower.

        2) It requires at least 282k of memory.

        3) Fits on a EGA screen, no scrolling implemented.

        4) Most of the GNUCHESS commands are implemented.

        5) When computer is playing itself abort by clicking on any square.

           The last move made will most likely show "draw" or "mate", ignore

           this message.  You may have to remove the last move.

        6) The file gnuchess.boo and chess.hlp must be in the same directory 

           as the executable.

        7) The program create a file "chess.ini" in your windows directory to

           store various settings.  Currently only the screen colors are saved.



Enjoy.



Daryl Baker

dkb@mitre.org



