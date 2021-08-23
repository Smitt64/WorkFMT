/*-----------------------------------------------------------------------*-

 File Name   : rscom.hpp

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 December 18,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __RSCOM_HPP
#define __RSCOM_HPP

# include "rstypes.h"

#include "packpshn.h"

// -----------------------------------------------------------------------------
#ifndef RSCOM_CODEPAGE
    #ifdef RSL_ANSI_STR
        #define  RSCOM_CODEPAGE  RSL_STR_ANSI
    #else
        #define  RSCOM_CODEPAGE  RSL_STR_OEM
    #endif
#endif

// -----------------------------------------------------------------------------
class IProxyStubMgr;
struct TAppObj;
struct TFsComplete;
struct TRsData;
struct TRsDataEx;
class IDispObj;
class TXServ;


// -----------------------------------------------------------------------------
#ifndef _INC_WINDOWS
    typedef void *         HWND;
    typedef unsigned long  DWORD;
#endif

// -----------------------------------------------------------------------------
#ifdef RSL_PL_WIN32
    #define  RSCOMCALL  __stdcall
#else
    #define  RSCOMCALL
#endif

// -----------------------------------------------------------------------------
#ifndef RSL_DLL_NAME_LEN
    #define  RSL_DLL_NAME_LEN  9
#endif

// -----------------------------------------------------------------------------
typedef unsigned long  TCallId;

// -----------------------------------------------------------------------------
// Commands for system table $$PS$
enum
   {
    CMD_PS_CREATE_SERVER = 1,
    CMD_CREATE_OBJECT,
    CMD_INVOKE
   };

// -----------------------------------------------------------------------------
enum RSL_STD_INTF
   {
    IID_RSOBJECT = 1,
    IID_RSMARSHAL,
    IID_RSPROXY,
    IID_RSSTUB,
    IID_PSMGR,
    IID_RSDISP,
    IID_RSDISPSERV,
    IID_RSSTREAM,
    IID_RSPARMINFO,
    IID_RSERRINFO,
    IID_PROXYDATA,
    IID_STUBDATA,
    IID_IMETAINFO,
    IID_RSCHANEL,
    IID_RSLIFETIME,
    IID_RSMODULE,
    IID_RSDISPEX,
    IID_RSGETBIND,
    IID_RsDataSet,
    IID_RSDISPBIND,     // 20
    IID_RSPROXYCTL,
    IID_RSLOCK,
    IID_RSEVENT,
    IID_RSSTUBMT,
    IID_RSBYVAL,
    IID_RSMARKER,
    IID_RSCTRLINTF,
    IID_RSSEMAPHORE,
    IID_IWINNHANDLE,
    IID_RSMLOCK,
    IID_RSISTREAM,      // Физически интерфейса IStream у нас нет

    IID_RSUSER = 1000
   };

// -----------------------------------------------------------------------------
typedef long  TRsStat;

// -----------------------------------------------------------------------------
enum RSL_ERR_STAT
   {
    RSL_STAT_OK                   = 0,
    RSL_STAT_EX                   = 1,
    RSL_STAT_NOREMOTE             = 2,
    RSL_STAT_NOTIMPL              = 3,
    RSL_STAT_UNKNOWNID            = 4,
    RSL_STAT_NOINTF               = 5,
    RSL_STAT_UNKNOWNSERVER        = 6,
    RSL_STAT_UNKNOWNCLASS         = 7,
    RSL_STAT_DISCON               = 8,
    RSL_STAT_NODEFAULTCHN         = 9,
    RSL_STAT_NOSTUB               = 10,
    RSL_STAT_NOPROXY              = 11,
    RSL_STAT_EXCEPTION            = 12, // Unknown RSException
    RSL_STAT_CNVDATA              = 13,
    RSL_STAT_TYPEINFO_CHANGED     = 14,
    RSL_STAT_UNKNOWNMODULE        = 15,
    RSL_STAT_PRMCOUNT             = 16, // The number of parameters too small
    RSL_STAT_NONAMED              = 17,
    RSL_STAT_UNKNDS               = 18, // Special for RslCreateDataSources impl.
    RSL_STAT_CHCLOSE              = 19,
    RSL_STAT_NOINIFILE            = 20,
    RSL_STAT_SYNCOBJ              = 21,
    RSL_STAT_CANCEL               = 22,
    RSL_STAT_CHECK_VERSION        = 23, // Не прошла проверка версии
    RSL_STAT_GEN                  = -1
   };

// -----------------------------------------------------------------------------
enum TRsCodePage
   {
    RSL_STR_OEM,
    RSL_STR_ANSI,
    RSL_STR_ASIS
   };

// -----------------------------------------------------------------------------
class IRsObj
{
 public:
   virtual unsigned    RSCOMCALL addRef() = 0;
   virtual unsigned    RSCOMCALL release() = 0;
   virtual bool        RSCOMCALL getInterface(int id, void **) = 0;

   virtual const char *RSCOMCALL getClassName() = 0;
   virtual TAppObj    *RSCOMCALL getAppObj() = 0;
};

// -----------------------------------------------------------------------------
class IRsObjControling
{
 public:
   virtual unsigned    RSCOMCALL addRef_() = 0;
   virtual unsigned    RSCOMCALL release_() = 0;
   virtual bool        RSCOMCALL getInterface_(int id, void **) = 0;

   virtual const char *RSCOMCALL getClassName_() = 0;
   virtual TAppObj    *RSCOMCALL getAppObj_() = 0;
};

// -----------------------------------------------------------------------------
#define  NullRsObj  ((IRsObj *)0)

// -----------------------------------------------------------------------------
// Проблема всплыла при сборке под MSVS 2017 x64, но, скорее всего, также
// актуальна и для MSVS 2010 x64.
// Пока исправляем только для MSVS 2017 x64

#if defined(_MSC_VER) && (_MSC_VER >= 1400) && defined(RSL_PL_WIN64)
    typedef RslHandle64  HSTUB;
#else
    typedef RslHandle32  HSTUB;
#endif

// -----------------------------------------------------------------------------
class IModuleObj : public IRsObj
{
 public:
   virtual void     RSCOMCALL lockModule()   = 0;
   virtual void     RSCOMCALL unLockModule() = 0;
   virtual unsigned RSCOMCALL getLockCount() = 0;
};

// -----------------------------------------------------------------------------
class ILifeTime : public IRsObj
{
 public:
   virtual void    RSCOMCALL finalRelease() = 0;
   virtual TRsStat RSCOMCALL finalConstruct(const char *objName, IModuleObj *pMod, void *uData) = 0;
};

// -----------------------------------------------------------------------------
#define  RSCOM_WAIT_TIMEOUT  258

// -----------------------------------------------------------------------------
class IRsLock : public IRsObj
{
 public:
   virtual bool RSCOMCALL lock(int timeOut) = 0;
   virtual bool RSCOMCALL unlock() = 0;
};

// -----------------------------------------------------------------------------
class IRsEvent : public IRsLock
{
 public:
   virtual bool RSCOMCALL setEvent()   = 0;
   virtual bool RSCOMCALL resetEvent() = 0;
   virtual bool RSCOMCALL isManual()   = 0;
};

// -----------------------------------------------------------------------------
class IRsSemaphore : public IRsLock
{
 public:
   virtual bool RSCOMCALL unlockEx(long count, long *prevCount) = 0;
};

// -----------------------------------------------------------------------------
class IRsMultiLock : public IRsLock
{
 public:
   virtual int  RSCOMCALL lockEx(int timeOut, bool waitForAll, unsigned wakeMask, bool *isAbandon) = 0;
   virtual bool RSCOMCALL unlockEx(long count, long *prevCount) = 0;
   virtual bool RSCOMCALL resetEvents() = 0;
   virtual bool RSCOMCALL isLocked(int index) = 0;
};

// -----------------------------------------------------------------------------
class IWinHandle : public IRsObj
{
 public:
   virtual uintptr_t RSCOMCALL getHandle() = 0;
};

// -----------------------------------------------------------------------------
class IChanel : public IRsObj
{
 public:
   virtual bool        RSCOMCALL cnSendAnswerMes(size_t sz, const void *in, uint32 cookie) = 0;
   virtual bool        RSCOMCALL cnAsyncSendMes(const void *in, size_t sz, const char *tblName, int cmdId, TFsComplete *cmplPtr) = 0;
   virtual bool        RSCOMCALL cnAsyncResult(TFsComplete *cmplPtr, void *out, size_t szOut, size_t *szOutPtr) = 0;
   virtual bool        RSCOMCALL cnAsyncWaitResult(TFsComplete *cmplPtr, DWORD timeOut, size_t *szOutPtr) = 0;
   virtual void        RSCOMCALL cnAsyncCancel(TFsComplete *cmplPtr) = 0;

   virtual const char *RSCOMCALL getName() = 0;

   virtual TRsStat     RSCOMCALL getErrorInfo(IProxyStubMgr *ps) = 0;

   virtual bool        RSCOMCALL cnAsyncSendMesMt(const void *in, size_t sz, const char *tblName,
                                                  int cmdId, TFsComplete *cmplPtr, TCallId id) = 0;

   virtual TRsStat     RSCOMCALL stopChannel() = 0;
};

// -----------------------------------------------------------------------------
class TAppImpl;

// -----------------------------------------------------------------------------
class IChanelHost
{
 public:
   virtual void           RSCOMCALL onNewMessage(const char *tblName, int cmdId, uint32 ansCookie,
                                                 const char *inMes, size_t sz, IChanel *cn, TCallId callId) = 0;

   virtual TRsStat        RSCOMCALL getNamedChannel(const char *name, IChanel **retVal) = 0;
   virtual TRsStat        RSCOMCALL addChannel(IChanel *cn) = 0;
   virtual TRsStat        RSCOMCALL remChannel(IChanel *cn) = 0;

   virtual TRsStat        RSCOMCALL addAppartment(TAppImpl *app) = 0;
   virtual TRsStat        RSCOMCALL remAppartment(TAppImpl *app) = 0;

   virtual unsigned long  RSCOMCALL decementAndRemChannel(IChanel *cn, unsigned long *refCount) = 0;

   virtual void           RSCOMCALL onError(const TXServ *er) = 0;

   virtual void           RSCOMCALL onChannelDisconnect(IChanel *cn) = 0;
};

// -----------------------------------------------------------------------------
class IChanelHost2
{
 public:
   virtual void     RSCOMCALL onNewMessage(const char *tblName, int cmdId, uint32 ansCookie, const char *inMes, size_t sz, IChanel *cn, TCallId callId) = 0;

   virtual TRsStat  RSCOMCALL getNamedChannel(const char *name, IChanel **retVal) = 0;
   virtual TRsStat  RSCOMCALL addChannel(IChanel *cn) = 0;
   virtual TRsStat  RSCOMCALL remChannel(IChanel *cn) = 0;
   virtual IChanel *RSCOMCALL getDefChannel() = 0;
   virtual IChanel *RSCOMCALL getIntraChannel() = 0;

   virtual TRsStat  RSCOMCALL addAppartment(TAppImpl *app) = 0;
   virtual TRsStat  RSCOMCALL remAppartment(TAppImpl *app) = 0;
};

// -----------------------------------------------------------------------------
#define  RSCOM_ACTION_CLEAR_ALWAYS   0
#define  RSCOM_ACTION_CLEAR_WAIT     1

// -----------------------------------------------------------------------------
namespace rsforms
{
 struct TSrvExtInf;

 TSrvExtInf *RslGetServExtIntf();
}

// -----------------------------------------------------------------------------
struct TPropertyHolder
     {
      const char  *key;
      const char  *value;
     };

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(PROPENUM);

// -----------------------------------------------------------------------------
class ISiteObj : public IRsObj
{
 public:
   virtual int          RSCOMCALL version() = 0;
   virtual const char  *RSCOMCALL getAppName() = 0;
   virtual const char  *RSCOMCALL getHlpName() = 0;

   virtual void         RSCOMCALL clearAction(int flag) = 0;
   virtual int          RSCOMCALL testBreak(int useDialog) = 0;

// In version 3;
   virtual const char  *RSCOMCALL getLbrPath() = 0;
   virtual HWND         RSCOMCALL getMainHWnd() = 0;
   virtual void         RSCOMCALL setMainHWnd(HWND wnd) = 0;

// In version 8;
   virtual int          RSCOMCALL HandleRsComError(TAppObj *app, TRsStat stat, unsigned long flags, unsigned long cat) = 0;
// In version 7;
   // virtual int          RSCOMCALL HandleRsComError (TAppObj *app, TRsStat stat, unsigned long flags) = 0;
// In version 4;
   //virtual void         RSCOMCALL HandleRsComError (TAppObj *app, TRsStat stat) = 0;

// In version 5;
   virtual void        *RSCOMCALL getWndManager() = 0;
   virtual void         RSCOMCALL setWndManager(void *) = 0;

// In version 6;
   virtual rsforms::TSrvExtInf *RSCOMCALL getSrvExtIntf() = 0;

// In version 9;

   virtual const char *RSCOMCALL getProperty(const char *key) = 0;
   virtual bool        RSCOMCALL setProperty(const char *key, const char *value) = 0;
   virtual PROPENUM    RSCOMCALL enumProperty(PROPENUM, TPropertyHolder *elem) = 0;
};

// -----------------------------------------------------------------------------
class IMarshalObj : public IRsObj
{
 public:
   virtual unsigned char  RSCOMCALL isProxy() = 0;
   virtual HSTUB          RSCOMCALL getStubHandle() = 0;

   virtual IProxyStubMgr *RSCOMCALL GetProxyStubMgr() = 0;
   virtual void           RSCOMCALL SetProxyStubMgr(IProxyStubMgr *m) = 0;
};

// -----------------------------------------------------------------------------
class IMarshalByValObj : public IMarshalObj
{
 public:
   virtual size_t RSCOMCALL dataSize() = 0;
   virtual size_t RSCOMCALL save(char *buff) = 0;
   virtual size_t RSCOMCALL load(const char *buff) = 0;
};

// -----------------------------------------------------------------------------
class IMarker : public IRsObj
{
 public:
   virtual void *RSCOMCALL getKeyBuff(size_t *bufSize) = 0;
};

// -----------------------------------------------------------------------------
class IProxyData : public IRsObj
{
};

// -----------------------------------------------------------------------------
class IStubData : public IRsObj
{
};

// -----------------------------------------------------------------------------
class IProxyControl : public IRsObj
{
 public:
   virtual TRsStat RSCOMCALL clrRmtStubsOnRelease(void) = 0;
};

// -----------------------------------------------------------------------------
class IProxy : public IMarshalObj
{
 public:
#ifdef RSL_MD_FLAT
   virtual void        RSCOMCALL connect(HSTUB p, IChanel *cn) = 0;
#endif

   virtual IChanel    *RSCOMCALL getChanel(void) = 0;

   virtual unsigned    RSCOMCALL getExtRefCount() = 0;
   virtual unsigned    RSCOMCALL incExtRefCount(unsigned n) = 0;
   virtual unsigned    RSCOMCALL decExtRefCount(unsigned n) = 0;

   virtual IProxyData *RSCOMCALL getProxyData() = 0;
   virtual void        RSCOMCALL setProxyData(IProxyData *) = 0;

};

// -----------------------------------------------------------------------------
class IStub : public IMarshalObj
{
 public:
   virtual const char *RSCOMCALL objectName() = 0;
   virtual const char *RSCOMCALL getProxyType() = 0;
   virtual size_t      RSCOMCALL unMarshalMes(int cmdId, const char *inBuff, size_t inSize, IChanel *cn) = 0;
   virtual void        RSCOMCALL setRunningObj(bool isRun) = 0;

   virtual IStubData  *RSCOMCALL getStubData() = 0;
   virtual void        RSCOMCALL setStubData(IStubData *p) = 0;
};

// -----------------------------------------------------------------------------
class ICallCtx;

// -----------------------------------------------------------------------------
class IStubMt : public IStub
{
 public:
   virtual size_t RSCOMCALL unMarshalMesMt(int cmdId, const char *inBuff, size_t inSize, IChanel *cn, ICallCtx *ctx) = 0;
};

// -----------------------------------------------------------------------------
enum
   {
    RSSTREAM_SEEK_SET = 0,
    RSSTREAM_SEEK_CUR = 1,
    RSSTREAM_SEEK_END = 2
   };

// -----------------------------------------------------------------------------
class IRsStream : public IRsObj
{
 public:
   virtual TRsStat RSCOMCALL read(void *buff, size_t sz, size_t *rdSz) = 0;
   virtual TRsStat RSCOMCALL write(const void *buff, size_t sz, size_t *wrSz) = 0;

   virtual TRsStat RSCOMCALL seek(long sz, int from) = 0;
   virtual TRsStat RSCOMCALL getPos(size_t *pos) = 0;

   virtual TRsStat RSCOMCALL getSize(size_t *sz) = 0;
   virtual TRsStat RSCOMCALL setSize(size_t sz) = 0;

   virtual TRsStat RSCOMCALL flush(void) = 0;

   virtual TRsStat RSCOMCALL getIStream(/*IStream*/ void **ptr) = 0;
};

// -----------------------------------------------------------------------------
class IParmInfo : public IRsObj
{
 public:
   virtual void             RSCOMCALL setData   (int index, const TRsData *ptr, int codePage) = 0;
   virtual void             RSCOMCALL setDataVal(int index, int tp, void *data) = 0;
   virtual void             RSCOMCALL setAttr   (int index, int attr) = 0;

   virtual const TRsData   *RSCOMCALL getData(int index) = 0;
   virtual int              RSCOMCALL getCount() = 0;

   virtual size_t           RSCOMCALL getMarshalDataSize(int index, IProxyStubMgr *ps) = 0;
   virtual size_t           RSCOMCALL marshalFromBuff(int index, const char *buff, IProxyStubMgr *ps, IChanel *cn) = 0;
   virtual size_t           RSCOMCALL marshalToBuff(int index, char *buff, IProxyStubMgr *ps, IChanel *cn) = 0;

   virtual void             RSCOMCALL clearData(int index) = 0;

   virtual const char      *RSCOMCALL toString(int index, int codePage) = 0;
   virtual void             RSCOMCALL setDataFromStringAs(int index, int tp, const char *p, int codePage) = 0;
   virtual int              RSCOMCALL getCodePage() = 0;

   virtual TRsStat          RSCOMCALL getDataVal(int index, int tp, void *data) = 0;

   virtual int              RSCOMCALL setCount(int count) = 0;

   virtual TRsStat          RSCOMCALL dataExchange(int dir) = 0;
   virtual TRsStat          RSCOMCALL getDispObj(IDispObj **obj) = 0;

   virtual void             RSCOMCALL setDataEx(int index, const TRsDataEx *ptr, int codePage) = 0;
   virtual const TRsDataEx *RSCOMCALL getDataEx(int index) = 0;

   virtual int              RSCOMCALL getAttr(int index) = 0;
   virtual int              RSCOMCALL getType(int index) = 0;

   virtual TRsStat          RSCOMCALL getDataValEx(int index, int tp, void *data, int moneyMode) = 0;
   virtual TRsStat          RSCOMCALL setDataValAs(int index, int asType, int tp, const void *data, int moneyMode) = 0;
};

// -----------------------------------------------------------------------------
class IRsErrorInfo : public IRsObj
{
 public:
   virtual int         RSCOMCALL getCount() = 0;
   virtual void        RSCOMCALL reset() = 0;

   virtual void        RSCOMCALL addError  (long erCode, int level, const char *desc, int codePage) = 0;

   virtual int         RSCOMCALL getLevel  (int index) = 0;
   virtual long        RSCOMCALL getErrCode(int index) = 0;
   virtual const char *RSCOMCALL getDesc   (int index, int codePage) = 0;
};

// -----------------------------------------------------------------------------
#define  RSCOM_DEF_LEVEL           0
#define  RS_AX_ERROR_LEVEL         1
#define  RSCOM_SYS_LEVEL           2
#define  RSCOM_AX_LEVEL            3
#define  RSCOM_EVENT_LEVEL         4
#define  RSCOM_COMMON_LEVEL        5
#define  RSCOM_SUPPORT_LEVEL       6
#define  RSCOM_RSL_ERROR_LEVEL     7
#define  RSCOM_WIN32_ERROR_LEVEL   8
#define  RSCOM_ITF_ERROR_LEVEL     9
#define  RSCOM_RTL_ERROR_LEVEL    10

// -----------------------------------------------------------------------------
// Use the same values as MS COM
enum
   {
    RS_DISP_METHOD         = 0x1,
    RS_DISP_PROPERTYGET    = 0x2,
    RS_DISP_METHODORPROP   = 0x3,
    RS_DISP_PROPERTYPUT    = 0x4,

    RS_DISP_PROPERTYPUTREF = 0x8,  // Do Not use in RSCOM
    RS_DISP_RAISEEVENT     = 0x10,
    RS_DISP_NOTIFY         = 0x20,
    RS_DISP_USEWNDLOOP     = 0x40,
    RS_DISP_NOBLOCKCALL    = 0x80
   };

// -----------------------------------------------------------------------------
#define  RSDISP_ENEVENT       -3
#define  RSDISP_ADVISE        -4
#define  RSDISP_UNADVISE      -5
#define  RSDISP_DEFAULT       -6
#define  RSDISP_GETJAVACLASS  -7

// -----------------------------------------------------------------------------
typedef long  RSDISPID;

// -----------------------------------------------------------------------------
enum TMetaFlags
   {
    RS_META_PROP    = 0x01,
    RS_META_METHOD  = 0x02,
    RS_META_EVENT   = 0x04,
    RS_META_CLASS   = 0x08,

    RS_META_RDONLY  = 0x10,
    RS_META_RDPROP  = 0x11,  // All propertys read & write + read only

    RS_META_CLSNAME = 0x20,  // To transfer class name

    RS_META_PROC    = 0x40,  // Global procedure in module

    RS_META_FUNC    = 0x80,  // Methods + Prop Get + Prop Put (with parms) + Global (static) proc
    RS_META_VAR     = 0x100, // Props without params

    RS_META_FULL    = 0xFFF
   };

// -----------------------------------------------------------------------------
struct TRsTypeElem
     {
      const char      *name;
      RSDISPID         id;
      unsigned char    retType;
      unsigned char   *parmsType;    // strlen (parmsType) == params count
      unsigned char    kind;         // One of TMetaFlags
      int              level;
      unsigned char    propSet;
      const char      *name_origin;  // AV 09.12.2010   - наименование в исходном регистре, так как написано в макросе
                                     //                   добавил за счет резерва (указатель - 4 байта)
      char             reserv[1];
     };

// -----------------------------------------------------------------------------
struct TClassData;

// -----------------------------------------------------------------------------
typedef const TClassData  *(*GetClassDataProc)(void);
typedef const TRsTypeElem *(*GetTypeElemProc)(void);

// -----------------------------------------------------------------------------
struct TClassData
     {
      GetClassDataProc  basePtr;
      GetTypeElemProc   elemsPtr;
      const char       *className;
     };

// -----------------------------------------------------------------------------
class IMetaInfo : public IRsObj
{
 public:
   virtual const char *RSCOMCALL getMetaClassName() = 0;
   virtual const char *RSCOMCALL getMetaClassNameCS() = 0;  //AV 09.12.2010 case sensitive

   virtual int         RSCOMCALL getNumByMask(unsigned mask) = 0;
   virtual TRsStat     RSCOMCALL getInfoByMask(int index, unsigned mask, TRsTypeElem *ptr) = 0;

   virtual TRsStat     RSCOMCALL findElem(const char *name, TRsTypeElem *ptr) = 0;
};

// -----------------------------------------------------------------------------
class IDispObj : public IRsObj
{
 public:
   virtual TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo *, int level) = 0;
   virtual TRsStat RSCOMCALL getId(const char *name, RSDISPID *id) = 0;

   virtual TRsStat RSCOMCALL getMetaInfo(int mask, IMetaInfo **info) = 0;
};

// -----------------------------------------------------------------------------
class IDispObjEx : public IDispObj
{
 public:
   virtual TRsStat RSCOMCALL getIdEx(const char *name, RSDISPID *id, int *level) = 0;
   virtual TRsStat RSCOMCALL getInfoKey(unsigned *infoKey) = 0;
};

// -----------------------------------------------------------------------------
class IDispCallBack : public IRsObj
{
 public:
   virtual TRsStat RSCOMCALL invokeImpl(RSDISPID id, int cmdType, IParmInfo *, int level) = 0;

};

// -----------------------------------------------------------------------------
class IDispBind : public IParmInfo
{
 public:                  // 0 - from IDispObj, 1 - to IDispObj
//   virtual TRsStat RSCOMCALL dataExchange (int dir) = 0;
//   virtual TRsStat RSCOMCALL getDispObj (IDispObj **obj) = 0;
};

// -----------------------------------------------------------------------------
class IGetBind : public IRsObj
{
 public:
   virtual TRsStat RSCOMCALL getBind(const char *names, int autoGet, IDispBind **out) = 0;
   virtual TRsStat RSCOMCALL getBind2(const char * const *names, int num, int autoGet, IDispBind **out) = 0;
};

// -----------------------------------------------------------------------------
class IDispServer : public IDispObj
{
 public:
   virtual TRsStat RSCOMCALL createObject(const char *typeName, const char *objName, int iid, void **obj) = 0;
};

// -----------------------------------------------------------------------------
class ITask
{
 public:
   virtual void RSCOMCALL run() = 0;
   virtual void RSCOMCALL complete() = 0;
};

// -----------------------------------------------------------------------------
class IThreadPool
{
 public:
   virtual void RSCOMCALL addTask(ITask *tsk) = 0;
};


// -----------------------------------------------------------------------------
class ICtxManager;
class TTask;
class TMsgElem;

// -----------------------------------------------------------------------------
#ifndef _INC_WINDOWS
    typedef void *  HANDLE;
#endif

// -----------------------------------------------------------------------------
class ICallCtx
{
 public:
   virtual char        *RSCOMCALL getBuffer(size_t sz) = 0;
   virtual char        *RSCOMCALL reAllocBuffer(size_t oldSize, size_t newSize) = 0;
   virtual char        *RSCOMCALL getSendBuff(size_t sz, const char *dllName, int cmd) = 0;
   virtual ICtxManager *RSCOMCALL getCtxMgr() = 0;
   virtual TTask       *RSCOMCALL getTask() = 0;

   virtual bool         RSCOMCALL addAsyncMesEx(TMsgElem *ptr, bool signal) = 0;

   virtual bool         RSCOMCALL addAsyncMes(const char *name, const void *in, size_t sz,
                                              int cmdId, uint32 ansCookie, IChanel *cn, TCallId callId) = 0;

   virtual TMsgElem    *RSCOMCALL getAsyncMes() = 0;
   virtual bool         RSCOMCALL checAsynckMesAndSignal() = 0;
   virtual size_t       RSCOMCALL handleAsyncMes(TMsgElem *ptr) = 0;
   virtual TCallId      RSCOMCALL getCurrentCallId() = 0;
   virtual void         RSCOMCALL setCurrentCallId(TCallId) = 0;
   virtual HANDLE       RSCOMCALL getHandle() = 0;

   virtual bool         RSCOMCALL getErrInfo(IRsErrorInfo **info) = 0;

   virtual void         RSCOMCALL releaseRemoteStub(HSTUB hst, unsigned extRefCount, IChanel *cn) = 0;

   virtual size_t       RSCOMCALL marshalReleaseStubInfoCount(IChanel *cn) = 0;
   virtual size_t       RSCOMCALL marshalReleaseStubInfo(char *buff, IChanel *cn, size_t count) = 0;

   virtual bool         RSCOMCALL getFirstReleaseChannel(IChanel **cn) = 0;

   virtual bool         RSCOMCALL setOneWayCall(bool oneWay) = 0;
   virtual bool         RSCOMCALL isOneWayCall() = 0;

   virtual bool         RSCOMCALL addBlockMes(TMsgElem *ptr) = 0;
   virtual TMsgElem    *RSCOMCALL getBlockMes() = 0;
};

// -----------------------------------------------------------------------------
class ICtxManager
{
 public:
   virtual ICallCtx *RSCOMCALL getCtxFor(TCallId id, bool *connectedToThread) = 0;
   virtual ICallCtx *RSCOMCALL getCtxForLocalThread() = 0;
   virtual bool      RSCOMCALL freeCtx(ICallCtx *ctx) = 0;
   virtual bool      RSCOMCALL lockCtx(ICallCtx *ctx) = 0;
   virtual ICallCtx *RSCOMCALL addMsgForCtx(TMsgElem *ptr, TCallId id, bool *connectedToThread, ICallCtx *alt) = 0;
   virtual void      RSCOMCALL onChannelDisconnect(IChanel *cn) = 0;
};

// -----------------------------------------------------------------------------
typedef void  (RSCOMCALL *FreeStrPtroc)(void *p);

// -----------------------------------------------------------------------------
struct TRsStrPtr
     {
      char          *pStr;
      FreeStrPtroc   proc;
     };

// -----------------------------------------------------------------------------
typedef bool (RSCOMCALL *TCreatePSProc)(TAppObj *appObj, const char *clsName, IProxy **p);

// -----------------------------------------------------------------------------
typedef int (RSCOMCALL *TCallHandler)(void *data);

// -----------------------------------------------------------------------------
struct TInterThreadCall
       {
        HANDLE         evOk;
        int            stat;
        TCallHandler   proc;
        void          *data;
       };

// -----------------------------------------------------------------------------
class IProxyStubMgr : public IRsObj
{
 public:
   virtual void          RSCOMCALL remRunningObj(IStub *obj) = 0;

   virtual void          RSCOMCALL remProxy(IProxy *obj) = 0;

   virtual void          RSCOMCALL ReleaseRemoteStub(HSTUB hst, unsigned extRefCount, IChanel *cn) = 0;


   virtual char         *RSCOMCALL getDeferBuff(IProxy *p, int cmdId, size_t sz) = 0;
   virtual void          RSCOMCALL queueMessage(IProxy *p, const char *mes) = 0;
   virtual char         *RSCOMCALL getSendBuff(IProxy *p, int cmdId, size_t sz) = 0;
   virtual void          RSCOMCALL sendMessage(IProxy *p, const char *mes) = 0;
   virtual char         *RSCOMCALL transactMessage(IProxy *p, const char *mes, size_t *outMesSz) = 0;

   virtual TRsStat       RSCOMCALL invoke(IProxy *p, RSDISPID id, int cmdType, IParmInfo *pInfo, int level) = 0;
   virtual TRsStat       RSCOMCALL getId(IProxy *p, const char *name, RSDISPID *id) = 0;
   virtual TRsStat       RSCOMCALL invokeName(IProxy *p, const char *name, int cmdType, IParmInfo *pInfo, int level, RSDISPID *id) = 0;


   virtual TRsStat       RSCOMCALL invokeCache(IProxy *p, RSDISPID id, int cmdType, IParmInfo *pInfo, int level) = 0;
   virtual TRsStat       RSCOMCALL getIdCache(IProxy *p, const char *name, RSDISPID *id) = 0;

   virtual char         *RSCOMCALL getReplyBuff(IStub *p, size_t sz, IChanel *cn) = 0;
   virtual size_t        RSCOMCALL unMarshalMesToIDisp(IDispObj *obj, int cmdId, const char *inBuff, size_t inSize, IStub *s, IChanel *cn) = 0;
   virtual size_t        RSCOMCALL unMarshalMesToIDispServ(IDispServer *obj, int cmdId, const char *inBuff, size_t inSize, IStub *s, IChanel *cn) = 0;

   virtual TRsStat       RSCOMCALL createRemoteObject(IProxy *p, const char *typeName, const char *objName, int iid, void **obj) = 0;

   virtual TRsStat       RSCOMCALL createRemoteServer(const char *moduleName, const char *servType, const char *servName,
                                                int iid, void **obj, IChanel *chanel, IParmInfo *pInfo, RSDISPID id) = 0;

   virtual size_t        RSCOMCALL processMessage(int cmd, const char *inMes, bool needAnswer, size_t mesSize, IChanel *cn) = 0;
   virtual int           RSCOMCALL ReleaseAllRemoteStubs(void) = 0;

   virtual bool          RSCOMCALL makeParmInfo(int numParms, bool useRetVal, int codePage, IParmInfo **pInfo) = 0;

   virtual TRsStat       RSCOMCALL MarshalObjFromStream(IMarshalObj **outObj, char *buff, IChanel *chanel, size_t *sz) = 0;
   virtual size_t        RSCOMCALL MarshalObjToStream(IMarshalObj *obj, char *buff, IChanel *cn) = 0;
   virtual size_t        RSCOMCALL MarshalObjToStreamSize(IMarshalObj *obj) = 0;

   virtual void          RSCOMCALL regPsCreator(const char *typeName, TCreatePSProc proc) = 0;
   virtual void          RSCOMCALL unRegPsCreator(const char *typeName) = 0;

   virtual bool          RSCOMCALL getErrInfo(IRsErrorInfo **info) = 0;

   virtual TRsStat       RSCOMCALL createLocalServer(const char *moduleName, const char *servType, const char *objName,
                                               int iid, void **obPtr, IParmInfo *pInfo, RSDISPID id) = 0;

   virtual TRsStat       RSCOMCALL getServerObject(const char *dllName, const char *servType, const char *objName, int iid,
                                             void **ptr, int isLocal, IChanel *chanel, IParmInfo *prm, RSDISPID id) = 0;

   virtual TRsStat       RSCOMCALL advise(IStub *obj, IDispObj *sink) = 0;
   virtual TRsStat       RSCOMCALL unAdvise(IStub *obj, IDispObj *sink) = 0;
   virtual TRsStat       RSCOMCALL enableEvent(IStub *obj, IDispObj *sink, RSDISPID id) = 0;
   virtual TRsStat       RSCOMCALL raiseEvent(IStub *obj, RSDISPID id, IParmInfo *pInfo, int level) = 0;

   virtual TRsStat       RSCOMCALL getRemoteMetaInfo(IProxy *p, int mask, IMetaInfo **info) = 0;
   virtual TRsStat       RSCOMCALL makeMetaInfo(const TClassData *el, const char *clsName, int mask, IMetaInfo **info) = 0;

   virtual TRsStat       RSCOMCALL unMarshalErrInfo(const char *ptr) = 0;

   virtual TRsStat       RSCOMCALL getMetaObject(const char *dllName, const char *servType, unsigned mask, IMetaInfo **ptr,
                                           int isLocal, IChanel *chanel) = 0;

   virtual const char   *RSCOMCALL getStatDesc(TRsStat stat, char *buff, int sz, int codePage) = 0;

   virtual TRsStat       RSCOMCALL attachObject(IRsObj *obj) = 0;

   virtual TRsStat       RSCOMCALL invokeHelper(int codePage, IDispObj *obj, RSDISPID id, int cmdType, int level,
                                                int retTp, const char *prmStr, ...) = 0;

   virtual long          RSCOMCALL calcErrInfoSize() = 0;
   virtual TRsStat       RSCOMCALL marshalErrInfo(char *ptr) = 0;

   virtual TRsStat       RSCOMCALL setExtObj(IStub *stub, IDispObj *obj) = 0;
   virtual TRsStat       RSCOMCALL getMixId(IStub *stub, const TClassData *el, const char *name, RSDISPID *id) = 0;
   virtual TRsStat       RSCOMCALL handleMixInvoke(IStub *stub, RSDISPID id, int cmdType, IParmInfo *pInfo, int level, IDispCallBack *p) = 0;
   virtual TRsStat       RSCOMCALL makeMetaInfoEx(IStub *stub, const TClassData *el, const char *clsName, int mask, IMetaInfo **info) = 0;

   virtual TRsStat       RSCOMCALL getIdCacheEx(IProxy *p, const char *name, RSDISPID *id, int *level) = 0;
   virtual TRsStat       RSCOMCALL getProxyInfoKey(IProxy *p, unsigned *infoKey) = 0;
   virtual TRsStat       RSCOMCALL getMixIdEx(IStub *stub, const TClassData *el, const char *name, RSDISPID *id, int *level) = 0;
   virtual TRsStat       RSCOMCALL getStubInfoKey(IStub *p, unsigned *infoKey) = 0;

   virtual size_t        RSCOMCALL unMarshalMesToStream(IRsStream *obj, int cmdId, const char *inBuff, size_t inSize, IStub *s, IChanel *cn) = 0;

// March 20,2003
   virtual const char   *RSCOMCALL getNameFromProxyCache(IProxy *p, RSDISPID id) = 0;
   virtual const char   *RSCOMCALL getNameFromMixCache(IStub *stub, RSDISPID id) = 0;

//
   virtual TRsStat       RSCOMCALL getBindStub(IStub *stub, const char *names, int autoGet, IDispBind **out) = 0;
   virtual TRsStat       RSCOMCALL getBindProxy(IProxy *p, const char *names, int autoGet, IDispBind **out) = 0;
   virtual TRsStat       RSCOMCALL getBindStub2(IStub *stub, const char * const *names, int num, int autoGet, IDispBind **out) = 0;
   virtual TRsStat       RSCOMCALL getBindProxy2(IProxy *p, const char * const *names, int num, int autoGet, IDispBind **out) = 0;

   virtual TRsStat       RSCOMCALL raiseEventEx(IStub *obj, RSDISPID id, int cmdType, IParmInfo *pInfo, int level) = 0;

   virtual TRsStat       RSCOMCALL clearRemoteStubsForChannel(IChanel *cn) = 0;

   virtual TRsStat       RSCOMCALL clearRemoteStubsForChannelIf(IChanel *cn) = 0;

// July 16,2004
   virtual char         *RSCOMCALL getSendBuffMt(IProxy *p, int cmdId, size_t sz, ICallCtx *ctx) = 0;
   virtual void          RSCOMCALL sendMessageMt(IProxy *p, const char *mes, ICallCtx *ctx) = 0;
   virtual char         *RSCOMCALL transactMessageMt(IProxy *p, const char *mes, size_t *outMesSz, ICallCtx *ctx) = 0;

   virtual char         *RSCOMCALL getReplyBuffMt(IStub *p, size_t sz, IChanel *cn, ICallCtx *ctx, bool noMarshal) = 0;

   virtual size_t        RSCOMCALL processMessageMt(int cmd, const char *inMes, bool needAnswer, size_t mesSize, IChanel *cn, ICallCtx *) = 0;


   virtual void          RSCOMCALL setCtxManager(ICtxManager *ptr) = 0;
   virtual ICtxManager  *RSCOMCALL getCtxManager() = 0;

   virtual TRsStat       RSCOMCALL createCriticalSection(const char *name, IRsLock **p) = 0;
   virtual TRsStat       RSCOMCALL createEvent(bool manual, bool signaled, const char *name, IRsEvent **p) =  0;

   virtual size_t        RSCOMCALL marshalReleaseInfoToReplyBuff(size_t sz, IChanel *cn, ICallCtx *ctx) = 0;

   virtual int           RSCOMCALL ReleaseAllRemoteStubsFor(ICallCtx *ctx) = 0;

   virtual size_t        RSCOMCALL unMarshalMesToIDispMt(IDispObj *obj, int cmdId, const char *inBuff, size_t inSize, IStub *s, IChanel *cn, ICallCtx *ctx) = 0;
   virtual size_t        RSCOMCALL unMarshalMesToStreamMt(IRsStream *obj, int cmdId, const char *inBuff, size_t inSize, IStub *s, IChanel *cn, ICallCtx *ctx) = 0;

   virtual unsigned long RSCOMCALL decrementAndRemProxy(IProxy *obj, unsigned long *refCount) = 0;
   virtual unsigned long RSCOMCALL decrementAndRemRunningObj(IStub *obj, unsigned long *refCount) = 0;

   virtual void          RSCOMCALL onChannelDisconnect(IChanel *cn) = 0;

   virtual int           RSCOMCALL MoveErrInfo(IRsErrorInfo *to, IRsErrorInfo *from) = 0;

   virtual int           RSCOMCALL synchronousCall(TCallHandler proc, void *data) = 0;

   virtual TRsStat       RSCOMCALL createMutex(bool initOwn, const char *name, IRsLock **p) = 0;
   virtual TRsStat       RSCOMCALL createSemaphore(long initCount, long maxCount, const char *name, IRsSemaphore **p) = 0;
   virtual TRsStat       RSCOMCALL createMultiLock(IRsLock *array[], int count, IRsMultiLock **p) = 0;

   virtual TRsStat       RSCOMCALL callSysProcHandler(int sysCmd, IParmInfo *pInfo) = 0;
};


// -----------------------------------------------------------------------------
IProxyStubMgr *CreatePSManager(TAppObj *app);

void StopPSManager(IProxyStubMgr *ptr);
void DeletePSManager(IProxyStubMgr *ptr);

ICtxManager *CreateCtxManagerSingle(unsigned char appId, IProxyStubMgr *ps);
ICtxManager *CreateCtxManagerMulti(unsigned char appId, IProxyStubMgr *ps);
void DeleteCtxManager(ICtxManager *ptr);

// -----------------------------------------------------------------------------
// Pass to dlm handler proc
struct TCreateObjData
     {
      int           version;

      // version 0 fields
      TAppObj      *app;
      const char   *servType;
      int           iid;
      void         *obj; // retVal
      const char   *objName;
      IModuleObj   *mod;
     };

#include "packpop.h"



// -----------------------------------------------------------------------------
#include "packpsh1.h"

// -----------------------------------------------------------------------------
struct TPutCreateObject  // For Server global object
     {
      char            dllName[RSL_DLL_NAME_LEN];
      db_uint16       tpLen;
      db_uint16       objNameLen;
      db_int32        id;
      unsigned char   marshalObj;
     };

// -----------------------------------------------------------------------------
struct TPutCreateObject2 // For IDispServer call
     {
      db_uint16  tpLen;
      db_uint16  objNameLen;
     };

// -----------------------------------------------------------------------------
struct TGetCreateObject
     {
      db_uint32  status;
     };

// -----------------------------------------------------------------------------
struct TObjMarshalData
     {
      db_handle<HSTUB>  stub;
      unsigned char     tpLen;
      unsigned char     isProxy;
      // ......
     };

// -----------------------------------------------------------------------------
struct TPutReleaseAllStubs
     {
      db_uint32  count;
     };

// -----------------------------------------------------------------------------
struct TStubInfoPut
     {
      db_handle<HSTUB>  hStub;
      db_uint32         count;
     };

// -----------------------------------------------------------------------------
struct TPutStubCall
     {
      db_handle<HSTUB>  hStub;
     };

// -----------------------------------------------------------------------------
struct TGetStubCall
     {
      unsigned char  tmp;
     };

// -----------------------------------------------------------------------------
struct TPutPSpref
     {
      db_uint16  numReleaseStubInfo;
     };

// -----------------------------------------------------------------------------
struct TPutInvokeID
     {
      db_int32   id;
      db_uint16  cmdType;
      db_uint16  level;
     };

// -----------------------------------------------------------------------------
struct TPutInvokeID2
     {
      db_int32   id;
      db_uint16  cmdType;
      db_uint16  level;
      db_uint32  infoKey;
     };

// -----------------------------------------------------------------------------
struct TGetInvoke
     {
      db_int32  status;
      db_int32  id;
     };

// -----------------------------------------------------------------------------
struct TGetInvoke2
     {
      db_int32   status;
      db_int32   id;
      db_int32   level;
      db_uint32  infoKey;
     };

// -----------------------------------------------------------------------------
struct TPutInvokeName
     {
      db_uint16  cmdType;
      db_uint16  nameLen;
      db_uint16  level;
      // .....name
     };

// -----------------------------------------------------------------------------
struct TPutMeta
     {
      db_uint32  needMask;
      db_uint32  haveMask;
      db_uint32  infoKey;
     };

// -----------------------------------------------------------------------------
struct TGetMeta
     {
      db_int32   status;
      db_uint32  getMask;
      db_uint32  newInfoKey;
      db_uint16  clsNameLen;
      db_uint16  elemCount;
      // .... clsName if len > 0
     };

// -----------------------------------------------------------------------------
struct TGetOneElem
     {
      db_uint32      id;
      unsigned char  retType;
      unsigned char  kind;
      db_uint16      level;
      db_uint16      nameLen;
      db_uint16      parmLen;
     };

// -----------------------------------------------------------------------------
// SYS_CMD_CREATE_META
struct TPutMetaObject  // For Server global object
     {
      char       dllName[RSL_DLL_NAME_LEN];
      db_uint32  mask;
      db_uint16  tpLen;
     };

// -----------------------------------------------------------------------------
// Stream support
struct TPutFillBuff
     {
      db_uint32  sz;
      db_uint32  pos;
      db_int16   from;
     };

// -----------------------------------------------------------------------------
struct TGetFillBuff
     {
      db_int32   stat;
      db_uint32  sz;
     };

// -----------------------------------------------------------------------------
struct TPutFlushBuff
     {
      db_uint32  sz;
      db_uint32  pos;
      db_int16   from;
     };

// -----------------------------------------------------------------------------
struct TPutFlushBuff_setSz
     {
      db_uint32  sz;
      db_uint32  pos;
      db_int16   from;
      db_uint32  newSize;
     };

// -----------------------------------------------------------------------------
struct TGetFlushBuff
     {
      db_int32   stat;
      db_uint32  sz;
     };

// -----------------------------------------------------------------------------
struct TGetFlushBuff_getSz
     {
      db_int32   stat;
      db_uint32  sz;
      db_uint32  strmSize;
     };

// -----------------------------------------------------------------------------
struct TPutCreateBnd
     {
      db_uint16      nmLen;
      unsigned char  autoGet;
     };

// -----------------------------------------------------------------------------
struct TGetCreateBnd
     {
      db_uint32  status;
      db_uint16  count;
     };

// -----------------------------------------------------------------------------
struct TPutExchange
     {
      char  dir;
     };

// -----------------------------------------------------------------------------
struct TGetExchange
     {
      db_uint32  status;
     };

// -----------------------------------------------------------------------------
struct TPutRawModuleMes  // For Server global object
     {
      char       dllName[RSL_DLL_NAME_LEN];
      db_uint32  rawCmd;
     };

// -----------------------------------------------------------------------------
struct TPutRawModuleMesEx  // For Server global object
     {
      char       dllName[RSL_DLL_NAME_LEN];
      db_uint32  rawCmd;
      db_uint32  flags;
     };

// -----------------------------------------------------------------------------
struct TGetRawModuleMes
     {
      db_uint32  status;
     };

// -----------------------------------------------------------------------------
// For SYS_CMD_SYSPROC
struct TPutSysProc
     {
      db_int32  code;
     };

// -----------------------------------------------------------------------------
struct TGetSysProc
     {
      db_int32  stat;
     };

#include "packpop.h"


// -----------------------------------------------------------------------------
#define  SYS_CMD_CREATE_SERVER      -6
#define  SYS_CMD_RELEASE_STUBS      -7
#define  SYS_CMD_CREATE_OBJECT      -8
#define  SYS_CMD_INVOKE_ID          -9
#define  SYS_CMD_INVOKE_NAME       -10
#define  SYS_CMD_GET_META          -11
#define  SYS_CMD_CREATE_META       -12
#define  SYS_CMD_REVOKE            -13
#define  SYS_CMD_INVOKE_ID2        -14
#define  SYS_CMD_INVOKE_NAME2      -15

#define  SYS_CMD_FILLBUFF          -16
#define  SYS_CMD_FLUSHBUFF         -17
#define  SYS_CMD_FLUSHBUFF_SETSZ   -18
#define  SYS_CMD_FLUSHBUFF_GETSZ   -19

#define  SYS_CMD_CREATE_BND        -20
#define  SYS_CMD_EXCHANGE          -21
#define  SYS_CMD_RAWMODULEMES      -22
#define  SYS_CMD_INTERTHREAD_CALL  -23
#define  SYS_CMD_SYSPROC           -24

#endif // __RSCOM_HPP

/* <-- EOF --> */