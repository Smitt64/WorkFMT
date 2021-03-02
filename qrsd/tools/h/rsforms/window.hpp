/*-----------------------------------------------------------------------*-

 File Name   : rsforms\window.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 28, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSWINDOW_H__
#define __RSFORMSWINDOW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*-----------------------------------------------------------------------*/

class TExternalWindowImpl : public TEventHandlerImpl,
                            public IWindow
   {
public:
   TExternalWindowImpl(TAppObj* App) : TEventHandlerImpl(App)
      {}

   virtual ~TExternalWindowImpl() 
      {}

RSCOM_INTF_BEGIN2(TEventHandlerImpl)
   RSCOM_INTF(IID_WINDOW, IWindow)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_WINDOW, "CExtWindow", (void**)&impl);
      TEventHandlerImpl::initComponent(impl);
      }

public: // IWindow
   HWND RSCOMCALL getHandle()
      { 
      HWND handle = impl->getHandle();
      if (!handle)
         checkErrorInfo(app);
      return handle;
      }

RSCOM_INVOKE_BEGIN(TExternalWindowImpl, TEventHandlerImpl)
   RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_PROPGET(RTM_GETHANDLE, RsCracker(this, &TExternalWindowImpl::_getHandle))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TExternalWindowImpl, TEventHandlerImpl, TExternalWindow)
   RSCOM_PROPGETEX("handle", RTM_GETHANDLE, RSCOM_UINT, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _getHandle(TValRef retVal)
      {
      retVal = (unsigned int)(uintptr_t)getHandle();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IWindow> impl;

   };

DECLARE_CLASSEX(TExternalWindow, TExternalWindowImpl);

/*-----------------------------------------------------------------------*/

class TWindowImpl : public TCmdTargetImpl, 
                    public IFrameWindow, 
                    public ICmdSetContainer,
                    public IDesignedObject
   {
public:
   TWindowImpl(TAppObj* App) : TCmdTargetImpl(App), 
      systemMenu(App), userMenu(App), adminMenu(App)
      {}

   virtual ~TWindowImpl() 
      {}

RSCOM_INTF_BEGIN2(TCmdTargetImpl)
   RSCOM_INTF(IID_WINDOW, IWindow)
   RSCOM_INTF(IID_FRAME_WND, IFrameWindow)
   RSCOM_INTF(IID_CMDSETCONT, ICmdSetContainer)
   RSCOM_INTF(IID_DESIGNEDOBJ, IDesignedObject)
RSCOM_INTF_END

protected:
   virtual TRsStat RSCOMCALL finalConstruct(const char* namePtr, IModuleObj* pMod, void* data)
      {
      TRsStat stat = TCmdTargetImpl::finalConstruct(namePtr, pMod, data);
      systemMenu.init(this, "system");
      userMenu.init(this, "user");
      adminMenu.init(this, "admin");
      return stat;
      }

   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_FRAME_WND2, "CFrameWindow", (void**)&impl);

      if (!impl->getInterface(IID_CMDSETCONT, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support ICmdSetContainer interface");         
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      if (!impl->getInterface(IID_DESIGNEDOBJ, (void**)&impl3))
         {
         impl.Release();
         impl2.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support IDesignedObject interface");
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      TCmdTargetImpl::initComponent(impl);
      }

public:
   auto_cmdset<TCmdSet> systemMenu;
   auto_cmdset<TCmdSet> userMenu;
   auto_cmdset<TCmdSet> adminMenu;    

public: // IWindow
   HWND RSCOMCALL getHandle()
      { 
      HWND handle = impl->getHandle();
      if (!handle)
         checkErrorInfo(app);
      return handle;
      }

public: // IFrameWindow
   bool RSCOMCALL create(const char* szName, unsigned long dwStyle, IWindow* pOwner = NULL) throw(XRSFormsError)
      { return impl->create(szName, dwStyle, pOwner) ? true : ( checkErrorInfo(app), false ); }

   bool RSCOMCALL create_ex(const char* szName, IFrameView* pView, IWindow* pOwner = NULL) throw(XRSFormsError)
      { return impl->create_ex(szName, pView, pOwner) ? true : ( checkErrorInfo(app), false ); }

   bool RSCOMCALL destroy()
      { return impl->destroy() ? true : ( throw(XRSFormsError(app)), false ); }

   bool RSCOMCALL show(ShowWindowFlags nCmdShow, ModalType nModal = MT_NONE)
      { return impl->show(nCmdShow, nModal) ? true : ( throw(XRSFormsError(app)), false ); }

   int RSCOMCALL showModal(ModalType nModal = MT_APPLICATION)
      { 
      int iRetCode;
      return impl->showModalImpl(nModal, &iRetCode) ? iRetCode : ( throw(XRSFormsError(app)), 0 ); 
      }

   bool RSCOMCALL close(int exitCode)
      { return impl->close(exitCode) ? true : ( throw(XRSFormsError(app)), false ); }

   bool RSCOMCALL openView(IFrameView* pView, bool bActivate = true) throw(XRSFormsError)
      { return impl->openView(pView, bActivate) ? true : ( checkErrorInfo(app), false ); }

   bool RSCOMCALL openDockView (IFrameView* pView, 
      unsigned long dockType, bool bHideOnClose = false, unsigned short accelFlag = 0, unsigned short accelKey = 0, 
      bool bActivate = true) throw(XRSFormsError)
      { 
      return impl->openDockView (pView, dockType, bHideOnClose, accelFlag, accelKey, bActivate); 
      }

   bool RSCOMCALL closeView(IFrameView* pView)
      {
      bool bClosed = impl->closeView(pView);
      return checkErrorInfo(app), bClosed;
      }

   bool RSCOMCALL showView(IFrameView* pView)
      { return impl->showView(pView); }

   bool RSCOMCALL remView(IFrameView* pView)
      { return impl->remView(pView); }

   IFrameView* RSCOMCALL getActiveView()
      { return impl->getActiveView(); }

   IFrameView* RSCOMCALL getView(const char* szName)
      { return impl->getView(szName); }

   const char* RSCOMCALL getName()
      { return impl->getName(); }
    
   bool RSCOMCALL isCreated()
      { return impl->isCreated(); }

   bool RSCOMCALL isVisible()
      { return impl->isVisible(); }

   bool RSCOMCALL setCmdFilter(const char* szFilter, const char* szExtParams)
      { return impl->setCmdFilter(szFilter, szExtParams); }

   //bool RSCOMCALL switchFrameMode(const char* szNewMode)
   //{ return impl->switchFrameMode(szNewMode); }

   bool RSCOMCALL centerWindow(IWindow* pAltOwner = NULL)
      { return impl->centerWindow(pAltOwner) ? true : ( throw(XRSFormsError(app)), false ); }

   bool RSCOMCALL moveWindow(int x, int y, int w, int h, bool bRedraw)
      { return impl->moveWindow(x, y, w, h, bRedraw) ? true : ( throw(XRSFormsError(app)), false ); }

   IRsObj* RSCOMCALL getStatusBar ()
      { 
      IRsObj* pBar = impl->getStatusBar ();
      return pBar ? pBar : ( throw(XRSFormsError(app)), (IRsObj*)NULL ); 
      }

   int RSCOMCALL getExitCode ()
      { return impl->getExitCode (); }

   bool RSCOMCALL setHelpInfo (const char* szFile, HelpType hn, const char* szKeyWord)
      { return impl->setHelpInfo (szFile, hn, szKeyWord) ? true : ( throw(XRSFormsError(app)), false ); }

   bool RSCOMCALL showStatusBar (bool bShow)
      { return impl->showStatusBar (bShow) ? true : ( throw(XRSFormsError(app)), false ); }

public: // ICmdSetContainer
   long RSCOMCALL cmdSetCount()
      { return impl2->cmdSetCount(); }

   bool RSCOMCALL addCmdSet(ICmdSet* pSet)
      { return impl2->addCmdSet(pSet); }

   bool RSCOMCALL remCmdSet(ICmdSet* pSet, bool bUpdate = true)
      { return impl2->remCmdSet(pSet, bUpdate); }

   void RSCOMCALL getCmdSet(long i, ICmdSet** ppSet)
      { impl2->getCmdSet(i, ppSet); }

   bool RSCOMCALL loadCmdSet(ICmdSet* pSet, bool bUpdate = true)
      { return impl2->loadCmdSet(pSet, bUpdate) ? true : ( throw(XRSFormsError(app)), false ); }

   bool RSCOMCALL findCmdSet(IDispObj* proxy, ICmdSet** ppSet)
      { return impl2->findCmdSet(proxy, ppSet); }

   bool RSCOMCALL findCommandByName(const char* szProgName, IDispObj** ppItem)
      { return impl2->findCommandByName(szProgName, ppItem); }

public: //IDesignedObject
   void RSCOMCALL setTemplate(const char* szLib, const char* szRes)
      { impl3->setTemplate(szLib, szRes); }
   const char* RSCOMCALL getLibName()
      { return impl3->getLibName(); }
   const char* RSCOMCALL getResName()
      { return impl3->getResName(); }

RSCOM_INVOKE_BEGIN(TWindowImpl, TCmdTargetImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_PROPGET(RTM_GETHANDLE, RsCracker(this, &TWindowImpl::_getHandle))
   RSCOM_DISP_METHOD(RTM_CREATE, RsCracker(this, &TWindowImpl::_create))
   RSCOM_DISP_METHOD(RTM_CREATE_EX, RsCracker(this, &TWindowImpl::_create_ex))
   RSCOM_DISP_METHOD(RTM_DESTROY, RsCracker(this, &TWindowImpl::_destroy))
   RSCOM_DISP_METHOD(RTM_SHOW, RsCracker(this, &TWindowImpl::_show))
   RSCOM_DISP_METHOD(RTM_SHOWMODAL, RsCracker(this, &TWindowImpl::_showModal))
   RSCOM_DISP_METHOD(RTM_CLOSE, RsCracker(this, &TWindowImpl::_close))
   RSCOM_DISP_METHOD(RTM_OPENVIEW, RsCracker(this, &TWindowImpl::_openView))
   RSCOM_DISP_METHOD(RTM_OPENDOCKVIEW, RsCracker(this, &TWindowImpl::_openDockView))
   RSCOM_DISP_METHOD(RTM_CLOSEVIEW, RsCracker(this, &TWindowImpl::_closeView))
   RSCOM_DISP_METHOD(RTM_SHOWVIEW, RsCracker(this, &TWindowImpl::_showView))
   RSCOM_DISP_METHOD(RTM_GETACTIVEVIEW, RsCracker(this, &TWindowImpl::_getActiveView))
   RSCOM_DISP_METHOD(RTM_GETVIEW, RsCracker(this, &TWindowImpl::_getView))
   //RSCOM_DISP_METHOD(RTM_GETNAME, RsCracker(this, &TWindowImpl::_getName))
   RSCOM_DISP_PROPGET(RTM_GETNAME, RsCracker(this, &TWindowImpl::_getName))
   RSCOM_DISP_PROPGET(RTM_ISCREATED, RsCracker(this, &TWindowImpl::_isCreated))
   RSCOM_DISP_PROPGET(RTM_ISVISIBLE, RsCracker(this, &TWindowImpl::_isVisible))
   RSCOM_DISP_METHOD(RTM_CMDSETCOUNT, RsCracker(this, &TWindowImpl::_cmdSetCount))
   RSCOM_DISP_METHOD(RTM_GETCMDSET, RsCracker(this, &TWindowImpl::_getCmdSet))    
   RSCOM_DISP_METHOD(RTM_ADDCMDSET, RsCracker(this, &TWindowImpl::_addCmdSet))
   RSCOM_DISP_METHOD(RTM_REMCMDSET, RsCrackerDef(this, &TWindowImpl::_remCmdSet, true))
   RSCOM_DISP_METHOD(RTM_LOADCMDSET, RsCrackerDef(this, &TWindowImpl::_loadCmdSet, true))
   RSCOM_DISP_PROPGET(RTM_SYSMENU, RsCracker(this, &TWindowImpl::_getSystemMenu))
   RSCOM_DISP_PROPGET(RTM_USERMENU, RsCracker(this, &TWindowImpl::_getUserMenu))
   RSCOM_DISP_PROPGET(RTM_ADMINMENU, RsCracker(this, &TWindowImpl::_getAdminMenu))
   RSCOM_DISP_METHOD(RTM_SETCMDFILTER, RsCracker(this, &TWindowImpl::_setCmdFilter))
   //RSCOM_DISP_METHOD(RTM_SWITCHFRMMODE, RsCracker(this, &TWindowImpl::_switchFrameMode))
   RSCOM_DISP_METHOD(RTM_FINDCMDITEM, RsCracker(this, &TWindowImpl::_findCommandByName))
   RSCOM_DISP_METHOD(RTM_CENTERWND, RsCrackerDef(this, &TWindowImpl::_centerWindow, NullRsObj))
   RSCOM_DISP_METHOD(RTM_MOVE, RsCracker(this, &TWindowImpl::_moveWindow))
   RSCOM_DISP_METHOD(RTM_GETSTATUSBAR, RsCracker(this, &TWindowImpl::_getStatusBar))
   RSCOM_DISP_PROPGET(RTM_EXITCODE, RsCracker(this, &TWindowImpl::_getExitCode))
   RSCOM_DISP_METHOD(RTM_SETHELPINFO, RsCracker(this, &TWindowImpl::_setHelpInfo))
   RSCOM_DISP_METHOD(RTM_SETTEMPLATE, RsCracker(this, &TWindowImpl::_setTemplate))
   RSCOM_DISP_METHOD(RTM_GETLIBNAME, RsCracker(this, &TWindowImpl::_getLibName))
   RSCOM_DISP_METHOD(RTM_GETRESNAME, RsCracker(this, &TWindowImpl::_getResName))
   RSCOM_DISP_METHOD(RTM_SHOWSTATUSBAR, RsCracker(this, &TWindowImpl::_showStatusBar))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TWindowImpl, TCmdTargetImpl, TWindow)
   RSCOM_PROPGETEX("handle", RTM_GETHANDLE, RSCOM_UINT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("create", RTM_CREATE, RSCOM_BOOL, RV_STR RV_UI4 RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("create_ex", RTM_CREATE_EX, RSCOM_BOOL, RV_STR RV_OBJ RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("destroy", RTM_DESTROY, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("show", RTM_SHOW, RSCOM_BOOL, RV_I4 RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("showModal", RTM_SHOWMODAL, RSCOM_INT, RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("close", RTM_CLOSE, RSCOM_BOOL, RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("openView", RTM_OPENVIEW, RSCOM_BOOL, RV_OBJ RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("openDockView", RTM_OPENDOCKVIEW, RSCOM_BOOL, RV_OBJ RV_I4 RV_BOOL RV_I2 RV_I2 RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("closeView", RTM_CLOSEVIEW, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("showView", RTM_SHOWVIEW, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getActiveView", RTM_GETACTIVEVIEW, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getView", RTM_GETVIEW, RSCOM_OBJ, RV_STR, LEVEL_RSFRUNTIME)
   //RSCOM_METHODEX("getName", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("name", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("created", RTM_ISCREATED, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("visible", RTM_ISVISIBLE, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("cmdSetCount", RTM_CMDSETCOUNT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getCmdSet", RTM_GETCMDSET, RSCOM_OBJ, RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("addCmdSet", RTM_ADDCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("remCmdSet", RTM_REMCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("loadCmdSet", RTM_LOADCMDSET, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("systemMenu", RTM_SYSMENU, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("userMenu", RTM_USERMENU, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("adminMenu", RTM_ADMINMENU, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setCmdFilter", RTM_SETCMDFILTER, RSCOM_BOOL, RV_STR RV_STR, LEVEL_RSFRUNTIME)
   //RSCOM_METHODEX("switchFrameMode", RTM_SWITCHFRMMODE, RSCOM_BOOL, RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("findCommandByName", RTM_FINDCMDITEM, RSCOM_BOOL, RV_STR RVO_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("centerWindow", RTM_CENTERWND, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("moveWindow", RTM_MOVE, RSCOM_BOOL, RV_I4 RV_I4 RV_I4 RV_I4 RV_BOOL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getStatusBar", RTM_GETSTATUSBAR, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("exitCode", RTM_EXITCODE, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setHelpInfo", RTM_SETHELPINFO, RSCOM_BOOL, RV_STR RV_I4 RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("setTemplate", RTM_SETTEMPLATE, NULL, RV_STR RV_STR, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getLibName", RTM_GETLIBNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getResName", RTM_GETRESNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("showStatusBar", RTM_SHOWSTATUSBAR, RSCOM_BOOL, RV_BOOL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _getHandle(TValRef retVal)
      {
      retVal = (unsigned int)(uintptr_t)getHandle();
      return RSL_STAT_OK;
      }

   TRsStat _create(TValRef retVal, const char* szName, unsigned long dwStyle, IRsObj* pOwner)
      {
      retVal = create(szName, dwStyle, TRsPtrX<IWindow, IID_WINDOW>(pOwner));
      return RSL_STAT_OK;
      }

   TRsStat _create_ex(TValRef retVal, const char* szName, IRsObj* pView, IRsObj* pOwner)
      {
      retVal = create_ex(szName, TRsPtrX<IFrameView, IID_FRAME_VIEW>(pView), TRsPtrX<IWindow, IID_WINDOW>(pOwner));
      return RSL_STAT_OK;
      }

   TRsStat _destroy(TValRef retVal)
      {
      retVal = destroy();
      return RSL_STAT_OK;
      }

   TRsStat _show(TValRef retVal, int nCmdShow, int nModalType)
      {
      retVal = show(ShowWindowFlags(nCmdShow), ModalType(nModalType));
      return RSL_STAT_OK;
      }

   TRsStat _showModal(TValRef retVal, int nModalType)
      {
      retVal = showModal(ModalType(nModalType));
      return RSL_STAT_OK;
      }

   TRsStat _close(TValRef retVal, int iRetCode)
      {
      retVal = close(iRetCode);
      return RSL_STAT_OK;
      }

   TRsStat _closeView(TValRef retVal, IRsObj* pView)
      { 
      retVal = closeView(TRsPtrX<IFrameView, IID_FRAME_VIEW>(pView)); 
      return RSL_STAT_OK;
      }

   TRsStat _openView(TValRef retVal, IRsObj* pView, bool bActivate)
      { 
      retVal = openView(TRsPtrX<IFrameView, IID_FRAME_VIEW>(pView), bActivate);
      return RSL_STAT_OK;
      }

   TRsStat _openDockView(TValRef retVal, IRsObj* pView, unsigned long dockType, bool bHideOnClose, unsigned short accelFlag, unsigned short accelKey, bool bActivate)
      { 
      retVal = openDockView(TRsPtrX<IFrameView, IID_FRAME_VIEW>(pView), dockType, bHideOnClose, accelFlag, accelKey, bActivate);
      return RSL_STAT_OK;
      }

   TRsStat _showView(TValRef retVal, IRsObj* pView)
      {
      retVal = showView(TRsPtrX<IFrameView, IID_FRAME_VIEW>(pView));
      return RSL_STAT_OK;
      }

   TRsStat _getActiveView(TValRef retVal)
      {
      retVal = TRsPtrX<IDispObj, IID_RSDISP>(getActiveView());
      return RSL_STAT_OK;
      }

   TRsStat _getView(TValRef retVal, const char* szName)
      {
      retVal = TRsPtrX<IDispObj, IID_RSDISP>(getView(szName));
      return RSL_STAT_OK;
      }

   TRsStat _getName(TValRef retVal)
      {
      retVal = getName();
      return RSL_STAT_OK;
      }

   TRsStat _isCreated(TValRef retVal)
      {
      retVal = isCreated();
      return RSL_STAT_OK;
      }

   TRsStat _isVisible(TValRef retVal)
      {
      retVal = isVisible();
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

   TRsStat _remCmdSet(TValRef retVal, IRsObj* pSet, bool bUpdate)
      { 
      remCmdSet(TRsPtrX<ICmdSet, IID_CMDSET>(pSet), bUpdate); 
      return RSL_STAT_OK;
      }

   TRsStat _loadCmdSet(TValRef retVal, IRsObj* pSet, bool bUpdate)
      {
      retVal = loadCmdSet(TRsPtrX<ICmdSet, IID_CMDSET>(pSet), bUpdate);
      return RSL_STAT_OK;
      }

   TRsStat _getSystemMenu(TValRef retVal)
      {
      retVal = systemMenu->rsObj();
      return RSL_STAT_OK;
      }

   TRsStat _getUserMenu(TValRef retVal)
      {
      retVal = userMenu->rsObj();
      return RSL_STAT_OK;
      }

   TRsStat _getAdminMenu(TValRef retVal)
      {
      retVal = adminMenu->rsObj();
      return RSL_STAT_OK;
      }

   TRsStat _setCmdFilter(TValRef retVal, const char* szFilter, const char* szExtParams)
      {
      retVal = setCmdFilter(szFilter, szExtParams);
      return RSL_STAT_OK;
      }

   TRsStat _findCommandByName(TValRef retVal, const char* szProgName, TObjRefX<IDispObj, IID_RSDISP> ppItem)
      {
      TRsPtr<IDispObj> p = NULL;
      retVal = findCommandByName(szProgName, &p);
      ppItem = p;
      return RSL_STAT_OK;
      }

   TRsStat _centerWindow(TValRef retVal, IRsObj* pAltOwner)
      {
      retVal = centerWindow(TRsPtrX<IWindow, IID_WINDOW>(pAltOwner));
      return RSL_STAT_OK;
      }

   TRsStat _moveWindow(TValRef retVal, int x, int y, int w, int h, bool bRedraw)
      {
      retVal = moveWindow(x, y, w, h, bRedraw);
      return RSL_STAT_OK;
      }

   TRsStat _getStatusBar (TValRef retVal)
      {
      retVal = getStatusBar ();
      return RSL_STAT_OK;
      }

   TRsStat _getExitCode (TValRef retVal)
      {
      retVal = getExitCode ();
      return RSL_STAT_OK;
      }

   TRsStat _setHelpInfo (TValRef retVal, const char* szFile, int hn, const char* szKeyWord)
      {
      retVal = setHelpInfo (szFile, (HelpType)hn, szKeyWord);
      return RSL_STAT_OK;
      }

   TRsStat _showStatusBar (TValRef retVal, bool bShow)
      {
      retVal = showStatusBar (bShow);
      return RSL_STAT_OK;
      }

   TRsStat _setTemplate (TValRef retVal, const char* szLib, const char* szRes)
      {
      setTemplate (szLib, szRes);
      return RSL_STAT_OK;
      }

   TRsStat _getLibName (TValRef retVal)
      {
      retVal = getLibName ();
      return RSL_STAT_OK;
      }

   TRsStat _getResName(TValRef retVal)
      {
      retVal = getResName();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IFrameWindow2> impl;
   TRsPtr<ICmdSetContainer> impl2;
   TRsPtr<IDesignedObject> impl3;
};

DECLARE_CLASSEX(TWindow, TWindowImpl);

/*-----------------------------------------------------------------------*/

class TReportWindowImpl : public TWindowImpl
   {
public:
   TReportWindowImpl(TAppObj* App) : TWindowImpl(App)
      {}

   virtual ~TReportWindowImpl() 
      {}

   RSCOM_INTF_BEGIN2(TWindowImpl)
   RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_FRAME_WND2, "CReportWindow", (void**)&impl);
      TWindowImpl::initComponent(impl);
      }

protected:
   TRsPtr<IFrameWindow2> impl;
   };

DECLARE_CLASSEX(TReportWindow, TReportWindowImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSWINDOW_H__
