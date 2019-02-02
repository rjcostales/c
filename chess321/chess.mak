#

#  C source for GNU CHESS

#

#  Revision: 1990-12-26

#

#  Modified by Daryl Baker for use in MS WINDOWS environment

#

#  This file is part of CHESS.

#

#  CHESS is distributed in the hope that it will be useful, but WITHOUT ANY

#  WARRANTY.  No author or distributor accepts responsibility to anyone for

#  the consequences of using it or for whether it serves any particular

#  purpose or works at all, unless he says so in writing.  Refer to the CHESS

#  General Public License for full details.

#

#  Everyone is granted permission to copy, modify and redistribute CHESS, but

#  only under the conditions described in the CHESS General Public License.

#  A copy of this license is supposed to have been given to you along with

#  CHESS so you can know your rights and responsibilities.  It should be in a

#  file named COPYING.  Among other things, the copyright notice and this

#  notice must be preserved on all copies.

#

#

PROJ = chess

PROJFILE = chess.mak

VER = 321



CC  = cl

CFLAGS  = /AS /W4 /Oxw /G2 /Zp /nologo

#CFLAGS  = /AM /W4 /G2s /Zp /nologo /Osge



LRF  = echo > NUL

LINKER  = link

LFLAGS  =   /BATCH /nologo /align:16 /map /NOF /NOP

LLIBS_R  =  /NOD:SLIBCE SLIBCEW

LLIBS_G  = LIBW.LIB

MAPFILE_R  = chess.map



ASM  = masm

AFLAGS_G  = /Mx /T



RC  = rc

IMPLIB  = implib



DEF_FILE  = CHESS.DEF

OBJS  = CHESS.obj INIT.obj BOARD.obj ABOUT.obj PIECE.obj \

        HITTEST.obj COLOR.obj TIMECNT.obj SEARCH.obj INITIALI.obj \

        EVAL.obj GLOBALS.obj DSP.obj MSWDSP.obj BOOK.obj CREATE.obj \

        SAVEOPEN.obj NUMDLG.obj INITMENU.obj REVIEW.obj STATS.obj \

        TEST.obj PROMOTE.obj MANUAL.obj



RT_OBJS = VERSION.obj



RESS = chess.res



BMPS = king.bmp kingm.bmp kingo.bmp pawn.bmp pawno.bmp pawnm.bmp \

       rook.bmp rooko.bmp rookm.bmp knight.bmp knightm.bmp knighto.bmp \

       bishop.bmp bishopm.bmp bishopo.bmp queen.bmp queenm.bmp queeno.bmp



HELPFILES = chess.rtf article.rtf article2.rtf copying.rtf move-gen.rtf heuristi.rtf match.rtf

HELPSOURCE = chess.doc article.doc article2.doc copying.doc move-gen.doc heuristi.doc match.doc



all: $(PROJ).exe $(PROJ).hlp



.SILIENT:



.SUFFIXES: .rc .res .c .obj .doc .rtf



CHESS.RES : chess.rc $(BMPS) color.dlg chess.ico timecnt.dlg saveopen.dlg \

            getnum.dlg review.dlg about.dlg stats.dlg test.dlg promote.dlg \

            manual.dlg



# The following routines contain window functions or Call backs and is

# compiled with the Gw switch



ABOUT.obj : ABOUT.C

        $(CC) /c $(CFLAGS) /Gw $*.c



CHESS.obj : CHESS.C gnuchess.h defs.h chess.h saveopen.h  color.h 

        $(CC) /c $(CFLAGS) /Gw /EM $*.c



COLOR.obj : COLOR.C chess.h color.h

        $(CC) /c $(CFLAGS) /Gw $*.c



MANUAL.obj : MANUAL.C chess.h

        $(CC) /c $(CFLAGS) /Gw $*.c



NUMDLG.obj : NUMDLG.C chess.h

        $(CC) /c $(CFLAGS) /Gw $*.c



PROMOTE.obj : PROMOTE.C chess.h

        $(CC) /c $(CFLAGS) /Gw $*.c



REVIEW.obj : REVIEW.C gnuchess.h chess.h defs.h

        $(CC) /c $(CFLAGS) /Gw $*.c



SAVEOPEN.obj : SAVEOPEN.C saveopen.h

        $(CC) /c $(CFLAGS) /Gw $*.c



STATS.obj : STATS.C gnuchess.h stats.h

        $(CC) /c $(CFLAGS) /Gw $*.c



TEST.obj : TEST.C gnuchess.h chess.h defs.h

        $(CC) /c $(CFLAGS) /Od /Gw $*.c



TIMECNT.obj : TIMECNT.C timecnt.h chess.h

        $(CC) /c $(CFLAGS) /Gw $*.c



# The following are compiled with the GW switch



BOARD.obj : BOARD.C defs.h

        $(CC) /c $(CFLAGS) /GW $*.c



BOOK.obj : BOOK.C gnuchess.h defs.h chess.h

        $(CC) /c $(CFLAGS) /GW $*.c



CREATE.obj : CREATE.C defs.h chess.h

        $(CC) /c $(CFLAGS) /GW $*.c



DSP.obj : DSP.C gnuchess.h defs.h chess.h

        $(CC) /c $(CFLAGS) /GW $*.c



EVAL.obj : EVAL.C gnuchess.h defs.h

        $(CC) /c $(CFLAGS) /EM /GW $*.c



GLOBALS.obj : GLOBALS.C gnuchess.h

        $(CC) /c $(CFLAGS) /GW $*.c



HITTEST.obj : HITTEST.C defs.h

        $(CC) /c $(CFLAGS) /GW $*.c



INIT.obj : INIT.C

        $(CC) /c $(CFLAGS) /GW $*.c



INITIALI.obj : INITIALI.C gnuchess.h defs.h

        $(CC) /c $(CFLAGS) /EM /GW $*.c



INITMENU.obj : INITMENU.C gnuchess.h chess.h

        $(CC) /c $(CFLAGS) /GW $*.c



MSWDSP.obj : MSWDSP.C gnuchess.h chess.h defs.h stats.h

        $(CC) /c $(CFLAGS) /GW $*.c



PIECE.obj : PIECE.C chess.h defs.h

        $(CC) /c $(CFLAGS) /GW $*.c



SEARCH.obj : SEARCH.C gnuchess.h defs.h

        $(CC) /c $(CFLAGS) /EM /GW $*.c



#

# Code to generate new time stamp when any module updated

#

makever.exe: makever.c

        cl /nologo makever.c



VERSION.obj: VERSION.c

        $(CC) /c $(CFLAGS) /GW $*.c



VERSION.c:  $(OBJS) $(RESS) $(BMPS) makever.exe

        makever >version.c



# Create the help file



chess.hlp: chess.hpj $(HELPFILES)

        -del chess.ph

        hc chess.hpj



chexe$(VER).zip:

distexe: chess.exe readme.txt gnuchess.boo chess.hlp copying

        -del chexe$(VER).zip

        pkzip -a chexe$(VER) readme.txt chess.exe gnuchess.boo chess.hlp copying



distsrc: chess.exe chexe$(VER).zip readme.txt chess.mak chess.hlp

        -del chess$(VER).zip

        pkzip -a chess$(VER) @files.lst



#        pkzip -a chess$(VER) readme.txt $(PROJ).exe *.boo $(PROJ).hlp files.lst

#        pkzip -a chess$(VER) $(PROJ).def $(PROJ).rc *.dlg $(PROJ).mak $(PROJ).ico 

#        pkzip -a chess$(VER) copying winport change.log todo *.h *.c *.bmp 

#        pkzip -a chess$(VER) chess.hpj $(HELPFILES)

#        pkzip -a chess$(VER) $(HELPSOURCE)



disttar: chess.exe chexe$(VER).zip readme.txt chess.mak chess.hlp

        -del ch$(VER)t.*

        -del chess.ta?

        tar -cf chess.tar -u bmp -u doc -u rtf -u hlp -u exe -T files.lst

        c:\ucs\compress chess.tar

        ren chess.taz ch$(VER)t.Z



distmail: chess.exe chexe$(VER).zip readme.txt chess.mak chess.hlp

        -del ch$(VER)t.*

        -del chess.ta?

        tar -cf chess.tar -u bmp -u doc -u rtf -u hlp -u exe -T files.lst

        c:\ucs\compress chess.tar

        ren chess.taz ch$(VER)t.Z

        uuxfer20 -s61000 -m -o -ech$(VER)t.Z 



clean:

        -del *.obj

        -del *.bak

        -del *.res

        -del *.err

        -del *.ph



$(PROJ).exe : $(DEF_FILE) $(OBJS) $(RT_OBJS) $(RESS)

        $(LRF) @<<$(PROJ).lrf

$(LFLAGS) $(RT_OBJS: = +^

) $(OBJS: = +^

)

$@

$(MAPFILE_R)

$(LLIBS_G: = +^

) +

$(LLIBS_R: = +^

) +

$(LIBS: = +^

)

$(DEF_FILE) ; 

<<

        $(LINKER) @$(PROJ).lrf

        $(RC) $(RESS) $@



.c.obj :

        $(CC) /c $(CFLAGS) /Gw $<



.rc.res :

        $(RC) /r $<



#

# this rule is here to trap changes to the doc files.  Ideally it

# would call a program todo RTF conversion.

#

.doc.rtf :

        @echo $< is out of date. Resave as RTF.

        @zzz### dummy command line need abort command



