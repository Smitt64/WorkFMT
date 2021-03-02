/*-----------------------------------------------------------------------*-

 File Name   : rsforms\container.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Jun 21, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSCONTAINER_H__
#define __RSFORMSCONTAINER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TContainerImpl : public TControlImpl, public IContainer
   {
public:
   TContainerImpl(TAppObj* app) : TControlImpl(app)
      {}
    
   virtual ~TContainerImpl() 
      {}

RSCOM_INTF_BEGIN2(TControlImpl)
   RSCOM_INTF(IID_CONTAINER, IContainer)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_CONTAINER, "CContainer", (void**)&impl);
      TControlImpl::initComponent(impl);
      }

public: //IContainer
   long RSCOMCALL count()
      { return impl->count(); }
   bool RSCOMCALL addItem(IControlLight* pControl)
      { return impl->addItem(pControl); }
   bool RSCOMCALL remItem(IControlLight* pControl)
      { return impl->remItem(pControl); }
   void RSCOMCALL item(long i, IControlLight** ppCtrl)
      { impl->item(i, ppCtrl); }
   bool RSCOMCALL findItem(IDispObj* proxy, IControlLight** ppCtrl)
      { return impl->findItem(proxy, ppCtrl); }
   void RSCOMCALL clear()
      { impl->clear(); }

RSCOM_INVOKE_BEGIN(TContainerImpl,TControlImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_COUNT, RsCracker(this, &TContainerImpl::_count))
   RSCOM_DISP_METHOD(RTM_ADDITEM, RsCracker(this, &TContainerImpl::_addItem))
   RSCOM_DISP_METHOD(RTM_REMITEM, RsCracker(this, &TContainerImpl::_remItem))
   //RSCOM_DISP_METHOD(RTM_ITEM, RsCracker(this, &TContainerImpl::_item))
   RSCOM_DISP_PROPGET(RTM_ITEM, RsCracker(this, &TContainerImpl::_item))
   RSCOM_DISP_METHOD(RTM_FINDITEM, RsCracker(this, &TContainerImpl::_findItem))
RSCOM_INVOKE_END
    
RSCOM_TYPE_BEGIN_IMP(TContainerImpl,TControlImpl,TContainer)
   RSCOM_METHODEX("count", RTM_COUNT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("addItem", RTM_ADDITEM, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("remItem", RTM_REMITEM, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   //RSCOM_METHODEX("item", RTM_ITEM, NULL, RV_UI4 RVO_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_PROPEX("item", RTM_ITEM, RSCOM_OBJ, RV_UI4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("findItem", RTM_FINDITEM, RSCOM_BOOL, RV_OBJ RVO_OBJ, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _count(TValRef retVal)
      {
      retVal = count();
      return RSL_STAT_OK;
      }

   TRsStat _addItem(TValRef retVal, IRsObj* pControl)
      {
      retVal = addItem(TRsPtrX<IControlLight, IID_CONTROL>(pControl));
      return RSL_STAT_OK;
      }

   TRsStat _remItem(TValRef retVal, IRsObj* pControl)
      {
      retVal = remItem(TRsPtrX<IControlLight, IID_CONTROL>(pControl));
      return RSL_STAT_OK;
      }

   /*
   TRsStat _item(TValRef retVal, long i, TObjRefX<IControl, IID_CONTROL> ppCtrl)
      {
      IControl* pItem = NULL;
      item(i, &pItem);
      ppCtrl = pItem;
      return RSL_STAT_OK;
      }
   */

   TRsStat _item(TValRef retVal, long i)
      {
      TRsPtr<IControlLight> pItem = NULL;
      item(i, &pItem);
      if(pItem) retVal = pItem;
      return RSL_STAT_OK;
      }

   TRsStat _findItem(TValRef retVal, IRsObj* proxy, TObjRefX<IControlLight, IID_CONTROL_LIGHT> ppCtrl)
      {
      TRsPtr<IControlLight> pItem = NULL;
      retVal = findItem(TRsPtrX<IDispObj, IID_RSDISP>(proxy), &pItem);
      ppCtrl = pItem;
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IContainer> impl;
   };

DECLARE_CLASSEX(TContainer, TContainerImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSCONTAINER_H__
