/*-----------------------------------------------------------------------*-

 File Name   : rsforms\control.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 28, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSCONTROL_H__
#define __RSFORMSCONTROL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TControlBaseImpl : public TCmdTargetImpl, 
                         public IControl, 
                         public IDesignedObject,
                         public ICmdSetContainer, 
                         public IClone
   {
public:
   TControlBaseImpl(TAppObj* app) : TCmdTargetImpl(app)
      {}

   virtual ~TControlBaseImpl() 
      {}

RSCOM_INTF_BEGIN2(TCmdTargetImpl)
   RSCOM_INTF(IID_CONTROL_LIGHT, IControlLight)
   RSCOM_INTF(IID_CONTROL, IControl)
   RSCOM_INTF(IID_DESIGNEDOBJ, IDesignedObject)
   RSCOM_INTF(IID_CMDSETCONT, ICmdSetContainer)
   RSCOM_INTF(IID_CLONE, IClone)
   case IID_FRAME_VIEW: *ptr = impl->getFrameView(); break;
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_CONTROL2, "CControl", (void**)&impl);

      if (!impl->getInterface(IID_CLONE, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support IClone interface");         
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      if (!impl->getInterface(IID_DESIGNEDOBJ, (void**)&impl3))
         {
         impl.Release();
         impl2.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support IDesignedObject interface");
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      if (!impl->getInterface(IID_CMDSETCONT, (void**)&impl4))
         {
         impl.Release();
         impl2.Release();
         impl3.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support ICmdSetContainer interface");         
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      TCmdTargetImpl::initComponent(impl);
      }

public: // IControl
   const char* RSCOMCALL getName()
      { return impl->getName(); }
   //const char* RSCOMCALL getFullName()
   //   { return impl->getFullName(); }
   const char* RSCOMCALL getControlType()
      { return impl->getControlType(); }
   evFilterType RSCOMCALL getEventFilterType()
      { return impl->getEventFilterType(); }
   IFrameWindow* RSCOMCALL getFrame(bool bRecursive = false)
      { return impl->getFrame(bRecursive); }
   IContainer* RSCOMCALL getContainer()
      {return impl->getContainer(); }
   int RSCOMCALL doModal(IWindow* pOwner = NULL, ModalType nModal = MT_THREAD)
      { return impl->doModal(pOwner, nModal); }
   bool RSCOMCALL showPopup(ShowWindowFlags nCmdShow = SW_SHOW, IWindow* pOwner = NULL, ModalType nModal = MT_NONE)
      { return impl->showPopup(nCmdShow, pOwner, nModal); }
   bool RSCOMCALL activate() throw(XRSFormsError)
      { return impl->activate() ? true : ( throw(XRSFormsError(app)), false ); } // exception?
   bool RSCOMCALL print(bool bPreview = true) throw(XRSFormsError)
      { 
      bool bRet = impl->print (bPreview);
      if (bRet && impl->isJobAborted ())
         return ( throw(XRSFormsError(app, RSL_STAT_CANCEL)), false ); 
      return bRet ? true : ( throw(XRSFormsError(app)), false ); 
      }
   bool RSCOMCALL isLoaded()
      { return impl->isLoaded(); }
   void RSCOMCALL setPrintCopies (long nCopies)
      { impl->setPrintCopies (nCopies); }

   void RSCOMCALL setName(const char* szName)
      { impl->setName(szName); }
   void RSCOMCALL setOwner(IContainer* pOwner)
      { impl->setOwner(pOwner); }
   //void RSCOMCALL alterPath(const char* szPath)
   //   { impl->alterPath(szPath); }

   bool RSCOMCALL setContextMenu (ICmdSet* pSet, SetContextMenuBehaviour scmb)
      { return impl->setContextMenu (pSet, scmb); }

public: // IClone
   IRsObj* RSCOMCALL getCloneData()
      { return impl2->getCloneData(); }

public: // IDesignedObject
   void RSCOMCALL setTemplate(const char* szLib, const char* szRes)
      { impl3->setTemplate(szLib, szRes); }
   const char* RSCOMCALL getLibName()
      { return impl3->getLibName(); }
   const char* RSCOMCALL getResName()
      { return impl3->getResName(); }

public: // ICmdSetContainer
   long RSCOMCALL cmdSetCount()
      { return impl4->cmdSetCount(); }
   bool RSCOMCALL addCmdSet(ICmdSet* pSet)
      { return impl4->addCmdSet(pSet); }
   bool RSCOMCALL remCmdSet(ICmdSet* pSet, bool bUpdate = true)
      { return impl4->remCmdSet(pSet, bUpdate); }
   void RSCOMCALL getCmdSet(long i, ICmdSet** ppSet)
      { impl4->getCmdSet(i, ppSet); }
   bool RSCOMCALL loadCmdSet(ICmdSet* pSet, bool bUpdate = true)
      { return impl4->loadCmdSet(pSet, bUpdate); }
   bool RSCOMCALL findCmdSet(IDispObj* proxy, ICmdSet** ppSet)
      { return impl4->findCmdSet(proxy, ppSet); }
   bool RSCOMCALL findCommandByName(const char* szProgName, IDispObj** ppItem)
      { return impl4->findCommandByName(szProgName, ppItem); }

RSCOM_INVOKE_BEGIN(TControlBaseImpl, TCmdTargetImpl)
RSCOM_INVOKE_LEVEL(RSCOM_SYS_LEVEL)
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_1, RsCracker(this, &TControlBaseImpl::_init1))
   RSCOM_DISP_METHOD(RTM_CONTROLINIT_2, RsCracker(this, &TControlBaseImpl::_init2))
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_PROPGET(RTM_NAME, RsCracker(this, &TControlBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_GETNAME, RsCracker(this, &TControlBaseImpl::_getName))
   RSCOM_DISP_METHOD(RTM_GETCONTROLTYPE, RsCracker(this, &TControlBaseImpl::_getControlType))
   RSCOM_DISP_METHOD(RTM_GETEVFILTERTYPE, RsCracker(this, &TControlBaseImpl::_getEventFilterType))
   RSCOM_DISP_METHOD(RTM_GETFRAME, RsCrackerDef(this, &TControlBaseImpl::_getFrame, false))
   RSCOM_DISP_METHOD(RTM_GETCONTAINER, RsCracker(this, &TControlBaseImpl::_getContainer))
   RSCOM_DISP_METHOD(RTM_DOMODAL, RsCrackerDef(this, &TControlBaseImpl::_doModal, NullRsObj, MT_APPLICATION))
   RSCOM_DISP_METHOD(RTM_SHOWPOPUP, RsCrackerDef(this, &TControlBaseImpl::_showPopup, SW_SHOW, NullRsObj, MT_NONE))
   RSCOM_DISP_METHOD(RTM_ACTIVATE, RsCracker(this, &TControlBaseImpl::_activate))
   RSCOM_DISP_METHOD(RTM_PRINT, RsCrackerDef(this, &TControlBaseImpl::_print, true))
   RSCOM_DISP_PROPGET(RTM_ISLOADED, RsCracker(this, &TControlBaseImpl::_isLoaded))
   RSCOM_DISP_METHOD(RTM_SETREPORTCOPIES, RsCracker(this, &TControlBaseImpl::_setPrintCopies))
   RSCOM_DISP_METHOD(RTM_SETCONTEXTMENU, RsCracker(this, &TControlBaseImpl::_setContextMenu))
   RSCOM_DISP_METHOD(RTM_SETTEMPLATE, RsCracker(this, &TControlBaseImpl::_setTemplate))
   RSCOM_DISP_METHOD(RTM_GETLIBNAME, RsCracker(this, &TControlBaseImpl::_getLibName))
   RSCOM_DISP_METHOD(RTM_GETRESNAME, RsCracker(this, &TControlBaseImpl::_getResName))
   RSCOM_DISP_METHOD(RTM_CMDSETCOUNT, RsCracker(this, &TControlBaseImpl::_cmdSetCount))
   RSCOM_DISP_METHOD(RTM_GETCMDSET, RsCracker(this, &TControlBaseImpl::_getCmdSet))    
   RSCOM_DISP_METHOD(RTM_ADDCMDSET, RsCracker(this, &TControlBaseImpl::_addCmdSet))
   RSCOM_DISP_METHOD(RTM_REMCMDSET, RsCracker(this, &TControlBaseImpl::_remCmdSet))
   RSCOM_DISP_METHOD(RTM_LOADCMDSET, RsCracker(this, &TControlBaseImpl::_loadCmdSet))
   RSCOM_DISP_METHOD(RTM_FINDCMDITEM, RsCracker(this, &TControlBaseImpl::_findCommandByName))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TControlBaseImpl, TCmdTargetImpl, TControlBase)
   RSCOM_PROPGETEX("name", RTM_NAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getName", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getControlType", RTM_GETCONTROLTYPE, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getEventFilterType", RTM_GETEVFILTERTYPE, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getFrame", RTM_GETFRAME, RSCOM_OBJ, RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getContainer", RTM_GETCONTAINER, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("doModal", RTM_DOMODAL, RSCOM_INT, RV_OBJ RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("showPopup", RTM_SHOWPOPUP, RSCOM_BOOL, RV_I4 RV_OBJ RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("activate", RTM_ACTIVATE, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("print", RTM_PRINT, RSCOM_BOOL, RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("loaded", RTM_ISLOADED, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setPrintCopies", RTM_SETREPORTCOPIES, RSCOM_NULL, RV_UI4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setContextMenu", RTM_SETCONTEXTMENU, RSCOM_BOOL, RV_OBJ RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setTemplate", RTM_SETTEMPLATE, NULL, RV_STR RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getLibName", RTM_GETLIBNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getResName", RTM_GETRESNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("cmdSetCount", RTM_CMDSETCOUNT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getCmdSet", RTM_GETCMDSET, RSCOM_OBJ, RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("addCmdSet", RTM_ADDCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("remCmdSet", RTM_REMCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("loadCmdSet", RTM_LOADCMDSET, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("findCommandByName", RTM_FINDCMDITEM, RSCOM_BOOL, RV_STR RVO_OBJ, LEVEL_RSFRUNTIME)
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

   TRsStat _getName(TValRef retVal)
      {
      retVal = getName();
      return RSL_STAT_OK;
      }

   //TRsStat _getFullName(TValRef retVal)
   //   {
   //   retVal = getFullName();
   //   return RSL_STAT_OK;
   //   }

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

   TRsStat _getFrame(TValRef retVal, bool bRecursive)
      {
      retVal = getFrame(bRecursive);
      return RSL_STAT_OK;
      }

   TRsStat _getContainer(TValRef retVal)
      {
      retVal = getContainer();
      return RSL_STAT_OK;
      }

   TRsStat _doModal(TValRef retVal, IRsObj* pOwner, int nModal)
      {
      retVal = doModal(TRsPtrX<IWindow, IID_WINDOW>(pOwner), ModalType(nModal));
      return RSL_STAT_OK;
      }

   TRsStat _showPopup(TValRef retVal, int nCmdShow, IRsObj* pOwner, int nModal)
      {
      retVal = showPopup(ShowWindowFlags(nCmdShow), TRsPtrX<IWindow, IID_WINDOW>(pOwner), ModalType(nModal));
      return RSL_STAT_OK;
      }

   TRsStat _activate(TValRef retVal)
      {
      retVal = activate();
      return RSL_STAT_OK;
      }

   TRsStat _print(TValRef retVal, bool bPreview)
      {
      retVal = print(bPreview);
      return RSL_STAT_OK;
      }

   TRsStat _isLoaded(TValRef retVal)
      {
      retVal = isLoaded();
      return RSL_STAT_OK;
      }

   TRsStat _setPrintCopies (TValRef retVal, long nCopies)
      {
      setPrintCopies (nCopies);
      return RSL_STAT_OK;
      }

   TRsStat _setContextMenu (TValRef retVal, IRsObj* pCmdSet, int scmb)
      {
      retVal = setContextMenu (TRsPtrX<ICmdSet, IID_CMDSET>(pCmdSet), SetContextMenuBehaviour (scmb));
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

   TRsStat _cmdSetCount(TValRef retVal)
      {
      retVal = cmdSetCount();
      return RSL_STAT_OK;
      }

   TRsStat _getCmdSet(TValRef retVal, long i)
      { 
      TRsPtr<ICmdSet> spSet;;
      getCmdSet(i, &spSet); 
      retVal = spSet;
      return RSL_STAT_OK;
      }

   TRsStat _addCmdSet(TValRef retVal, IRsObj* pSet)
      { 
      addCmdSet(TRsPtrX<ICmdSet, IID_CMDSET>(pSet)); 
      return RSL_STAT_OK;
      }

   TRsStat _remCmdSet(TValRef retVal, IRsObj* pSet)
      { 
      remCmdSet(TRsPtrX<ICmdSet, IID_CMDSET>(pSet)); 
      return RSL_STAT_OK;
      }

   TRsStat _loadCmdSet(TValRef retVal, IRsObj* pSet)
      {
      retVal = loadCmdSet(TRsPtrX<ICmdSet, IID_CMDSET>(pSet));
      return RSL_STAT_OK;
      }

   TRsStat _findCommandByName(TValRef retVal, const char* szProgName, TObjRefX<IDispObj, IID_RSDISP> ppItem)
      {
      TRsPtr<IDispObj> p = NULL;
      retVal = findCommandByName(szProgName, &p);
      ppItem = p;
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IControl2> impl;
   TRsPtr<IClone> impl2;
   TRsPtr<IDesignedObject> impl3;
   TRsPtr<ICmdSetContainer> impl4;
};

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSCONTROL_H__
