/*************************************************************************
        File Name               : krnlintf.h                    05.10.2000
        Programmer              : A.Tokmashev

        Subsystem               : Interpreter RSL
        Description             : interface for RSL-instance and interface for Cache
                    instances.

        Copyright(C) 1991 - 2000 by R-Style Software Lab.
        All Rights Reserved.

*************************************************************************/

#ifndef __KRNLINTF_H
#define __KRNLINTF_H

# include <platform.h>

#include <packpshn.h>

///////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
#define _MAX_NAMESPACE     32

// -----------------------------------------------------------------------------
#if !defined(__IDATA_H) && !defined(__RSLTYPE_H)
RSL_MAKE_HANDLE(HRD);
#endif

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(HRSLSYM);
RSL_MAKE_HANDLE(HRSLCOM);

///////////////////////////////////////////////////////////////////////////////
//
// The messages handler base.
//
///////////////////////////////////////////////////////////////////////////////

typedef int (*TRSLInstMsgProc)(struct tagTRSLMsgHandler *handler, struct tagTRSLInstance *inst, IMESSAGE mes, void *ptr);

// -----------------------------------------------------------------------------
typedef struct tagTRSLMsgHandler
      {
       union
           {
            SOBJ                       parent;
            struct tagTRSLMsgHandler  *pNextHandler;
           };

       TRSLInstMsgProc                 msgProc;
      } TRSLMsgHandler;

// -----------------------------------------------------------------------------
typedef struct tagTObjNameSpace
      {
       TRSLMsgHandler  *nameSpace;
      } TObjNameSpace; 

// -----------------------------------------------------------------------------
typedef struct tagTRSLHostData
      {
       TRSLMsgHandler  *host;
      } TRSLHostData;

///////////////////////////////////////////////////////////////////////////////
typedef enum
      {
       STATE_UNKNOWN,
       STATE_CLOSE,
       STATE_OPEN,
       STATE_RUN
      } RSLINST_STATE;
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//
// Interface for RSL-instance
//
///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
typedef struct tagTRSLInstance
      {
       struct tagRSLInstIntf  *vtbl;
      } TRSLInstance, *HRSLINST;

// -----------------------------------------------------------------------------
typedef struct tagTRslInstData
      {
       TRSLInstance  *data;
      } TRslInstData;

// -----------------------------------------------------------------------------
typedef struct tagRSLInstIntf
      {
       bool           (RSLAPI *AddModule)            (HRSLINST inst, const char *name);
       bool           (RSLAPI *PushModule)           (HRSLINST inst, const char *name, bool privateMode);
       bool           (RSLAPI *PopModule)            (HRSLINST inst);
       bool           (RSLAPI *Execute)              (HRSLINST inst);
       void           (RSLAPI *Stop)                 (HRSLINST inst);
       
       RSLINST_STATE  (RSLAPI *GetState)             (HRSLINST inst);

       HRSLSYM        (RSLAPI *RslGetInstSymbol)     (HRSLINST inst, const char *name, const char *moduleName);
       bool           (RSLAPI *RslCallInstSymbol)    (HRSLINST inst, HRSLSYM hSym, int code, int nPar, VALUE *par, VALUE *propORret);

       int            (RSLAPI *GetCountError)        (HRSLINST inst);
       ERRINFO       *(RSLAPI *GetErrorInfo)         (HRSLINST inst, int index);
       
       bool           (RSLAPI *TestExistModule)      (HRSLINST inst, const char *modName);

       bool           (RSLAPI *AttachMsgHandler)     (HRSLINST inst, TRSLMsgHandler *handler);
       bool           (RSLAPI *DetachMsgHandler)     (HRSLINST inst, TRSLMsgHandler *handler);
       
       bool           (RSLAPI *CheckActualFiles)     (HRSLINST inst);
       int            (RSLAPI *RslSendMes)           (HRSLINST inst, IMESSAGE mes, void *ptr);

       const char    *(RSLAPI *GetName)              (HRSLINST inst);
       HRD            (RSLAPI *GetHRD)               (HRSLINST inst);
       short int      (RSLAPI *GetVersion)           (HRSLINST inst, short int *verLo);
       
       //Add by Ushakov
       bool           (RSLAPI *SetDbgModule)         (HRSLINST inst, const char *name);    
       bool           (RSLAPI *ExecuteEx)            (HRSLINST inst);
       bool           (RSLAPI *PushModuleNotExec)    (HRSLINST inst, const char *name, bool privateMode);

       bool           (RSLAPI *UninitModule)         (HRSLINST inst, const char *name);
       bool           (RSLAPI *SetModuleNotExecState)(HRSLINST inst, const char *name);

       int            (RSLAPI *GetRSCOMerror)        (HRSLINST inst);
      } TRSLInstIntf; 

// -----------------------------------------------------------------------------
#define  HRSLINST_PTR(ptr)  ((HRSLINST)(ptr))
#define  HRSLINST_FUN(ptr)  (((HRSLINST)(ptr))->vtbl)

// -----------------------------------------------------------------------------
#define  MAKE_TABLE_RSLINSTANCE(name, pref )                                                      \
                                               TRSLInstIntf  name =                               \
                                                                    {                             \
                                                                     pref##AddModule,             \
                                                                     pref##PushModule,            \
                                                                     pref##PopModule,             \
                                                                     pref##Execute,               \
                                                                     pref##Stop,                  \
                                                                     pref##GetState,              \
                                                                     pref##RslGetInstSymbol,      \
                                                                     pref##RslCallInstSymbol,     \
                                                                     pref##GetCountError,         \
                                                                     pref##GetErrorInfo,          \
                                                                     pref##TestExistModule,       \
                                                                     pref##AttachMsgHandler,      \
                                                                     pref##DetachMsgHandler,      \
                                                                     pref##CheckActualFiles,      \
                                                                     pref##RslSendMes,            \
                                                                     pref##GetName,               \
                                                                     pref##GetHRD,                \
                                                                     pref##GetVersion,            \
                                                                     pref##SetDbgModule,          \
                                                                     pref##ExecuteEx,             \
                                                                     pref##PushModuleNotExec,     \
                                                                     pref##UninitModule,          \
                                                                     pref##SetModuleNotExecState, \
                                                                     pref##GetRSCOMerror          \
                                                                    }


///////////////////////////////////////////////////////////////////////////////
//
// interface for RSL-Cache instances
//
///////////////////////////////////////////////////////////////////////////////

typedef struct tagRSLCache
      {
       struct tagRSLCacheIntf  *vtbl;
      } TRSLCache, *HRSLCACHE; 

// -----------------------------------------------------------------------------
typedef struct tagRSLCacheIntf
      {
       bool      (RSLAPI *Enable)         (HRSLCACHE cache, bool fEnable);
       bool      (RSLAPI *IsEnable)       (HRSLCACHE cache);
       void      (RSLAPI *Clear)          (HRSLCACHE cache);

       HRSLINST  (RSLAPI *FindInstance)   (HRSLCACHE cache, const char *nameSpace, bool *flag);
       bool      (RSLAPI *PutInstance )   (HRSLCACHE cache, HRSLINST inst);
       void      (RSLAPI *DoneInstance)   (HRSLCACHE cache, HRSLINST inst);
       void      (RSLAPI *DeleteInstance) (HRSLCACHE cache, HRSLINST inst);

       short int (RSLAPI *GetVersion)     (HRSLCACHE cache, short int *verLo);
       HRSLINST  (RSLAPI *SearchInstance) (HRSLCACHE cache, const char *nameSpace);
       bool      (RSLAPI *GetSize)        (HRSLCACHE cache, int *maxSize, int *curSize);
       bool      (RSLAPI *HasFreeInstance)(HRSLCACHE cache);
       bool      (RSLAPI *CheckInstance)  (HRSLCACHE cache, const char *nameSpace, int *status);
      } TRSLCacheIntf;

// -----------------------------------------------------------------------------
#define  HRSLCACHE_PTR(ptr)  ((HRSLCACHE)(ptr))
#define  HRSLCACHE_FUN(ptr)  (((HRSLCACHE)(ptr))->vtbl)

// -----------------------------------------------------------------------------
#define  MAKE_TABLE_RSLCACHE(name, pref )                                                  \
                                           TRSLCacheIntf  name =                           \
                                                                 {                         \
                                                                    pref##Enable,          \
                                                                    pref##IsEnable,        \
                                                                    pref##Clear,           \
                                                                    pref##FindInstance,    \
                                                                    pref##PutInstance,     \
                                                                    pref##DoneInstance,    \
                                                                    pref##DeleteInstance,  \
                                                                    pref##GetVersion,      \
                                                                    pref##SearchInstance,  \
                                                                    pref##GetSize,         \
                                                                    pref##HasFreeInstance, \
                                                                    pref##CheckInstance    \
                                                                 }

// -----------------------------------------------------------------------------
typedef struct tagTRSLCacheData
      {
       HRSLCACHE    cache;           // Хендл кэша

       // #3344
       int          maxCacheSize;    // Максимальный размер кэша
       int          curCacheSize;    // Текущий размер кэша
       const char  *defaultName;     // Имя дефолтного наймспейса (из RslDefInstName или "Default")
      } TRSLCacheData;

///////////////////////////////////////////////////////////////////////////////
//
// interface for RSL-Value
//
///////////////////////////////////////////////////////////////////////////////

typedef struct tagRSLValueIntf
      {
       void      (RSLAPI *Make)      (HRSLINST inst, VALUE *val); 
       void      (RSLAPI *Clear)     (HRSLINST inst, VALUE *val);
       void      (RSLAPI *Copy)      (HRSLINST inst, VALUE *from, VALUE *to);
       bool      (RSLAPI *IsEqual)   (HRSLINST inst, VALUE *This, VALUE *dest); 
       void      (RSLAPI *Move)      (HRSLINST inst, VALUE *from, VALUE *to);
       void      (RSLAPI *Set)       (HRSLINST inst, VALUE *val, VALTYPE v_type, void *ptr);
       short int (RSLAPI *GetVersion)(HRSLINST inst, short int *verLo);

       bool      (RSLAPI *Get)       (HRSLINST inst, VALUE *val, VALTYPE v_type, void *ptr);
       bool      (RSLAPI *CvtTo)     (HRSLINST inst, VALUE *val, VALTYPE v_type);
       bool      (RSLAPI *SetAs)     (HRSLINST inst, VALUE *val, VALTYPE needType, VALTYPE v_type, void *ptr);
      } TRSLValueIntf;

///////////////////////////////////////////////////////////////////////////////
//
// interface for RSL-GenObject
//
///////////////////////////////////////////////////////////////////////////////

typedef struct tagRSLGenObjIntf
      {
       unsigned short (RSLAPI *AddRef)              (HRSLINST inst, TGenObject *pObj);
       unsigned short (RSLAPI *Release)             (HRSLINST inst, TGenObject *pObj);

       int            (RSLAPI *RslObjMemberFromName)(HRSLINST inst, TGenObject *pObj, const char *name, long *id);
       int            (RSLAPI *RslObjInvoke)        (HRSLINST inst, TGenObject *pObj, long id, int code, int nPar, 
                                                     VALUE *par, VALUE *propORret);

       short int      (RSLAPI *GetVersion)          (HRSLINST inst, short int *verLo);
      } TRSLGenObjIntf;

///////////////////////////////////////////////////////////////////////////////
//
// interface of language RSL
//
///////////////////////////////////////////////////////////////////////////////

typedef struct tagTInterpreterIntf
      {
       HRSLINST         (RSLAPI *CreateRSLInstance)(const char *nameSpace, TRSLMsgHandler *host,
                                                    TRSLMsgHandler* (*CreateNameSpace)(void *initData),
                                                    void *initData, HRSLCOM hCom);
       unsigned long    (RSLAPI *DeleteRSLInstance)(HRSLINST inst);

       HRSLCACHE        (RSLAPI *CreateRSLCache)(int szLimit);
       void             (RSLAPI *DeleteRSLCache)(HRSLCACHE cache);

       TRSLValueIntf   *(RSLAPI *GetValueInterface)(void);
       TRSLGenObjIntf  *(RSLAPI *GetGenObjInterface)(void);
     
       HRSLINST         (RSLAPI *CreateRSLInstanceEx)(const char *, TRSLMsgHandler *, TRSLMsgHandler *, HRSLCOM);
       unsigned long    (RSLAPI *DeleteRSLInstanceEx)(HRSLINST);
      } TInterpreterIntf; 

///////////////////////////////////////////////////////////////////////////////

#include <packpop.h>

#ifdef __cplusplus
extern "C" {
#endif

DLMLINK TInterpreterIntf *RSLDLMAPI GetInterpreterInterface(void);

RSLLINK void RSLAPI EnableRunTime(HRSLINST inst, RSLINST_STATE runTime); // for conintf.hpp

#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////

#endif // __KRNLINST_H

/* <-- EOF --> */