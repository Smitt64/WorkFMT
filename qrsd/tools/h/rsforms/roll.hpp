//-*---------------------------------------------------------------------*-
//
// File Name   : rsforms\roll.hpp
// Programmer  : Leonid S. Pavlov
//
// Copyright (c) 1991 - 2005 by R-Style Software Lab.
// All Rights Reserved.
//
//-*---------------------------------------------------------------------*-

#ifndef __RSFORMSROLL_H__
#define __RSFORMSROLL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-*---------------------------------------------------------------------*-

class TRollSectionBaseImpl : public TContainerImpl,
                             public IRollSection
   {
public:
   TRollSectionBaseImpl(TAppObj* appPtr) : TContainerImpl(appPtr) 
      {}

   virtual ~TRollSectionBaseImpl()
      {}

RSCOM_INTF_BEGIN2(TContainerImpl)
   RSCOM_INTF(IID_ROLLSECTION, IRollSection)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_ROLLSECTION, "CRollSection", (void**)&impl);
      TContainerImpl::initComponent(impl);
      }

public: // IRollSection
   bool RSCOMCALL addControl(IControl* pCtrl, const char* szName = 0)
      { return impl->addControl(pCtrl, szName); }

   bool RSCOMCALL addForm(IForm* pFrm, const char* szName = 0, IDispObj* pData = NULL)
      { return impl->addForm(pFrm, szName, pData); }

RSCOM_INVOKE_BEGIN(TRollSectionBaseImpl, TContainerImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_ADDCONTROL, RsCracker(this, &TRollSectionBaseImpl::_addControl))
   RSCOM_DISP_METHOD(RTM_ADDFORM, RsCrackerDef(this, &TRollSectionBaseImpl::_addForm, NullRsObj, ((const char*)NULL), NullRsObj))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TRollSectionBaseImpl, TContainerImpl, TRollSection)
   RSCOM_METHODEX("addControl", RTM_ADDCONTROL, RSCOM_BOOL, RV_OBJ RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("addForm", RTM_ADDFORM, RSCOM_BOOL, RV_OBJ RV_STR RV_OBJ, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _addControl(TValRef retVal, IRsObj* pCtrl, const char* szName)
      {
      retVal = addControl(TRsPtrX<IControl, IID_CONTROL>(pCtrl), szName);
      return RSL_STAT_OK;
      }

   TRsStat _addForm(TValRef retVal, IRsObj* pForm, const char* szName, IRsObj* pData)
      {
      retVal = addForm(TRsPtrX<IForm, IID_FORM>(pForm), szName, TRsPtrX<IDispObj, IID_RSDISP>(pData));
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IRollSection> impl;
   };

//-*---------------------------------------------------------------------*-

class TRollFormImpl : public TContainerImpl,
                      public IRollForm
   {
public:
   TRollFormImpl(TAppObj* appPtr) : TContainerImpl(appPtr) 
      {}

   virtual ~TRollFormImpl()
      {}

RSCOM_INTF_BEGIN2(TContainerImpl)
   RSCOM_INTF(IID_ROLLFORM, IRollForm)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_ROLLFORM, "CRollForm", (void**)&impl);
      TContainerImpl::initComponent(impl);
      }

public: // IRollForm
   bool RSCOMCALL addSection(IRollSection* pSect, const char* szName, long index, const char* szCaption)
      { return impl->addSection(pSect, szName, index, szCaption); }

   bool RSCOMCALL save (const char* szFileName)
      { return impl->save (szFileName); }

   void RSCOMCALL setAsyncMode ()
      { impl->setAsyncMode(); }

RSCOM_INVOKE_BEGIN(TRollFormImpl, TContainerImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_ADDFORM, RsCracker(this, &TRollFormImpl::_addSection))
   RSCOM_DISP_METHOD(RTM_SAVE, RsCracker(this, &TRollFormImpl::_save))
   RSCOM_DISP_METHOD(RTM_SETASYNCMODE, RsCracker(this, &TRollFormImpl::_setAsyncMode))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TRollFormImpl, TContainerImpl, TRollForm)
   RSCOM_METHODEX("addSection", RTM_ADDFORM, RSCOM_BOOL, RV_OBJ RV_STR RV_I4 RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("save", RTM_SAVE, RSCOM_BOOL, RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setAsyncMode", RTM_SETASYNCMODE, RSCOM_NULL, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _addSection(TValRef retVal, IRsObj* pForm, const char* szName, long index, const char* szCaption)
      {
      retVal = addSection(TRsPtrX<IRollSection, IID_ROLLSECTION>(pForm), szName, index, szCaption);
      return RSL_STAT_OK;
      }

   TRsStat _save (TValRef retVal, const char* szFileName)
      {
      retVal = save (szFileName);
      return RSL_STAT_OK;
      }

   TRsStat _setAsyncMode (TValRef retVal)
      {
      setAsyncMode ();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IRollForm> impl;
   };

DECLARE_CLASSEX(TRollForm, TRollFormImpl);

//-*---------------------------------------------------------------------*-
#endif // __RSFORMSROLL_H__