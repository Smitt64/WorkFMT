/**********************************************************************
*        File Name:   viewed.h             February 7,1994
*       Programmer:   S. Kubrin
*      Description:   File viewer   25.08.1992
*                     File editor   25.02.1994
*  Public routines:
* Private routines:
 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/edit/viewed.h $
                  $Revision: 13 $
                  $Date: 27.01.00 19:02 $
                  $Author: Lebedev $
                  $Modtime: 26.01.00 18:52 $
***********************************************************************/

#ifndef __EDIT_VIEWED_H
#define __EDIT_VIEWED_H

#include "pexpdef.h"

#ifndef __BMSEARCH_H
    #include "bmsearch.h"
#endif

#ifndef   __EDIT_FBUFF_H
    #include "edit/fbuff.h"
#endif

// December 29,1995

// -----------------------------------------------------------------------------
#define  EFL_CANMODIFY  0x01
#define  EFL_NOCURSOR   0x02
#define  EFL_HEXVIEW    0x04
#define  EFL_TEXTVIEW   0x08  // Else test if Binary file use HEX view
#define  EFL_UNIX       0x08

#define  EFL_CANCREATE  0x10

#define  EFL_IMAGEVIEW  0x20

// -----------------------------------------------------------------------------
enum
   {
    V_RDONLY = 0,
    V_EDITOR = EFL_CANMODIFY,
    V_VIEWER = EFL_NOCURSOR,
    V_DUMPER = EFL_HEXVIEW,

    V_RDONLY_TXT = EFL_TEXTVIEW,
    V_EDITOR_TXT = EFL_CANMODIFY | EFL_TEXTVIEW,

    V_RDONLY_IMAGE = EFL_IMAGEVIEW
   };


#include <packpshn.h>

// -----------------------------------------------------------------------------
typedef struct tagRepSchData
      {
       SchData  sch;
       char     origSearch[127];
       char     origReplace[127];
       char     replaceData[127]; // In used codePage
      } RepSchData;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ           hist_elem;
       char           filename[_MAX_PATH];
       long           topY;
       short int      topX;
       long           Y;
       short int      X;
       char           isDump;
       unsigned char  codePage;
       char           reserv[40];
      } HistElem;

// -----------------------------------------------------------------------------
typedef struct tagFilePosition
      {
       LOBJ           listElem;
       char          *name;
       char          *fileName;
       char          *message;    // Error message or search string for tagsList
       long           topY;    // Window position or -1
       short int      topX;    // Ignored id topY == -1
       long           Y;    // Current position
       short int      X;
       char           isDump;
       unsigned char  codePage;
      } TFilePosition;

// -----------------------------------------------------------------------------
typedef struct tagSelectFilePosData
      {
       CNTLIST        *list;
       TFilePosition  *selectedPos;
      } TSelectFilePosData;

// -----------------------------------------------------------------------------
typedef struct
      {
       View  *v;
      } TDeactViewData;

// -----------------------------------------------------------------------------
typedef struct
      {
       const char  *incDir;
      } TGetRslDirsMSG;

// -----------------------------------------------------------------------------
typedef struct
      {
       const char  *fileName;
       char         localName[256];
       char         remoteName[256];
      } TFindServerFileMSG;

#include <packpop.h>


#ifdef __cplusplus
 extern "C" {
#endif

void ViewInitSearch(FBUFF *pF, RepSchData *data, int isReplace, unsigned char viewCodePage);
void ViewInsertForm(View *v, const char *flName, const char *name);


TFilePosition *AddFilePosition    (CNTLIST *list, const char *name, const char *filename, const char *mes,
                                   long topY, int topX, long Y, int X, unsigned char codePage);
void           DonePositionList   (CNTLIST *list);
void           InitPositionList   (CNTLIST *list, unsigned char flag);
void           RemoveFilePosition (CNTLIST *list, TFilePosition *pos);
TFilePosition *PrevFilePosition   (CNTLIST *list, TFilePosition *pos);
TFilePosition *NextFilePosition   (CNTLIST *list, TFilePosition *pos);
TFilePosition *FirstFilePosition  (CNTLIST *list);
TFilePosition *LastFilePosition   (CNTLIST *list);
void           CorrectPositionList(CNTLIST *list, ViewUpInfo *info, FBUFF *pF);

int ViewByFilePosition(View *v, int mode, TFilePosition *pos);

void InitPosStack(void);
void DonePosStack(void);

TFilePosition *PushCurPosition(View *v);
int PopCurPosition(View *v, int mode);
void CorrectPopStack(ViewUpInfo *info, FBUFF *pF);

void InitBookMarkList(const char *file);
void DoneBookMarkList(const char *file);
CNTLIST *GetBookMarkList(void);
TFilePosition *AddBookMark(View *v, const char *name);
int GoToBookMark(View *v, int mode, const char *name);
void CorrectBookMarks(ViewUpInfo *info, FBUFF *pF);

void DoneTagsBuffer(void);
int ProcessTag(View *v, int mode, const char *tagToFind, const char *tagsFile, int part);

View *NextView(View *v, int mode);
View *PrevView(View *v, int mode);
char *FirstHist(void);
CNTLIST *GetHistoryList (void);
void DoneHist(char *file);
void InitHist(char *file);
void AddHist(View *v);
void PositionHist(View *v);
TFilePosition *GetFilePositionFromHist(const char *fileName);


char *GetErrorMessage(void);
int BuildErrList(char *file);
void ClearErrList(void);
CNTLIST *GetErrorList(void);
void  CorrectErrorList(ViewUpInfo *info, FBUFF *pF);
View *PositionError(View *v, int next, int mode);
View *NewViewErr(int x, int y, int dx, int dy, int mode);

// -----------------------------------------------------------------------------
_RSPNLEXP extern RepSchData RSL_FAR  sch;

//****@H********* Member functions ***********************************
_RSPNLEXP int  InsertData (View *v, char *buf, int size);
_RSPNLEXP int  DeleteData (View *v, int size);
_RSPNLEXP int  DeleteLine (View *v);
_RSPNLEXP void DeleteBlock(View *v);

_RSPNLEXP View *NewView       (int x, int y, int dx, int dy, char *filename, int mode);
_RSPNLEXP View *NewViewMem    (int x, int y, int dx, int dy, int size, int mode);
_RSPNLEXP View *NewViewForBuff(int x, int y, int dx, int dy, FBUFF *pF, int mode);



_RSPNLEXP View *ViewRealloc    (View *v, int size);
_RSPNLEXP void  ViewSetString  (View *v, char *str);
_RSPNLEXP void  ViewGetString  (View *v, char *buf, long size);
_RSPNLEXP void  ViewAddString  (View *v, char *str, int len);
_RSPNLEXP int   ViewSetUnixMode(View *v, int mode);
_RSPNLEXP void  ViewSetOverMode(View *v, int over);

_RSPNLEXP void   DoneView      (View *v);
_RSPNLEXP void   DetachView    (View *v);
          FBUFF *ViewDetachBuff(View *v);
_RSPNLEXP void   AttachView    (View *v, FBUFF *pF, int *replaced);
_RSPNLEXP FBUFF *GetFbuff(const char *fileName, int zbFlag, int withUndo, unsigned char codePage);
_RSPNLEXP void   CheckSyntaxColor(View *v,int assume);

_RSPNLEXP void  DrawView    (View *v);
_RSPNLEXP void  DrawCurLine (View *v);
_RSPNLEXP void  DrawCursor  (View *v);
_RSPNLEXP void  UpdateView  (View *v);
_RSPNLEXP void  SelectView  (View *v, int activate);
_RSPNLEXP int   HandleEvent (View *v, int event, int kbflags);
_RSPNLEXP void  SetFocusTo  (View *v, long poz, int colom, int dx, int dy);

_RSPNLEXP int   SearchLine  (View *v, RepSchData *sch);
_RSPNLEXP int   ReplaceLine (View *v, RepSchData *sch);

// -----------------------------------------------------------------------------
typedef long (*VDialogProc_f)(View *v, int action, ...);

_RSPNLEXP void  ViewSetDialog(View *v, VDialogProc_f fun);
_RSPNLEXP void  ViewSetColors(View *v, unsigned normattr, unsigned selattr, unsigned activeattr);

_RSPNLEXP void  SetViewBuff (unsigned view,unsigned edit,unsigned change);

_RSPNLEXP View *ViewChangePosition (View *v,char *fullname,long Y,int X,int mode,int *change);
_RSPNLEXP int   IsViewChange (View *v);
_RSPNLEXP int   ViewSave     (View *v);
_RSPNLEXP int   ViewCanClose (View *v);
_RSPNLEXP int   ViewCanExit  (void);
_RSPNLEXP int   ViewSaveAll  (void);
_RSPNLEXP int   ViewReloadAll (void);
_RSPNLEXP int   ViewSetTabSize (int sz);
_RSPNLEXP int   ViewSetTabToSpace (int tabToSpace);
_RSPNLEXP void  ViewSetDefaultCodePage (unsigned char file, unsigned char view);
_RSPNLEXP void  ViewSetPartLineVisiable (View *v, int isPart);
_RSPNLEXP int   ViewSetIndentMode (View *v, int isIndent);
unsigned char ViewDefCodePage (void);


_RSPNLEXP void  ScrollTo   (View *v, long line, int colom);
_RSPNLEXP void  DScrollTo  (View *v, long dl, int colom);

void  ScrollToOffset (View *v, long offs, int colom);
void ViewAdjustPosition (View *v);


_RSPNLEXP void  ViewSuspend   (View *v);
_RSPNLEXP int   ViewResume    (View *v, int mode);
_RSPNLEXP int   ViewSelfInsert(View *v, int event);

_RSPNLEXP int   ViewOver      (View *v);

_RSPNLEXP void  DoneClipBoard (void);
int WriteClip (const char *file);
int ReadClip (const char *file);

_RSPNLEXP void  SetMark (View *v,char SelType,int isFixed);
_RSPNLEXP void  Paste (View *v);
_RSPNLEXP void  Copy (View *v);
_RSPNLEXP void  Cut (View *v);

_RSPNLEXP long  VD_Default( View *v, int fun,... );
_RSPNLEXP int   PrintEngine (View *v, RunBlockProc proc, RunBlockData *data);


_RSPNLEXP int ViewSetNewName(View *v, char *newname, int doSave, int encode);
_RSPNLEXP int ReadWord      (View *v, char *buff, int sz);
_RSPNLEXP int MarkWord      (View *v);

_RSPNLEXP View *DeactivatePrevView (void);

int  LineHandle    (View *v, int event, int shiftState);
void SetDumpData   (View *v);
int  DumpLineHandle(View *v, int event, int kbflags);

void ConsoleOutputLine     (View *v, int y, const char *buf, int sz, int nMark, const TMarkPos *mark, unsigned char *attrStr);
void ConsoleOutputLineEnded(View *v);

void KedCvtLineTo (int codePage, int fromCodePage, char* buff);
void KedCvtBuffTo (int codePage, int fromCodePage, char* buff, int len);

View *NewViewMenu (int x,int y,int dx, int dy, const char *head, FBUFF *pF);

// -----------------------------------------------------------------------------
enum EditManagerCmd
   {
    RSEM_LoadFile  = 0x01,
    RSEM_SaveAll,
    RSEM_ReloadAll,
    RSEM_CheckOut,
    RSEM_CheckIn,
    RSEM_UndoCheckOut,
    RSEM_CallRsBank,
    RSEM_SetDesignerId,
    RSEM_GetRslDirs,
    RSEM_FindServerFile
   };

typedef int (*EditManagerCallback)(int cmd, const char *fname, int mode);
typedef int (*EditManagerCallbackEx)(int cmd, void *msgData);

_RSPNLEXP bool ConnectToRsEditManager (const char *diplayName, EditManagerCallback func, int isAnsi);
_RSPNLEXP bool ConnectToRsEditManagerEx (const char *diplayName, EditManagerCallback func, int isAnsi, EditManagerCallbackEx func2);
_RSPNLEXP bool DisconnectFromRsEditManager ();
_RSPNLEXP unsigned long GetRsEditorCookie (void);


int ViewLoadFile(int cmd, const char *flName, int mode);

_RSPNLEXP int ReadFromFile(View *v, const char *filename);

#ifdef __cplusplus
}
#endif


//********************** END  Class View ******************************




//**@H****************** BEGIN Event  handling ************************

enum
   {
    LINEUP = 2001,
    LINEDN,
    CHARLT,
    CHARRT,
    TPGUP,
    TPGDN,
    TBEG,
    TEND,
    TLBEG,
    TLEND,
    UNSELBLK,
    PRNBLK,
    GOTOPOS,
    SEARCH,
    SEARCHN,
    VHEX,
    SAVEFILE,
    VREMLINE,
    INSWITCH,
    SELSTREAM,
    SELLINE,
    SELCOLOMN,
    BLCOPY,
    BLPASTE,
    BLCUT,
    VREPLACE,
    VUNDO,
    SCROLLUP,
    SCROLLDN,
    SCROLLLT,
    SCROLLRT,
    VREMEND,
    GOTOP,
    GOBOTTOM,
    GOLEFT,
    GORIGHT,
    WORDLEFT,
    WORDRIGHT,
    DELWLEFT,
    DELWRIGHT,
    CRLTOP,
    CRLBOT,
    CRLCEN,
    SCROLLUP3,
    SCROLLDN3,
    BLPASTE2,
    BLCOPY2,
    BLCUT2,
    TLBEG2
   };

// -----------------------------------------------------------------------------
#define  VA_CANCEL  1
#define  VA_NO      2
#define  VA_YES     3
#define  VA_GLOBAL  4
#define  VA_WRITE   5
#define  VA_EXIT    6
//********************** END    Event  handling ************************

#endif  // __EDIT__VIEWED_H

/* <-- EOF --> */