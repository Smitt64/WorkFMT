/*-----------------------------------------------------------------------*-

 File Name   : rsforms\axobject.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Jun 21, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSAXOBJECT_H__
#define __RSFORMSAXOBJECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TAxObjectImpl : public TEventHandlerImpl, public IAxObject
{   
public:
   TAxObjectImpl(TAppObj* app) : TEventHandlerImpl(app) 
      {}
    
   virtual ~TAxObjectImpl() 
      {}

RSCOM_INTF_BEGIN2(TEventHandlerImpl)
    RSCOM_INTF(IID_AXOBJ, IAxObject)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_AXOBJ, "CAxObject", (void**)&impl);
      TEventHandlerImpl::initComponent(impl);
      }

public:
   TRsStat RSCOMCALL CreateComObject(const char* szProgID, const char* szEvType, bool bUseActive, bool bRemote)
      { return impl->CreateComObject(szProgID, szEvType, bUseActive, bRemote); }

   void RSCOMCALL Unsubscribe()
      { impl->Unsubscribe(); }

RSCOM_INVOKE_BEGIN(TAxObjectImpl,TEventHandlerImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_CREATEAX, RsCracker(this, &TAxObjectImpl::_CreateObject))
   RSCOM_DISP_METHOD(RTM_UNSUBSCRIBE, RsCracker(this, &TAxObjectImpl::_Unsubscribe))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TAxObjectImpl,TEventHandlerImpl, TAxObject)
   RSCOM_METHODEX("CreateComObject", RTM_CREATEAX, NULL, RV_STR RV_STR RV_BOOL RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("Unsubscribe", RTM_UNSUBSCRIBE, NULL, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _CreateObject(TValRef retVal, const char* szProgID, const char* szEvType, bool bUseActive, bool bRemote)
      { return CreateComObject(szProgID, szEvType, bUseActive, bRemote); }

   TRsStat _Unsubscribe(TValRef retVal)
      { return Unsubscribe(), RSL_STAT_OK; }

protected:
    TRsPtr<IAxObject> impl;
};

DECLARE_CLASS(TAxObject, TAxObjectImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSAXOBJECT_H__
