/*-----------------------------------------------------------------------*-

 File Name   : rsforms\cmdset.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2004 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Jan 08, 2004 Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __cmdset_hpp__
#define __cmdset_hpp__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TCmdSetBaseImpl : public TObjectImpl, public ICmdSet, public IDesignedObject
   {
public:
   TCmdSetBaseImpl(TAppObj* App) : TObjectImpl(App) 
      {}

   virtual ~TCmdSetBaseImpl() 
      {}

RSCOM_INTF_BEGIN2(TObjectImpl)
   RSCOM_INTF(IID_CMDSET, ICmdSet)
   RSCOM_INTF(IID_DESIGNEDOBJ, IDesignedObject)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_CMDSET, "CCmdSet", (void**)&impl);

      if (!impl->getInterface(IID_DESIGNEDOBJ, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support IDesignedObject interface");         
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      TObjectImpl::initComponent(impl);
      }

public: // IEventHandler
   TRsStat RSCOMCALL onEvent(RSDISPID id, IParmInfo* pInfo, int level)
      { 
      ((IDispObj*)(this))->addRef();
      TRsStat stat = TRsPtrX<IEventHandler, IID_EVENTHANDLER>(impl)->onEvent(id, pInfo, level); 
      ((IDispObj*)(this))->release();
      return stat;
      }

public: //ICmdSet
   bool RSCOMCALL setOwner(ICmdSetContainer* pOwner)
      { return impl->setOwner(pOwner); }

   const char* RSCOMCALL getName()
      { return impl->getName(); }

   //void RSCOMCALL alterPath(const char* szPath)
   //   { impl->alterPath(szPath); }

public: //IDesignedObject
   void RSCOMCALL setTemplate(const char* szLib, const char* szRes)
      { impl2->setTemplate(szLib, szRes); }

   const char* RSCOMCALL getLibName()
      { return impl2->getLibName(); }

   const char* RSCOMCALL getResName()
      { return impl2->getResName(); }
    
RSCOM_INVOKE_BEGIN(TCmdSetBaseImpl, TObjectImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_PROPGET(RTM_NAME, RsCracker(this, &TCmdSetBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_GETNAME, RsCracker(this, &TCmdSetBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_SETOWNER, RsCracker(this, &TCmdSetBaseImpl::_setOwner))
   RSCOM_DISP_METHOD(RTM_SETNAME, RsCracker(this, &TCmdSetBaseImpl::_setName))
   RSCOM_DISP_METHOD(RTM_SETTEMPLATE, RsCracker(this, &TCmdSetBaseImpl::_setTemplate))
   RSCOM_DISP_METHOD(RTM_GETLIBNAME, RsCracker(this, &TCmdSetBaseImpl::_getLibName))
   RSCOM_DISP_METHOD(RTM_GETRESNAME, RsCracker(this, &TCmdSetBaseImpl::_getResName))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TCmdSetBaseImpl, TObjectImpl, TCmdSetBase)
   RSCOM_PROPGETEX("name", RTM_NAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getName", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setOwner", RTM_SETOWNER, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setName", RTM_SETNAME, NULL, RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setTemplate", RTM_SETTEMPLATE, NULL, RV_STR RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getLibName", RTM_GETLIBNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getResName", RTM_GETRESNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _getName(TValRef retVal)
      {
      retVal = getName();
      return RSL_STAT_OK;
      }

   TRsStat _setOwner(TValRef retVal, IRsObj* ptr)
      {
      setOwner(TRsPtrX<ICmdSetContainer, IID_CMDSETCONT>(ptr));
      return RSL_STAT_OK;
      }

   TRsStat _setName(TValRef retVal, const char* szName)
      {
      RSASSERT(false);
//      setName(szName);
      return RSL_STAT_OK;
      }

   TRsStat _setTemplate(TValRef retVal, const char* szLib, const char* szRes)
      {
      setTemplate(szLib, szRes);
      return RSL_STAT_OK;
      }

   TRsStat _getLibName(TValRef retVal)
      {
      retVal = getLibName();
      return RSL_STAT_OK;
      }

   TRsStat _getResName(TValRef retVal)
      {
      retVal = getResName();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<ICmdSet> impl;
   TRsPtr<IDesignedObject> impl2;
};

/*-----------------------------------------------------------------------*/

class TCmdTargetImpl : public TEventHandlerImpl, public ICmdTarget
{
public:
    TCmdTargetImpl(TAppObj* app) : TEventHandlerImpl(app)
      {}
    
   virtual ~TCmdTargetImpl() 
      {}

RSCOM_INTF_BEGIN2(TEventHandlerImpl)
   RSCOM_INTF(IID_CMDTARGET, ICmdTarget)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_CMDTARGET, "CCmdTarget", (void**)&impl);
      TEventHandlerImpl::initComponent(impl);
      }

public: //ICmdTarget
   void RSCOMCALL setParentTarget(ICmdTarget* pParent)
      { impl->setParentTarget(pParent); }

   ICmdTarget* RSCOMCALL getParentTarget()
      { return impl->getParentTarget(); }

   ICmdHandler* RSCOMCALL getCmdHandlerFor(const char* szSetName)
      { return impl->getCmdHandlerFor(szSetName); }

   ICmdHandler* RSCOMCALL getCmdHandler(ICmdSet* pSet)
      { return impl->getCmdHandler(pSet); }

   bool RSCOMCALL onCommand(ICmdSet* pSender, unsigned long id, IDispObj* pFocusedObj, const char* szParams, IDispObj* pCommandItem)
      { return impl->onCommand(pSender, id, pFocusedObj, szParams, pCommandItem); }

public:
   ICmdHandler* RSCOMCALL getCmdHandlerFor(ICmdSet* pSet) // הכÿ סמגלוסעטלמסעט
      { return getCmdHandler(pSet); }


RSCOM_INVOKE_BEGIN(TCmdTargetImpl, TEventHandlerImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_SETPARENT, RsCracker(this, &TCmdTargetImpl::_setParentTarget))
   RSCOM_DISP_METHOD(RTM_GETPARENT, RsCracker(this, &TCmdTargetImpl::_getParentTarget))
   RSCOM_DISP_METHOD(RTM_GETCMDHANDLER, RsCracker(this, &TCmdTargetImpl::_getCmdHandlerFor))
   RSCOM_DISP_METHOD(RTM_GETCMDHANDLER2, RsCracker(this, &TCmdTargetImpl::_getCmdHandler))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TCmdTargetImpl, TEventHandlerImpl, TCmdTarget)
   RSCOM_METHODEX("setParentTarget", RTM_SETPARENT, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getParentTarget", RTM_GETPARENT, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getCmdHandlerFor", RTM_GETCMDHANDLER, RSCOM_OBJ, RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getCmdHandler", RTM_GETCMDHANDLER2, RSCOM_OBJ, RV_OBJ, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _setParentTarget(TValRef retVal, IRsObj* ptr)
      {
      setParentTarget(TRsPtrX<ICmdTarget, IID_CMDTARGET>(ptr));
      return RSL_STAT_OK;
      }

   TRsStat _getParentTarget(TValRef retVal)
      {
      retVal = getParentTarget();
      return RSL_STAT_OK;
      }

   TRsStat _getCmdHandlerFor(TValRef retVal, const char* szName)
      {
      retVal = getCmdHandlerFor(szName);
      return RSL_STAT_OK;
      }

   TRsStat _getCmdHandler(TValRef retVal, IRsObj* ptr)
      {
      retVal = getCmdHandler(TRsPtrX<ICmdSet, IID_CMDSET>(ptr));
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<ICmdTarget> impl;

};

DECLARE_CLASS(TCmdTarget, TCmdTargetImpl);

/*-----------------------------------------------------------------------*/
#endif // __cmdset_hpp__
