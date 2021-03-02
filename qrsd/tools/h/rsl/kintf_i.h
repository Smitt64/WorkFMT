/*************************************************************************
        File Name               : kintf_i.h                     02.10.2000
        Programmer              : A.Tokmashev

        Subsystem               : Interpreter RSL
        Description             : Class wrapper for working with RSL and cache RSL-instances.

        Copyright(C) 1991 - 2000 by R-Style Software Lab.
        All Rights Reserved.

*************************************************************************/

#ifndef __KRNLINTF_IMPL_H
#define __KRNLINTF_IMPL_H

///////////////////////////////////////////////////////////////////////////////
//
// class TRSLInstIntf_impl
//
///////////////////////////////////////////////////////////////////////////////
struct TAddExecStack
     {
      LOBJ         elem;
      TAddExecData data;
      bool    bIsExecut;
      bool    bIsInited;

      // --------------------------------------------
      void *operator new(size_t size)
      {
       return malloc(size);
      }

      // --------------------------------------------
      void operator delete(void *ptr)
      {
       free(ptr);
      }
     };


// -----------------------------------------------------------------------------
class TRSLInstIntf_impl : public TRSLInstance
{
   // --------------------------------------------
   TRSLInstIntf_impl(const TRSLInstIntf_impl &) {}
   TRSLInstIntf_impl &operator= (const TRSLInstIntf_impl &) { return *this; }

 public:
   // --------------------------------------------
   TRSLInstIntf_impl(const char *nameSpace, TRSLMsgHandler *host, HRSLCACHE cache, bool bIsNew);
   virtual ~TRSLInstIntf_impl();

   // --------------------------------------------
   void *operator new(size_t size)
   {
    return malloc(size);
   }

   // --------------------------------------------   
   void operator delete(void *ptr)
   {
    free(ptr);
   }

   // --------------------------------------------
   static bool RSLAPI           stAddModule            (HRSLINST inst, const char *name);
   static bool RSLAPI           stPushModule           (HRSLINST inst, const char *name, bool privateMode);
   static bool RSLAPI           stPopModule            (HRSLINST inst);
   static bool RSLAPI           stExecute              (HRSLINST inst);
   static void RSLAPI           stStop                 (HRSLINST inst);
   static RSLINST_STATE RSLAPI  stGetState             (HRSLINST inst);
   static HRSLSYM RSLAPI        stRslGetInstSymbol     (HRSLINST inst, const char *name, const char *moduleName);
   static bool RSLAPI           stRslCallInstSymbol    (HRSLINST inst, HRSLSYM hSym, int code, int nPar, VALUE *par, VALUE *propORret);
   static int  RSLAPI           stGetCountError        (HRSLINST inst);
   static ERRINFO *RSLAPI       stGetErrorInfo         (HRSLINST inst, int index);
   static bool RSLAPI           stTestExistModule      (HRSLINST inst, const char *modName);
   static bool RSLAPI           stAttachMsgHandler     (HRSLINST inst, TRSLMsgHandler *handler);
   static bool RSLAPI           stDetachMsgHandler     (HRSLINST inst, TRSLMsgHandler *handler);
   static bool RSLAPI           stCheckActualFiles     (HRSLINST inst);
   static int  RSLAPI           stRslSendMes           (HRSLINST inst, IMESSAGE mes, void *ptr);
   static const char *RSLAPI    stGetName              (HRSLINST inst);
   static HRD  RSLAPI           stGetHRD               (HRSLINST inst);
   static short int RSLAPI      stGetVersion           (HRSLINST inst, short int *verLo);
   //Add by Ushakov
   static bool RSLAPI           stSetDbgModule         (HRSLINST, const char *); 
   static bool RSLAPI           stExecuteEx            (HRSLINST inst);
   static bool RSLAPI           stPushModuleNotExec    (HRSLINST, const char *, bool);
   static bool RSLAPI           stUninitModule         (HRSLINST, const char *);
   static bool RSLAPI           stSetModuleNotExecState(HRSLINST, const char *);
   static int  RSLAPI           stGetRSCOMerror        (HRSLINST);

   // --------------------------------------------
   // --------------------------------------------
   static TRSLInstIntf  instTable;

   // --------------------------------------------
   bool AllocInstance();
   void FreeInstance();
   bool CreateInstance();

   // --------------------------------------------   
   RSLINST_STATE GetState()
   {
    return m_state;
   }

   // --------------------------------------------   
   void SetState(RSLINST_STATE st)
   {
    m_state = st;
   }

   // --------------------------------------------   
   bool IsCreateInCache()
   {
    return m_createInCache;
   }

   // --------------------------------------------   
   void SetCreateInCache(bool flag)
   {
    m_createInCache = flag;
   }

   // --------------------------------------------   
   HRSLCACHE GetCache()
   {
    return m_cache;
   }

   // --------------------------------------------   
   HRD GetHRD()
   {
    return m_instRsl;
   }

   // --------------------------------------------   
   const char *GetNameSpace()
   {
    return m_nameSpace;
   }

   // --------------------------------------------   
   void DetachAllMsgHandlers();


   // --------------------------------------------
   // --------------------------------------------
   bool  m_bNewImpl;

 protected:
   // --------------------------------------------
   CNTLIST *GetListMsgHandlers()
   {
    return &m_listMsgHandlers;
   }

   // --------------------------------------------   
   bool AddModule(const char *name, bool fOverlay, bool privateMode, bool notExecute = false);   
   bool CheckActualFiles();
   bool InstModuleClose();

   static  int msgproc(IMESSAGE mes, void *ptr, void *UserData);
   virtual int message(IMESSAGE mes, void *ptr, void *UserData);
   static  int output(const char *buf);

   static int  SetExecStackStatus(TAddExecStack *, bool *);
   static int  PushModuleExecut(TAddExecStack *, void *);

   bool ExecuteAtOpen();

   TAddExecStack *FindAddExecData(const char *);

   // --------------------------------------------
   // --------------------------------------------
   char                  m_nameSpace[_MAX_NAMESPACE];
   HRD                   m_instRsl;  
   RSLINST_STATE         m_state;   
   CNTLIST               m_stackAddExec;
   TRSLMsgHandler       *m_pFirstHandler;
   TRSLMsgHandler       *m_pPtrToHost;
   CNTLIST               m_listMsgHandlers;
   bool                  m_createInCache;
   HRSLCACHE             m_cache;   
};

///////////////////////////////////////////////////////////////////////////////
//
// class - TRSLCacheInsts_impl
//
///////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
enum ST_ITEM
   {
    sEMPTY,
    sLINKING,
    sFREE,
    sTEMPORARY
   };

// -----------------------------------------------------------------------------
struct TDataCache
     {
      LOBJ           elem;
      HRSLINST       instance;
      ST_ITEM        status;
      unsigned long  countUse;

      // --------------------------------------------
      TDataCache() : instance(NULL), status(sEMPTY), countUse(0) {}

      // --------------------------------------------
      void *operator new(size_t size)
      {
       return malloc(size);
      }

      // --------------------------------------------      
      void operator delete(void *ptr)
      {
       free(ptr);
      }
     };

// -----------------------------------------------------------------------------
class TRSLCacheInsts_impl : public TRSLCache
{
   // --------------------------------------------
   TRSLCacheInsts_impl(const TRSLCacheInsts_impl &) {}
   TRSLCacheInsts_impl &operator= (const TRSLCacheInsts_impl &) { return *this; }


 public:
   // --------------------------------------------
   TRSLCacheInsts_impl(int size = 10);
   virtual ~TRSLCacheInsts_impl();

   // --------------------------------------------
   void *operator new(size_t size)
   {
    return malloc(size);
   }

   // --------------------------------------------
   void operator delete(void *ptr)
   {
    free(ptr);
   }

   // --------------------------------------------
   static bool      RSLAPI chEnable         (HRSLCACHE cache, bool fEnable);
   static bool      RSLAPI chIsEnable       (HRSLCACHE cache);
   static void      RSLAPI chClear          (HRSLCACHE cache);
   static HRSLINST  RSLAPI chFindInstance   (HRSLCACHE cache, const char *nameSpace, bool *flag);
   static bool      RSLAPI chPutInstance    (HRSLCACHE cache, HRSLINST inst);
   static void      RSLAPI chDoneInstance   (HRSLCACHE cache, HRSLINST inst);
   static void      RSLAPI chDeleteInstance (HRSLCACHE cache, HRSLINST inst);
   static short int RSLAPI chGetVersion     (HRSLCACHE cache, short int *verLo);
   static HRSLINST  RSLAPI chSearchInstance (HRSLCACHE cache, const char *nameSpace);
   static bool      RSLAPI chGetSize        (HRSLCACHE cache, int *maxSize, int *curSize);
   static bool      RSLAPI chHasFreeInstance(HRSLCACHE cache);
   static bool      RSLAPI chCheckInstance  (HRSLCACHE cache, const char *nameSpace, int *status);
  
   // --------------------------------------------
   // --------------------------------------------
   static TRSLCacheIntf  cacheTable;

 protected:
   // --------------------------------------------
   bool GetEnable()
   {
    return m_fEnable;
   }

   // --------------------------------------------
   void SetEnable(bool fEnable)
   {
    m_fEnable = fEnable;
   }

   // --------------------------------------------
   HRSLINST    FindInstance    (const char *nameSpace, bool *flag);
   TDataCache *FindDataCache   (const char *nameKey);
   bool        CheckActualFiles(CNTLIST *list);
   TDataCache *NewDataCache    (HRSLINST inst, ST_ITEM status);
   int         FindFreeItem    ();
   bool        PutInstance     (HRSLINST inst);
   void        Clear           ();
   void        DoneInstance    (HRSLINST inst);
   void        DeleteInstance  (HRSLINST inst);
   HRSLINST    SearchInstance  (const char *nameSpace);
   bool        GetSize         (int *maxSize, int *curSize);
   bool        HasFreeInstance (void);
  
   // --------------------------------------------
   // --------------------------------------------
   int            m_szLimit;
   bool           m_fEnable;
   CNTLIST        m_listInsts;
};

///////////////////////////////////////////////////////////////////////////////
#endif // __KRNLINTF_IMPL_H

/* <-- EOF --> */