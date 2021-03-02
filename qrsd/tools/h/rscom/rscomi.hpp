//-*--------------------------------------------------------------------------*-
//
// File Name   : rscomi.hpp
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// December 18,2001  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-
#ifndef __RSCOMI_HPP
#define __RSCOMI_HPP


#ifndef __RSCOM_HPP
# include "rscom/rscom.hpp"
#endif      

#ifndef __RSEXCEPT_H
# include "rsexcept.h"
#endif

#ifndef RSASSERT
# include "rsassert.h"
#endif

#ifndef __STRHLD_HPP
# include "rscom/strhld.hpp"
#endif

// -----------------------------------------------------------------------------
class XRSComError : public RSException
{
 public:
    // -------------------------------------------
    XRSComError(TAppObj *a, TRsStat s = RSL_STAT_GEN, const char *what = "RSCom exception") 
              : RSException(what), app(a), stat(s) 
    {
    }

    // -------------------------------------------
    XRSComError(IProxyStubMgr *ps, TRsStat s = RSL_STAT_GEN, const char *what = "RSCom exception") 
              : RSException(what), app(ps->getAppObj()), stat(s) 
    {
    }

    // -------------------------------------------
    const TRsStat getStat() const
    {
     return stat;
    }

    // -------------------------------------------
    bool isExtended() const
    {
     return stat == RSL_STAT_EX;
    }

    // -------------------------------------------
    TAppObj *getErrApp() const
    {
     return app;
    }

 private:
    // -------------------------------------------
    // -------------------------------------------
    TAppObj  *app;
    TRsStat   stat;
};


// -----------------------------------------------------------------------------
#if 0
// -----------------------------------------------------------------------------
class TRsStrHolder
{
 public:
   // -------------------------------------------
   TRsStrHolder(const char *ptr = 0)
   {
    strElem.pStr = 0;
    strElem.proc = 0;

    set(ptr);
   }

   // -------------------------------------------
   TRsStrHolder(const TRsStrHolder &p)
   {
    strElem.pStr = 0;
    strElem.proc = 0;

    set(p.strElem.pStr);
   }

   // -------------------------------------------
   TRsStrHolder &operator = (const TRsStrHolder &p)
   {
    if(&strElem != &(p.strElem))
      set(p.strElem.pStr);

    return *this;
   }

   // -------------------------------------------
  ~TRsStrHolder()
   {
    freeString();
   }

   // -------------------------------------------
   operator const char *() const
   {
    return strElem.pStr;
   }

   // -------------------------------------------
   TRsStrPtr * operator & ()
   {
    return &strElem;
   }

   // -------------------------------------------
   bool isEmpty()
   {
    return !strElem.pStr;
   }

   // -------------------------------------------
   const char *append(const char *str, const char *separator = ";")
   {
    size_t  addLen = strlen(str) + sizeof(char) + (separator ? strlen(separator) : 0);
    size_t  curLen = strElem.pStr ? strlen(strElem.pStr) : 0;

    char   *ptr    = (char *)allocMem(addLen + curLen);


    if(ptr)
     {
      *ptr = '\0';

      if(strElem.pStr)
       {
        strcat(ptr, strElem.pStr);

        if(separator)
          strcat(ptr, separator);
       }

      strcat(ptr, str);

      freeString();

      strElem.pStr = ptr;
      strElem.proc = freeMem;
     }

    return ptr;
   }

   // -------------------------------------------
   void set(const char *ptr, int len)
   {
    freeString();

    if(ptr)
     {
      strElem.pStr = (char *)allocMem(len + 1);

      if(strElem.pStr)
       {
        strncpy(strElem.pStr, ptr, len);

        strElem.pStr[len] = '\0';
       }

      strElem.proc = freeMem;
     }
   }

 private:
   // -------------------------------------------
   static void *allocMem(size_t sz)
   {
    return malloc(sz);
   }

   // -------------------------------------------
   static void RSCOMCALL freeMem(void *p)
   {
    free(p);
   }

   // -------------------------------------------
   void set(const char *ptr)
   {
    freeString();

    if(ptr)
     {
      strElem.pStr = (char *)allocMem(strlen(ptr) + 1);

      if(strElem.pStr)
        strcpy(strElem.pStr, ptr);

      strElem.proc = freeMem;
     }
   }

   // -------------------------------------------
   void freeString()
   {
    if(strElem.pStr && strElem.proc)
     {
      strElem.proc(strElem.pStr);

      strElem.pStr = 0;
     }
   }

   // -------------------------------------------
   // -------------------------------------------
   TRsStrPtr  strElem;
};

// -----------------------------------------------------------------------------
typedef TRsStrHolder  TRsString;
#endif

// -----------------------------------------------------------------------------
template<class T>
class IMarshalObjImpl : public T
{
 public:
   // -------------------------------------------
   IMarshalObjImpl(TAppObj *app)
   {
    mgr = fs_getPSmgrA(app);
   }

   // -------------------------------------------
   virtual IProxyStubMgr *RSCOMCALL GetProxyStubMgr()
   {
    return mgr;
   }

   // -------------------------------------------
   virtual void RSCOMCALL SetProxyStubMgr(IProxyStubMgr *m)
   {
    mgr = m;
   }

 protected:
   // -------------------------------------------
   // -------------------------------------------
   IProxyStubMgr  *mgr;
};


// -----------------------------------------------------------------------------
#define  RSCOM_INTF_BEGIN                                                        \
              void getInterfaceImplNoAddRef(int id, void **ptr)                  \
              {                                                                  \
               switch(id)                                                        \
                    {                                                            \
                     default: *ptr = NULL; break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF_BEGIN2(Base)                                                 \
              void getInterfaceImplNoAddRef(int id, void **ptr)                  \
              {                                                                  \
               switch(id)                                                        \
                    {                                                            \
                     default:                                                    \
                              Base::getInterfaceImplNoAddRef(id, ptr);           \
                              break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF_BEGIN3(Base1, Base2)                                         \
              void getInterfaceImplNoAddRef(int id, void **ptr)                  \
              {                                                                  \
               switch(id)                                                        \
                    {                                                            \
                     default:                                                    \
                              Base1::getInterfaceImplNoAddRef(id, ptr);          \
                              if(!(*ptr))                                        \
                                Base2::getInterfaceImplNoAddRef(id, ptr);        \
                              break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF_BEGIN4(Base1, Base2, Base3)                                  \
              void getInterfaceImplNoAddRef(int id, void **ptr)                  \
              {                                                                  \
               switch(id)                                                        \
                    {                                                            \
                     default:                                                    \
                              Base1::getInterfaceImplNoAddRef(id, ptr);          \
                              if(!(*ptr))                                        \
                                Base2::getInterfaceImplNoAddRef(id, ptr);        \
                              if(!(*ptr))                                        \
                                Base3::getInterfaceImplNoAddRef(id, ptr);        \
                              break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF(iid, Type)                                                   \
               case iid: *ptr = (Type *)this; break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF2(iid, TypeTarget, TypeFirst)                                 \
               case iid: *ptr = (TypeTarget *)(TypeFirst *)this; break;

// -----------------------------------------------------------------------------
#define  RSCOM_INTF_END                                                          \
              }} bool getInterfaceImpl(int id, void **ptr)                       \
                 {                                                               \
                  getInterfaceImplNoAddRef(id, ptr);                             \
                  if(*ptr)                                                       \
                    {                                                            \
                     ((IRsObj *)(*ptr))->addRef();                               \
                     return true;                                                \
                    }                                                            \
                  return false;                                                  \
                 }

// -----------------------------------------------------------------------------
#define  RSCOM_INTF_END_EX(obj)                                                  \
              }} bool getInterfaceImpl(int id, void **ptr)                       \
                 {                                                               \
                  getInterfaceImplNoAddRef(id, ptr);                             \
                  if(*ptr)                                                       \
                   {                                                             \
                    ((IRsObj *)(*ptr))->addRef();                                \
                    return true;                                                 \
                   }                                                             \
                  return obj->getInterface(id, ptr);                             \
                 }


// -----------------------------------------------------------------------------
class IProxyImpl : public IMarshalObjImpl<IProxy>, public IProxyControl
{
 protected:
   // -------------------------------------------
   void remProxyFromMgr()
   {
    if(needRemProxy)
      mgr->remProxy(this);

    needRemProxy = false;
   }

 public:
   // -------------------------------------------
   IProxyImpl(TAppObj *app) : IMarshalObjImpl<IProxy>(app), stub(0), extRefCount(0),
                              chanel(0), data(0), needClrRmtStubs(false), needRemProxy(true)
   {
   }

   // -------------------------------------------
   unsigned long decrementRef(unsigned long *refCount)
   {
    unsigned long  res = mgr->decrementAndRemProxy(this, refCount);


    if(!res)
      needRemProxy = false;

    return res;
   }

   // -------------------------------------------
   virtual ~IProxyImpl()
   {
    remProxyFromMgr();

    if(stub)
     {
      mgr->ReleaseRemoteStub(stub, extRefCount, chanel);

      if(needClrRmtStubs)
        mgr->clearRemoteStubsForChannel(chanel);
//       else
//          mgr->clearRemoteStubsForChannelIf (chanel);
     }

    if(data)
      data->release();

    if(chanel)
      chanel->release();
   }

   // -------------------------------------------
   virtual void RSCOMCALL connect(HSTUB p, IChanel *cn)
   {
    if(chanel)
      chanel->release();

    stub        = p;  
    chanel      = cn; 
    extRefCount = 1;

    if(chanel)
      chanel->addRef();
   }

   // -------------------------------------------
   virtual IChanel *RSCOMCALL getChanel(void)
   {
    return chanel;
   }

   // -------------------------------------------
   virtual HSTUB RSCOMCALL getStubHandle()
   {
    return stub;
   }

   // -------------------------------------------
   virtual unsigned char RSCOMCALL isProxy()
   {
    return 1;
   }

   // -------------------------------------------
   virtual unsigned RSCOMCALL getExtRefCount()
   {
    return extRefCount;
   }

   // -------------------------------------------
   virtual unsigned RSCOMCALL incExtRefCount(unsigned n)
   {
    return extRefCount += n;
   }

   // -------------------------------------------
   virtual unsigned RSCOMCALL decExtRefCount(unsigned n)
   {
    return extRefCount -= n;
   }

   // -------------------------------------------
   char *getDeferBuff(int cmd, size_t sz)
   {
    return mgr->getDeferBuff(this, cmd, sz);
   }

   // -------------------------------------------
   void queueMessage(const char *mes)
   {
    mgr->queueMessage(this, mes);
   }

   // -------------------------------------------
   char *getSendBuff(int cmd, size_t sz)
   {
    return mgr->getSendBuff(this, cmd, sz);
   }

   // -------------------------------------------
   void sendMessage(const char *mes)
   {
    mgr->sendMessage(this, mes);
   }

   // -------------------------------------------
   char *transactMessage(const char *mes, size_t *outMesSz)
   {
    return mgr->transactMessage(this, mes, outMesSz);
   }

   // -------------------------------------------
   char *getSendBuffMt(int cmd, size_t sz, ICallCtx *ctx)
   {
    return mgr->getSendBuffMt(this, cmd, sz, ctx);
   }

   // -------------------------------------------
   void sendMessageMt(const char *mes, ICallCtx *ctx)
   {
    mgr->sendMessageMt(this, mes, ctx);
   }

   // -------------------------------------------
   char *transactMessageMt(const char *mes, size_t *outMesSz, ICallCtx *ctx)
   {
    return mgr->transactMessageMt(this, mes, outMesSz, ctx);
   }

   // -------------------------------------------
   virtual IProxyData *RSCOMCALL getProxyData()
   {
    return data;
   }

   // -------------------------------------------
   virtual void RSCOMCALL setProxyData(IProxyData *p)
   {
    if(data)
      data->release();

    data = p;

    if(data)
      data->addRef();
   }

   // -------------------------------------------
   virtual TRsStat RSCOMCALL clrRmtStubsOnRelease(void)
   {
    needClrRmtStubs = true;

    return RSL_STAT_OK;
   }

   // -------------------------------------------
   RSCOM_INTF_BEGIN
     RSCOM_INTF2(IID_RSOBJECT,  IRsObj, IProxy)
     RSCOM_INTF2(IID_RSMARSHAL, IMarshalObj, IProxy)
     RSCOM_INTF(IID_RSPROXY,    IProxy)
     RSCOM_INTF(IID_RSPROXYCTL, IProxyControl)
   RSCOM_INTF_END

 private:
   // -------------------------------------------
   // -------------------------------------------
   HSTUB        stub;
   IChanel     *chanel;
   unsigned     extRefCount;
   IProxyData  *data;
   bool         needClrRmtStubs;
   bool         needRemProxy;
};


// -----------------------------------------------------------------------------
#pragma warning (push)
#pragma warning (disable: 4311)  // 'type cast' : pointer truncation from 'IStubImpl *const ' to 'HSTUB'


// -----------------------------------------------------------------------------
class IStubImpl : public IMarshalObjImpl<IStubMt>, public ILifeTime
{
 public:
   // -------------------------------------------
   IStubImpl(TAppObj *p) : IMarshalObjImpl<IStubMt>(p), isRunningObj(false), data(0), moduleObj(0) 
   {
   }

   // -------------------------------------------
   unsigned long decrementRef(unsigned long *refCount)
   {
    unsigned long  res;


    if(isRunningObj)
     {
      res = mgr->decrementAndRemRunningObj(this, refCount);

      if(!res)
        isRunningObj = false;
     }
    else 
      res = fs_interlockedDecrement(mgr->getAppObj(), (long *)refCount);

    return res;
   }

   // -------------------------------------------
   virtual ~IStubImpl()
   {
    if(isRunningObj)
      mgr->remRunningObj(this);

    if(data)
      data->release();
   }

   // -------------------------------------------
   // This method do not used now by the current PS mgr.
   virtual HSTUB RSCOMCALL getStubHandle()
   {
    return (HSTUB)this;
   }

   // -------------------------------------------
   virtual unsigned char RSCOMCALL isProxy()
   {
    return 0;
   }

   // -------------------------------------------
   virtual const char *RSCOMCALL getProxyType()
   {
    return NULL;
   }

   // -------------------------------------------
   virtual void RSCOMCALL setRunningObj(bool isRun)
   {
    isRunningObj = isRun;
   }

   // -------------------------------------------
   virtual void RSCOMCALL finalRelease()
   {
    if(moduleObj)
      moduleObj->unLockModule();
   }

   // -------------------------------------------
   virtual TRsStat RSCOMCALL finalConstruct(const char *namePtr, IModuleObj *pMod, void *)
   {
    objName   = namePtr;
    moduleObj = pMod;

    if(moduleObj)
      moduleObj->lockModule();

    return RSL_STAT_OK;
   }

   // -------------------------------------------
   virtual const char *RSCOMCALL objectName()
   {
    return objName;
   }

   // -------------------------------------------
   RSCOM_INTF_BEGIN
     RSCOM_INTF2(IID_RSOBJECT,  IRsObj, IStub)
     RSCOM_INTF2(IID_RSMARSHAL, IMarshalObj, IStub)
     RSCOM_INTF(IID_RSLIFETIME, ILifeTime)
     RSCOM_INTF(IID_RSSTUB,     IStub)
     RSCOM_INTF(IID_RSSTUBMT,   IStubMt)
   RSCOM_INTF_END

 protected:
   // -------------------------------------------
   char *getReplyBuff(IChanel *cn, size_t sz)
   {
    return mgr->getReplyBuff(this, sz, cn);
   }

   // -------------------------------------------
   char *getReplyBuffMt(IChanel *cn, size_t sz, ICallCtx *ctx, bool noMarshalInfo = false)
   {
    return mgr->getReplyBuffMt(this, sz, cn, ctx, noMarshalInfo);
   }

   // -------------------------------------------
   virtual IStubData *RSCOMCALL getStubData()
   {
    return data;
   }

   // -------------------------------------------
   virtual void RSCOMCALL setStubData(IStubData *p)
   {
    if(data)
      data->release();

    data = p;

    if(data)
      data->addRef();
   }

 private:
   // -------------------------------------------
   // -------------------------------------------
   bool           isRunningObj;
   IStubData     *data;
   TRsStrHolder   objName;

 protected:
   // -------------------------------------------
   // -------------------------------------------
   IModuleObj    *moduleObj;
};

// -----------------------------------------------------------------------------
#pragma warning (pop)


// -----------------------------------------------------------------------------
class IByValImpl : public IMarshalObjImpl<IMarshalByValObj>
{
 public:
   // This method do not used now by the current PS mgr.
   IByValImpl(TAppObj *app) : IMarshalObjImpl<IMarshalByValObj>(app)
   {
   };

   // This method do not used now by the current PS mgr.
   virtual ~IByValImpl()
   {
   }

   // This method do not used now by the current PS mgr.
   virtual HSTUB RSCOMCALL getStubHandle()
   {
    return 0;
   }

   // This method do not used now by the current PS mgr.
   virtual unsigned char RSCOMCALL isProxy()
   {
    return 255;
   }

   // This method do not used now by the current PS mgr.
   RSCOM_INTF_BEGIN
     RSCOM_INTF(IID_RSOBJECT,  IRsObj)
     RSCOM_INTF(IID_RSMARSHAL, IMarshalObj)
     RSCOM_INTF(IID_RSBYVAL,   IMarshalByValObj)
   RSCOM_INTF_END

   // This method do not used now by the current PS mgr.
   virtual size_t RSCOMCALL dataSize()
   {
    return 0;
   }

   // This method do not used now by the current PS mgr.
   virtual size_t RSCOMCALL save(char *buff)
   {
    return 0;
   }

   // This method do not used now by the current PS mgr.
   virtual size_t RSCOMCALL load(const char *buff)
   {
    return 0;
   }
};


// -----------------------------------------------------------------------------
template <class T> class TRsObjAutoBase;


// -----------------------------------------------------------------------------
template<class T>
class TRsObjImpl : public T
{
 protected:
   // --------------------------------------------
   TRsObjImpl(TAppObj *appPtr, const char *clsNamePtr) : T(appPtr), refCount(0), appObj(appPtr)
   {
   }
      
   // --------------------------------------------
   TRsObjImpl(TAppObj *appPtr) : T(appPtr), refCount(0), appObj(appPtr)
   {
   }

 public:
   // --------------------------------------------
   virtual ~TRsObjImpl()
   {
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL addRef()
   {
    return ++refCount;
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL release()
   {
    unsigned  cnt = --refCount;


    if(!cnt)
     {
      ILifeTime  *ltPtr;


      refCount = 1;

      if(T::getInterfaceImpl(IID_RSLIFETIME, (void **)&ltPtr))
        ltPtr->finalRelease();

      delete this;
     }

    return cnt;
   }

   // --------------------------------------------
   virtual bool RSCOMCALL getInterface(int id, void **ptr)
   {
    return T::getInterfaceImpl(id, ptr);
   }

   // --------------------------------------------
   virtual const char *RSCOMCALL getClassName()
   {
    return T::getClassMetaName();
   }

   // --------------------------------------------
   virtual TAppObj *RSCOMCALL getAppObj()
   {
    return appObj;
   }

   // --------------------------------------------
   static TRsObjImpl<T> *construct(TAppObj *appPtr)
   {
    return new TRsObjImpl<T>(appPtr);
   }

   // --------------------------------------------
   IRsObj *rsObj()
   {
    IRsObj *ptr;


    T::getInterfaceImplNoAddRef(IID_RSOBJECT, (void **)&ptr);

    return ptr;
   }

 protected:
   // This method do not used now by the current PS mgr.
   unsigned internalAddRef()
   {
    return ++refCount;
   }

   // This method do not used now by the current PS mgr.
   unsigned internalRelease()
   {
    return --refCount;
   }

   // This method do not used now by the current PS mgr.
   unsigned long     refCount;

   TAppObj          *appObj;

   friend class TRsObjAutoBase<TRsObjImpl<T> >;
};


// -----------------------------------------------------------------------------
template<class T>
class TRsObjImplOwner : public TRsObjImpl<T>, public IRsObjControling
{
 public:
   // --------------------------------------------
   TRsObjImplOwner(TAppObj *appPtr) : TRsObjImpl<T>(appPtr), outerObj(0)
   {
   }

   // --------------------------------------------
   static TRsObjImplOwner<T> *construct(TAppObj *appPtr)
   {
    return new TRsObjImplOwner<T>(appPtr);
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL addRef()
   {
    if(outerObj)
      return outerObj->addRef();

    return TRsObjImpl<T>::addRef();
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL release()
   {
    if(outerObj)
      return outerObj->release();

    return TRsObjImpl<T>::release();
   }

   // --------------------------------------------
   virtual bool RSCOMCALL getInterface(int id, void **ptr)
   {
    if(id == IID_RSCTRLINTF)
     {
      *ptr = (IRsObjControling *)this;

      ((IRsObj *)(*ptr))->addRef();

      return true;
     }

    return TRsObjImpl<T>::getInterface(id, ptr);
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL addRef_()
   {
    return TRsObjImpl<T>::addRef();
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL release_()
   {
    return TRsObjImpl<T>::release();
   }

   // --------------------------------------------
   virtual bool RSCOMCALL getInterface_(int id, void **ptr)
   {
    return getInterface(id, ptr);
   }

   // --------------------------------------------
   virtual const char *RSCOMCALL getClassName_()
   {
    return TRsObjImpl<T>::getClassName();
   }

   // --------------------------------------------
   virtual TAppObj *RSCOMCALL getAppObj_()
   {
    return TRsObjImpl<T>::getAppObj();
   }

   // --------------------------------------------
   void setOuterObj(IRsObj *p)
   {
    outerObj = p;
   }

   // --------------------------------------------
   // --------------------------------------------
   IRsObj  *outerObj;
};


// -----------------------------------------------------------------------------
template<class T>
class TRsObjMTImpl : public TRsObjImpl<T>
{
 public:
   // --------------------------------------------
   TRsObjMTImpl(TAppObj *appPtr) : TRsObjImpl<T>(appPtr)
   {
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL addRef()
   {
    return fs_interlockedIncrement(appObj, (long *)&refCount);
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL release()
   {
//      unsigned long res = (refCount == 1 ? T::decrementRef (&refCount) : fs_interlockedDecrement (appObj,&refCount));
    unsigned long  res = T::decrementRef(&refCount);

    
    if(!res)
     {
      // The last client calls release
      internalAddRef();

      return TRsObjImpl<T>::release();
     }

    return res;
   }

   // --------------------------------------------
   static TRsObjMTImpl<T> *construct(TAppObj *appPtr)
   {
    return new TRsObjMTImpl<T>(appPtr);
   }
};


// -----------------------------------------------------------------------------
#define  RSCOM_CONSTRUCT(Type)                                                       \
              virtual const char *RSCOMCALL getClassName()                           \
              {                                                                      \
               return Type::getClassMetaName();                                      \
              }                                                                      \
              static Type *construct(TAppObj *app)                                   \
              {                                                                      \
               return new Type(app /*,Type::getClassMetaName () */);                 \
              }

// -----------------------------------------------------------------------------
#define  RSCOM_TYPE_BEGIN0_IMP(cls, className)                                           \
              static const TClassData *getClassMetaData()                                \
              {                                                                          \
               static TClassData  classData = { NULL, cls::getClassElems, #className };  \
               return &classData;                                                        \
              }                                                                          \
              static const TRsTypeElem *getClassElems()                                  \
              {                                                                          \
               static TRsTypeElem  classElems[] = {

// -----------------------------------------------------------------------------
#define  RSCOM_TYPE_BEGIN_IMP(cls, Base, className)                                                       \
              static const TClassData *getClassMetaData()                                                 \
              {                                                                                           \
               static TClassData  classData = { Base::getClassMetaData, cls::getClassElems, #className }; \
               return &classData;                                                                         \
              }                                                                                           \
              static const TRsTypeElem *getClassElems()                                                   \
              {                                                                                           \
                 static TRsTypeElem  classElems[] = {

// -----------------------------------------------------------------------------
#define  RSCOM_TYPE_BEGIN0(cls)                                                      \
              RSCOM_CONSTRUCT(cls)                                                   \
              RSCOM_TYPE_BEGIN0_IMP(cls, cls)

// -----------------------------------------------------------------------------
#define  RSCOM_TYPE_BEGIN(cls,Base)                                                  \
              RSCOM_CONSTRUCT(cls)                                                   \
              RSCOM_TYPE_BEGIN_IMP(cls, Base, cls)

// -----------------------------------------------------------------------------
#define  RSCOM_TYPE_END                                                              \
              { NULL, -1 } };  return classElems;  }                                 \
              static const char *getClassMetaName()                                  \
              {                                                                      \
               return getClassMetaData()->className;                                 \
              }                                                                      \
              virtual const TClassData *getTypeElems()                               \
              {                                                                      \
               return getClassMetaData();                                            \
              }

// -----------------------------------------------------------------------------
// For simple NonDispatch Imp classes
#define  RSCOM_TYPE_DUMMY(clsName)                                                   \
              static const char *getClassMetaName()                                  \
              {                                                                      \
               return #clsName;                                                      \
              }

// -----------------------------------------------------------------------------
// For simple NonDispatch classes
#define  RSCOM_TYPE(cls)                                                             \
              RSCOM_CONSTRUCT(cls)                                                   \
              RSCOM_TYPE_DUMMY(cls)                                                  \


#define  RSCOM_METHOD(nm, id, ret, parms)            { nm, id, ret, (unsigned char *)parms, RS_META_METHOD, RSCOM_COMMON_LEVEL },
#define  RSCOM_PROP(nm, id, ret, parms)              { nm, id, ret, (unsigned char *)parms, RS_META_PROP, RSCOM_COMMON_LEVEL },
#define  RSCOM_PROPGET(nm, id, ret, parms)           { nm, id, ret, (unsigned char *)parms, RS_META_RDPROP, RSCOM_COMMON_LEVEL },
#define  RSCOM_EVENT(nm, id, parms)                  { nm, id, RSCOM_NULL, (unsigned char *)parms, RS_META_EVENT, RSCOM_EVENT_LEVEL },
#define  RSCOM_CLASS(nm)                             { nm,  0, RSCOM_NULL, NULL, RS_META_CLASS },
#define  RSCOM_PROC(nm, id)                          { nm, id, RSCOM_NULL, NULL, RS_META_PROC },

#define  RSCOM_METHODEX(nm, id, ret, parms, level)   { nm, id, ret, (unsigned char *)parms, RS_META_METHOD, level },
#define  RSCOM_PROPEX(nm, id, ret, parms, level)     { nm, id, ret, (unsigned char *)parms, RS_META_PROP,   level },
#define  RSCOM_PROPGETEX(nm, id, ret, parms, level)  { nm, id, ret, (unsigned char *)parms, RS_META_RDPROP, level },

#define  RSCOM_ID(nm, id)                            { nm, id, RSCOM_UNDEF, NULL, RS_META_METHOD, RSCOM_COMMON_LEVEL },


// -----------------------------------------------------------------------------
class TDispProxyImpl : public IProxyImpl, public IDispObjEx, public IGetBind
{
 public:
   // --------------------------------------------
   TDispProxyImpl(TAppObj *p) : IProxyImpl(p)
   {
   }

   // --------------------------------------------
   virtual ~TDispProxyImpl()
   {
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo *pInfo, int level)
   {
    return mgr->invokeCache(this, id, cmdType, pInfo, level);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getId(const char *name, RSDISPID *id)
   {
    return mgr->getIdCache(this, name, id);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getMetaInfo(int mask, IMetaInfo **info)
   {
    return mgr->getRemoteMetaInfo(this, mask, info);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getIdEx(const char *name, RSDISPID *id, int *level)
   {
    return mgr->getIdCacheEx(this, name, id, level);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getInfoKey(unsigned *infoKey)
   {
    return mgr->getProxyInfoKey(this, infoKey);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getBind(const char *names, int autoGet, IDispBind **out)
   {
    if(fs_appVersionA(mgr->getAppObj()) < 7)
      return RSL_STAT_NOTIMPL;

    return mgr->getBindProxy(this, names, autoGet, out);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getBind2(const char * const *names, int num, int autoGet, IDispBind **out)
   {
    if(fs_appVersionA(mgr->getAppObj()) < 7)
      return RSL_STAT_NOTIMPL;

    return mgr->getBindProxy2(this, names, num, autoGet, out);
   }

/*
   virtual TRsStat RSCOMCALL invoke (RSDISPID id,int cmdType,IParmInfo *pInfo,int level)
      { return GetProxyStubMgr ()->invoke (this,id,cmdType,pInfo,level); }

   virtual TRsStat RSCOMCALL getId (const char *name,RSDISPID *id)
      { return GetProxyStubMgr ()->getId (this,name,id); }
*/

   // --------------------------------------------
   RSCOM_INTF_BEGIN2(IProxyImpl)
     RSCOM_INTF(IID_RSDISP,     IDispObj)
     RSCOM_INTF(IID_RSDISPEX,   IDispObjEx)
     RSCOM_INTF(IID_RSGETBIND,  IGetBind)
   RSCOM_INTF_END

   // --------------------------------------------
   RSCOM_TYPE_DUMMY(TDispProxy)
};

// -----------------------------------------------------------------------------
typedef TRsObjMTImpl<TDispProxyImpl>  TDispProxy;
        

// -----------------------------------------------------------------------------
#define  RSCOM_INVOKE_BEGIN(ThisClass, Base)                                                                  \
              typedef Base  theBaseInvokeClass;                                                               \
              virtual TRsStat RSCOMCALL invokeImpl(RSDISPID id, int cmdType, IParmInfo *ptr, int level)       \
              {                                                                                               \
               TRsStat  stat = RSL_STAT_UNKNOWNID;                                                            \
               try                                                                                            \
                 {                                                                                            \
                  switch(level)                                                                               \
                       {                                                                                      \
                        default:                                                                              \
                                 stat = theBaseInvokeClass::invokeImpl(id, cmdType, ptr, level);              \
                                 break;                                                                       \
                        case RSCOM_COMMON_LEVEL:                                                              \
                             switch(id)                                                                       \
                                  {                                                                           \
                                   default:                                                                   \
                                            stat = theBaseInvokeClass::invokeImpl(id, cmdType, ptr, level);   \
                                            break;

// -----------------------------------------------------------------------------
#define  RSCOM_INVOKE_LEVEL(Level)                                                                 \
                    }                                                                              \
                 break;                                                                            \
              case Level:                                                                          \
                   switch(id)                                                                      \
                        {                                                                          \
                         default:                                                                  \
                                  stat = theBaseInvokeClass::invokeImpl(id, cmdType, ptr, level);  \
                                  break;

// -----------------------------------------------------------------------------
#define  RSCOM_INVOKE_END                                                             \
                         }                                                            \
                      break;                                                          \
                   }                                                                  \
                }                                                                     \
              catch(XRSComError &er)                                                  \
                {                                                                     \
                 stat = er.getStat();                                                 \
                }                                                                     \
              catch(RSException&)                                                     \
                {                                                                     \
                 stat = RSL_STAT_EXCEPTION;                                           \
                }                                                                     \
              return stat;                                                            \
             }

// -----------------------------------------------------------------------------
#define  RSCOM_DISP_GENERAL(id, MethodName)                                           \
            case id:                                                                  \
                 stat = MethodName(id, cmdType, ptr, level);                          \
                 break;

// -----------------------------------------------------------------------------
#define  RSCOM_DISP_PROP(id, GetProp, PutProp)                                        \
              case id:                                                                \
                   if(cmdType & (RS_DISP_PROPERTYGET | RS_DISP_METHOD))               \
                     stat = GetProp(ptr);                                             \
                   else if(cmdType & (RS_DISP_PROPERTYPUT | RS_DISP_PROPERTYPUTREF))  \
                     stat = PutProp(ptr);                                             \
                   else                                                               \
                     stat = RSL_STAT_NOTIMPL;                                         \
                   break;


// -----------------------------------------------------------------------------
#define  RSCOM_DISP_METHOD(id, MethodName)                                            \
              case id:                                                                \
                   if(cmdType & (RS_DISP_PROPERTYGET | RS_DISP_METHOD))               \
                     stat = MethodName(ptr);                                          \
                   else                                                               \
                     stat = RSL_STAT_NOTIMPL;                                         \
                   break;

// -----------------------------------------------------------------------------
#define  RSCOM_DISP_PROPGET(id, GetProp)  RSCOM_DISP_METHOD(id, GetProp)

// -----------------------------------------------------------------------------
// Now obsolete
#define  RSCOM_DISP_ADVISE                                                            \
              case RSDISP_ADVISE: case RSDISP_UNADVISE:                               \
                   stat = RsComHandleAdvise(this, id, cmdType, ptr, level);           \
                   break;


// -----------------------------------------------------------------------------
class TDispStubImpl : public IStubImpl, public IDispObjEx, public IDispCallBack, public IGetBind
{
 public:
   // --------------------------------------------
   TDispStubImpl(TAppObj *p) : IStubImpl(p), useMix(false)
   {
   }
   
   // --------------------------------------------
   virtual ~TDispStubImpl()
   {
   }

   // --------------------------------------------
   virtual size_t RSCOMCALL unMarshalMes(int cmdId, const char *inBuff, size_t inSize, IChanel *cn)
   {
    return mgr->unMarshalMesToIDisp(this, cmdId, inBuff, inSize, this, cn);
   }

   // --------------------------------------------
   virtual size_t RSCOMCALL unMarshalMesMt(int cmdId, const char *inBuff, size_t inSize, IChanel *cn, ICallCtx *ctx)
   {
    return mgr->unMarshalMesToIDispMt(this, cmdId, inBuff, inSize, this, cn, ctx);
   }

   // --------------------------------------------
   RSCOM_INTF_BEGIN2(IStubImpl)
     RSCOM_INTF(IID_RSDISP,     IDispObj)
     RSCOM_INTF(IID_RSDISPEX,   IDispObjEx)
     RSCOM_INTF(IID_RSGETBIND,  IGetBind)
   RSCOM_INTF_END

   // --------------------------------------------
   RSCOM_TYPE_BEGIN0_IMP(TDispStubImpl, TDispStub)
   RSCOM_TYPE_END

   // --------------------------------------------
   TRsStat advise(IDispObj *sink)
   {
    return mgr->advise(this, sink);
   }

   // --------------------------------------------
   TRsStat unAdvise(IDispObj *sink)
   {
    return mgr->unAdvise(this, sink);
   }

   // --------------------------------------------
   TRsStat enableEvent(IDispObj *sink, RSDISPID id)
   {
    return mgr->enableEvent(this, sink, id);
   }

   // --------------------------------------------
   TRsStat raiseEvent(RSDISPID id, IParmInfo *pInfo, int level = RSCOM_EVENT_LEVEL)
   {
    return mgr->raiseEvent(this, id, pInfo, level);
   }

   // --------------------------------------------
   TRsStat raiseEventEx(RSDISPID id, int cmdType, IParmInfo *pInfo, int level = RSCOM_EVENT_LEVEL)
   {
    if(fs_appVersionA(mgr->getAppObj()) < 8)
      return mgr->raiseEvent(this, id, pInfo, level);

    return mgr->raiseEventEx(this, id, cmdType, pInfo, level);
   }

   // --------------------------------------------
   TRsStat setExtObj(IDispObj *obj)
   {
    return mgr->setExtObj(this, obj);
   }

   // --------------------------------------------
   void setUseMixObj(bool use)
   {
    useMix = use;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL invokeImpl(RSDISPID id, int cmdType, IParmInfo *, int level)
   {
    return RSL_STAT_UNKNOWNID;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo *pInfo, int level)
   {
    if(cmdType & RS_DISP_RAISEEVENT)
      return raiseEventEx(id, cmdType, pInfo, level);

    return mgr->handleMixInvoke((level == RSCOM_DEF_LEVEL && !useMix) ? NULL : this, id, cmdType, pInfo, level, this);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getId(const char *name, RSDISPID *id)
   {
    return mgr->getMixId(useMix ? this : NULL, getTypeElems(), name, id);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getMetaInfo(int mask, IMetaInfo **info)
   {
    return mgr->makeMetaInfoEx(useMix ? this : NULL, getTypeElems(), ((IDispObj *)this)->getClassName(), mask, info);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getIdEx(const char *name, RSDISPID *id, int *level)
   {
    return mgr->getMixIdEx(useMix ? this : NULL, getTypeElems(), name, id, level);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getInfoKey(unsigned *infoKey)
   {
    return mgr->getStubInfoKey(this, infoKey);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getBind(const char *names, int autoGet, IDispBind **out)
   {
    if(fs_appVersionA(mgr->getAppObj()) < 7)
      return RSL_STAT_NOTIMPL;

    return mgr->getBindStub(this, names, autoGet, out);
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getBind2(const char * const *names, int num, int autoGet, IDispBind **out)
   {
    if(fs_appVersionA(mgr->getAppObj()) < 7)
      return RSL_STAT_NOTIMPL;

    return mgr->getBindStub2(this, names, num, autoGet, out);
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   bool useMix;
};

// -----------------------------------------------------------------------------
typedef TRsObjMTImpl<TDispStubImpl>  TDispStub;


// -----------------------------------------------------------------------------
//
// finalConstruct receive NULL pointer to moduleObj.
// So lockCount is not keep in mind the moduleObj.
//
class TModuleObjImpl : public TDispStubImpl, public IModuleObj
{
 public:
   // --------------------------------------------
   TModuleObjImpl(TAppObj *p) : TDispStubImpl(p), lockCount(0)
   {
   }

   // --------------------------------------------
   virtual void RSCOMCALL lockModule()
   {
    ++lockCount;
   }

   // --------------------------------------------
   virtual void RSCOMCALL unLockModule()
   {
    --lockCount;
   }

   // --------------------------------------------
   virtual unsigned RSCOMCALL getLockCount()
   {
    return lockCount;
   }

   // --------------------------------------------
   RSCOM_INTF_BEGIN2(TDispStubImpl)
     RSCOM_INTF(IID_RSMODULE, IModuleObj)
   RSCOM_INTF_END

   // --------------------------------------------
   RSCOM_TYPE_BEGIN_IMP(TModuleObjImpl, TDispStubImpl, TModuleObj)
   RSCOM_TYPE_END

 private:
   // --------------------------------------------
   // --------------------------------------------
   unsigned  lockCount;
};

// -----------------------------------------------------------------------------
typedef TRsObjMTImpl<TModuleObjImpl>  TModuleObj;


// -----------------------------------------------------------------------------
class TRsStreamStubImpl : public IStubImpl, public IDispObj, public IRsStream
{
 public:
   // --------------------------------------------
   TRsStreamStubImpl(TAppObj *p) : IStubImpl(p)
   {
   }

   // --------------------------------------------
   RSCOM_INTF_BEGIN2(IStubImpl)
     RSCOM_INTF(IID_RSSTREAM,   IRsStream)
     RSCOM_INTF(IID_RSDISP,     IDispObj)
   RSCOM_INTF_END

   // --------------------------------------------
   virtual size_t RSCOMCALL unMarshalMes(int cmdId, const char *inBuff, size_t inSize, IChanel *cn)
   {
    return mgr->unMarshalMesToStream(this, cmdId, inBuff, inSize, this, cn);
   }

   // --------------------------------------------
   virtual size_t RSCOMCALL unMarshalMesMt(int cmdId, const char *inBuff, size_t inSize, IChanel *cn, ICallCtx *ctx)
   {
    return mgr->unMarshalMesToStreamMt(this, cmdId, inBuff, inSize, this, cn, ctx);
   }

   // --------------------------------------------
   virtual const char *RSCOMCALL getProxyType()
   {
    return "$STRM";
   }

   // --------------------------------------------
   RSCOM_TYPE_DUMMY(TRsStreamStub)

   // --------------------------------------------
   virtual TRsStat RSCOMCALL read(void *buff, size_t sz, size_t *rdSz)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL write(const void *buff, size_t sz, size_t *wrSz)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL seek(long sz, int from)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getPos(size_t *pos)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getSize(size_t *sz)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL setSize(size_t sz)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL flush(void)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo *, int level)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getId(const char *name, RSDISPID *id)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getMetaInfo(int mask, IMetaInfo **info)
   {
    return RSL_STAT_NOTIMPL;
   }

   // --------------------------------------------
   virtual TRsStat RSCOMCALL getIStream(/*IStream*/ void **ptr)
   {
    return RSL_STAT_NOTIMPL;
   }
};

// -----------------------------------------------------------------------------
typedef TRsObjMTImpl< TRsStreamStubImpl >  TRsStreamStub;


// -----------------------------------------------------------------------------
template<class T>
class RsComCreator
{
 public:
   // --------------------------------------------
   static TRsStat initInst(T *ptr, const char *namePtr, IModuleObj *pMod, void *uData)
   {
    TRsStat     stat = RSL_STAT_OK;
    ILifeTime  *ltPtr;


    if(ptr->getInterface(IID_RSLIFETIME, (void **)&ltPtr))
     {
      stat = ltPtr->finalConstruct(namePtr, pMod, uData);

      ltPtr->release();
     }

    return stat;
   }

   // --------------------------------------------
   static TRsStat createInst(TCreateObjData *inMes)
   {
    TRsStat  stat = RSL_STAT_GEN;


    try 
      {
       T  *ptr = T::construct(inMes->app);


       inMes->obj = NULL;

       if(ptr)
        {
         ptr->addRef();

         stat = initInst(ptr, inMes->objName, inMes->mod, NULL);

         if(stat == RSL_STAT_OK)
          {
           if(!ptr->getInterface(inMes->iid, &inMes->obj))
             stat = RSL_STAT_NOINTF;
          }

         ptr->release();
        }
      }
    catch(XRSComError &er)
      {
       stat = er.getStat();
      }
    catch(RSException&)
      {
       stat = RSL_STAT_EXCEPTION;
      }

    return stat;
   }

   // --------------------------------------------
   static TRsStat createInst(TAppObj *app, const char *objName, IModuleObj *mod, void *data, int iid, void **obj)
   {
    TRsStat  stat = RSL_STAT_GEN;


    *obj = NULL;

    try
      {
       T  *ptr = T::construct(app);


       if(ptr)
        {
         ptr->addRef();

         stat = initInst(ptr, objName, mod, data);

         if(stat == RSL_STAT_OK)
          {
           if(!ptr->getInterface(iid, obj))
             stat = RSL_STAT_NOINTF;
          }

         ptr->release();
        }
      }
    catch(XRSComError &er)
      {
       stat = er.getStat();
      }
    catch(RSException&)
      {
       stat = RSL_STAT_EXCEPTION;
      }

    return stat;
   }

   // --------------------------------------------
   static TRsStat createInst(TAppObj *app, const char *objName, IModuleObj *mod, void *data, T **obj)
   {
    TRsStat  stat = RSL_STAT_GEN;


    *obj = NULL;

    try
      {
       *obj = T::construct(app);

       if(*obj)
        {
         (*obj)->addRef();

         stat = initInst(*obj, objName, mod, data);
        }
      }
    catch(XRSComError &er)
      {
       stat = er.getStat();
      }
    catch(RSException&)
      {
       stat = RSL_STAT_EXCEPTION;
      }

    return stat;
   }
};

// -----------------------------------------------------------------------------
template<class T>
class RsComCreator2
{
 public:
   // --------------------------------------------
   static TRsStat createInst(IRsObj *outerObj, TAppObj *app, const char *objName, IModuleObj *mod, void *data,
                             T **obj, IRsObj **ctrl)
   {
    TRsStat  stat = RSL_STAT_GEN;


    *obj  = NULL;
    *ctrl = NULL;

    try
      {
       *obj = T::construct(app);

       if(*obj)
        {
         if(!(*obj)->getInterface(IID_RSCTRLINTF, (void **)ctrl))
          {
           stat = RSL_STAT_NOINTF;

           delete *obj;
          }
         else
          {
           (*obj)->setOuterObj(outerObj);

           stat = RsComCreator<T>::initInst(*obj, objName, mod, data);

           if(stat != RSL_STAT_OK)
            {
             *obj = NULL;

             (*ctrl)->release();
             (*ctrl) = NULL;
            }
          }
        }
      }
    catch(XRSComError &er)
      {
       stat = er.getStat();
      }
    catch(RSException&)
      {
       stat = RSL_STAT_EXCEPTION;
      }

    return stat;
   }
};

// -----------------------------------------------------------------------------
template<class T>
class RsComMetaCreator
{
 public:
   // --------------------------------------------
   static TRsStat createMeta(TCreateObjData *inMes)
   {
    IProxyStubMgr  *ps = fs_getPSmgrA(inMes->app);

    return ps->makeMetaInfo(T::getClassMetaData(), inMes->servType, RS_META_FULL, (IMetaInfo **)&inMes->obj);
   }
};


// -----------------------------------------------------------------------------
#define  RSCOM_CREATE_OBJ(Type)                                            \
              if(strcmp(inMes->servType, Type::getClassMetaName()) == 0)   \
                return RsComCreator<Type>::createInst(inMes)

// -----------------------------------------------------------------------------
#define  RSCOM_CREATE_OBJ_MODULE(Type)                                     \
              if(strcmp(inMes->servType, "RsComModule") == 0)              \
               {                                                           \
                TRsStat  stat = RSL_STAT_OK;                               \
                if(inMes->mod)                                             \
                 {                                                         \
                  if(!inMes->mod->getInterface(inMes->iid, &inMes->obj))   \
                    stat = RSL_STAT_NOINTF;                                \
                 }                                                         \
                else                                                       \
                  stat = RsComCreator<Type>::createInst(inMes);            \
                return stat;                                               \
               }

// -----------------------------------------------------------------------------
#define  RSCOM_CREATE_META(Type)                                           \
              if(strcmp(inMes->servType, Type::getClassMetaName()) == 0)   \
                return RsComMetaCreator<Type>::createMeta(inMes)

// -----------------------------------------------------------------------------
#define  RSMODULECLS  "RsComModule"

// -----------------------------------------------------------------------------
#define  RSCOM_CREATE_META_MODULE(Type)                                    \
              if(strcmp (inMes->servType,"RsComModule") == 0)              \
                return RsComMetaCreator<Type>::createMeta(inMes)


// -----------------------------------------------------------------------------
// Smart ptr like ATL CComPtr
template <class T>
class _NoAddRefReleaseOnTRsPtr : public T
{
 private:
   virtual unsigned RSCOMCALL addRef() = 0;
   virtual unsigned RSCOMCALL release() = 0;
};


// -----------------------------------------------------------------------------
template <class T>
class TRsPtr
{
 public:
   // --------------------------------------------
   TRsPtr() : p(NULL)
   {
   }

   // --------------------------------------------
   TRsPtr(T *lp, bool doAddRef = true) : p(lp)
   {
    if(p && doAddRef) 
      p->addRef();
   }

   // --------------------------------------------
   TRsPtr(const TRsPtr<T> &lp)
   {
    if((p = lp.p) != NULL)
      p->addRef();
   }

   // --------------------------------------------
  ~TRsPtr()
   {
    if(p)
      p->release();
   }

   // --------------------------------------------
   void Release()
   {
    if(p)
      p->release();

    p = NULL;
   }

   // --------------------------------------------
   operator T * () const
   {
    return (T *)p;
   }

   // --------------------------------------------
   T & operator * () const
   {
    return *p;
   }

   // --------------------------------------------
   T ** operator & ()
   {
    RSASSERT(p == NULL);

    return &p;
   }

   // --------------------------------------------
   _NoAddRefReleaseOnTRsPtr<T> *operator -> () const
   {
    RSASSERT(p);

    return (_NoAddRefReleaseOnTRsPtr<T> *)p;
   }

   // --------------------------------------------
   T * operator = (T *lp)
   {
    if(p)
      p->release();

    p = lp;

    if(p)
      p->addRef();

    return p;
   }

   // --------------------------------------------
   T * operator = (const TRsPtr<T> &lp)
   {
    if(p)
      p->release();

    p = lp.p;

    if(p)
      p->addRef();

    return p;
   }

   // --------------------------------------------
   bool operator ! () const
   {
    return (p == NULL);
   }

   // --------------------------------------------
   bool operator < (T *pT) const
   {
    return p < pT;
   }

   // --------------------------------------------
   bool operator == (T *pT) const
   {
    return p == pT;
   }

   // --------------------------------------------
   void Attach(T *p2)
   {
    if(p)
      p->release();

    p = p2;
   }

   // --------------------------------------------
   T *Detach()
   {
    T  *pt = p;


    p = NULL;

    return pt;
   }

   // --------------------------------------------
   // --------------------------------------------
   T  *p;
};


// -----------------------------------------------------------------------------
template <class T>
class TRsObjDyn : public TRsPtr<T>
{
 public:
   // --------------------------------------------
   TRsObjDyn(TAppObj *app, const char *name = 0, IModuleObj *pMod = 0, void *data = 0)
   {
    TRsStat  stat = createObject(app, name, pMod, data);


    if(stat != RSL_STAT_OK)
      throw XRSComError(app, stat);
   }

   // --------------------------------------------
   TRsStat createObject(TAppObj *app, const char *name = 0, IModuleObj *pMod = 0, void *data = 0)
   {
    return RsComCreator<T>::createInst(app, name, pMod, data, &p);
   }
};


// -----------------------------------------------------------------------------
template <class T>
class TRsObjAutoBase
{
 public:
   // --------------------------------------------
   TRsObjAutoBase(TAppObj *app) : p(app), bInitInst(false)
   {
    p.addRef();  // It desable deletion
   }

   // --------------------------------------------
  ~TRsObjAutoBase()
   {
    ILifeTime  *ltPtr;


    if(bInitInst && p.getInterfaceImpl(IID_RSLIFETIME, (void **)&ltPtr))
      ltPtr->finalRelease();
    // Do not call obj.release () !!!
   }

   // --------------------------------------------
   TRsStat initInst(const char *namePtr, IModuleObj *pMod, void *uData)
   {
    TRsStat  stat = RsComCreator<T>::initInst(&p, namePtr, pMod, uData);


    bInitInst = (stat == RSL_STAT_OK);

    return stat;
   }

   // --------------------------------------------
   _NoAddRefReleaseOnTRsPtr<T> *operator -> () const
   {
    return (_NoAddRefReleaseOnTRsPtr<T> *)(&p);
   }

   // --------------------------------------------
   operator T * () const
   {
    return (T *)(&p);
   }        
 
   // --------------------------------------------
   T & operator * () const
   {
    return *((T *)(&p));
   } 
 
   // --------------------------------------------
   bool operator ! () const
   {
    return false;
   }

   // --------------------------------------------
   bool operator == (T *pT) const
   {
    return (T *)(&p) == pT;
   }

   // --------------------------------------------
   // --------------------------------------------
   T  p;

 protected:
   // --------------------------------------------
   // --------------------------------------------
   bool  bInitInst;
};


// -----------------------------------------------------------------------------
template <class T>
class TRsObjAuto : public TRsObjAutoBase<T>
{
 public:
   // --------------------------------------------
   TRsObjAuto(TAppObj *app, const char *name = 0, IModuleObj *pMod = 0, void *data = 0) : TRsObjAutoBase<T>(app)
   {
    initInst(name, pMod, data);
   }

   // --------------------------------------------
  ~TRsObjAuto()
   {
   }
};


// -----------------------------------------------------------------------------
template <class T>
class TRsHeapOwnerCreator : public TRsPtr<T>
{
 public:
   // --------------------------------------------
   typedef T  UserClassType;

   // --------------------------------------------
   // --------------------------------------------
   TRsHeapOwnerCreator(TAppObj *app) : appObj(app)
   {
   }

   // --------------------------------------------
   TRsStat constructObject(IRsObj *outerObj = 0, const char *name = 0, IModuleObj *pMod = 0, void *data = 0)
   {
    return RsComCreator<T>::createInst(appObj, name, pMod, data, &p);
   }

   // --------------------------------------------
   TAppObj *getApp()
   {
    return appObj;
   }

   // --------------------------------------------
   bool isInited() const
   {
    return p != NULL;
   }

   // --------------------------------------------
   // --------------------------------------------
   TAppObj  *appObj;
};


// -----------------------------------------------------------------------------
template <class T>
class TRsHeapMemberOwnerCreator : public TRsPtr<T>
{
 public:
   // --------------------------------------------
   typedef T  UserClassType;

   // --------------------------------------------
   TRsHeapMemberOwnerCreator(TAppObj *app) : appObj(app), ctrl(0)
   {
   }

   // --------------------------------------------
  ~TRsHeapMemberOwnerCreator ()
   {
    if(p)
      p->setOuterObj(0);

    p = NULL;

    if(ctrl)
      ctrl->release();
   }

   // --------------------------------------------
   TRsStat constructObject(IRsObj *outerObj = 0, const char *name = 0, IModuleObj *pMod = 0, void *data = 0)
   {
    return RsComCreator2<T>::createInst(outerObj, appObj, name, pMod, data, &p, &ctrl);
   }

   // --------------------------------------------
   TAppObj *getApp()
   {
    return appObj;
   }

   // --------------------------------------------
   bool isInited() const
   {
    return p != NULL;
   }


   // --------------------------------------------
   // --------------------------------------------
   TAppObj  *appObj;

 private:
   void  Release();
   T    *Detach();
   void  Attach(T *p2);

   // --------------------------------------------
   // --------------------------------------------
   IRsObj  *ctrl;
};


// -----------------------------------------------------------------------------
template <class T>
class TRsAutoOwnerCreator : public TRsObjAutoBase<T>
{
 public:
   // --------------------------------------------
   typedef T  UserClassType;

   // --------------------------------------------
   TRsAutoOwnerCreator(TAppObj *app) : TRsObjAutoBase<T>(app), bInited(false)
   {
   }

   // --------------------------------------------
  ~TRsAutoOwnerCreator()
   {
    p.setOuterObj(0);
   }

   // --------------------------------------------
   TRsStat constructObject(IRsObj *outerObj = 0, const char *name = 0, IModuleObj *pMod = 0, void *data = 0)
   {
    bInited = true;

    p.setOuterObj(outerObj);

    return initInst(name, pMod, data);
   }

   // --------------------------------------------
   TAppObj *getApp()
   {
    return p.getAppObj();
   }

   // --------------------------------------------
   bool isInited() const
   {
    return bInited;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   bool  bInited;
};


// -----------------------------------------------------------------------------
template <class T, int iid>
class TRsPtrX
{
 public:
   // --------------------------------------------
   TRsPtrX() : p(0)
   {
   }

   // --------------------------------------------
   TRsPtrX(T *lp, bool doAddRef = true) : p(lp)
   {
    if(p && doAddRef) 
      p->addRef();
   }

   // --------------------------------------------
   TRsPtrX(IRsObj *lp) : p(0)
   {
    if(lp)
      lp->getInterface(iid, (void **)&p);
   }

   // --------------------------------------------
   TRsPtrX(const TRsPtrX<T, iid> &lp)
   {
    if((p = lp.p) != NULL)
      p->addRef();
   }

   // --------------------------------------------
  ~TRsPtrX()
   {
    if(p)
      p->release();
   }

   // --------------------------------------------
   void Release()
   {
    if(p)
      p->release();

    p = NULL;
   }

   // --------------------------------------------
   operator T * () const
   {
    return (T *)p;
   }

   // --------------------------------------------
   T & operator * () const
   {
    return *p;
   }

   // --------------------------------------------
   T ** operator & ()
   {
    RSASSERT(p == NULL);

    return &p;
   }

   // --------------------------------------------
   _NoAddRefReleaseOnTRsPtr<T> *operator -> () const
   {
    RSASSERT(p);

    return (_NoAddRefReleaseOnTRsPtr<T> *)p;
   }

   // --------------------------------------------
   T * operator = (T *lp)
   {
    if(p)
      p->release();

    p = lp;

    if(p)
      p->addRef();

    return p;
   }

   // --------------------------------------------
   T * operator = (const TRsPtr<T> &lp)
   {
    if(p)
      p->release();

    p = lp.p;

    if(p)
      p->addRef();

    return p;
   }

   // --------------------------------------------
   T * operator = (const TRsPtrX<T, iid> &lp)
   {
    if(p)
      p->release();

    p = lp.p;

    if(p)
      p->addRef();

    return p;
   }

   // --------------------------------------------
   T * operator = (IRsObj *lp)
   {
    if(p)
      p->release();

    p = NULL;

    if(lp)
      lp->getInterface(iid, (void **)&p);

    return p;
   }

   // --------------------------------------------
   bool operator ! () const
   {
    return (p == NULL);
   }

   // --------------------------------------------
   bool operator < (T *pT) const
   {
    return p < pT;
   }

   // --------------------------------------------
   bool operator == (T *pT) const
   {
    return p == pT;
   }

   // --------------------------------------------
   void Attach(T *p2)
   {
    if(p)
      p->release();

    p = p2;
   }

   // --------------------------------------------
   T *Detach()
   {
    T  *pt = p;


    p = NULL;

    return pt;
   }

   // --------------------------------------------
   // --------------------------------------------
   T  *p;
};


// -----------------------------------------------------------------------------
class TRsError
{
 private:
   // --------------------------------------------
   // --------------------------------------------
   IRsErrorInfo  *intf;
   ICallCtx      *ctx;
   TRsCodePage    codePage;

 public:
   // --------------------------------------------
   TRsError(TRsCodePage code = RSCOM_CODEPAGE) : intf(0), ctx(0), codePage(code)
   {
   }

   // --------------------------------------------
   TRsError(IRsErrorInfo *ptr, TRsCodePage code = RSCOM_CODEPAGE) : intf(ptr), ctx(0), codePage(code)
   {
   }

   // --------------------------------------------
   TRsError(IProxyStubMgr *ps, TRsCodePage code = RSCOM_CODEPAGE) : intf(0), codePage(code)
   {
    ctx = ps->getCtxManager()->getCtxForLocalThread();

    ctx->getErrInfo(&intf);
   }

   // --------------------------------------------
   TRsError(TAppObj *obj, TRsCodePage code = RSCOM_CODEPAGE) : intf(0), codePage(code)
   {
    IProxyStubMgr  *ps = fs_getPSmgrA(obj);


    ctx = ps->getCtxManager()->getCtxForLocalThread();

    ctx->getErrInfo(&intf);
   }

   // --------------------------------------------
   TRsError(ICallCtx *ctx_, TRsCodePage code = RSCOM_CODEPAGE) : ctx(ctx_), intf(0), codePage(code)
   {
    ctx->getCtxMgr()->lockCtx(ctx);

    ctx->getErrInfo(&intf);
   }

   // --------------------------------------------
   IRsErrorInfo ** operator & ()
   {
    RSASSERT(intf == NULL);

    return &intf;
   }

   // --------------------------------------------
   operator IRsErrorInfo * ()
   {
    return intf;
   }

   // --------------------------------------------
  ~TRsError()
   {
    if(intf)
      intf->release();

    if(ctx)
      ctx->getCtxMgr()->freeCtx(ctx);
   }

   // --------------------------------------------
   int getCount()
   {
    return intf->getCount();
   }

   // --------------------------------------------
   void reset()
   {
    intf->reset();
   }

   // --------------------------------------------
   int getLevel(int index)
   {
    return intf->getLevel(index);
   } 

   // --------------------------------------------
   long getErrCode(int index)
   {
    return intf->getErrCode(index);
   } 

   // --------------------------------------------
   void addError(long erCode, int level, const char *desc)
   {
      intf->addError (erCode, level, desc, codePage);
   }

   // --------------------------------------------
   const char *getDesc(int index)
   {
    return intf->getDesc(index, codePage);
   }

   // --------------------------------------------
   int loadFrom(IRsErrorInfo *from)
   {
    TAppObj        *app = intf->getAppObj();
    IProxyStubMgr  *ps  = fs_getPSmgrA(app);

    return ps->MoveErrInfo(intf, from);
   }
};


// -----------------------------------------------------------------------------
inline void RSComTrowErrorEx (TAppObj *app,long erCode,int level,const char *mes,TRsCodePage codePage = RSCOM_CODEPAGE)
{
   TRsError err(app,codePage);
   err.addError(erCode, level, mes);
   throw XRSComError (app, RSL_STAT_EX);
}

// -----------------------------------------------------------------------------
inline void RSComTrowErrorEx (IProxyStubMgr *ps,long erCode,int level,const char *mes,TRsCodePage codePage = RSCOM_CODEPAGE)
{
   TRsError err(ps,codePage);
   err.addError(erCode, level, mes);
   throw XRSComError (ps,RSL_STAT_EX);
}


// -----------------------------------------------------------------------------
class ThreadCtx
{
 public:
   // --------------------------------------------
   ThreadCtx(ICtxManager *mgr_) : mgr(mgr_)
   {
    ctx = mgr->getCtxForLocalThread();
   }

   // --------------------------------------------
   ThreadCtx(TAppObj *app) : mgr(0)
   {                              
    if(app)
     {
      mgr = fs_getPSmgrA(app)->getCtxManager();
      ctx = mgr->getCtxForLocalThread();
     }
   }

   // --------------------------------------------
   ThreadCtx(IProxyStubMgr *ps) : mgr(0)
   {
    if(ps)
     {
      mgr = ps->getCtxManager();
      ctx = mgr->getCtxForLocalThread();
     }
   }

   // --------------------------------------------
  ~ThreadCtx()
   {   
    if(mgr && ctx)
      mgr->freeCtx(ctx);
   }  
      
   // --------------------------------------------
   void release()
   {
    if(mgr && ctx)
      mgr->freeCtx(ctx);

    mgr = 0;
   }       

   // --------------------------------------------
   ICallCtx * operator -> () const
   {
    return ctx;
   }

   // --------------------------------------------
   operator ICallCtx * () const
   {
    return ctx;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   ICtxManager  *mgr;
   ICallCtx     *ctx;
};


// -----------------------------------------------------------------------------
class TRsLock
{
   // --------------------------------------------
   // --------------------------------------------
   IRsLock  *lockObj;
   bool      isLocked;

 public:
   // --------------------------------------------
   TRsLock(IRsLock *obj, bool doLock = true) : lockObj(obj), isLocked(false)
   {
    if(doLock)
      Lock();
   }

   // --------------------------------------------
  ~TRsLock()
   {
    Unlock();
   }

   // --------------------------------------------
   bool Lock(int timeOut = -1)
   {
    if(!isLocked && lockObj)
     {
      isLocked = lockObj->lock(timeOut);

      if(!isLocked && timeOut == -1)
        RSComTrowErrorEx(lockObj->getAppObj(), 1, RSCOM_ITF_ERROR_LEVEL, "Ошибка блокировки объекта синхронизации");
     }

    return isLocked;
   }

   // --------------------------------------------
   bool Unlock()
   {
    if(isLocked)
      isLocked = !lockObj->unlock();

    return !isLocked;
   }
};


// -----------------------------------------------------------------------------
class TRsMultiLock
{
   // --------------------------------------------
   // --------------------------------------------
   TRsPtr<IRsMultiLock>     lockObj;

 public:
   // --------------------------------------------
   TRsMultiLock(IRsMultiLock *lock, bool doLock = true) : lockObj(lock)
   {
    if(doLock)
      Lock();
   }

   // --------------------------------------------
   TRsMultiLock(TAppObj *app, IRsLock *array[], int count, bool doLock = false)
   {
    if(!fs_getPSmgrA(app)->createMultiLock(array, count, &lockObj))
      RSComTrowErrorEx(app, 2, 1, "Create multi Lock");

    if(doLock)
      Lock();
   }

   // --------------------------------------------
  ~TRsMultiLock()
   {
    Unlock();
   }

   // --------------------------------------------
   int LockEx(int timeOut, bool waitForAll, unsigned wakeMask, bool *isAbandon)
   {
    Unlock();

    int  stat = lockObj->lockEx(timeOut, waitForAll, wakeMask, isAbandon);


    if(stat == -1)
      RSComTrowErrorEx(lockObj->getAppObj(), 1, RSCOM_ITF_ERROR_LEVEL, "Ошибка блокировки объекта синхронизации");

    return stat;
   }

   // --------------------------------------------
   bool Lock(int timeOut = -1)
   {
    bool  isAbandon;
    int   stat = LockEx(timeOut, true, 0, &isAbandon);


    if(stat != RSCOM_WAIT_TIMEOUT)
      return true;

    return false;
   }

   // --------------------------------------------
   bool Unlock()
   {
    return lockObj->unlock();
   }

   // --------------------------------------------
   bool UnlockEx(long count, long *prevCount)
   {
    return lockObj->unlockEx(count, prevCount);
   }

   // --------------------------------------------
   bool ResetEvents()
   {
    return lockObj->resetEvents();
   }

   // --------------------------------------------
   bool IsLocked(int index)
   {
    return lockObj->isLocked(index);
   }
};

#endif // __RSCOMI_HPP

/* <-- EOF --> */