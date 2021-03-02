/*-----------------------------------------------------------------------*-

 File Name   : dlmintf.h  

 Sybsystem   : DLM SDK  
 Description : Интерфейс для создания расширений RSL
               Этот файл необходимо использовать при при создании
               расширений RSL при помощи МS Visual C++

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 April 23,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __DLMINTF_H
#define __DLMINTF_H

#if !defined(BUILD_DLM) && defined(__RSLCOMMON_H)
    #error You must include dlmintf.h before any RSL .h files!!!
#endif

#ifndef BUILD_DLM
    #define BUILD_DLM
#endif


#include "rsl/common.h"

#ifndef __UNIOBJ_H
#  include "rsl/uniobj.h"
#endif
         
#ifndef __IEXEC_H
#  include "rsl/iexec.h"
#endif

#ifndef __STDOBJ_H
#  include "rsl/stdobj.h"
#endif

#ifndef __RSLACTX_H
#  include "rsl/rslactx.h"
#endif

#ifndef __BTREXP_H
#  include "rsl/btrexp.h"
#endif

#ifndef __FS_H
#  include "fs.h"
#endif

#ifndef __RSLDLM_H
#  include "rsl/rsldlm.h"
#endif

#ifndef __MSG_H
#  include "rsl/msg.h"
#endif

#ifndef __KRNLINTF_H
#  include "rsl/krnlintf.h"
#endif

#ifndef __APPINTF_H
# include "appintf.h"
#endif               

// -----------------------------------------------------------------------------
// Минимальный номер версии библиотеки поддержки DLM-ов в EXE файле, 
// с которой могут использоваться DLM-ы собранный при помощи данной
// версии DLM SDK
#define  RSL_RTM_VERSION_MIN  304


// -----------------------------------------------------------------------------              
#define  RSL_TRY                  try
#define  RSL_CATCH(arg_and_code)  catch arg_and_code
#define  RSL_THROW(expr)          throw expr
#define  RSL_RETHROW              throw


#include <packpsh1.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// Функции импортируемые из RSL

unsigned       DLMAPI  RslDlmVersion(void);
char          *DLMAPI  DlmIntOutBuf(void);
uintptr_t      DLMAPI  RslTArrayID(void);

int            DLMAPIC print(char *fmt, ...);
int            DLMAPIC Message(char *fmt, ...);
int            DLMAPIC MsgBox(char *fmt, ...);
long           DLMAPI  UserNumber(void);
void           DLMAPIC RslError(char *fmt, ...);
char          *DLMAPI  GetStdDataBase(void);
char          *DLMAPI  GetStdDataPath(void);

// -----------------------------------------------------------------------------
#define  InstBuf  DlmIntOutBuf()

// -----------------------------------------------------------------------------
#ifdef RSL_MD_FLAT
    // -----------------------------------------------------------------------------
    typedef struct
          {
           int              (DLMAPIC *ptr_print) (char *fmt, ...);
           int              (DLMAPIC *ptr_Message) (char *fmt, ...);
           int              (DLMAPIC *ptr_MsgBox) (char *fmt, ...);
           bool                     (*ptr_GetParm) (int n, VALUE **val);
           bool              (DLMAPI *ptr_PutParm) (int n, VALTYPE v_type, void *ptr);
           int                      (*ptr_GetNumParm) (void);
           void                     (*ptr_ReturnVal)  (VALTYPE v_type, void *ptr);
           ISYMBOL                 *(*ptr_AddSymGlobal) (VALTYPE type, const char *name);
           ISYMBOL                 *(*ptr_FindSymbolProc) (const char *name, SYMPROC *sym);
           void                     (*ptr_SymGlobalSet) (ISYMBOL *sym, VALTYPE v_type, void *ptr);
           VALUE                   *(*ptr_SymGlobalGet) (ISYMBOL *sym);
           SYMPROC                 *(*ptr_AddStdProc) (VALTYPE type, const char *name, void (DLMAPIC *proc)(void), int attr);
           void             (DLMAPIC *ptr_RslError) (char *fmt, ...);
           char             *(DLMAPI *ptr_GetStdDataBase) (void);
           char             *(DLMAPI *ptr_GetStdDataPath) (void);
           int                      (*ptr_toupc) (int ch);
           int                      (*ptr_stricmpR) (const char *str1, const char *str2);
           int                      (*ptr_strnicmpR) (const char *str1, const char *str2, int len);
           int                      (*ptr_toDownc)   (int ch);
           char                    *(*ptr_struprR)   (char *str);
           char                    *(*ptr_strnuprR)  (char *str, int len);
           char                    *(*ptr_strlwrR)   (char *str);
           char                    *(*ptr_strnlwrR)  (char *str, int len);
           char                    *(*ptr_MakeFirstPath) (char *Buff, const char *file, const char *IncDir, const char *ext);
           int                      (*ptr_FindIncFile) (char *Buff, const char *file, const char *IncPath, const char *ext, int fCurDir);
           char                    *(*ptr_RunPath) (char *name);
           int                      (*ptr_TestExistFile) (const char *file);
           void                     (*ptr_RslSplitFile) (const char *fname, char *dir, char *name, char *ext);
           char                    *(*ptr_RslMergeFile) (char *buff, const char *dir, const char *name, const char *ext);
           BDHANDLE           (BTAPI *ptr_BtOpenDataBase)   (char RSL_FAR *path, char RSL_FAR *data, char RSL_FAR *globalPassword, int reportError);
           BDHANDLE           (BTAPI *ptr_BtOpenDataBaseEx)(
                                                            char RSL_FAR    *path,            // Dictionary pathname  (c:\data\mydb.def)
                                                            char RSL_FAR    *data,            // Paths to data files  (c:\data;d:\samples;d:\base)
                                                            TBtMesProc       aBtMesProc,      // May be NULL
                                                            TBtMemAllocProc  allocPtr,        // May be NULL
                                                            TBtMemFreeProc   freePtr,         // May be NULL
                                                            char RSL_FAR    *globalPassword,  // Owner name for all files
                                                            int              reportError,     // If 0 do not show Btrieve error dialog box
                                                                                              // Else show only errors that not in Scip List
                                                                                              // Default Beof,Benorec,BEdupkey
                                                            TConId           id               // Connection ID (reserved)
                                                           );

           void               (BTAPI *ptr_BtCloseDataBase)      (BDHANDLE db);
           void               (BTAPI *ptr_BtErrorDialog)        (BDHANDLE db, int er, int show);
           TAltColProc        (BTAPI *ptr_BtSetAltColProc)      (BDHANDLE db,TAltColProc newProc, void RSL_FAR *data);
           void               (BTAPI *ptr_BtSetBlobType)        (BDHANDLE db, int tp);
           int                (BTAPI *ptr_BtCreateTable)        (BDHANDLE db, char RSL_FAR *strName, char RSL_FAR *file);
           BTHANDLE           (BTAPI *ptr_BtOpenTable)          (BDHANDLE db, char RSL_FAR *name,
                                          int mode, int keypath, char RSL_FAR *fileName);
           void               (BTAPI *ptr_BtCloseTable)         (BTHANDLE hd);
           void               (BTAPI *ptr_BtSetLockFlag)        (BDHANDLE db, int reclock);
           void               (BTAPI *ptr_BtSetNoCurChangeFlag) (BDHANDLE db, int isNoChange);
           void              *(BTAPI *ptr_BtGetVarlenInfo)      (BTHANDLE hd, unsigned RSL_FAR *size);
           unsigned           (BTAPI *ptr_BtGetVarlenSize)      (BTHANDLE hd);
           void               (BTAPI *ptr_BtSetVarlenSize)      (BTHANDLE hd, unsigned size);
           int                (BTAPI *ptr_BtFetchNext)          (BTHANDLE hd);
           int                (BTAPI *ptr_BtFetchPrev)          (BTHANDLE hd);
           int                (BTAPI *ptr_BtUpdate)             (BTHANDLE hd);
           int                (BTAPI *ptr_BtInsert)             (BTHANDLE hd);
           int                (BTAPI *ptr_BtDelete)             (BTHANDLE hd);
           void               (BTAPI *ptr_BtReset)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetEQ)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetGE)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetLE)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetPos)             (BTHANDLE hd, BKMHANDLE RSL_FAR *pos);
           int                (BTAPI *ptr_BtGetDirect)          (BTHANDLE hd, BKMHANDLE pos);

           int                (BTAPI *ptr_BtUnlock)             (BTHANDLE tb, int mode, unsigned long addr);
           int                (BTAPI *ptr_BtGetLastError)       (BDHANDLE db);
           BNDHANDLE          (BTAPI *ptr_BtBindField)          (BTHANDLE hd, char RSL_FAR *name, void RSL_FAR *data, unsigned char dType, int len);
           int                (BTAPI *ptr_BtUnbindField)        (BTHANDLE hd, BNDHANDLE id);
           void               (BTAPI *ptr_BtUnbindAll)          (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetFieldID)         (BTHANDLE hd, char RSL_FAR *name);
           int                (BTAPI *ptr_BtGetField)           (BTHANDLE hd, int ind, void RSL_FAR *aPtr, unsigned char dType, int bufLen);
           int                (BTAPI *ptr_BtPutField)           (BTHANDLE hd, int ind, void RSL_FAR *aPtr, unsigned char dType);
           int                (BTAPI *ptr_BtBeginTransaction)   (BDHANDLE db, int flags);
           int                (BTAPI *ptr_BtEndTransaction)     (BDHANDLE db);
           int                (BTAPI *ptr_BtAbortTransaction)   (BDHANDLE db);
           void               (BTAPI *ptr_BtSetBlobBuffSize)    (BDHANDLE db, unsigned size); // Default 1024
           int                (BTAPI *ptr_BtOpenBlob)           (BTHANDLE bf, int openMode);
           int                (BTAPI *ptr_BtCloseBlob)          (BTHANDLE bf);
           int                (BTAPI *ptr_BtReadBlob)           (BTHANDLE bf, char RSL_FAR *buff, int size);
           int                (BTAPI *ptr_BtWriteBlob)          (BTHANDLE bf, char RSL_FAR *buff, int size);
           int                (BTAPI *ptr_BtPosBlob)            (BTHANDLE bf, unsigned long pos);
           int                (BTAPI *ptr_BtGetBlobSize)        (BTHANDLE bf, unsigned long *size);
           int                (BTAPI *ptr_BtTruncateBlob)       (BTHANDLE bf);
           char              *(BTAPI *ptr_BtErrorText)          (int erCode);
           void               (BTAPI *ptr_BtStopClient)         (TConId  id);
           int                (BTAPI *ptr_BtLoopTables)         (BDHANDLE db, TLoopProc proc, void RSL_FAR *data);
           int                (BTAPI *ptr_BtGetGT)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtGetLT)              (BTHANDLE hd);
           int                (BTAPI *ptr_BtChangeKeyPath)      (BTHANDLE hd, int path);
           void                     (*ptr_AddObjectProviderMod) (TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create);
           void                     (*ptr_ValueMake)     (VALUE *val);
           void                     (*ptr_ValueClear)    (VALUE *val);
           void                     (*ptr_ValueCopy)     (VALUE *from, VALUE *to);
           int                      (*ptr_ValueIseq)     (VALUE *This, VALUE *dest);
           void                     (*ptr_ValueMove)     (VALUE *from, VALUE *to);
           void                     (*ptr_ValueSet)      (VALUE *val, VALTYPE v_type, const void *ptr);
           VALUE                   *(*ptr_PushValue)     (VALUE *val);
           bool                     (*ptr_PopValue)      (void);
           bool              (DLMAPI *ptr_PutParm2)      (int n, VALUE *v);
           void                     (*ptr_ReturnVal2)    (VALUE *v);
           void              (DLMAPI *ptr_LobjInitList)  (CNTLIST *lst, int delta);
           void              (DLMAPI *ptr_LobjDoneList)  (CNTLIST *lst, LCLRFUNC func);
           void                     (*ptr_LobjRemove)    (CNTLIST *lst, void *obj, LCLRFUNC func);
           int                      (*ptr_LobjForEach)   (CNTLIST *lst, LITRFUNC func, void *args );
           int                      (*ptr_LobjForEachL)  (CNTLIST *lst, LITRFUNC func, void *args );
           void                    *(*ptr_LobjFirstThat) (CNTLIST *lst, LCMPFUNC func, void *args);
           void                    *(*ptr_LobjLastThat)  (CNTLIST *lst, LCMPFUNC func, void *args);
           int                      (*ptr_LobjNitems)    (CNTLIST *lst);
           void                     (*ptr_LobjInsert)    (CNTLIST *lst, void *obj);
           void                    *(*ptr_LobjPrevItem)  (CNTLIST *lst, void *obj);
           void                    *(*ptr_LobjNextItem)  (CNTLIST *lst, void *obj);
           void                    *(*ptr_LobjFirstItem) (CNTLIST *lst);
           void                    *(*ptr_LobjLastItem)  (CNTLIST *lst);
           void                     (*ptr_SobjInit)      (SOBJ *obj);
           void                     (*ptr_SobjUnlink)    (SOBJ *obj);
           SYMPROC                 *(*ptr_FindMacro) (const char *name);
           bool                     (*ptr_SwitchToMacro) (SYMPROC *sym, int n);

        //#ifdef USE_PROVIDER
           void                     (*ptr_AddObjectProviderModEx) (TInitRslProvider init, TDoneRslProvider done, TCreateRslObject create,
                                      TRslTypeProc tpProc);
        //#else
        //   void (*thunk1)(void);
        //#endif

           long              (DLMAPI *ptr_UserNumber) (void);
           bool                     (*ptr_RunMacro) (SYMPROC *sym,...);
           void               (BTAPI *ptr_BtClearRecordBuff) (BTHANDLE hd);
           void              (DLMAPI *ptr_fs_screenSize) (int *nucols, int *numrows);
           void             *(DLMAPI *ptr_fs_saveStat) (void);
           void              (DLMAPI *ptr_fs_restStat) (void*);
           int               (DLMAPI *ptr_fs_event) (TFSEvent *ev, int waitTime);
           void                     (*ptr_fs_wrtnatr) (int x, int y, int number, int attr);
           void                     (*ptr_fs_setattr) (unsigned  color);      /* Set default attribute        */
           unsigned                 (*ptr_fs_curattr) (void);             /* Вернуть текущий атрибут      */
           void                     (*ptr_fs_wrtstr) (int x, int y, const char *str);
           void                     (*ptr_fs_wrtncell)     (int x, int y, int number, int charattr);
           void                     (*ptr_fs_gettext) (int x1, int y1, int x2, int y2, void *buf);
           void                     (*ptr_fs_puttext) (int x1, int y1, int x2, int y2, void *buf);
           void                     (*ptr_fs_movetext) (int x1, int y1, int x2, int y2, int dx, int dy);
           void                     (*ptr_fs_clr) (int ch, int atr);
           void                     (*ptr_fs_bar) (int x1, int y1, int x2, int y2, int ch, int atr);
           void                     (*ptr_fs_box) (int style, int attr, unsigned lcol, unsigned trow, unsigned rcol, unsigned brow);
           void                     (*ptr_fs_setcurtype) (unsigned int cur);
           void                     (*ptr_fs_setcurpoz) (int x, int y);
           unsigned                 (*ptr_fs_getcurpoz) (int *x, int *y);
           void                     (*ptr_fs_version) (TFsVersion *ver);
           void                     (*ptr_fs_padstr) (int x, int y, char *str, int outlen);
           void                     (*ptr_fs_wrtItemStr) (int x, int y, const char *str, int outlen, int selAtr);
           void                     (*ptr_fs_wrtMarkStr) (int x, int y, const   char *str, int outlen, int num, TMarkPos *pos);
           void             *(DLMAPI *ptr_fs_saveStat2) (void);
           void              (DLMAPI *ptr_fs_restStat2) (void*);
           void                     (*ptr_fs_statLine) (const char *str, int atr, int selAtr);
           int                      (*ptr_fs_getBuffSize) (int x1, int y1, int x2, int y2);
           int               (DLMAPI *ptr_fs_event2) (TFSEvent *ev, int waitTime);
           void              (DLMAPI *ptr_fs_screenSize2) (int *nucols, int *numrows);
           int                      (*ptr_NWcallEx) (int OP, void *POS_BLK, void *DATA_BUF, int *DATA_LEN, void *KEY_BUF,
                             int KEY_LEN, signed char KEY_NUM, unsigned long reserved);
           char                    *(*ptr_fs_getDeferBuff) (size_t sz, const char *dllName, int cmd);
           void                     (*ptr_fs_queueMessage) (const char *mes);
           char                    *(*ptr_fs_getSendBuff) (size_t sz, const char *dllName, int cmd);
           void                     (*ptr_fs_sendMessage) (const char *mes);
           char                    *(*ptr_fs_transactMessage) (const char *mes, size_t *sz);
           void                     (*ptr_copyToLow) ();
           void                     (*ptr_copyFromLow) ();
           void                     (*ptr_GetLowMem) ();
           void                     (*ptr_FreeLowMem) ();
           int                      (*ptr_ReadIniFile) (char *name, int (DLMAPIC *fun)(char *key, char *name, void *data), void *data);
           int                      (*ptr_RunProg) (char *prog, char *com, char *initmes, char *pausemes, int swap_size);
           int                      (*ptr_GetParmAttr) (int n);
           unsigned          (DLMAPI *ptr_LocRslDlmVersion) (void);
           unsigned long     (DLMAPI *ptr_LocRslTArrayID) (void);
           TGenObject       *(DLMAPI *ptr_LocRslTArrayCreate) (int rootSize, int incSize);
           void              (DLMAPI *ptr_LocRslTArrayPut) (TGenObject *obj, long id,VALUE *val);
           VALUE            *(DLMAPI *ptr_LocRslTArrayGet) (TGenObject *obj, long id);
           long              (DLMAPI *ptr_LocRslTArraySize) (TGenObject *obj);
           char             *(DLMAPI *ptr_LocInstBuf) (void);
           void             *(DLMAPI *ptr_LociNewMem) (size_t size);
           void             *(DLMAPI *ptr_LociNewMemNull) (size_t size);
           void              (DLMAPI *ptr_LociDoneMem) (void *ptr);
           long              (DLMAPI *ptr_LocRslObjGetUniqID) (TGenObject *rslObj, const char *name);
           int               (DLMAPI *ptr_LocRslObjMemberFromID) (TGenObject *rslObj, long dispid, long *id);
           int               (DLMAPI *ptr_LocRslObjInvoke) (TGenObject *pObj, long id, int code, int nPar, VALUE *par, VALUE *propORret);
           int               (DLMAPI *ptr_LocRslObjMemberFromName) (TGenObject *pObj, const char *name, long *id);

           void              (DLMAPI *ptr_LocDoRaiseEvents) (void (DLMAPIC * proc) (void *data, char *name), void *data);
           HRD               (DLMAPI *ptr_LocRslGetInstance) (void);
           HRD               (DLMAPI *ptr_LocRslSetInstance) (HRD newInst);
           unsigned          (DLMAPI *ptr_LocRslLockInstance) (HRD inst);
           unsigned          (DLMAPI *ptr_LocRslUnLockInstance) (HRD inst);
           int               (DLMAPI *ptr_LocRslInstGetNumModule) (HRD inst);
           int               (DLMAPI *ptr_LocRslSendMes) (int,void *ptr);
           bool              (DLMAPI *ptr_LocRslTArrayCnvType) (TGenObject *obj, bool cnvToSafeAr);
           ISYMBOL          *(DLMAPI *ptr_LocRslGetInstSymbol) (const char *pName);
           bool              (DLMAPI *ptr_LocRslCallInstSymbol) (ISYMBOL *sym, int code, int nPar, VALUE *par, VALUE *propORret);
           TGenObject       *(DLMAPI *ptr_LocRslIsTArray) (TGenObject *obj);
           void              (DLMAPI *ptr_LocSetSymAttr) (ISYMBOL *sym, int attr);

        // Version 3.10
#ifdef USE_PROVIDER
           void (*thunk2)(void);
           void (*thunk3)(void);
           void (*thunk4)(void);
           void (*thunk5)(void);
           void (*thunk6)(void);
           void (*thunk7)(void);
           void (*thunk8)(void);
           void (*thunk9)(void);
           void (*thunk10)(void);
           void (*thunk11)(void);
           void (*thunk12)(void);
#else  // USE_PROVIDER
           TGenClass        *(DLMAPI *ptr_LocRslAddUniClass)     (TMethodTable *clsInfo, bool visibleFromMacro);
           TGenObject       *(DLMAPI *ptr_LocCreateObjectOfClass) (TGenClass *cls, TGenObject *outer);
           void              (DLMAPI *ptr_LocUniObjectInit)       (TGenObject *obj, int firstParOffset);
           void              (DLMAPI *ptr_LocAddObjectProviderModEx2) (TInitRslProvider init, TDoneRslProvider done,
                                TCreateRslObject create, TRslClassProc clsProc);
           TGenObject       *(DLMAPI *ptr_LocRslGetInnerObj)      (TGenObject *pObj);
           TGenObject       *(DLMAPI *ptr_LocGenObjCreateEx)      (const char *typeName, TGenObject *ctrlObj);
           VALUE            *(DLMAPI *ptr_LocRslUniPropPtr)       (int id, int *tp, int *stat);

           TGenObject       *(DLMAPI *ptr_LocCreateObjectOfClassEx) (TGenClass *cls, TGenObject *outer, void **userObj);
           TGenClass        *(DLMAPI *ptr_LocRslFindUniClass)     (const char *typeName);
           TGenClass        *(DLMAPI *ptr_LocRslGetUniClass)      (TGenObject *obj);
           TGenObject       *(DLMAPI *ptr_LocRslUniCast)          (const char *clsName, TGenObject *obj, void **userClass);
#endif // USE_PROVIDER

           int               (DLMAPI *ptr_LocBtSetCnvMode) (BDHANDLE hdb, int mode);
           TBtError         *(DLMAPI *ptr_LocBtGetErrorInfo) (BDHANDLE db, int *numInfo);
           BDHANDLE          (DLMAPI *ptr_LocBtOpenDataBaseSimple) (char RSL_FAR *path, char RSL_FAR *data, int rdOnly, TBtError RSL_FAR *erInfo);
           TBtStructure     *(DLMAPI *ptr_LocBtGetStructure) (BDHANDLE hd, const char *name);
           void              (DLMAPI *ptr_LocBtFreeStructure) (TBtStructure *str);
           char             *(DLMAPI *ptr_LocBtStrGetFieldName) (TBtStructure *str, int nf);
           char             *(DLMAPI *ptr_LocBtStrGetComment) (TBtStructure *str, int nf);

           int               (DLMAPI *ptr_LocImportModule) (const char *name);

           void              (DLMAPI *ptr_addDispTable) (const char *name, fs_asyncProc_t p);
           void              (DLMAPI *ptr_remDispTable) (const char *name);
           void              (DLMAPI *ptr_makeEvent)    (TFSEvent *ev);

           TGenObject       *(DLMAPI *ptr_LocRslActiveXCreateEx) (IDispatch *pDisp, const char *tpName, const char *pcName, bool doAddRef);
           IDispatch        *(DLMAPI *ptr_LocRslObjGetIDispatch) (TGenObject *obj);
           TGenObject       *(DLMAPI *ptr_LocRslObjGetTGenObj)   (IDispatch *pDisp);
           IDispatch        *(DLMAPI *ptr_LocRslGetActiveXDispatch) (TGenObject *obj);

           TGenObject       *(DLMAPI *ptr_LocCreateObjectOfRslClass) (ISYMBOL *pSym);
           ISYMBOL          *(DLMAPI *ptr_LocRslAddObjVar)     (const char *name, int tp, int attr, const char *clsName, TGenClass *genCls, TVarInitProc proc);
           ISYMBOL          *(DLMAPI *ptr_LocRslAddVar)        (const char *name, int tp, int attr, void *val);
           void              (DLMAPI *ptr_LocRslSetClientData) (void *ptr, const char *name, void *reserved);
           void             *(DLMAPI *ptr_LocRslGetClientData) (const char *name, int reserved);

           void             *(DLMAPI *ptr_LocSobjFirstItem)(SOBJ *sobj);
           void             *(DLMAPI *ptr_LocSobjLastItem) (SOBJ *sobj);
           void             *(DLMAPI *ptr_LocSobjNextItem) (SOBJ *sobj);
           void             *(DLMAPI *ptr_LocSobjPrevItem) (SOBJ *sobj);

           int               (DLMAPI *ptr_LocImportModulePath)       (const char *name, const char *path);
           TInterpreterIntf *(DLMAPI *ptr_LocGetInterpreterInterface)(void);
           int                      (*ptr_LocGetRecordHandlerInfo)   (TGenObject *obj, TRecHandlerInfo *data);
           TGenObject       *(DLMAPI *ptr_LocCreateRecordHandler)    (char *name, char *altDic, void *buff);

           void             *(DLMAPI *ptr_getAppIntf)  (void);

           bool                     (*ptr_ValueSetAs)  (VALUE *val, VALTYPE needType, VALTYPE v_type, const void *ptr);
           bool                     (*ptr_SetParmAs)   (int n, VALTYPE needType, VALTYPE v_type, const void *ptr);
           bool                     (*ptr_ValueGet)    (VALUE *val, VALTYPE v_type, void *ptr);
           bool                     (*ptr_CnvType)     (VALUE *val, VALTYPE v_type);
           VALUE                   *(*ptr_GetReturnVal)();

           int               (DLMAPI *ptr_LocRslTArrayCnvBinType)(TGenObject *obj, int cnvToBinAr);

           VALUE                   *(*ptr_RunResult)(void);

           bool              (DLMAPI *ptr_LocRslTArrayCnvCheckBounds)(TGenObject *obj, bool checkBounds);
          } RSLCALLTBL;

    // -----------------------------------------------------------------------------
    extern BACKENTRY  *ExeExports;

    // -----------------------------------------------------------------------------
    #define  RSLCALL(fun)   ((RSLCALLTBL *)ExeExports)->ptr_##fun

    #define  DlmPrint       ((RSLCALLTBL *)ExeExports)->ptr_print
    #define  DlmMessage     ((RSLCALLTBL *)ExeExports)->ptr_Message
    #define  DlmMsgBox      ((RSLCALLTBL *)ExeExports)->ptr_MsgBox
    #define  DlmRunMacro    ((RSLCALLTBL *)ExeExports)->ptr_RunMacro
    #define  DlmError       ((RSLCALLTBL *)ExeExports)->ptr_RslError
            
    #define  toupc_DLM      ((RSLCALLTBL *)ExeExports)->ptr_toupc
    #define  stricmpR_DLM   ((RSLCALLTBL *)ExeExports)->ptr_stricmpR
    #define  strnicmpR_DLM  ((RSLCALLTBL *)ExeExports)->ptr_strnicmpR
    #define  toDownc_DLM    ((RSLCALLTBL *)ExeExports)->ptr_toDownc
    #define  struprR_DLM    ((RSLCALLTBL *)ExeExports)->ptr_struprR
    #define  strnuprR_DLM   ((RSLCALLTBL *)ExeExports)->ptr_strnuprR
    #define  strlwrR_DLM    ((RSLCALLTBL *)ExeExports)->ptr_strlwrR
    #define  strnlwrR_DLM   ((RSLCALLTBL *)ExeExports)->ptr_strnlwrR

    // -----------------------------------------------------------------------------
    #if defined(RSL_USE_MACRO)

        #define  print                          ((RSLCALLTBL *)ExeExports)->ptr_print
        #define  Message                        ((RSLCALLTBL *)ExeExports)->ptr_Message
        #define  MsgBox                         ((RSLCALLTBL *)ExeExports)->ptr_MsgBox
        #define  GetParm                        ((RSLCALLTBL *)ExeExports)->ptr_GetParm
        #define  PutParm                        ((RSLCALLTBL *)ExeExports)->ptr_PutParm
        #define  GetNumParm                     ((RSLCALLTBL *)ExeExports)->ptr_GetNumParm
        #define  ReturnVal                      ((RSLCALLTBL *)ExeExports)->ptr_ReturnVal
        #define  AddSymGlobal                   ((RSLCALLTBL *)ExeExports)->ptr_AddSymGlobal
        #define  FindSymbolProc                 ((RSLCALLTBL *)ExeExports)->ptr_FindSymbolProc
        #define  SymGlobalSet                   ((RSLCALLTBL *)ExeExports)->ptr_SymGlobalSet
        #define  SymGlobalGet                   ((RSLCALLTBL *)ExeExports)->ptr_SymGlobalGet
        #define  AddStdProc                     ((RSLCALLTBL *)ExeExports)->ptr_AddStdProc
        #define  RslError                       ((RSLCALLTBL *)ExeExports)->ptr_RslError
        #define  GetStdDataBase                 ((RSLCALLTBL *)ExeExports)->ptr_GetStdDataBase
        #define  GetStdDataPath                 ((RSLCALLTBL *)ExeExports)->ptr_GetStdDataPath

        #define  MakeFirstPath                  ((RSLCALLTBL *)ExeExports)->ptr_MakeFirstPath
        #define  FindIncFile                    ((RSLCALLTBL *)ExeExports)->ptr_FindIncFile
        #define  RunPath                        ((RSLCALLTBL *)ExeExports)->ptr_RunPath
        #define  TestExistFile                  ((RSLCALLTBL *)ExeExports)->ptr_TestExistFile
        #define  RslSplitFile                   ((RSLCALLTBL *)ExeExports)->ptr_RslSplitFile
        #define  RslMergeFile                   ((RSLCALLTBL *)ExeExports)->ptr_RslMergeFile
        #define  BtOpenDataBase                 ((RSLCALLTBL *)ExeExports)->ptr_BtOpenDataBase
        #define  BtOpenDataBaseEx               ((RSLCALLTBL *)ExeExports)->ptr_BtOpenDataBaseEx
        #define  BtCloseDataBase                ((RSLCALLTBL *)ExeExports)->ptr_BtCloseDataBase
        #define  BtErrorDialog                  ((RSLCALLTBL *)ExeExports)->ptr_BtErrorDialog
        #define  BtSetAltColProc                ((RSLCALLTBL *)ExeExports)->ptr_BtSetAltColProc
        #define  BtSetBlobType                  ((RSLCALLTBL *)ExeExports)->ptr_BtSetBlobType
        #define  BtCreateTable                  ((RSLCALLTBL *)ExeExports)->ptr_BtCreateTable
        #define  BtOpenTable                    ((RSLCALLTBL *)ExeExports)->ptr_BtOpenTable
        #define  BtCloseTable                   ((RSLCALLTBL *)ExeExports)->ptr_BtCloseTable
        #define  BtSetLockFlag                  ((RSLCALLTBL *)ExeExports)->ptr_BtSetLockFlag
        #define  BtSetNoCurChangeFlag           ((RSLCALLTBL *)ExeExports)->ptr_BtSetNoCurChangeFlag
        #define  BtGetVarlenInfo                ((RSLCALLTBL *)ExeExports)->ptr_BtGetVarlenInfo
        #define  BtGetVarlenSize                ((RSLCALLTBL *)ExeExports)->ptr_BtGetVarlenSize
        #define  BtSetVarlenSize                ((RSLCALLTBL *)ExeExports)->ptr_BtSetVarlenSize
        #define  BtFetchNext                    ((RSLCALLTBL *)ExeExports)->ptr_BtFetchNext
        #define  BtFetchPrev                    ((RSLCALLTBL *)ExeExports)->ptr_BtFetchPrev
        #define  BtUpdate                       ((RSLCALLTBL *)ExeExports)->ptr_BtUpdate
        #define  BtInsert                       ((RSLCALLTBL *)ExeExports)->ptr_BtInsert
        #define  BtDelete                       ((RSLCALLTBL *)ExeExports)->ptr_BtDelete
        #define  BtReset                        ((RSLCALLTBL *)ExeExports)->ptr_BtReset
        #define  BtGetEQ                        ((RSLCALLTBL *)ExeExports)->ptr_BtGetEQ
        #define  BtGetGE                        ((RSLCALLTBL *)ExeExports)->ptr_BtGetGE
        #define  BtGetLE                        ((RSLCALLTBL *)ExeExports)->ptr_BtGetLE
        #define  BtGetPos                       ((RSLCALLTBL *)ExeExports)->ptr_BtGetPos
        #define  BtGetDirect                    ((RSLCALLTBL *)ExeExports)->ptr_BtGetDirect
        #define  BtUnlock                       ((RSLCALLTBL *)ExeExports)->ptr_BtUnlock
        #define  BtGetLastError                 ((RSLCALLTBL *)ExeExports)->ptr_BtGetLastError
        #define  BtBindField                    ((RSLCALLTBL *)ExeExports)->ptr_BtBindField
        #define  BtUnbindField                  ((RSLCALLTBL *)ExeExports)->ptr_BtUnbindField
        #define  BtUnbindAll                    ((RSLCALLTBL *)ExeExports)->ptr_BtUnbindAll
        #define  BtGetFieldID                   ((RSLCALLTBL *)ExeExports)->ptr_BtGetFieldID
        #define  BtGetField                     ((RSLCALLTBL *)ExeExports)->ptr_BtGetField
        #define  BtPutField                     ((RSLCALLTBL *)ExeExports)->ptr_BtPutField
        #define  BtBeginTransaction             ((RSLCALLTBL *)ExeExports)->ptr_BtBeginTransaction
        #define  BtEndTransaction               ((RSLCALLTBL *)ExeExports)->ptr_BtEndTransaction
        #define  BtAbortTransaction             ((RSLCALLTBL *)ExeExports)->ptr_BtAbortTransaction
        #define  BtSetBlobBuffSize              ((RSLCALLTBL *)ExeExports)->ptr_BtSetBlobBuffSize
        #define  BtOpenBlob                     ((RSLCALLTBL *)ExeExports)->ptr_BtOpenBlob
        #define  BtCloseBlob                    ((RSLCALLTBL *)ExeExports)->ptr_BtCloseBlob
        #define  BtReadBlob                     ((RSLCALLTBL *)ExeExports)->ptr_BtReadBlob
        #define  BtWriteBlob                    ((RSLCALLTBL *)ExeExports)->ptr_BtWriteBlob
        #define  BtPosBlob                      ((RSLCALLTBL *)ExeExports)->ptr_BtPosBlob
        #define  BtGetBlobSize                  ((RSLCALLTBL *)ExeExports)->ptr_BtGetBlobSize
        #define  BtTruncateBlob                 ((RSLCALLTBL *)ExeExports)->ptr_BtTruncateBlob
        #define  BtErrorText                    ((RSLCALLTBL *)ExeExports)->ptr_BtErrorText
        #define  BtStopClient                   ((RSLCALLTBL *)ExeExports)->ptr_BtStopClient
        #define  BtLoopTables                   ((RSLCALLTBL *)ExeExports)->ptr_BtLoopTables
        #define  BtGetGT                        ((RSLCALLTBL *)ExeExports)->ptr_BtGetGT
        #define  BtGetLT                        ((RSLCALLTBL *)ExeExports)->ptr_BtGetLT
        #define  BtChangeKeyPath                ((RSLCALLTBL *)ExeExports)->ptr_BtChangeKeyPath
        #define  AddObjectProviderMod           ((RSLCALLTBL *)ExeExports)->ptr_AddObjectProviderMod
        #define  ValueMake                      ((RSLCALLTBL *)ExeExports)->ptr_ValueMake
        #define  ValueClear                     ((RSLCALLTBL *)ExeExports)->ptr_ValueClear
        #define  ValueCopy                      ((RSLCALLTBL *)ExeExports)->ptr_ValueCopy
        #define  ValueIseq                      ((RSLCALLTBL *)ExeExports)->ptr_ValueIseq
        #define  ValueMove                      ((RSLCALLTBL *)ExeExports)->ptr_ValueMove
        #define  ValueSet                       ((RSLCALLTBL *)ExeExports)->ptr_ValueSet
        #define  PushValue                      ((RSLCALLTBL *)ExeExports)->ptr_PushValue
        #define  PopValue                       ((RSLCALLTBL *)ExeExports)->ptr_PopValue
        #define  PutParm2                       ((RSLCALLTBL *)ExeExports)->ptr_PutParm2
        #define  ReturnVal2                     ((RSLCALLTBL *)ExeExports)->ptr_ReturnVal2
        #define  LobjInitList                   ((RSLCALLTBL *)ExeExports)->ptr_LobjInitList
        #define  LobjDoneList                   ((RSLCALLTBL *)ExeExports)->ptr_LobjDoneList
        #define  LobjRemove                     ((RSLCALLTBL *)ExeExports)->ptr_LobjRemove
        #define  LobjForEach                    ((RSLCALLTBL *)ExeExports)->ptr_LobjForEach
        #define  LobjForEachL                   ((RSLCALLTBL *)ExeExports)->ptr_LobjForEachL
        #define  LobjFirstThat                  ((RSLCALLTBL *)ExeExports)->ptr_LobjFirstThat
        #define  LobjLastThat                   ((RSLCALLTBL *)ExeExports)->ptr_LobjLastThat
        #define  LobjNitems                     ((RSLCALLTBL *)ExeExports)->ptr_LobjNitems
        #define  LobjInsert                     ((RSLCALLTBL *)ExeExports)->ptr_LobjInsert
        #define  LobjPrevItem                   ((RSLCALLTBL *)ExeExports)->ptr_LobjPrevItem
        #define  LobjNextItem                   ((RSLCALLTBL *)ExeExports)->ptr_LobjNextItem
        #define  LobjFirstItem                  ((RSLCALLTBL *)ExeExports)->ptr_LobjFirstItem
        #define  LobjLastItem                   ((RSLCALLTBL *)ExeExports)->ptr_LobjLastItem
        #define  SobjUnlink                     ((RSLCALLTBL *)ExeExports)->ptr_SobjUnlink
        #define  FindMacro                      ((RSLCALLTBL *)ExeExports)->ptr_FindMacro
        #define  SwitchToMacro                  ((RSLCALLTBL *)ExeExports)->ptr_SwitchToMacro
        #define  AddObjectProviderModEx         ((RSLCALLTBL *)ExeExports)->ptr_AddObjectProviderModEx
        #define  UserNumber                     ((RSLCALLTBL *)ExeExports)->ptr_UserNumber
        #define  RunMacro                       ((RSLCALLTBL *)ExeExports)->ptr_RunMacro
        #define  BtClearRecordBuff              ((RSLCALLTBL *)ExeExports)->ptr_BtClearRecordBuff
        #define  fs_screenSize                  ((RSLCALLTBL *)ExeExports)->ptr_fs_screenSize
        #define  fs_saveStat                    ((RSLCALLTBL *)ExeExports)->ptr_fs_saveStat
        #define  fs_restStat                    ((RSLCALLTBL *)ExeExports)->ptr_fs_restStat
        #define  fs_event                       ((RSLCALLTBL *)ExeExports)->ptr_fs_event
        #define  fs_wrtnatr                     ((RSLCALLTBL *)ExeExports)->ptr_fs_wrtnatr
        #define  fs_setattr                     ((RSLCALLTBL *)ExeExports)->ptr_fs_setattr
        #define  fs_curattr                     ((RSLCALLTBL *)ExeExports)->ptr_fs_curattr
        #define  fs_wrtstr                      ((RSLCALLTBL *)ExeExports)->ptr_fs_wrtstr
        #define  fs_wrtncell                    ((RSLCALLTBL *)ExeExports)->ptr_fs_wrtncell
        #define  fs_gettext                     ((RSLCALLTBL *)ExeExports)->ptr_fs_gettext
        #define  fs_puttext                     ((RSLCALLTBL *)ExeExports)->ptr_fs_puttext
        #define  fs_movetext                    ((RSLCALLTBL *)ExeExports)->ptr_fs_movetext
        #define  fs_clr                         ((RSLCALLTBL *)ExeExports)->ptr_fs_clr
        #define  fs_bar                         ((RSLCALLTBL *)ExeExports)->ptr_fs_bar
        #define  fs_box                         ((RSLCALLTBL *)ExeExports)->ptr_fs_box
        #define  fs_setcurtype                  ((RSLCALLTBL *)ExeExports)->ptr_fs_setcurtype
        #define  fs_setcurpoz                   ((RSLCALLTBL *)ExeExports)->ptr_fs_setcurpoz
        #define  fs_getcurpoz                   ((RSLCALLTBL *)ExeExports)->ptr_fs_getcurpoz
        #define  fs_version                     ((RSLCALLTBL *)ExeExports)->ptr_fs_version
        #define  fs_padstr                      ((RSLCALLTBL *)ExeExports)->ptr_fs_padstr
        #define  fs_wrtItemStr                  ((RSLCALLTBL *)ExeExports)->ptr_fs_wrtItemStr
        #define  fs_wrtMarkStr                  ((RSLCALLTBL *)ExeExports)->ptr_fs_wrtMarkStr
        #define  fs_saveStat                    ((RSLCALLTBL *)ExeExports)->ptr_fs_saveStat
        #define  fs_restStat                    ((RSLCALLTBL *)ExeExports)->ptr_fs_restStat
        #define  fs_statLine                    ((RSLCALLTBL *)ExeExports)->ptr_fs_statLine
        #define  fs_getBuffSize                 ((RSLCALLTBL *)ExeExports)->ptr_fs_getBuffSize
        #define  fs_event                       ((RSLCALLTBL *)ExeExports)->ptr_fs_event
        #define  fs_screenSize                  ((RSLCALLTBL *)ExeExports)->ptr_fs_screenSize
        #define  NWcallEx                       ((RSLCALLTBL *)ExeExports)->ptr_NWcallEx
        #define  fs_getDeferBuff                ((RSLCALLTBL *)ExeExports)->ptr_fs_getDeferBuff
        #define  fs_queueMessage                ((RSLCALLTBL *)ExeExports)->ptr_fs_queueMessage
        #define  fs_getSendBuff                 ((RSLCALLTBL *)ExeExports)->ptr_fs_getSendBuff
        #define  fs_sendMessage                 ((RSLCALLTBL *)ExeExports)->ptr_fs_sendMessage
        #define  fs_transactMessage             ((RSLCALLTBL *)ExeExports)->ptr_fs_transactMessage
        #define  copyToLow                      ((RSLCALLTBL *)ExeExports)->ptr_copyToLow
        #define  copyFromLow                    ((RSLCALLTBL *)ExeExports)->ptr_copyFromLow
        #define  GetLowMem                      ((RSLCALLTBL *)ExeExports)->ptr_GetLowMem
        #define  FreeLowMem                     ((RSLCALLTBL *)ExeExports)->ptr_FreeLowMem
        #define  ReadIniFile                    ((RSLCALLTBL *)ExeExports)->ptr_ReadIniFile
        #define  RunProg                        ((RSLCALLTBL *)ExeExports)->ptr_RunProg
        #define  GetParmAttr                    ((RSLCALLTBL *)ExeExports)->ptr_GetParmAttr
        #define  LocRslDlmVersion               ((RSLCALLTBL *)ExeExports)->ptr_LocRslDlmVersion
                               
        #define  LocRslTArrayID                 ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayID    
        #define  LocRslTArrayCreate             ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayCreate
        #define  LocRslTArrayPut                ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayPut   
        #define  LocRslTArrayGet                ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayGet   
        #define  LocRslTArraySize               ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArraySize  
        #define  LocInstBuf                     ((RSLCALLTBL *)ExeExports)->ptr_LocInstBuf        
        #define  LociNewMem                     ((RSLCALLTBL *)ExeExports)->ptr_LociNewMem
        #define  LociNewMemNull                 ((RSLCALLTBL *)ExeExports)->ptr_LociNewMemNull
        #define  LociDoneMem                    ((RSLCALLTBL *)ExeExports)->ptr_LociDoneMem
        #define  LocRslObjGetUniqID             ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjGetUniqID
        #define  LocRslObjMemberFromID          ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjMemberFromID
        #define  LocRslObjInvoke                ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjInvoke
        #define  LocRslObjMemberFromName        ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjMemberFromName

        #define  LocDoRaiseEvents               ((RSLCALLTBL *)ExeExports)->ptr_LocDoRaiseEvents
        #define  LocRslGetInstance              ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetInstance
        #define  LocRslSetInstance              ((RSLCALLTBL *)ExeExports)->ptr_LocRslSetInstance
        #define  LocRslLockInstance             ((RSLCALLTBL *)ExeExports)->ptr_LocRslLockInstance
        #define  LocRslUnLockInstance           ((RSLCALLTBL *)ExeExports)->ptr_LocRslUnLockInstance
        #define  LocRslInstGetNumModule         ((RSLCALLTBL *)ExeExports)->ptr_LocRslInstGetNumModule
        #define  LocRslSendMes                  ((RSLCALLTBL *)ExeExports)->ptr_LocRslSendMes
        #define  LocRslTArrayCnvType            ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayCnvType
        #define  LocRslTArrayCnvBinType         ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayCnvBinType
        #define  LocRslTArrayCnvCheckBounds     ((RSLCALLTBL *)ExeExports)->ptr_LocRslTArrayCnvCheckBounds
        #define  LocRslGetInstSymbol            ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetInstSymbol
        #define  LocRslCallInstSymbol           ((RSLCALLTBL *)ExeExports)->ptr_LocRslCallInstSymbol
        #define  LocRslIsTArray                 ((RSLCALLTBL *)ExeExports)->ptr_LocRslIsTArray
        #define  LocSetSymAttr                  ((RSLCALLTBL *)ExeExports)->ptr_LocSetSymAttr

        #define  LocRslAddUniClass              ((RSLCALLTBL *)ExeExports)->ptr_LocRslAddUniClass          
        #define  LocCreateObjectOfClass         ((RSLCALLTBL *)ExeExports)->ptr_LocCreateObjectOfClass     
        #define  LocUniObjectInit               ((RSLCALLTBL *)ExeExports)->ptr_LocUniObjectInit           
        #define  LocAddObjectProviderModEx2     ((RSLCALLTBL *)ExeExports)->ptr_LocAddObjectProviderModEx2 
        #define  LocRslGetInnerObj              ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetInnerObj
        #define  LocGenObjCreateEx              ((RSLCALLTBL *)ExeExports)->ptr_LocGenObjCreateEx
        #define  LocRslUniPropPtr               ((RSLCALLTBL *)ExeExports)->ptr_LocRslUniPropPtr

        #define  LocCreateObjectOfClassEx       ((RSLCALLTBL *)ExeExports)->ptr_LocCreateObjectOfClassEx
        #define  LocRslFindUniClass             ((RSLCALLTBL *)ExeExports)->ptr_LocRslFindUniClass
        #define  LocRslGetUniClass              ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetUniClass
        #define  LocRslUniCast                  ((RSLCALLTBL *)ExeExports)->ptr_LocRslUniCast

        #define  LocBtSetCnvMode                ((RSLCALLTBL *)ExeExports)->ptr_LocBtSetCnvMode         
        #define  LocBtGetErrorInfo              ((RSLCALLTBL *)ExeExports)->ptr_LocBtGetErrorInfo       
        #define  LocBtOpenDataBaseSimple        ((RSLCALLTBL *)ExeExports)->ptr_LocBtOpenDataBaseSimple 
        #define  LocBtGetStructure              ((RSLCALLTBL *)ExeExports)->ptr_LocBtGetStructure
        #define  LocBtFreeStructure             ((RSLCALLTBL *)ExeExports)->ptr_LocBtFreeStructure
        #define  LocBtStrGetFieldName           ((RSLCALLTBL *)ExeExports)->ptr_LocBtStrGetFieldName
        #define  LocBtStrGetComment             ((RSLCALLTBL *)ExeExports)->ptr_LocBtStrGetComment

        #define  LocImportModule                ((RSLCALLTBL *)ExeExports)->ptr_LocImportModule
                         
        #define  Locfs_addDispTable             ((RSLCALLTBL *)ExeExports)->ptr_addDispTable
        #define  Locfs_remDispTable             ((RSLCALLTBL *)ExeExports)->ptr_remDispTable
        #define  Locfs_makeEvent                ((RSLCALLTBL *)ExeExports)->ptr_makeEvent
        #define  Locfs_getAppIntf               ((RSLCALLTBL *)ExeExports)->ptr_getAppIntf

        #define  LocRslActiveXCreateEx          ((RSLCALLTBL *)ExeExports)->ptr_LocRslActiveXCreateEx
        #define  LocRslObjGetIDispatch          ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjGetIDispatch
        #define  LocRslObjGetTGenObj            ((RSLCALLTBL *)ExeExports)->ptr_LocRslObjGetTGenObj
        #define  LocRslGetActiveXDispatch       ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetActiveXDispatch

        #define  LocCreateObjectOfRslClass      ((RSLCALLTBL *)ExeExports)->ptr_LocCreateObjectOfRslClass
        #define  LocRslAddObjVar                ((RSLCALLTBL *)ExeExports)->ptr_LocRslAddObjVar
        #define  LocRslAddVar                   ((RSLCALLTBL *)ExeExports)->ptr_LocRslAddVar
        #define  LocRslSetClientData            ((RSLCALLTBL *)ExeExports)->ptr_LocRslSetClientData
        #define  LocRslGetClientData            ((RSLCALLTBL *)ExeExports)->ptr_LocRslGetClientData

        #define  SobjUnlink                     ((RSLCALLTBL *)ExeExports)->ptr_SobjUnlink

        #define  LocSobjFirstItem               ((RSLCALLTBL *)ExeExports)->ptr_LocSobjFirstItem
        #define  LocSobjLastItem                ((RSLCALLTBL *)ExeExports)->ptr_LocSobjLastItem
        #define  LocSobjNextItem                ((RSLCALLTBL *)ExeExports)->ptr_LocSobjNextItem
        #define  LocSobjPrevItem                ((RSLCALLTBL *)ExeExports)->ptr_LocSobjPrevItem

        #define  LocImportModulePath            ((RSLCALLTBL *)ExeExports)->ptr_LocImportModulePath
        #define  LocGetInterpreterInterface     ((RSLCALLTBL *)ExeExports)->ptr_LocGetInterpreterInterface
        #define  LocGetRecordHandlerInfo        ((RSLCALLTBL *)ExeExports)->ptr_LocGetRecordHandlerInfo
        #define  LocCreateRecordHandler         ((RSLCALLTBL *)ExeExports)->ptr_LocCreateRecordHandler

        #define  ValueSetAs                     ((RSLCALLTBL *)ExeExports)->ptr_ValueSetAs   
        #define  SetParmAs                      ((RSLCALLTBL *)ExeExports)->ptr_SetParmAs    
        #define  ValueGet                       ((RSLCALLTBL *)ExeExports)->ptr_ValueGet     
        #define  CnvType                        ((RSLCALLTBL *)ExeExports)->ptr_CnvType      
        #define  GetReturnVal                   ((RSLCALLTBL *)ExeExports)->ptr_GetReturnVal 
        #define  RunResult                      ((RSLCALLTBL *)ExeExports)->ptr_RunResult 
    #endif // RSL_NO_MACRO
          
    #define  SobjInitSys           ((RSLCALLTBL *)ExeExports)->ptr_SobjInit
    #define  SobjUnlinkSys         ((RSLCALLTBL *)ExeExports)->ptr_SobjUnlink
    #define  SobjNextItemSys(obj)  SobjNextItem(obj)
#else  // RSL_MD_FLAT
    // -----------------------------------------------------------------------------
    #define  RSLCALL(fun)  fun
#endif // RSL_MD_FLAT

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif // __MSCALL_H

/* <-- EOF --> */