/*-----------------------------------------------------------------------*-

File Name   : rsforms\binder.hpp
Programmer  : Leonid S. Pavlov

Copyright (c) 1991 - 2007 by R-Style Software Lab.
All Rights Reserved.

-*- History -------------------------------------------------------------*-
Mar 21, 2007  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSBINDER_H__
#define __RSFORMSBINDER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TBinderBaseImpl : public TEventHandlerImpl, public IControlLight
   {
public:
   TBinderBaseImpl(TAppObj* app) : TEventHandlerImpl(app)
      {}

   virtual ~TBinderBaseImpl() 
      {}

RSCOM_INTF_BEGIN2(TEventHandlerImpl)
   RSCOM_INTF(IID_CONTROL_LIGHT, IControlLight)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_CONTROL_LIGHT, "CBinder", (void**)&impl);
      TEventHandlerImpl::initComponent(impl);
      }

public: // IControlLight
   const char* RSCOMCALL getName()
      { return impl->getName(); }
   const char* RSCOMCALL getControlType()
      { return impl->getControlType(); }
   evFilterType RSCOMCALL getEventFilterType()
      { return impl->getEventFilterType(); }
   IContainer* RSCOMCALL getContainer()
      {return impl->getContainer(); }
   bool RSCOMCALL isLoaded()
      { return impl->isLoaded(); }
   void RSCOMCALL setName(const char* szName)
      { impl->setName(szName); }
   void RSCOMCALL setOwner(IContainer* pOwner)
      { impl->setOwner(pOwner); }

RSCOM_INVOKE_BEGIN(TBinderBaseImpl, TEventHandlerImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_PROPGET(RTM_NAME, RsCracker(this, &TBinderBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_GETNAME, RsCracker(this, &TBinderBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_GETCONTROLTYPE, RsCracker(this, &TBinderBaseImpl::_getControlType))
   RSCOM_DISP_METHOD(RTM_GETEVFILTERTYPE, RsCracker(this, &TBinderBaseImpl::_getEventFilterType))
   RSCOM_DISP_METHOD(RTM_GETCONTAINER, RsCracker(this, &TBinderBaseImpl::_getContainer))
   RSCOM_DISP_PROPGET(RTM_ISLOADED, RsCracker(this, &TBinderBaseImpl::_isLoaded))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TBinderBaseImpl, TEventHandlerImpl, TBinderBase)
   RSCOM_PROPGETEX("name", RTM_NAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getName", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getControlType", RTM_GETCONTROLTYPE, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getEventFilterType", RTM_GETEVFILTERTYPE, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getContainer", RTM_GETCONTAINER, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("loaded", RTM_ISLOADED, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _getName(TValRef retVal)
      {
      retVal = getName();
      return RSL_STAT_OK;
      }

   TRsStat _getControlType(TValRef retVal)
      {
      retVal = getControlType();
      return RSL_STAT_OK;
      }

   TRsStat _getEventFilterType(TValRef retVal)
      {
      retVal = getEventFilterType();
      return RSL_STAT_OK;
      }

   TRsStat _getContainer(TValRef retVal)
      {
      retVal = getContainer();
      return RSL_STAT_OK;
      }

   TRsStat _isLoaded(TValRef retVal)
      {
      retVal = isLoaded();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IControlLight> impl;
   };

DECLARE_CLASSEX(TBinderBase, TBinderBaseImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSBINDER_H__
