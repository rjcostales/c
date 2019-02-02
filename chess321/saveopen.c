/*

  C source for GNU CHESS



  Revision: 1990-09-30



  Modified by Daryl Baker for use in MS WINDOWS environment

  Based on ideas and code segments from "programming windows" C. Petzold



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

   File open/save dialog box routines

   

   int FAR DoFileOpenDlg ( hInst, hWnd, szFileSpecIn, szDefExtIn, wFileAttrIn,

                           szFileNameOut, pofIn)



   int FAR DoWildFileOpenDlg ( hInst, hWnd, szFileSpecIn, szDefExtIn, wFileAttrIn,

                           szFileNameOut, pofIn)



   int FAR DoFileSaveDlg ( hInst, hWnd, szFileSpecIn, szDefExtIn, pwStatusOut,

                           szFileNameOut, pofIn)





   DoWildFileOpenDlg Allows a wildcard to be returned.



   to use:

   in rc file:

      #rcinclude "saveopen.dlg"



   in def file:

      Exports

         FileOpenDlgProc

         FileSaveDlgProc

         WildFileOpenDlgProc



   in source:

   #include "saveopen.h"

   ...

   if ( !DoFileSaveDlg ( hInst, hWnd, "chess.chs", ".chs", &Status,

                               FileName, &pof) ) break;

               if ( Status == 1 ) {

                  strcpy ( str, "Replace Existing ");

                  strcat ( str, FileName);

                  if ( MessageBox ( hWnd, str, szAppName, MB_YESNO | MB_ICONQUESTION) == IDNO)

                     break;

                  

               } else OpenFile (FileName, pof, OF_PARSE);

 

               SaveGame ( pof->szPathName );



   if ( !DoFileOpenDlg ( hInst, hWnd, "*.chs", ".chs", 0x0, FileName, &pof))

                  break;



               if ( (hFile=OpenFile(FileName, pof, OF_READ|OF_REOPEN)) == -1) {

                  strcpy ( str, "Cannot open file: ");

                  strcat ( str, FileName);

                  MessageBox (hWnd, str, szAppName, MB_OK);

                  break;

               }

               _lclose ( hFile );

               GetGame ( pof->szPathName );

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

#include "saveopen.h"



#define IDD_FNAME    0x10

#define IDD_FPATH    0x11

#define IDD_FLIST    0x12

#define IDD_DLIST    0x13

#define IDD_BROWSE   0x14



BOOL FAR PASCAL FileOpenDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam);

BOOL FAR PASCAL FileSaveDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam);

BOOL FAR PASCAL WildFileOpenDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam);



/* FileDlg.c */



static char szDefExt[5];

static char szFileName[96];

static char szFileSpec[16];

static WORD wFileAttr, wStatus;

static POFSTRUCT pof;



int FAR DoFileOpenDlg (HANDLE hInst, HWND   hWnd, char   *szFileSpecIn,

                       char   *szDefExtIn, WORD   wFileAttrIn,

                       char   *szFileNameOut, POFSTRUCT pofIn)

{

   FARPROC lpfnFileOpenProc;

   int iReturn;



   lstrcpy ( szFileSpec, szFileSpecIn);

   lstrcpy ( szDefExt,   szDefExtIn);

   wFileAttr = wFileAttrIn;

   pof = pofIn;



   lpfnFileOpenProc = MakeProcInstance ( FileOpenDlgProc, hInst);

   iReturn = DialogBox ( hInst, MAKEINTRESOURCE(FILEOPEN), hWnd, lpfnFileOpenProc);

   FreeProcInstance ( lpfnFileOpenProc);



   lstrcpy ( szFileNameOut, szFileName);

   return iReturn;

}



int FAR DoWildFileOpenDlg (HANDLE hInst, HWND   hWnd, char   *szFileSpecIn,

                           char   *szDefExtIn, WORD   wFileAttrIn,

                           char   *szFileNameOut, POFSTRUCT pofIn)

{

   FARPROC lpfnFileOpenProc;

   int iReturn;



   lstrcpy ( szFileSpec, szFileSpecIn);

   lstrcpy ( szDefExt,   szDefExtIn);

   wFileAttr = wFileAttrIn;

   pof = pofIn;



   lpfnFileOpenProc = MakeProcInstance ( WildFileOpenDlgProc, hInst);

   iReturn = DialogBox ( hInst, MAKEINTRESOURCE(WILDFILEOPEN), hWnd, lpfnFileOpenProc);

   FreeProcInstance ( lpfnFileOpenProc);



   lstrcpy ( szFileNameOut, szFileName);

   return iReturn;

}





int FAR DoFileSaveDlg (HANDLE hInst, HWND hWnd, char *szFileSpecIn,

                       char *szDefExtIn, WORD *pwStatusOut,

                       char *szFileNameOut, POFSTRUCT pofIn)

{

   FARPROC lpfnFileSaveProc;

   int iReturn;



   lstrcpy ( szFileSpec, szFileSpecIn);

   lstrcpy ( szDefExt,   szDefExtIn);

   pof = pofIn;



   lpfnFileSaveProc = MakeProcInstance ( FileSaveDlgProc, hInst);

   iReturn = DialogBox ( hInst, MAKEINTRESOURCE(FILESAVE), hWnd, lpfnFileSaveProc);

   FreeProcInstance ( lpfnFileSaveProc);



   lstrcpy ( szFileNameOut, szFileName);

   *pwStatusOut = wStatus;

   return iReturn;

}



BOOL FAR PASCAL FileOpenDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam)

{

   char cLastChar;

   short nEditLen;



   switch ( iMessage ){

      case WM_INITDIALOG:

         SendDlgItemMessage ( hDlg, IDD_FNAME, EM_LIMITTEXT, 80, 0L);

         DlgDirList ( hDlg, szFileSpec, IDD_FLIST, IDD_FPATH, wFileAttr);

         DlgDirList ( hDlg, szFileSpec, IDD_DLIST, NULL, 0x4010|0x8000);

         SetDlgItemText ( hDlg, IDD_FNAME, szFileSpec);

         return TRUE;



      case WM_COMMAND:

         switch (wParam) {

            case IDD_FLIST:

               switch ( HIWORD(lParam) ) {

                  case LBN_SELCHANGE:

                     if (DlgDirSelect (hDlg, szFileName, IDD_FLIST))

                        lstrcat ( szFileName, szFileSpec) ;

                     SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     break;



                  case LBN_DBLCLK:

                     if ( DlgDirSelect (hDlg, szFileName, IDD_FLIST)) {

                        lstrcat ( szFileName, szFileSpec);

                        DlgDirList ( hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                                       wFileAttr);

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     } else {

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                        SendMessage ( hDlg, WM_COMMAND, IDOK, 0L);

                     }

                     break;

               }

               break;



            case IDD_DLIST:

               switch ( HIWORD(lParam) ) {

                  case LBN_DBLCLK:

                     if ( DlgDirSelect (hDlg, szFileName, IDD_DLIST)) {

                        lstrcat ( szFileName, szFileSpec);

                        DlgDirList ( hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                                       wFileAttr);

                        DlgDirList ( hDlg, szFileName, IDD_DLIST, NULL, 0x4010|0x8000);

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     }

                     break;

               }

               break;





            case IDD_FNAME:

               if (HIWORD (lParam) == EN_CHANGE )

                  EnableWindow ( GetDlgItem(hDlg,IDOK),

                     (BOOL) SendMessage(LOWORD(lParam), WM_GETTEXTLENGTH,

                                         0,0L));

               break;



            case IDOK:

               GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80);

               nEditLen = lstrlen ( szFileName);

               cLastChar = *AnsiPrev (szFileName, szFileName+nEditLen);



               if ( cLastChar == '\\' || cLastChar == ':')

                  lstrcat ( szFileName, szFileSpec);



               if ( strchr (szFileName, '*') || strchr(szFileName, '?') ){

                  if ( DlgDirList (hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                   wFileAttr)) {

                     lstrcpy (szFileSpec, szFileName);

                     SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

                  } else

                     MessageBeep (0);

                  break;

               }

               lstrcat ( lstrcat(szFileName,"\\"), szFileSpec);



               if (DlgDirList ( hDlg, szFileName, IDD_FLIST,

                                                  IDD_FPATH,wFileAttr)) {

                  lstrcpy ( szFileSpec, szFileName);

                  SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

                  break;

               }



               szFileName[nEditLen] = '\0';



               if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                  lstrcat ( szFileName, szDefExt);

                  if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                     MessageBeep (0);

                     break;

                  }

               }

               lstrcpy ( szFileName,  AnsiNext(strrchr(pof->szPathName,'\\')));

               OemToAnsi( szFileName, szFileName);

               EndDialog (hDlg, TRUE);

               break;



            case IDCANCEL:

               EndDialog (hDlg, FALSE);

               break;



            default:

               return FALSE;

         }

      default:

         return FALSE;

   }

   return TRUE;

}



BOOL FAR PASCAL WildFileOpenDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam)

{

   char cLastChar;

   short nEditLen;



   switch ( iMessage ){

      case WM_INITDIALOG:

         SendDlgItemMessage ( hDlg, IDD_FNAME, EM_LIMITTEXT, 80, 0L);

         DlgDirList ( hDlg, szFileSpec, IDD_FLIST, IDD_FPATH, wFileAttr);

         DlgDirList ( hDlg, szFileSpec, IDD_DLIST, NULL, 0x4010|0x8000);

         SetDlgItemText ( hDlg, IDD_FNAME, szFileSpec);

         return TRUE;



      case WM_COMMAND:

         switch (wParam) {

            case IDD_FLIST:

               switch ( HIWORD(lParam) ) {

                  case LBN_SELCHANGE:

                     if (DlgDirSelect (hDlg, szFileName, IDD_FLIST))

                        lstrcat ( szFileName, szFileSpec) ;

                     SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     break;



                  case LBN_DBLCLK:

                     if ( DlgDirSelect (hDlg, szFileName, IDD_FLIST)) {

                        lstrcat ( szFileName, szFileSpec);

                        DlgDirList ( hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                                       wFileAttr);

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     } else {

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                        SendMessage ( hDlg, WM_COMMAND, IDOK, 0L);

                     }

                     break;

               }

               break;



            case IDD_DLIST:

               switch ( HIWORD(lParam) ) {

                  case LBN_DBLCLK:

                     if ( DlgDirSelect (hDlg, szFileName, IDD_DLIST)) {

                        lstrcat ( szFileName, szFileSpec);

                        DlgDirList ( hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                                       wFileAttr);

                        DlgDirList ( hDlg, szFileName, IDD_DLIST, NULL, 0x4010|0x8000);

                        SetDlgItemText ( hDlg, IDD_FNAME, szFileName);

                     }

                     break;

               }

               break;





            case IDD_FNAME:

               if (HIWORD (lParam) == EN_CHANGE )

                  EnableWindow ( GetDlgItem(hDlg,IDOK),

                     (BOOL) SendMessage(LOWORD(lParam), WM_GETTEXTLENGTH,

                                         0,0L));

               break;



            case IDD_BROWSE:

               GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80);

               nEditLen = lstrlen ( szFileName);

               cLastChar = *AnsiPrev (szFileName, szFileName+nEditLen);



               if ( cLastChar == '\\' || cLastChar == ':')

                  lstrcat ( szFileName, szFileSpec);



               if ( strchr (szFileName, '*') || strchr(szFileName, '?') ){

                  if ( DlgDirList (hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                   wFileAttr)) {

                     lstrcpy (szFileSpec, szFileName);

                     SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

                  } else

                     MessageBeep (0);

                  break;

               }

               lstrcat ( lstrcat(szFileName,"\\"), szFileSpec);



               if (DlgDirList ( hDlg, szFileName, IDD_FLIST,

                                                  IDD_FPATH,wFileAttr)) {

                  lstrcpy ( szFileSpec, szFileName);

                  SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

                  break;

               }



               szFileName[nEditLen] = '\0';



               if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                  lstrcat ( szFileName, szDefExt);

                  if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                     MessageBeep (0);

                     break;

                  }

               }

/*

               lstrcpy ( szFileName,  AnsiNext(strrchr(pof->szPathName,'\\')));

               OemToAnsi( szFileName, szFileName);

               EndDialog (hDlg, TRUE);

*/

               break;



           case IDOK:

               GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80);

               nEditLen = lstrlen ( szFileName);

               cLastChar = *AnsiPrev (szFileName, szFileName+nEditLen);



               if ( cLastChar == '\\' || cLastChar == ':')

                  lstrcat ( szFileName, szFileSpec);



               if ( strchr (szFileName, '*') || strchr(szFileName, '?') ){

                  if ( DlgDirList (hDlg, szFileName, IDD_FLIST, IDD_FPATH,

                                   wFileAttr)) {

/*

                     lstrcpy (szFileSpec, szFileName);

                     SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

*/

                     OemToAnsi( szFileName, szFileName);

                     EndDialog (hDlg, TRUE);

                  } else

                     MessageBeep (0);

                  break;

               }

               lstrcat ( lstrcat(szFileName,"\\"), szFileSpec);



               if (DlgDirList ( hDlg, szFileName, IDD_FLIST,

                                                  IDD_FPATH,wFileAttr)) {

                  lstrcpy ( szFileSpec, szFileName);

                  SetDlgItemText (hDlg, IDD_FNAME, szFileSpec);

                  break;

               }



               szFileName[nEditLen] = '\0';



               if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                  lstrcat ( szFileName, szDefExt);

                  if (OpenFile(szFileName, pof, OF_READ|OF_EXIST) == -1) {

                     MessageBeep (0);

                     break;

                  }

               }

               lstrcpy ( szFileName, AnsiNext(strrchr(pof->szPathName,'\\')));

               OemToAnsi( szFileName, szFileName);

               EndDialog (hDlg, TRUE);

               break;



           case IDCANCEL:

               EndDialog (hDlg, FALSE);

               break;



            default:

               return FALSE;

         }

      default:

         return FALSE;

   }

   return TRUE;

}



BOOL FAR PASCAL FileSaveDlgProc (HWND hDlg, unsigned iMessage, WORD wParam, LONG lParam)

{



   switch ( iMessage ){

      case WM_INITDIALOG:

         SendDlgItemMessage ( hDlg, IDD_FNAME, EM_LIMITTEXT, 80, 0L);

         DlgDirList ( hDlg, szFileSpec, 0, IDD_FPATH, 0);

         SetDlgItemText ( hDlg, IDD_FNAME, szFileSpec);

         return TRUE;



      case WM_COMMAND:

         switch (wParam) {

            case IDD_FNAME:

               if (HIWORD(lParam) == EN_CHANGE)

                  EnableWindow ( GetDlgItem(hDlg, IDOK),

                     (BOOL) SendMessage (LOWORD(lParam), WM_GETTEXTLENGTH,0,0L));

               break;



            case IDOK:

               GetDlgItemText (hDlg, IDD_FNAME, szFileName, 80);

               if ( OpenFile (szFileName, pof,OF_PARSE) == -1) {

                  MessageBeep (0);

                  break;

               }



               if ( !strchr ((PSTR)AnsiNext (strrchr (pof->szPathName,'\\')),'.'))

                  lstrcat (szFileName, szDefExt);



               if ( OpenFile (szFileName, pof, OF_WRITE|OF_EXIST) != -1) {

                  wStatus = 1;

               } else if ( OpenFile (szFileName, pof, OF_CREATE|OF_EXIST) != -1) {

                  wStatus = 0;

               } else {

                  MessageBeep(0);

                  break;

               }



               lstrcpy ( szFileName, AnsiNext(strrchr(pof->szPathName,'\\')));

               OemToAnsi(szFileName, szFileName);

               EndDialog (hDlg, TRUE);

               break;

            

            case IDCANCEL:

               EndDialog (hDlg, FALSE);

               break;



            default:

               return FALSE;

      }



      default:

         return FALSE;

   }



   return TRUE;

}

