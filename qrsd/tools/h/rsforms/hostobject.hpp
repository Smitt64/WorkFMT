/*-----------------------------------------------------------------------*-

 File Name   : rsforms\hostobject.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 28, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSHOSTOBJECT_H__
#define __RSFORMSHOSTOBJECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TObjectImpl : public TDispStubImpl, public IDispStubImpl,
                    public IHostObject, public IEventHandler
   {
public:
   TObjectImpl(TAppObj* App) : TDispStubImpl(App), app(App)
      { 
      setUseMixObj(true); 
      }

   virtual ~TObjectImpl() 
      {}

RSCOM_INTF_BEGIN2(TDispStubImpl)
   RSCOM_INTF(IID_HOSTOBJECT, IHostObject)
   RSCOM_INTF(IID_EVENTHANDLER, IEventHandler)
RSCOM_INTF_END_EX(impl)

protected:
   virtual TRsStat RSCOMCALL finalConstruct(const char* namePtr, IModuleObj* pMod, void* data)
      { 
      TRsStat stat = TDispStubImpl::finalConstruct(namePtr, pMod, data);
      if (stat == RSL_STAT_OK) 
         {
         // механизм создания и инициализации импла
         initComponent(NULL);
         if (!impl->init(this, (IParmInfo*)data))
            checkErrorInfo(app);
         }
      return stat; 
      }

   // Дочерние классы должны переопределять данную функцию при необходимости 
   // создания собственных имплов со скрытой реализацией.
   // Базовую функцию всегда необходимо вызывать в последнюю очередь.
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_HOSTOBJECT2, "CHostObject", (void**)&impl);

      if (!impl->getInterface(IID_RSDISP, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support IDispObj interface");
         throw XRSFormsError(app, RSL_STAT_EX);
         }
      }

   virtual void RSCOMCALL finalRelease()
      { 
      RSASSERT (impl); // метод не должен вызываться, если объект не был сконструирован до конца
      if (impl) 
         impl->done(); // деинициализация импла
      TDispStubImpl::finalRelease();
      }

protected:
   // создание/проверка переданного импла
   void checkImpl(IRsObj* pImpl, int iid, const char* implClass, void** ppIntf) throw(XRSFormsError)
      {
      if (!pImpl)
         {
         // создание локального серверного объекта
         TRsStat stat = fs_getServerObjectA(app, runtime_module, implClass, NULL, iid, ppIntf, true, 0, NULL, 0);
         RTM_CHECK_2(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete %s from %s.", implClass, runtime_module);
         if(stat) throw(XRSFormsError(app, stat));
         }
      else if (!pImpl->getInterface(iid, ppIntf))
         {
         RTM_ADDERROR_1(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support interface with id %u", iid);
         throw XRSFormsError(app, RSL_STAT_EX);
         }
      }

public: // IDispObj
   TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
      { return impl2->invoke(id, cmdType, pInfo, level); }

public: // IDispStubImpl
   TRsStat RSCOMCALL invokeBase(RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
      { return TDispStubImpl::invoke(id, cmdType, pInfo, level); }

   TRsStat RSCOMCALL raiseEventBase(RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
      { return TDispStubImpl::raiseEventEx(id, cmdType, pInfo, level); }

public: // IHostObject
   void RSCOMCALL attach(IDispObj* pRemote)
      { impl->attach(pRemote); }

   void RSCOMCALL detach()
      { impl->detach(); }

   IDispObj* RSCOMCALL getControlObj()
      { return impl->getControlObj(); }

   IDispObj* RSCOMCALL getSink()
      { return impl->getSink(); }

   TRsStat raiseEvent(RSDISPID id, IParmInfo* pInfo, int level = RSCOM_EVENT_LEVEL)
      { return impl->raiseEventEx(id, RS_DISP_METHOD, pInfo, level); }

   TRsStat raiseEventEx(RSDISPID id, int cmdType, IParmInfo* pInfo, int level = RSCOM_EVENT_LEVEL)
      { return impl->raiseEventEx(id, cmdType, pInfo, level); }

public: // IEventHandler
   TRsStat RSCOMCALL onEvent(RSDISPID id, IParmInfo* pInfo, int level)
      { return RSL_STAT_OK; }

protected:
   IDispObj* getControlObjEx() throw(XRSFormsError)
      {
      IDispObj* p = getControlObj();
      // Borland can not compile this code
      //return p ? p : (throw(XRSFormsError(app, RSL_STAT_EX, "Object not attached.")), NULL);
      if (!p)
         throw XRSFormsError (app, RSL_STAT_EX, "Object not attached.");
      return p;
      }

   RSCOM_INVOKE_BEGIN(TObjectImpl,TDispStubImpl)
   RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
      RSCOM_DISP_METHOD(RTM_THIS, RsCracker(this, &TObjectImpl::_this))
      RSCOM_DISP_METHOD(RTM_ATTACH, RsCracker(this, &TObjectImpl::_attach))
      RSCOM_DISP_METHOD(RTM_DETACH, RsCracker(this, &TObjectImpl::_detach))
      RSCOM_DISP_METHOD(RTM_GETCONTROLOBJ, RsCracker(this, &TObjectImpl::_getControlObj))
      RSCOM_DISP_METHOD(RTM_GETSINK, RsCracker(this, &TObjectImpl::_getSink))
   RSCOM_INVOKE_END

   RSCOM_TYPE_BEGIN_IMP(TObjectImpl,TDispStubImpl,TObject)
      RSCOM_METHODEX("attach", RTM_ATTACH, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("detach", RTM_DETACH, NULL, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("getControlObj", RTM_GETCONTROLOBJ, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("getSink", RTM_GETSINK, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_TYPE_END

private:
   TRsStat _this(TValRef retVal)
      {
      retVal = static_cast<IDispObj*>(this);
      return RSL_STAT_OK;
      }

   TRsStat _attach(TValRef retVal, IRsObj* pRemote)
      {
      attach(TRsPtrX<IDispObj, IID_RSDISP>(pRemote));
      return RSL_STAT_OK;
      }

   TRsStat _detach(TValRef retVal)
      {
      detach();
      return RSL_STAT_OK;
      }

   TRsStat _getControlObj(TValRef retVal)
      {
      retVal = getControlObj();
      return RSL_STAT_OK;
      }

   TRsStat _getSink(TValRef retVal)
      {
      retVal = getSink();
      return RSL_STAT_OK;
      }

protected:
   TAppObj* app;
   TRsPtr<IHostObject2> impl;
   TRsPtr<IDispObj> impl2;
   };

DECLARE_CLASS(TObject, TObjectImpl);

/*-----------------------------------------------------------------------*/

class TEventHandlerImpl : public TObjectImpl,
                          public IEventHandler2
   {
public:
   TEventHandlerImpl(TAppObj* app) : TObjectImpl(app) 
      {}

   virtual ~TEventHandlerImpl() 
      {}

RSCOM_INTF_BEGIN2(TObjectImpl)
   RSCOM_INTF(IID_EVENTHANDLER2, IEventHandler2)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_EVENTHANDLER2, "CEventHandler", (void**)&impl);
      TObjectImpl::initComponent(impl);
      }

public: // IEventHandler2
   TRsStat RSCOMCALL onEvent(RSDISPID id, IParmInfo* pInfo, int level)
      { return impl->onEvent(id, pInfo, level); }

   void RSCOMCALL addHandler(RSDISPID id, int level, MemberFunction* ptr)
      { impl->addHandler(id, level, ptr); }

   void RSCOMCALL remHandler(RSDISPID id, int level, MemberFunction* ptr)
      { impl->remHandler(id, level, ptr); }

   long RSCOMCALL getHandlersCount()
      { return impl->getHandlersCount(); }

   bool RSCOMCALL getHandlerByIndex(int index, RSDISPID* pId, int* pLevel, MemberFunction** ppf)
      { return impl->getHandlerByIndex(index, pId, pLevel, ppf); }

   RSCOM_INVOKE_BEGIN(TEventHandlerImpl,TObjectImpl)
      RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
      RSCOM_DISP_METHOD(RTM_ADDHANDLER, RsCracker(this, &TEventHandlerImpl::_addHandler))
      RSCOM_DISP_METHOD(RTM_REMHANDLER, RsCracker(this, &TEventHandlerImpl::_remHandler))
      RSCOM_INVOKE_END

      RSCOM_TYPE_BEGIN_IMP(TEventHandlerImpl,TObjectImpl,TEventHandler)
      RSCOM_METHODEX("addHandler", RTM_ADDHANDLER, NULL, RV_I4 RV_I4 RV_OBJ, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("remHandler", RTM_REMHANDLER, NULL, RV_I4 RV_I4 RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_TYPE_END

private:
   TRsStat _addHandler(TValRef retVal, RSDISPID id, int level, IRsObj* ptr)
      {
      addHandler(id, level, TRsPtrX<IDispObj, IID_RSDISP>(ptr));
      return RSL_STAT_OK;
      }

   TRsStat _remHandler(TValRef retVal, RSDISPID id, int level, IRsObj* ptr)
      {
      remHandler(id, level, TRsPtrX<IDispObj, IID_RSDISP>(ptr));
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IEventHandler2> impl;
   };

DECLARE_CLASS(TEventHandler, TEventHandlerImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSHOSTOBJECT_H__
