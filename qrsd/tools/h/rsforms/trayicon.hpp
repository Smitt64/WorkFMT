/*-----------------------------------------------------------------------*-

File Name   : rsforms\trayicon.hpp
Programmer  : Leonid S. Pavlov

Copyright (c) 1991 - 2009 by R-Style Software Lab.
All Rights Reserved.

-*- History -------------------------------------------------------------*-
Mar 02, 2009  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSTRAYICON_H__
#define __RSFORMSTRAYICON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TTrayIconImpl : public TCmdTargetImpl, 
                      public ICmdSetContainer, 
                      public ITrayIcon
   {
public:
   TTrayIconImpl(TAppObj* app) : TCmdTargetImpl(app)
      {}

   virtual ~TTrayIconImpl() 
      {}

RSCOM_INTF_BEGIN2(TCmdTargetImpl)
   RSCOM_INTF(IID_WINDOW, IWindow)
   RSCOM_INTF(IID_TRAYICON, ITrayIcon)
   RSCOM_INTF(IID_CMDSETCONT, ICmdSetContainer)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_TRAYICON, "CTrayIcon", (void**)&impl);

      if (!impl->getInterface(IID_CMDSETCONT, (void**)&impl2))
         {
         impl.Release();
         RTM_ADDERROR(ERR_RTMINTF, LEVEL_RSFRUNTIME, "Component implementation doesn't support ICmdSetContainer interface");         
         throw XRSFormsError(app, RSL_STAT_EX);
         }

      TCmdTargetImpl::initComponent(impl);
      }

public: // IWindow
   HWND RSCOMCALL getHandle()
      { 
      HWND handle = impl->getHandle();
      if (!handle)
         checkErrorInfo(app);
      return handle;
      }

public: // ITrayIcon
   bool RSCOMCALL show ()
      { return impl->show(); }
   
   bool RSCOMCALL hide ()
      { return impl->hide(); }

   bool RSCOMCALL setIcon (const char* szResLib, const char* szResName, unsigned long dwIndex)
      { return impl->setIcon(szResLib, szResName, dwIndex); }

   bool RSCOMCALL setTooltip (const char* szText)
      { return impl->setTooltip(szText); }

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

   RSCOM_INVOKE_BEGIN(TTrayIconImpl, TCmdTargetImpl)
   RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
      RSCOM_DISP_PROPGET(RTM_GETHANDLE, RsCracker(this, &TTrayIconImpl::_getHandle))
      RSCOM_DISP_METHOD(RTM_ADDITEM, RsCracker(this, &TTrayIconImpl::_show))
      RSCOM_DISP_METHOD(RTM_REMITEM, RsCracker(this, &TTrayIconImpl::_hide))
      RSCOM_DISP_METHOD(RTM_SETICON, RsCracker(this, &TTrayIconImpl::_setIcon))
      RSCOM_DISP_METHOD(RTM_SETTEXT, RsCracker(this, &TTrayIconImpl::_setTooltip))

      RSCOM_DISP_METHOD(RTM_CMDSETCOUNT, RsCracker(this, &TTrayIconImpl::_cmdSetCount))
      RSCOM_DISP_METHOD(RTM_GETCMDSET, RsCracker(this, &TTrayIconImpl::_getCmdSet))    
      RSCOM_DISP_METHOD(RTM_ADDCMDSET, RsCracker(this, &TTrayIconImpl::_addCmdSet))
      RSCOM_DISP_METHOD(RTM_REMCMDSET, RsCrackerDef(this, &TTrayIconImpl::_remCmdSet, true))
      RSCOM_DISP_METHOD(RTM_LOADCMDSET, RsCrackerDef(this, &TTrayIconImpl::_loadCmdSet, true))
      RSCOM_DISP_METHOD(RTM_FINDCMDITEM, RsCracker(this, &TTrayIconImpl::_findCommandByName))
   RSCOM_INVOKE_END

   RSCOM_TYPE_BEGIN_IMP(TTrayIconImpl, TCmdTargetImpl, TTrayIcon)
      RSCOM_PROPGETEX("handle", RTM_GETHANDLE, RSCOM_UINT, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("show", RTM_ADDITEM, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("hide", RTM_REMITEM, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("setIcon", RTM_SETICON, RSCOM_BOOL, RV_STR RV_STR RV_UI4, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("setTooltip", RTM_SETTEXT, RSCOM_BOOL, RV_STR, LEVEL_RSFRUNTIME)

      RSCOM_METHODEX("cmdSetCount", RTM_CMDSETCOUNT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("getCmdSet", RTM_GETCMDSET, RSCOM_OBJ, RV_I4, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("addCmdSet", RTM_ADDCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("remCmdSet", RTM_REMCMDSET, NULL, RV_OBJ, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("loadCmdSet", RTM_LOADCMDSET, RSCOM_BOOL, RV_OBJ, LEVEL_RSFRUNTIME)
      RSCOM_METHODEX("findCommandByName", RTM_FINDCMDITEM, RSCOM_BOOL, RV_STR RVO_OBJ, LEVEL_RSFRUNTIME)
   RSCOM_TYPE_END

private:
   TRsStat _getHandle(TValRef retVal)
      {
      retVal = (unsigned int)(uintptr_t)getHandle();
      return RSL_STAT_OK;
      }

   TRsStat _show(TValRef retVal)
      {
      retVal = show ();
      return RSL_STAT_OK;
      }

   TRsStat _hide(TValRef retVal)
      {
      retVal = hide ();
      return RSL_STAT_OK;
      }

   TRsStat _setIcon(TValRef retVal,  const char* szResLib,  const char* szResName, unsigned long dwIndex)
      {
      retVal = setIcon (szResLib, szResName, dwIndex);
      return RSL_STAT_OK;
      }

   TRsStat _setTooltip(TValRef retVal, const char* szText)
      {
      retVal = setTooltip (szText);
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

   TRsStat _findCommandByName(TValRef retVal, const char* szProgName, TObjRefX<IDispObj, IID_RSDISP> ppItem)
      {
      TRsPtr<IDispObj> p = NULL;
      retVal = findCommandByName(szProgName, &p);
      ppItem = p;
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<ITrayIcon> impl;
   TRsPtr<ICmdSetContainer> impl2;
   };

DECLARE_CLASSEX(TTrayIcon, TTrayIconImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSTRAYICON_H__
