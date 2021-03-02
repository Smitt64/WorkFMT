/*-----------------------------------------------------------------------*-

 File Name   : rsforms\infoview.hpp
 Programmer  : Leonid S. Pavlov

 Copyright (c) 1991 - 2005 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 24, 2005  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSINFOVIEW_H__
#define __RSFORMSINFOVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TInfoViewImpl : public TCmdTargetImpl, public IFrameView
{
public:
   TInfoViewImpl(TAppObj* app) : TCmdTargetImpl(app)
      {}

   virtual ~TInfoViewImpl() 
      {}

RSCOM_INTF_BEGIN2(TCmdTargetImpl)
   RSCOM_INTF(IID_FRAME_VIEW, IFrameView)
RSCOM_INTF_END

protected:
   virtual void initComponent(IRsObj* pObjImpl) throw(XRSFormsError)
      {
      checkImpl(pObjImpl, IID_FRAME_VIEW, "CInfoView", (void**)&impl);
      TCmdTargetImpl::initComponent(impl);
      }

public: // IFrameView
   bool RSCOMCALL create(IFrameWindow* pWndInTo, IWindow* pWndOwner, unsigned long dockType, unsigned long dockStyle, unsigned short accelFlag, unsigned short accelKey)
      { return impl->create(pWndInTo, pWndOwner, dockType, dockStyle, accelFlag, accelKey); }
   bool RSCOMCALL close(int exitCode)
      { return impl->close(exitCode); }
   IFrameWindow* RSCOMCALL getFrame()
      { return impl->getFrame(); }
   const char* RSCOMCALL getName()
      { return impl->getName(); }
   void RSCOMCALL lock()
      { impl->lock(); }
   void RSCOMCALL unlock()
      { impl->unlock(); }
   bool RSCOMCALL isVisible()
      { return impl->isVisible(); }

public: // C++ wrappers
   void setText(const char* text) throw(XRSFormsError)
      {
      TParmArray prm(app, 1, false, RSFM_CODEPAGE);
      prm[0] = text;
      TRsStat stat = invoke(1, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw(XRSFormsError(app, stat));
      }

   void addLine(const char* text) throw(XRSFormsError)
      {
      TParmArray prm(app, 1, false, RSFM_CODEPAGE);
      prm[0] = text;
      TRsStat stat = invoke(2, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw(XRSFormsError(app, stat));
      }

   void getText(TRsString& retVal) throw(XRSFormsError)
      {
      TParmArray prm(app, 0, true, RSFM_CODEPAGE);
      TRsStat stat = invoke(3, RS_DISP_METHOD, prm, RSCOM_AX_LEVEL);
      if (stat) throw(XRSFormsError(app, stat));
      retVal = TRsString (prm.retVal ());
      }

RSCOM_INVOKE_BEGIN(TInfoViewImpl, TCmdTargetImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
   RSCOM_DISP_METHOD(RTM_GETFRAME, RsCracker(this, &TInfoViewImpl::_getFrame))
   RSCOM_DISP_METHOD(RTM_GETNAME, RsCracker(this, &TInfoViewImpl::_getName))
   RSCOM_DISP_METHOD(RTM_CLOSE, RsCracker(this, &TInfoViewImpl::_close))
   RSCOM_DISP_PROPGET(RTM_ISVISIBLE, RsCracker(this, &TInfoViewImpl::_isVisible))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TInfoViewImpl, TCmdTargetImpl, TInfoView)
   RSCOM_METHODEX("getFrame", RTM_GETFRAME, RSCOM_OBJ, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("getName", RTM_GETNAME, RSCOM_STR, NULL, LEVEL_RSFRUNTIME)
   RSCOM_METHODEX("close", RTM_CLOSE, RSCOM_BOOL, RV_I4, LEVEL_RSFRUNTIME)
   RSCOM_PROPGETEX("visible", RTM_ISVISIBLE, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
   TRsStat _getFrame(TValRef retVal)
      { 
      retVal = getFrame();
      return RSL_STAT_OK; 
      }
   
   TRsStat _getName(TValRef retVal)
      { 
      retVal = getName();
      return RSL_STAT_OK; 
      }
   
   TRsStat _close(TValRef retVal, int iRetCode)
      {
      retVal = close(iRetCode);
      return RSL_STAT_OK;
      }

   TRsStat _isVisible(TValRef retVal)
      {
      retVal = isVisible();
      return RSL_STAT_OK;
      }

protected:
   TRsPtr<IFrameView> impl;
};

DECLARE_CLASSEX(TInfoView, TInfoViewImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSINFOVIEW_H__
