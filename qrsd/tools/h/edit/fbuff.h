//-*--------------------------------------------------------------------------*-
//
// File Name   : fbuff.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// February 4,1994  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-
#ifndef __EDIT_FBUFF_H
#define __EDIT_FBUFF_H

#include "pexpdef.h"

#ifndef __EDIT_VBUFF_H
    #include "edit/vbuff.h"
#endif

#ifndef __FS_H
    #include "fs.h"    // for TMarkPos
#endif

// -----------------------------------------------------------------------------
#define  EOLINE      ('\n')
#define  EOL10       ('\n')
#define  EOL13       ('\r')

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif

//**@H******************** BEGIN CLASS FBUFF ***************************
typedef struct tagSelArea
      {
       char   SelType;              // Тип выделения
       long   SbY;                  // Начало выделенной области
       int    SbX;
       long   SeY;                  // Конец выделенной области
       int    SeX;
       long   offsB, offsE;
       char   isFixed;
       char   expandTop;
      } SelArea;

// -----------------------------------------------------------------------------
typedef struct tagFBUFF  FBUFF;

// -----------------------------------------------------------------------------
struct tagFBUFF
     {
      VBUFF           vbuff;        // Виртуальный буфер

      long            boffset;      // offset in file of the first byte in buffer
      unsigned char  *pCur;         // Curent pozition in buffer
      unsigned char  *pEnd;         // Last pozition in buffer

      // Текущая позиция в файле для вставки и удаления.
      // Разделена на две части. Реальная строка Y+vy, колонка X+vx.
      long            offs;         // Смещение в файле текущей позиции
      long            Y;
      int             X;
      int             vx;
      long            vy;

      int             bsize;        // text buffer size. It must be divided to 1024
      unsigned char  *buff;
      int             dbeg;
      int             dend;

      long            lastline;     // Номер последней строки в файле
      CNTLIST         view_list;    // List of View objects

      SelArea         sel;          // Выделенная область

      long            SaveY;        // Здесь сохраняем текущую позицию
      int             SaveX;        // FbSaveCurPos

      long            curLineOffs;

      FBUFF          *undo;
      unsigned        grpUndoLevel;
      unsigned        lockCount;
      unsigned char   unlockAll;
      unsigned char   codePage;
     };


//-@H------ BEGIN __Syntax coloring____________________________ --------@@-

#define  MAXTOKLEN  127

// -----------------------------------------------------------------------------
typedef enum
      {
       SEQ_NONE,            //
       SEQ_WORD,            //
       SEQ_SPECWORD,        //
       SEQ_STRING,          //
       SEQ_CMT1,            //
       SEQ_CMT2,            //
       SEQ_NUMBER,          //
       SEQ_OPER             //
      } SEQTYPE;

// -----------------------------------------------------------------------------
typedef enum
      {
       KED_NOCOLOR = 0,     //
       KED_KEYWORD,         //
       KED_STDFUNC,         //
       KED_TYPEWORD         //
      } KEDCOLORID;

// -----------------------------------------------------------------------------
typedef struct tagSyntColors
      {
       unsigned char  keyColor;
       unsigned char  typeColor;
       unsigned char  stdFuncColor;
       unsigned char  numberColor;
       unsigned char  operColor;
       unsigned char  stringColor;
       unsigned char  specWordColor;
       unsigned char  comentColor;
      } TSyntColors;

// -----------------------------------------------------------------------------
typedef struct tagKeyWordColorTable
      {
       char       **tokens;
       KEDCOLORID   color;
      } TKeyWordColorTable;

// -----------------------------------------------------------------------------
typedef struct tagSyntColorInfo
      {
       TKeyWordColorTable   *keyTable;
       char                 *operString;
       int                   lang; // 0 - RSL, 1 - C/C++
      } TSyntColorInfo;

// -----------------------------------------------------------------------------
typedef struct tagTokenData
      {
       SEQTYPE          seqType;
       int              tokStartPos;
       int              tokLen;
       int              level;
       char            *curPtr;
       char             tokenBuff[MAXTOKLEN];

       TSyntColorInfo  *syntaxInfo; // For Syntax coloring
       TSyntColors     *colorsTable;
       unsigned char    codePage;
      } TTokenData;

// -----------------------------------------------------------------------------
extern TSyntColorInfo  RSLSyntInfo;
extern TSyntColorInfo  CPPSyntInfo;

// -----------------------------------------------------------------------------
void TokenReset    (TTokenData *pT);
void TokenWriteAttr(TTokenData *pT, int dl, int br, unsigned char *attrStr);
int  TokenCheckCh  (TTokenData *pT, int ch, int pos, int *handled);


//-@@------ END __Syntax coloring______________________________ --------@E-

// Тип выделения
#define  ST_STREAM   1
#define  ST_LINES    2
#define  ST_COLOMN   3
#define  ST_OFFS     4

// -----------------------------------------------------------------------------
typedef struct tagView
      {
       int       x, y, dx, dy;    // View coorditans on the screen
       long      topoffs;         // Offset of the first line in view window
       long      topY;            // Line number of the first line in view window
       int       topX;            // Colomn number of the first line in view window

       long      oldY;            // Здесь запоминаем текущую позицию во View
       int       oldX;            // во время переключения View
       SelArea   oldsel;          // Выделенная область

       LOBJ      view_elem;       // View list element
       FBUFF    *pF;              // Подключенный буфер
       int       flags;           // VF_ флаги состояния

       void     *ob;              // Данные для поддержки оконной системы

       unsigned  normattr;
       unsigned  selattr;
       unsigned  activeattr;

       void (*FormatLine)(FBUFF *pF, int  dl, int outlen, long Y, int *nMark, TMarkPos *mark, unsigned selattr, TTokenData *pT);
       void (*OutputLine)(struct tagView *v, int y, const char *buf, int sz, int nMark, const TMarkPos *mark, unsigned char *attrStr);

       int  (*Handle)(struct tagView *v, int event, int kbflags);

       void (*SwapProc)(FBUFF *pF, int dl, int outlen, long Y, int *nMark, TMarkPos *mark, unsigned, TTokenData *pT);
       int  (*GetOutlen)(int len, int *pad);
       int  (*SwapOutlen)(int len, int *pad);

       long  (*VDialog)(struct tagView *v, int action, ...);

       char      OverLap;

       TSyntColorInfo  *syntaxInfo;   // For Syntax coloring
       TSyntColors     *colorsTable;

       unsigned char    codePage;     // KED_OEM or KED_ANSI
      } View;

// -----------------------------------------------------------------------------
#define  VF_CURLOCK        1   // Для поддержки режима просмотра без курсора
#define  VF_SELECTED       2   // 1 если фокус ввода в данном View
#define  VF_DUMP           8   // Dump Mode flag
#define  VF_INDENTMODE    16
#define  VF_PARTVISIABLE  32   // Last line is partially visiable.

// -----------------------------------------------------------------------------
// Структура с информацией необходимой для модификации данных во View
typedef struct
      {
       long   offs;   // Смещение и строка в которой произведены изменения
       long   Y;

       long   nbytes; // Количество байт и строк которые добавлены или удалены
       long   nlines; // < 0 , если удаление

       long   Yb;     // Y + nlines;  (необходимо для удаления)
       long   offsb;  // offs + nbytes;
      } ViewUpInfo;

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned char  ch;
       unsigned char  at;
      } DRAWBUF;

//**@H*************** BEGIN работа с блоками ****************************
typedef struct
      {
       FBUFF  *pF;
       int     bX, eX, bX2, eX2, bX3, eX3;
       long    Y;      // Номер текущей строки
       long    done;   // Количество обработанных строк
       long    nlines; // Количество строк для обработки
      } RunBlockData;

// Можно использовать наследников RunBlockData

// -----------------------------------------------------------------------------
typedef int (*RunBlockProc)(RunBlockData *data);

// -----------------------------------------------------------------------------
_RSPNLEXP int RunBlockLines   (FBUFF *pF, RunBlockProc proc, RunBlockData *data, int flag);
_RSPNLEXP int ChangeBlockLines(FBUFF *pF, RunBlockProc proc, RunBlockData *data);

// -----------------------------------------------------------------------------
int IndentBlock  (FBUFF *pF);
int UnIndentBlock(FBUFF *pF);

// -----------------------------------------------------------------------------
// Значения flag
#define  RUN_GLOBAL   1  // - во всем файле
#define  RUN_BLOCK    2  // - в блоке
#define  RUN_TOP_50   4  // - Top 50 lines

//******************* END    работа с блоками ****************************

enum VDialogCmd
   {
    VD_initp,             //
    VD_showp,             //
    VD_donep,             //
    VD_pos,               //
    VD_focuslN,           //
    VD_focusoN,           //
    VD_focusl,            //
    VD_focuso,            //
    VD_search,            //
    VD_searchN,           //
    VD_notfound,          //
    VD_nosearch,          //
    VD_notchange,         //
    VD_exit,              //
    VD_begsave,           //
    VD_save,              //
    VD_endsave,           //
    VD_overtype,          //
    VD_changeinfo,        //
    VD_linecut,           //
    VD_blockcut,          //
    VD_linecopy,          //
    VD_blockcopy,         //
    VD_blockdel,          //
    VD_paste,             //
    VD_replace,           //
    VD_repanswer,         //
    VD_nrep,              //
    VD_undo,              //
    VD_close,             //
    VD_message,           //
    VD_ersave,            //
    VD_eropen,            //
    VD_nomes,             //
    VD_newfile,           //
    VD_printCANCEL,       //
    VD_printOK,           //
    VD_print,             //
    VD_exist,             //
    VD_eropenWrite,       //
    VD_reload,            //
    VD_noundo,            //
    VD_selectTag,         //
    VD_bkmDrop,           //
    VD_notag,             //
    VD_nobkm,             //
    VD_fileSaved,         //
    VD_fileSaveError,     //
    VD_begHistList,       //
    VD_endHistList,       //
    VD_notagFile,         //
    VD_noform,            //
    VD_setnewname,        //
    VD_fileRemoved,       //
    VD_viewActivated,     //
    VD_viewDeactivated,   //
    VD_clipLimit,         //
    VD_fileSaveAsError
   };

// -----------------------------------------------------------------------------
_RSPNLEXP extern void (*FormatLine)(FBUFF *pF, int dl, int outlen, long Y, int *nMark, TMarkPos *pos);

//**@H************ MEMBER functions *************************************

_RSPNLEXP void SetSyntaxColors(View *v, TSyntColors *clr);

_RSPNLEXP int  FbSave            (FBUFF *pF);
_RSPNLEXP int  FbTestClose       (FBUFF *pF, VBCMDDATA *dt);
_RSPNLEXP int  FbCheckAndReload  (FBUFF *pF);

_RSPNLEXP int  FbSaveCurPos      (FBUFF *pF);
_RSPNLEXP int  FbRestCurPos      (FBUFF *pF);

_RSPNLEXP int  ReadLine          (FBUFF *pF, char  *buf, int len);
_RSPNLEXP int  ReadBytes         (FBUFF *pF, char  *buf, int len);
_RSPNLEXP void FormatLineText    (FBUFF *pF, int  dl, int outlen, long Y, int *nMark, TMarkPos *pos, unsigned selattr, TTokenData *pT);
_RSPNLEXP int  DoWrap            (FBUFF *pF, int  dl, int outlen, int *X);
_RSPNLEXP void FormatLineTextWrap(FBUFF *pF, int  dl, int outlen, long Y, int *nMark, TMarkPos *mark, unsigned selattr, TTokenData *pT);
_RSPNLEXP void FormatFixLineA    (FBUFF *pF, int  dl, int outlen, long Y, int *nMark, TMarkPos *pos, unsigned selattr, TTokenData *pT);
_RSPNLEXP void FormatFixLineH    (FBUFF *pF, int  dl, int outlen, long Y, int *nMark, TMarkPos *pos, unsigned selattr, TTokenData *pT);
_RSPNLEXP void FbUpdateAllViews  (FBUFF *pF, ViewUpInfo *inf);

long CalcPosForOffset(FBUFF *pF, long beginOffs, long beginY, long dest, long *destY, int *destX, int outlen);

_RSPNLEXP int  FbGoTo            (FBUFF *pF, long Y, int X);
_RSPNLEXP int  FbFindCol         (FBUFF *pF, int  X, long *offset);
_RSPNLEXP long FbFindLine        (FBUFF *pF, long Y, long *offset);
_RSPNLEXP long FbFindLineWrap    (FBUFF *pF, long Y, long *offset, int outlen);

int FbGoToWrap   (FBUFF *pF, long Y, int X, int outlen);
int FbFindColWrap(FBUFF *pF, int X, long *offs, int outlen);

long FbFindLineAtOffset    (FBUFF *pF, long offs, long *offset);
long FbFindLineAtOffsetWrap(FBUFF *pF, long Y, long *offset, int outlen);


// -----------------------------------------------------------------------------
_RSPNLEXP FBUFF *NewFbuff   (char *fname, unsigned bsize, unsigned vsize, int flag);
          FBUFF *NewFbuffEncode(char *fname, unsigned bsize, unsigned vsize, int flag, unsigned char codePage);
_RSPNLEXP FBUFF *FindFbuff  (char *filename);
_RSPNLEXP void   DoneFbuff  (FBUFF *pF);
_RSPNLEXP void   FreeFbuff  (FBUFF *pF);
_RSPNLEXP void   FbSuspend  (FBUFF *pF);
_RSPNLEXP long   FbDataSize (FBUFF *pF);
_RSPNLEXP int    FbResume   (FBUFF *pF, unsigned bufsize, unsigned vsize);
          int    FbReset    (FBUFF *pF);

_RSPNLEXP int FbGetForward  (FBUFF *pF);
_RSPNLEXP int FbGetBackward (FBUFF *pF);
_RSPNLEXP int FbGetBufChar  (FBUFF *pF);
_RSPNLEXP int FbGetPrevChar (FBUFF *pF);

_RSPNLEXP long FbGetPosition(FBUFF *pF);
_RSPNLEXP int FbSetPosition (FBUFF *pF, long pos);

_RSPNLEXP int FbFillToend   (FBUFF *pF);
_RSPNLEXP int FbReadBuff    (FBUFF *pF, long offset);

_RSPNLEXP int FbInsert      (FBUFF *pF, char *buf, int size);
_RSPNLEXP int FbDelete      (FBUFF *pF, int size);
_RSPNLEXP void FbLock       (FBUFF *pF);
_RSPNLEXP void FbUnlock     (FBUFF *pF, int UpAll);
_RSPNLEXP int FbDeleteBlock (FBUFF *pF);
_RSPNLEXP int FbCopy        (FBUFF *pF);
_RSPNLEXP int FbPaste       (FBUFF *pF);
_RSPNLEXP int FbCut         (FBUFF *pF);
_RSPNLEXP int FbClipSelType (void);
_RSPNLEXP FBUFF *NewClipBoard(void);
_RSPNLEXP int FbCopyColomn  (FBUFF *pF);
_RSPNLEXP int FbPasteColomn (FBUFF *pF);
_RSPNLEXP int FbDelColomn   (FBUFF *pF);

_RSPNLEXP int    GoToPrevLine  (FBUFF *pF);
_RSPNLEXP long GoToLineBegin(FBUFF *pF, long offs);
_RSPNLEXP long GoToLineEnd  (FBUFF *pF, long offs);
_RSPNLEXP int  GoToNextLine (FBUFF *pF);

_RSPNLEXP int  FbAllocBuff  (FBUFF *pF, unsigned *bsize, unsigned *vsize);

_RSPNLEXP int  IsTextFile   (FBUFF *pF);

_RSPNLEXP void FbSetMark    (FBUFF *pF, char SelType);
_RSPNLEXP void FbExpandMark (FBUFF *pF);

// -----------------------------------------------------------------------------
_RSPNLEXP extern char  *linebuf;    // Имеет размер LINELEN*2+2
_RSPNLEXP extern int    LINELEN;

// -----------------------------------------------------------------------------
extern int  RsTabSize;
extern int  RsTabToSpace;

int   ScipTag      (FBUFF *pF, int ch, char **buf);
char *ScipTagBuff  (FBUFF *pF, char *buf, int *size);
int   FndPos       (FBUFF *pF, char *buf, int pos, int offs);
void  ScipEndOfLine(FBUFF *pF);
void  SetViewTagMode (View *pF, int hide);
void  SetFileCodePage(View *v, int codePage);


int FbReopenForWrite (FBUFF *pF,int *isReset);

void FbInsVirtual  (FBUFF *pF, ViewUpInfo *info);
long FbBlockSize   (FBUFF *pF, int move_cur_pos);
void FbTransferSize(FBUFF *from, FBUFF *to, long size);
void FbTransfer    (FBUFF *from, FBUFF *to);

int  FbInsertData  (FBUFF *pF, char *buf, int size);
int  FbDeleteData  (FBUFF *pF, int size, int *delBytes);
int  FbReadData    (FBUFF *pF, char *buff, int size);
long FbMoveData    (FBUFF *from, FBUFF *to, long size, int isLineInsert, int *numberOfLines);

void FbSetCodePage (FBUFF *pF, unsigned char codePage);

// -----------------------------------------------------------------------------
typedef int (*TFndViewProc)(View *v, void *data);

_RSPNLEXP View *GetExistView (const char *fileName, TFndViewProc func, void *data);
_RSPNLEXP View *FbGetBestView(FBUFF *pF, TFndViewProc func, void *data);

//********************** END   CLASS FBUFF ***************************


//-@H------ BEGIN __ Undo support _____________________________ --------@@-

typedef enum
      {
       UNDO_INSERTDATA,
       UNDO_DELETEDATA,
       UNDO_BEGINGROUP,
       UNDO_ENDGROUP,
       UNDO_SAVEPOS,
       UNDO_FILENOTCHANGED
      } TUndoAction;

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned char  action;
       short int      x;
       long           y;
       int            n;
      } TUndoInfo;

// -----------------------------------------------------------------------------
FBUFF *SetUpUndoBuff      (FBUFF *pF, int addBuffNotChanged);
int    FbLogUndoInsert    (FBUFF *pF, long Y, int X, int size);
int    FbLogUndoDelete    (FBUFF *pF, long Y, int X, int size);
int    FbLogBeginGroup    (FBUFF *pF, long Y, int X);
int    FbLogEndGroup      (FBUFF *pF, int updateAll);
int    FbLogSavePos       (FBUFF *pF, long Y, int X);
int    FbLogFileNotChanged(FBUFF *pF);
int    FbMakeUndo         (FBUFF *pF);


//-@@------ END __ Undo support _______________________________ --------@E-

int FbSearchTag           (FBUFF *pF, const char *strToFind, int part);
int GetPosOfNonWhiteSpace (FBUFF *pF, long offs);
int GetPosOfNonWhiteSpace2(FBUFF *pF, long offs);


int FbSearchString (FBUFF *pF, const char *strToFind, int sFlags, int blFlags, unsigned char codePage);
int FbReplaceString(FBUFF *pF, const char *strToFind, const char *repStr, int sFlags, int blFlags, unsigned char codePage);

int FbProcessForms (FBUFF *pF, const char *formsFile, const char *formName);

// -----------------------------------------------------------------------------
#define  INMEM(pF)     ((pF)->vbuff.flags & ZB_NOFILE)
#define  UNIXMODE(pF)  ((pF)->vbuff.flags & ZB_UNIXMODE)
#define  ISBINARY(pF)  ((pF)->vbuff.flags & ZB_BINARY)

// -----------------------------------------------------------------------------
int FbUseWinClipboard(void);
int FbSetUseWinClipboard(int use);

int GetNextTabPos(int pos);
int GetPrevTabPos(int pos);

char *FbGetTagLine(FBUFF *pF, const char *prefix);
void FbFreeTagLine(char *ptr);

#ifdef __cplusplus
} ;
#endif

#include <packpop.h>

#endif   // __EDIT_FBUFF_H

/* <-- EOF --> */