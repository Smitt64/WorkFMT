//-*---------------------------------------------------------------------*-
//
// File Name   : rsforms\form.hpp
// Programmer  : Leonid S. Pavlov
//
// Copyright (c) 1991 - 2002 by R-Style Software Lab.
// All Rights Reserved.
//
//-*---------------------------------------------------------------------*-

#ifndef __RSFORMSFORM_H__
#define __RSFORMSFORM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-*---------------------------------------------------------------------*-

class TFormBaseImpl : public TContainerImpl, 
                      public IForm, 
                      public ISlotContainer
   {
public:
   TFormBaseImpl(TAppObj* app) : TContainerImpl(app)
      {}

   ~TFormBaseImpl()
      {}

RSCOM_INTF_BEGIN2(TContainerImpl)
   RSCOM_INTF(IID_FORM, IForm)
   RSCOM_INTF(IID_SLOTCONT, ISlotContainer)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_FORM, "CForm", (void**)&impl);

      if (!impl->getInterface(IID_SLOTCONT, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support ISlotContainer interface");
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      TContainerImpl::initComponent(impl);
      }

public: // IForm
   bool RSCOMCALL addControl(IControl* pCtrl, const char* szName = 0, bool bVisible = true)
      { return impl->addControl(pCtrl, szName, bVisible); }
   bool RSCOMCALL addForm(IForm* pFrm, const char* szName = 0, bool bVisible = true)
      { return impl->addForm(pFrm, szName, bVisible); }
   void RSCOMCALL alterBaseName (const char* szName)
      { impl->alterBaseName(szName); }
   bool RSCOMCALL remControl (IControl* pCtrl)
      { return impl->remControl (pCtrl); }

public: // ISlotContainer
   bool RSCOMCALL bindSlot(IControl* pCtrl, const char* szName)
      { return impl2->bindSlot(pCtrl, szName); }
   long RSCOMCALL slotsCount()
      { return impl2->slotsCount(); }
   void RSCOMCALL getSlotInfo(long i, const char** pszName, IParmInfo** ppInf)
      { impl2->getSlotInfo(i, pszName, ppInf); }

RSCOM_INVOKE_BEGIN(TFormBaseImpl, TContainerImpl)
RSCOM_INVOKE_LEVEL(RSCOM_SYS_LEVEL)
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_1, RsCracker(this, &TFormBaseImpl::_init1))
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_2, RsCracker(this, &TFormBaseImpl::_init2))
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_ADDCONTROL, RsCracker(this, &TFormBaseImpl::_addControl))
   RSCOM_DISP_METHOD(RTM_ADDFORM, RsCracker(this, &TFormBaseImpl::_addForm))
   RSCOM_DISP_METHOD(RTM_ALTERBASE, RsCracker(this, &TFormBaseImpl::_alterBaseName))
   RSCOM_DISP_METHOD(RTM_BINDSLOT, RsCracker(this, &TFormBaseImpl::_bindSlot))
   RSCOM_DISP_METHOD(RTM_REMCONTROL, RsCracker(this, &TFormBaseImpl::_remControl))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TFormBaseImpl, TContainerImpl, TFormBase)
   RSCOM_METHODEX("addControl", RTM_ADDCONTROL, RSCOM_BOOL, RV_OBJ RV_STR RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("addForm", RTM_ADDFORM, RSCOM_BOOL, RV_OBJ RV_STR RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("alterBaseName", RTM_ALTERBASE, NULL, RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("bindSlot", RTM_BINDSLOT, RSCOM_BOOL, RV_OBJ RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("remControl", RTM_REMCONTROL, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
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

   TRsStat _addControl(TValRef retVal, IRsObj* pCtrl, const char* szName, bool bVisible)
      {
      retVal = addControl (TRsPtrX<IControl, IID_CONTROL>(pCtrl), szName, bVisible);
      return RSL_STAT_OK;
      }

   TRsStat _addForm(TValRef retVal, IRsObj* pForm, const char* szName, bool bVisible)
      {
      retVal = addForm (TRsPtrX<IForm, IID_FORM>(pForm), szName, bVisible);
      return RSL_STAT_OK;
      }

   TRsStat _alterBaseName(TValRef retVal, const char* szName)
      {
      alterBaseName (szName);
      return RSL_STAT_OK;
      }

   TRsStat _bindSlot(TValRef retVal, IRsObj* pObj, const char* szName)
      {
      retVal = bindSlot(TRsPtrX<IControl, IID_CONTROL>(pObj), szName);
      return RSL_STAT_OK;
      }

   TRsStat _remControl(TValRef retVal, IRsObj* pCtrl)
      {
      retVal = remControl (TRsPtrX<IControl, IID_CONTROL>(pCtrl));
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IForm> impl;
   TRsPtr<ISlotContainer> impl2;
   };

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSFORM_H__
