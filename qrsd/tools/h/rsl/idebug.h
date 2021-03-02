/*-----------------------------------------------------------------------*-

 File Name   : idebug.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/idebug.h $
               $Revision: 33 $
               $Date: 22.09.00 19:02 $
               $Author: Kubrin $
               $Modtime: 22.09.00 16:03 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 December 3,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __IDEBUG_H
#define __IDEBUG_H


#ifndef __DBGINTF_H
#  include "rsl/dbgintf.h"
#endif

//-@H------ BEGIN __New iterators objects______________________ --------@@-

// -----------------------------------------------------------------------------
typedef struct
      {
       int          numVar;
       int          numFunc;
       int          numProp;

       TGenObject  *obj;
       VALUE        value;
      } TRslObjIter;

// -----------------------------------------------------------------------------
typedef struct
      {
       char   *name;
       VALUE   value;
      } TRslVarInfo;

// -----------------------------------------------------------------------------
typedef struct
      {
       int        numVar;
       int        numFunc;
       RSLMODULE  hmod;
      } TRslGlobIter;

// -----------------------------------------------------------------------------
typedef struct
      {
       int        numLocals;
       int        numFormal;
       int        numGlobals;
       RSLSTACK   hst;
      } TRslLocalIter;

// -----------------------------------------------------------------------------
typedef struct
      {
       long         idNext;
       long         idItem;
       int          tpNext;
       int          tpItem;

       int          curId;

       TGenObject  *enumObj;
       VALUE        value;
      } TRslArrIter;

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned char       tag;

       union
           {
            TRslObjIter    obj;
            TRslGlobIter   glob;
            TRslLocalIter  loc;
            TRslVarInfo    var;
            TRslArrIter    arr;
           } u;
      } TRslIter;


// -----------------------------------------------------------------------------
enum
   {
    RSL_ITER_OBJ = 1,
    RSL_ITER_GLOB,
    RSL_ITER_LOC,
    RSL_ITER_VAR,
    RSL_ITER_ARRAY
   };

#ifdef __cplusplus
extern "C" {
#endif

int  ArrayIterInit      (TRslIter *iter, TGenObject *obj);

int  ObjIterInit        (TRslIter *iter, TGenObject *obj);
int  GlobIterInit       (TRslIter *iter);
int  LocalIterInit      (TRslIter *iter, RSLSTACK hst);
int  RslIterGet         (TRslIter *iter, int num, int kind, TMemberInfo *info);
int  RslIterGetID       (TRslIter *iter, long id, TMemberInfo *info);
int  RslIterGetValue    (TRslIter *iter, long id, VALUE **v);
int  RslIterSetValue    (TRslIter *iter, long id, VALUE *v);
int  RslIterRefreshValue(TRslIter *iter, long id, VALUE **v);
void RslIterInit        (TRslIter *iter);
void RslIterDone        (TRslIter *iter);
void RslIterCopy        (TRslIter *to, TRslIter *from, VALUE **val);

#ifdef __cplusplus
}
#endif

//-@@------ END __New iterators objects________________________ --------@E-

// -----------------------------------------------------------------------------
struct BreakData
     {
      int             num;
      TRslBreakInfo  *ptr;
      int             cur;
      int             setNumber;
     };

// -----------------------------------------------------------------------------
typedef struct
      {
       TGenObject  *obj;
       int          nProps;

       VALUE        value;
      } RSLPROP;

// -----------------------------------------------------------------------------
typedef struct
      {
       TGenObject   *obj; // Common part with RSLPROP!
       int           nProps;
      } RSLFUNINFO;

// -----------------------------------------------------------------------------
typedef struct
      {
       RSLSTACK     hst;
       int          numLocals;
       int          numFormal;
       int          numGlobals;
      } RSLLOCAL;

// -----------------------------------------------------------------------------
typedef struct
      {
       RSLMODULE   hmod;
       int         numGlobals;
      } RSLGLOBAL;

// -----------------------------------------------------------------------------
typedef struct
      {
       const char  *name;
       const char  *tpName;  
       VALUE       *val;
       int          parNumber;  // if name == NULL: number of parms
       long         rslId;      // uniq Id of value

       TRslIter     iter;
      } TRslValueInfo;

// -----------------------------------------------------------------------------
#ifndef RSL_DLM
    typedef struct
          {
           LOBJ           listElem;
           TRslValueInfo  info;
           int            id;
           RSLMODULE      hmod;

        #ifdef USE_DBGHANDLE32
           RslHandle32    key;
        #endif
           char          *newVal;
          } TRslValueInfoEx;
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       int   numVar;
       int   numFunc;
       bool  calcStatic;
      } TGlobInfo;

// -----------------------------------------------------------------------------
#define  VAL_EMPTY    0
#define  VAL_LOC      1
#define  VAL_PROP     2
#define  VAL_GLOB     3
#define  VAL_FUNC     4

// -----------------------------------------------------------------------------
typedef struct
      {
       int  offs;
       int  len;
      } TRslEnumStmt;


#ifdef __cplusplus
extern "C" {
#endif

RSLLINK int        RSLAPI RslSetDbgFlag        (int newFlag);
RSLLINK int        RSLAPI RslSetTraceFlag      (int newFlag);

RSLLINK RSLSTACK   RSLAPI RslGetStackTop       (void);
RSLLINK RSLSTACK   RSLAPI RslGetStackPrev      (RSLSTACK);

RSLLINK uintptr_t  RSLAPI RslGetStackAddress   (RSLSTACK cur);

RSLLINK RSLMODULE  RSLAPI RslFindModuleByName  (const char *name);
RSLLINK RSLMODULE  RSLAPI RslFindModuleByCookie(unsigned long cookie);
RSLLINK uintptr_t  RSLAPI RslGetModuleCookie   (RSLMODULE);
RSLLINK char      *RSLAPI RslGetModuleFile     (RSLMODULE hmod, int *isBtrStream);
RSLLINK char      *RSLAPI RslGetModuleName     (RSLMODULE hmod);

RSLLINK RSLMODULE  RSLAPI RslGetModuleFirst    (void);
RSLLINK RSLMODULE  RSLAPI RslGetModuleNext     (RSLMODULE hmod);

RSLLINK RSLMODULE  RSLAPI RslModuleForStatement(RSLSTMT);

RSLLINK RSLMODULE  RSLAPI RslGetModule         (RSLSTACK);
RSLLINK RSLPROC    RSLAPI RslGetProc           (RSLSTACK);
RSLLINK char      *RSLAPI RslGetProcName       (RSLSTACK);
RSLLINK RSLSTMT    RSLAPI RslGetStatement      (RSLSTACK);

RSLLINK RSLSTMT    RSLAPI RslGetStatementOfPos (RSLMODULE hmod, int offs, int len);
RSLLINK void       RSLAPI RslGetStatementPos   (RSLSTMT hst, int *offs, int *len);
void                      RslGetStatementPosEx (RSLSTMT hst, int *offs, int *len, int *line);
RSLLINK RSLSTMT    RSLAPI RslEnumStatementOfPos(RSLMODULE hmod, TRslEnumStmt *p);

RSLLINK void       RSLAPI RslBreakPointAt      (RSLSTMT stmt, void *data);
RSLLINK void       RSLAPI RslClrBreakPoint     (RSLSTMT stmt);

RSLLINK int        RSLAPI RslSetBreakPoint     (RSLMODULE hm, int num, TRslBreakInfo *ptr);
RSLLINK void       RSLAPI RslRemBreakPoint     (RSLMODULE hm, int num, TRslBreakInfo *ptr);

bool                      RslDbgExecExpression (const char *txt);

int                       RslInitObjectIter    (TGenObject *obj, TRslValueInfo *data);

RSLLINK void       RSLAPI RslInitValueInfo     (TRslValueInfo *data);
RSLLINK void       RSLAPI RslFreeValueInfo     (TRslValueInfo *data);
RSLLINK void       RSLAPI RslInitVarInfo       (const char *name, VALUE *val, TRslValueInfo *data);
RSLLINK void       RSLAPI RslCopyValueInfo     (TRslValueInfo *to, TRslValueInfo *from);
RSLLINK int        RSLAPI RslInitPropsIter     (TGenObject *obj, TRslValueInfo *data);
RSLLINK int        RSLAPI RslInitLocalsIter    (RSLSTACK hst, TRslValueInfo *data);
RSLLINK int        RSLAPI RslInitGlobalsIter   (TRslValueInfo *data);
RSLLINK bool       RSLAPI RslGetValueInfo      (TRslValueInfo *data, int curNum);
RSLLINK bool       RSLAPI RslSetValueInfo      (TRslValueInfo *data, VALUE *val);
RSLLINK bool       RSLAPI RslRefreshValueInfo  (TRslValueInfo *data);

RSLLINK bool       RSLAPI RslEnblDbg           (HRD d, bool enable);

RSLLINK RSLEXPCTX  RSLAPI RslParseExp          (RSLPROC proc, const char *txt);
RSLLINK bool       RSLAPI RslExecExpAt         (RSLEXPCTX exp, RSLSTACK st, VALUE *ret);
RSLLINK RSLVALUE   RSLAPI RslExecExpAtEx       (RSLEXPCTX exp, RSLSTACK st, int *isUnloaded);
RSLLINK void       RSLAPI RslRemExp            (RSLEXPCTX exp);
RSLLINK bool       RSLAPI RslIsValidExpAt      (RSLEXPCTX exp, RSLSTACK st);

RSLLINK RSLEXPCTX         RslParseExpEx        (RSLPROC proc, const char *txt, int *isLval);
RSLLINK bool              RslSetExpVal         (RSLEXPCTX expL, RSLSTACK st, const char *txt);

void                      RslSetDbgModule      (SYMMODULE *dbgModule);

#ifndef RSL_DLM
        ISYMBOL          *GetSymModForGlobal   (SYMMODULE *mod, int n);
#endif

#ifndef RSL_NO_PCODE
        void              PCodeRemBreakPoints  (HPCODE hPCode, unsigned int numModule, struct BreakData *data);
        void             *PCodeFindStmt        (HPCODE hPCode, unsigned int numModule, struct tagTRslBreakInfo *data);
#endif

RSLLINK HDBG RSLAPI       RslSetDbg            (HDBG h);
RSLLINK HDBG RSLAPI       RslGetDbg            (void);


RSLVINFO RslGetFirstArrayInfo(RSLVALUE hv);
RSLVINFO RslGetFirstLocalInfo(RSLSTACK hst);
RSLVINFO RslGetFirstPropInfo (RSLVALUE hv);
RSLVINFO RslGetNextInfo      (RSLVINFO curVal);
void     RslRefreshInfo      (RSLVINFO hval);
bool     RslSetInfo          (RSLVINFO hinf, RSLSTACK st, const char *txt);
void     RslDoneInfo         (RSLVINFO hval);



HDBG implRemote_InitDebugProxy    (HRD hinst, TDbgIntf *intf);
void implRemote_DoneDebugProxy    (HDBG hdbg);
bool implRemote_CallDebugBreakProc(HDBG hdbg, uint32 data);
int  implRemote_DebugBreakPoint   (HDBG hdbg, uint32 data);
void implRemote_TryUnload         (void);


HDBG RslInitDebugProxyEx (HRD hinst, TDbgIntf *dbgIntf, bool useRemote);
HDBG RslInitDebugProxy   (HRD hinst, TDbgIntf *dbgIntf);
void RslDoneDebugProxy   (HDBG hdbg);
int  RslDebugBreakPoint  (HDBG hdbg, uint32 data);
void RslCallTraceProc    (HDBG hdbg, const char *str);
void RslCallNotifyModProc(HDBG hdbg, RSLMODULE hmod, bool add);

RSLLINK TDbgIntf *RSLAPI RslGetDbgInterface(void);
        bool             RslSetDbgIntfFlag (bool newFlag);

RSLLINK void RSLAPI      RslSetErrorResumeAction(int result);

bool RslAssign(EXPRESSION *exp, VALUE *val);

#ifdef __cplusplus
}
#endif

#endif // __IDEBUG_H

/* <-- EOF --> */