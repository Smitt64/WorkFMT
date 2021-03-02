/*-----------------------------------------------------------------------*-

 File Name   : rsforms\tab.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Apr 22, 2004  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSTAB_H__
#define __RSFORMSTAB_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TTabBaseImpl : public TContainerImpl, public ITab
   {
public:
   TTabBaseImpl(TAppObj* app) : TContainerImpl(app)
      {}

   ~TTabBaseImpl()
      {}

RSCOM_INTF_BEGIN2(TContainerImpl)
   RSCOM_INTF(IID_TABCTL, ITab)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_TABCTL, "CTab", (void**)&impl);
      TContainerImpl::initComponent(impl);
      }
   
public: // ITab
   bool RSCOMCALL addPage(IForm* pFrm, const char* szName, long index, const char* szCaption)
      { return impl->addPage(pFrm, szName, index, szCaption); }

RSCOM_INVOKE_BEGIN(TTabBaseImpl, TContainerImpl)
RSCOM_INVOKE_LEVEL(RSCOM_SYS_LEVEL)
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_1, RsCracker(this, &TTabBaseImpl::_init1))
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_2, RsCracker(this, &TTabBaseImpl::_init2))
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_ADDFORM, RsCracker(this, &TTabBaseImpl::_addPage))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TTabBaseImpl, TContainerImpl, TTabBase)
   RSCOM_METHODEX("addPage", RTM_ADDFORM, RSCOM_BOOL, RV_OBJ RV_STR RV_I4 RV_STR, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _init1(TValRef retVal, const char* szType, const char* szName, int EvFilterType)
      {
      RSASSERT(false);
//      init(szType, szName, evFilterType(EvFilterType));
      return RSL_STAT_OK;
      }

   TRsStat _init2(TValRef retVal, IRsObj* owner, const char* szName)
      {
      RSASSERT(false);
//      init(TRsPtrX<IContainer, IID_CONTAINER>(owner), szName);
      return RSL_STAT_OK;
      }

   TRsStat _addPage(TValRef retVal, IRsObj* pForm, const char* szName, long index, const char* szCaption)
      {
      retVal = addPage(TRsPtrX<IForm, IID_FORM>(pForm), szName, index, szCaption);
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<ITab> impl;
   };

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSTAB_H__
