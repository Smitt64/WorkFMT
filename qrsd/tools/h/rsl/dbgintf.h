/*-----------------------------------------------------------------------*-

 File Name   : dbgintf.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/dbgintf.h $
               $Revision: 32 $
               $Date: 23.09.00 11:32 $
               $Author: Kubrin $
               $Modtime: 23.09.00 11:25 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 11,2000  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __DBGINTF_H
#define __DBGINTF_H

#ifndef __PLATFORM_H
# include <platform.h>
#endif

#ifndef __SYSTYPES_H
# include <systypes.h>
#endif

#ifndef __LISTOBJ_H
# include "listobj.h"
#endif

#ifndef __IDATA_H
    RSL_MAKE_HANDLE(HDBG);

# ifndef __RSLTYPE_H
    RSL_MAKE_HANDLE(HRD);
# endif
#endif

#ifndef _INC_WINDOWS
    //RSL_MAKE_HANDLE(HWND);
    typedef void * HWND;
    typedef unsigned long  DWORD;
#endif

#ifndef __RSDBHD_HPP
    typedef unsigned long  RslHandle32;
#endif

// -----------------------------------------------------------------------------
#define  RSL_STEP_OVER           1
#define  RSL_STEP_INTO           2
#define  RSL_DBG_RUN             3
#define  RSL_DBG_ABORT           4
#define  RSL_STEP_OUT            5
#define  RSL_RUN_NEXTMOD         6
#define  RSL_RUN_THISMOD         7
#define  RSL_DBG_ABORT_DEFAULT   8
#define  RSL_EXEC_INTEREXP       9

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(RSLMODULE);
RSL_MAKE_HANDLE(RSLSTACK);
RSL_MAKE_HANDLE(RSLSTMT);
RSL_MAKE_HANDLE(RSLPROC);
RSL_MAKE_HANDLE(RSLEXPCTX);
RSL_MAKE_HANDLE(RSLVALUE);
RSL_MAKE_HANDLE(RSLVINFO);
RSL_MAKE_HANDLE(RSLSRC);

// -----------------------------------------------------------------------------
// The types of RSL modules
#define  RSL_MOD_MAC   0
#define  RSL_MOD_RSM   1
#define  RSL_MOD_DLM   2
#define  RSL_MOD_STD   3

// -----------------------------------------------------------------------------
// Notify codes
#define  RSL_NOTIFY_MOD_ADD  0
#define  RSL_NOTIFY_MOD_REM  1

#include <packpshn.h>

// -----------------------------------------------------------------------------
#if defined(RSL_PL_WIN32)
    #define RSDBG  __stdcall
#elif defined(RSL_PL_DOS)
    #define RSDBG  _loadds _far
#else
    #define RSDBG
#endif

// -----------------------------------------------------------------------------
typedef int (RSDBG *TRslDbgCallBack)(const char *name, char *buff, int sz, void *data);

// -----------------------------------------------------------------------------
typedef struct tagTRslBreakInfo
      {
       RSLSTMT    stmt;
       uint32     data;

       int32      offs;
       int16      len;
       int16      codeLine;
      }  TRslBreakInfo;


// -----------------------------------------------------------------------------
enum InteractExpStatusTypes
   {
    INTR_EXP_NOEXP,      // No expression
    INTR_EXP_INEXP,      // Now is executing
    INTR_EXP_ERROR,      // Error in expression, see lastErrMessage
    INTR_EXP_UNDEF,      // Expression result is NULL
    INTR_EXP_OK          // Result in char string is in lastErrMessage
   };

// -----------------------------------------------------------------------------
typedef struct tagDbgIntf
      {
       short int  verHi;
       short int  verLo;

       RSLSTACK  (RSDBG *EnumStack)  (HRD inst, RSLSTACK st,
                                      RSLPROC *prc, RSLMODULE *mod, RSLSTMT *stmt, int *offs, int *len,
                                      char *procName, int szProcName, char *modName, int szModName);

       RSLMODULE (RSDBG *EnumModule) (HRD inst,RSLMODULE hmod, char *modName, int szModName,int *modType);

       RSLSTMT   (RSDBG *GetStatementOfPos) (HRD inst, RSLMODULE hmod, int offs, int len,
                                             int *realOffs, int *realLen, int *line);

       void      (RSDBG *BreakPointAt)  (HRD inst, RSLSTMT stmt, uint32 data);
       void      (RSDBG *ClrBreakPoint) (HRD inst, RSLSTMT stmt);

       int       (RSDBG *SetBreakPoint) (HRD inst, RSLMODULE hm, int num, TRslBreakInfo *ptr);
       void      (RSDBG *RemBreakPoint) (HRD inst, RSLMODULE hm, int num, TRslBreakInfo *ptr);

       RSLEXPCTX (RSDBG *ParseExp)      (HRD inst, RSLPROC proc, const char *txt, int *isLval);
       void      (RSDBG *RemExp)        (HRD inst, RSLEXPCTX exp);

       RSLVALUE  (RSDBG *ExecExpAt)     (HRD inst, RSLEXPCTX exp, RSLSTACK st,
                                         int *isObject, char *tpName, int szTpName, char *value, int szValue );

       bool      (RSDBG *SetExpValue)   (HRD inst, RSLEXPCTX exp, RSLSTACK st, const char *txt);


       RSLVINFO  (RSDBG *GetFirstLocalInfo)(HRD inst, RSLSTACK hst, RSLVALUE *val, int *isObject,
                                            char *name, int szName, char *tpName, int szTpName, char *value, int szValue);
       RSLVINFO  (RSDBG *GetFirstPropInfo) (HRD inst, RSLVALUE hv, RSLVALUE *val, int *isObject,
                                            char *name, int szName, char *tpName, int szTpName, char *value, int szValue);
       RSLVINFO  (RSDBG *GetNextInfo)      (HRD inst, RSLVINFO hinfo, RSLVALUE *val, int *isObject,
                                            char *name, int szName, char *tpName, int szTpName, char *value, int szValue);
       void      (RSDBG *RefreshInfo)      (HRD inst, RSLVINFO hinfo, RSLVALUE *val, int *isObject,
                                            char *name, int szName, char *tpName, int szTpName, char *value, int szValue);
       void      (RSDBG *DoneInfo)         (HRD inst, RSLVINFO hinfo);
       bool      (RSDBG *SetNewInfo)       (HRD inst, RSLVINFO hinfo,RSLSTACK st, const char *txt);

       RSLSRC    (RSDBG *OpenSrc)          (HRD inst, RSLMODULE hmod, TRslDbgCallBack proc, void *data);
       int       (RSDBG *ReadSrc)          (HRD inst, RSLSRC hsrc, char *buff, int size);
       int       (RSDBG *GetSizeSrc)       (HRD inst, RSLSRC hsrc);
       void      (RSDBG *CloseSrc)         (HRD inst, RSLSRC hsrc);

       void      (RSDBG *Interrupt)        (HRD inst);

       void      (RSDBG *ExecContinue)     (HRD inst, int traceFlag);

       void      (RSDBG *GetAppWnd)        (HRD inst, HWND *wnd);

       int       (RSDBG *SetDbgFlag)       (HRD inst, int newFlag);

       int       (RSDBG *GetVersion)       (HRD inst, short int *verLo);

    // In version 3
    #ifndef NO_HANDLECVT
       RSLSTACK    (RSDBG *GetStackFromHandle)  (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromStack)  (HRD inst, RSLSTACK st);
       RSLMODULE   (RSDBG *GetModuleFromHandle) (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromModule) (HRD inst, RSLMODULE mod);
       RSLSTMT     (RSDBG *GetStmtFromHandle)   (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromStmt)   (HRD inst, RSLSTMT st);
       RSLPROC     (RSDBG *GetProcFromHandle)   (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromProc)   (HRD inst, RSLPROC proc);


       RSLEXPCTX   (RSDBG *GetExpFromHandle)    (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromExp)    (HRD inst, RSLEXPCTX exp);
       RSLVALUE    (RSDBG *GetValFromHandle)    (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromVal_inf)(HRD inst, RSLVALUE val, RSLVINFO vinfo);
       RslHandle32 (RSDBG *GetHandleFromVal_exp)(HRD inst, RSLVALUE val, RSLEXPCTX exp);
       RSLVINFO    (RSDBG *GetValInfoFromHandle)(HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromValInfo)(HRD inst, RSLVINFO vinfo);
       RSLSRC      (RSDBG *GetSrcFromHandle)    (HRD inst, RslHandle32 key);
       RslHandle32 (RSDBG *GetHandleFromSrc)    (HRD inst, RSLSRC src);
    #else
       void (RSDBG *Tmp1)();
       void (RSDBG *Tmp2)();
       void (RSDBG *Tmp3)();
       void (RSDBG *Tmp4)();
       void (RSDBG *Tmp5)();
       void (RSDBG *Tmp6)();
       void (RSDBG *Tmp7)();
       void (RSDBG *Tmp8)();
       void (RSDBG *Tmp9)();
       void (RSDBG *Tmp10)();
       void (RSDBG *Tmp11)();
       void (RSDBG *Tmp12)();
       void (RSDBG *Tmp13)();
       void (RSDBG *Tmp14)();
       void (RSDBG *Tmp15)();
       void (RSDBG *Tmp16)();
       void (RSDBG *Tmp17)();
    #endif

       // In version 4
       RSLSRC    (RSDBG *OpenSrcEncode)(HRD inst, RSLMODULE hmod, TRslDbgCallBack proc, void *data, int *encode);

       // In version 5
       void      (RSDBG *GetValueSize)(HRD inst, RSLVINFO hinfo, int *size);
       void      (RSDBG *GetValueData)(HRD inst, RSLVINFO hinfo, char *valBuff, int size, int offs, int *outSz);
       void      (RSDBG *SetValueData)(HRD inst, RSLVINFO hinfo, int phase, const char *valBuff, int size);

       void      (RSDBG *GetExpSize)  (HRD inst, RSLEXPCTX exp, int *size);
       void      (RSDBG *GetExpData)  (HRD inst, RSLEXPCTX exp, char *valBuff, int size, int offs, int *outSz);
       void      (RSDBG *SetExpData)  (HRD inst, RSLEXPCTX exp, int phase, const char *valBuff, int size);


       // In version 6
       RSLVINFO  (RSDBG *GetFirstArrayInfo)  (HRD inst, RSLVALUE hv, RSLVALUE *val, int *isObject,
                                              char *name, int szName, char *tpName, int szTpName, char *value, int szValue);

       // In version 7
       void      (RSDBG *GetLastErrorText)   (HRD inst, char *errBuff, int szBuff);

       // In version 8
       bool      (RSDBG *ExecExpInteractive) (HRD inst, const char* txt);
       unsigned short (RSDBG *GetExpInteractiveStatus) (HRD inst);
      } TDbgIntf;

// -----------------------------------------------------------------------------
typedef struct tagRslDbg
      {
       int  (RSDBG *DbgVersion)  (void);
       int  (RSDBG *DbgCount)    (void);

       HDBG (RSDBG *DbgInit)     (HRD hinst, TDbgIntf *ptr);
       void (RSDBG *DbgDone)     (HDBG);
       int  (RSDBG *DbgBreak)    (HDBG hinst, uint32 data);

       void (RSDBG *DbgTrace)    (HDBG hinst, const char *str);

       void (RSDBG *DbgAddModule)(HDBG hinst, RSLMODULE hmod);
       void (RSDBG *DbgRemModule)(HDBG hinst, RSLMODULE hmod);

       void (RSDBG *DbgSetMode)  (HDBG hinst, int mode);
      } TRslDbg;

// -----------------------------------------------------------------------------
typedef TRslDbg *(RSDBG *TRslDbgInterfaceProc)(void);

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(RSLDBGPROXY);
RSL_MAKE_HANDLE(RSLDBGSTUB);

// -----------------------------------------------------------------------------
typedef struct
      {
       bool         isRemoteStub;
       bool         isEnabled;

       HDBG         hdbg;  // Local pointer to DEBUGGER
       RSLDBGSTUB   hstub;

       LOBJ         listElem;
       RslHandle32  key;

       DWORD        threadID;
      } TDbgProxy;

// -----------------------------------------------------------------------------
typedef struct
      {
       bool          isRemoteStub;
       bool          isEnabled;

       HDBG          hdbg; // Local pointer to DEBUGGER
       RSLDBGSTUB    hstub;

       LOBJ          listElem;
       RslHandle32   key;

       // ----------------------------
       const char   *cashDir;
       const char   *macDir;
      } TDbgProxyForRemoteStub;

// -----------------------------------------------------------------------------
typedef struct
      {
       bool          isRemoteStub;
       RSLDBGPROXY   hdbg; // if isRemoteStub then uint32 handle else pointer
       TDbgIntf     *intf;
       HRD           hinst;
       int           continueFlag;
       bool          needExit;

       LOBJ          listElem;
       RslHandle32   key;
      } TDbgStub;


// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

typedef int (*DbgCallsProc)(int cmd, const void *inMes, void *outMes);
DbgCallsProc RslGetDbgDispatch (int isNewIntf);

TDbgIntf *RslGetRmtIntf(void);


RslHandle32  AddProxyToList(TDbgProxy *ptr);
void RemProxyFromList(TDbgProxy *ptr);
TDbgProxy *FindProxyByKey(RslHandle32 key);
TDbgProxy *GetFirstProxyFromList(void);

bool LoadDbgDll(void);
void TryUnloadDbgDll(void);
bool RslInitLocalDebugger(TDbgProxy *ptr);
void RslDoneLocalDebugger(TDbgProxy *ptr);

bool RslCallLocalStub(TDbgProxy *prx, const void *in, size_t sz, const char *tblName, int cmdId);
bool RslTrnLocalStub(TDbgProxy *prx, const void *in, size_t sz, const char *tblName, int cmdId,
                       void *out, size_t szOut, size_t *szOutPtr, unsigned long timeOut);
void  RslInterruptLocalStub(TDbgProxy *prx);

int RslProcessDbgLocalCalls(RSLDBGSTUB hstub);
bool CallLocalDebugBreakProc(RSLDBGSTUB hdbg, uint32 data);

int RslProcessDbgRemoteCalls(RSLDBGSTUB hstub);
bool CallRemoteDebugBreakProc(RSLDBGSTUB hdbg, uint32 data);

RSLDBGPROXY RslCreateProxyForRemoteStub (RslHandle32 hstub, const char *cashDir, const char *macDir);
void RslRemoveProxyForRemoteStub(RSLDBGPROXY prx);
bool RslCallLocalDebuggerBreakProc(TDbgProxy *prx, uint32 data);
void RslCallLocalDebuggerTraceProc(TDbgProxy *prx, const char *str);
void RslCallLocalDebuggerNotifyProc(TDbgProxy *prx, RSLMODULE hmod, bool add);
void RslRemoveAllProxyForRemoteStub(void);

bool RslCallRemoteStub(const void *in, size_t sz, const char *tblName, int cmdId);
bool RslTrnRemoteStub(const void *in, size_t sz, const char *tblName, int cmdId,
                      void *out, size_t szOut, size_t *szOutPtr, unsigned long timeOut);

TDbgIntf *TermGetRmtIntf (void);
bool TermSetRmtIntfFlag(bool newFlag);

// -----------------------------------------------------------------------------
#ifdef __TRMINTF_H
    RSLLINK int RSLAPI RslSetDbgFlag(int newFlag);
    HDBG RSLAPI RslSetDbg(HDBG h);
    HDBG RSLAPI RslGetDbg(void);
    RSLLINK TDbgIntf * RSLAPI RslGetDbgInterface(void);
#endif

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
// If C++, it is real C++ functions.
RSLDBGSTUB RslCreateLocalStub     (HRD inst, TDbgIntf *intf);
void       RslRemoveLocalStub     (RSLDBGSTUB hstub);
int        RslLocalBreakPointProc (RSLDBGSTUB hdbg, uint32 data);
void       RslLocalTraceProc      (RSLDBGSTUB hdbg, const char *str);
void       RslLocalNotifyProc     (RSLDBGSTUB hdbg, RSLMODULE hmod, bool add);
void       RslSetDbgMode          (RSLDBGSTUB hstub, int mode);  // 1 == GUI, 0 - DOS

RSLDBGSTUB RslCreateRemoteStub    (HRD inst, TDbgIntf *intf);
void       RslRemoveRemoteStub    (RSLDBGSTUB hstub);
int        RslRemoteBreakPointProc(RSLDBGSTUB hdbg, uint32 data);
void       RslRemoteTraceProc     (RSLDBGSTUB hdbg, const char *str);
void       RslRemoteNotifyProc    (RSLDBGSTUB hdbg, RSLMODULE hmod, bool add);


// pApp - TAppObj
RSLDBGSTUB RslCreateRemoteStubA    (void *pApp, HRD inst, TDbgIntf *intf);
void       RslRemoveRemoteStubA    (void *pApp, RSLDBGSTUB hstub);
int        RslRemoteBreakPointProcA(void *pApp, RSLDBGSTUB hdbg, uint32 data);
void       RslRemoteTraceProcA     (void *pApp, RSLDBGSTUB hdbg, const char *str);
void       RslRemoteNotifyProcA    (void *pApp, RSLDBGSTUB hdbg, RSLMODULE hmod, bool add);

#include <packpop.h>

#endif // __DBGINTF_H

/* <-- EOF --> */