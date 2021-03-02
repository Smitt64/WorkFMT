/*-----------------------------------------------------------------------*-

 File Name   : RSForms.h
 Programmer  : Leonid S. Pavlov

 Sybsystem   : RSForms C++ library header
 Description : 

 Source      : 

 Library(s)  : 

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Feb 26, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMS_H__
#define __RSFORMS_H__

#if _MSC_VER > 1000
    #pragma once
#endif // _MSC_VER > 1000

#include "appintf.h"
#include "rscomerr.h"

#ifdef RSL_ANSI_STR
    #define  RSFM_CODEPAGE  RSL_STR_ANSI
#else
    #define  RSFM_CODEPAGE  RSL_STR_OEM
#endif

#include "rscom\cracker.hpp"
#include "rscom\cracker2.hpp"


#ifdef _MSC_VER
    #ifndef stricmp
        #define  stricmp(s1, s2)   _stricmp((s1), (s2))
    #endif
#endif


/*-----------------------------------------------------------------------*/

#pragma warning( push )
#pragma warning( disable: 4065 )  // switch statement contains 'default' but no 'case' labels
#pragma warning( disable: 4355 )  // 'this' : used in base member initializer list

/*-----------------------------------------------------------------------*/

namespace rsforms
{
 /*-----------------------------------------------------------------------*/

 template<class T>
 class TRsFormsObj : public TRsObjImplOwner<T>
     {
      protected:
        TRsFormsObj(TAppObj *appPtr) : TRsObjImplOwner<T>(appPtr)
        {
         internalAddRef();
        } 
           
        TRsStat RSCOMCALL finalConstruct(const char *namePtr, IModuleObj *pMod, void *data)
        {
         TRsStat  stat = TRsObjImpl<T>::finalConstruct(namePtr, pMod, data);

         internalRelease();

         return stat;
        }

      public:
        static TRsFormsObj<T> * construct(TAppObj *appPtr)
        {
         return new TRsFormsObj<T>(appPtr);
        }

        friend class TRsObjAutoBase<TRsFormsObj<T>>;
     };

 #define  DECLARE_CLASS(cls, impl)  typedef TRsObjImplOwner<impl> cls

 #define  DECLARE_CLASSEX(cls, impl)  typedef TRsFormsObj<impl> cls

 /*-----------------------------------------------------------------------*/

 #include "RSDlgAPI.h"

 #pragma warning( push )
 #pragma warning( disable : 4065 ) // switch statement contains 'default' but no 'case' labels

 #include "RSForms\constants.hpp"
 #include "RSForms\interfaces.hpp"
 #include "RSForms\mfunction.hpp"
 #include "RSForms\exception.hpp"
 #include "RSForms\creator.hpp"
 #include "RSForms\msgloop.hpp"

 #include "RSForms\hostobject.hpp"
 #include "RSForms\axobject.hpp"
 #include "RSForms\cmdset.hpp"

 #include "RSForms\cmdset2.hpp"
 #include "RSForms\infoview.hpp"
 #include "RSForms\window.hpp"
 #include "RSForms\control.hpp"
 #include "RSForms\control2.hpp"
 #include "RSForms\container.hpp"
 #include "RSForms\form.hpp"
 #include "RSForms\tab.hpp"
 #include "RSForms\roll.hpp"
 #include "RSForms\form2.hpp"
 #include "RSForms\report.hpp"
 #include "RSForms\rscontrols.hpp"
 #include "RSForms\binder.hpp"
 #include "RSForms\binder2.hpp"
 #include "RSForms\binder3.hpp"
 #include "RSForms\rsgrid.hpp"
 #include "RSForms\trayicon.hpp"

 #pragma warning( pop )

 /*-----------------------------------------------------------------------*/

 class TDynamicDispStubImpl : public TDispStubImpl
     {
      public:
        TDynamicDispStubImpl(TAppObj *app) throw(XRSFormsError) : TDispStubImpl(app)
        {
         // создание локального серверного объекта
         TRsStat  stat = fs_getServerObjectA(app, runtime_module, "CCppObj", NULL, IID_DYN_DISP, &impl, true, 0, NULL, RSDISP_DEFAULT);

         RTM_CHECK_1(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete CCppObj from %s.", runtime_module);

         if(stat)
           throw(XRSFormsError(app, stat));
        }

        virtual TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo *pInfo, int level)
        {
         return impl->invoke(id, cmdType, pInfo, level);
        }

        virtual TRsStat RSCOMCALL getId(const char *name, RSDISPID *id)
        {
         return impl->getId(name, id);
        }
        
        virtual void AddMemberFunction(RSDISPID id, int level, MemberFunction *ptr)
        {
         impl->AddMemberFunction(id, level, ptr);
        }

        virtual void RemMemberFunction(RSDISPID id, int level, MemberFunction *ptr)
        {
         impl->RemMemberFunction(id, level, ptr);
        }

      RSCOM_TYPE_BEGIN_IMP(TDynamicDispStubImpl,TDispStubImpl,TDynamicDispStub)
      RSCOM_TYPE_END

      protected:
        TRsPtr<IDynamicDispObj>  impl;
     };

 DECLARE_CLASS(TDynamicDispStub, TDynamicDispStubImpl);

 /*-----------------------------------------------------------------------*/

 _TOOLEXP bool RslMenuCommand(ICmdSet *pSender, unsigned long id, IDispObj *pFocusedObj, const char *parms);
 _TOOLEXP bool RslGetDsCommand(const char *parms, IRsObj **result);
 _TOOLEXP bool LoadAddIns();
 _TOOLEXP void UnloadAddIns();
 _TOOLEXP void AddInOnCreateWindow(IRsObj *wnd);
 _TOOLEXP void RsFmSyncWithTerminal(const char *appName);
 _TOOLEXP bool AddInOnQueryExitApp();

 class TRsFmMgr
     {
      public:
        static int DoneEx(TAppObj *app)
        {                      
         if(app)
          {
           TParmArray  prm(app, 0);

           return fs_getServerObjectA(app, runtime_module, NULL, RSMODULECLS, IID_RSDISP, NULL, true, 0, prm, RTM_REMALLFRAMES); 
          }

         return RSL_STAT_OK;
        }

        static int Done()
        {                      
         return DoneEx((TAppObj *)fs_getAppIntf());
        }
     };

 /*-----------------------------------------------------------------------*/

 class TWndManagerImpl : public TCmdTargetImpl, public ICmdSetContainer, public ISiteObj
     {
      public:
        TWndManagerImpl(TAppObj *app) : TCmdTargetImpl(app),
                                        OnGetDsEventPtr   (this, &TWndManagerImpl::OnGetDsEvent),
                                        OnAboutPtr        (this, &TWndManagerImpl::OnAbout),
                                        OnErrorPtr        (this, &TWndManagerImpl::OnError),
                                        OnCreateWndPtr    (this, &TWndManagerImpl::OnCreateWnd),
                                        OnQueryExitAppPtr (this, &TWndManagerImpl::OnQueryExitApp)
        {
        }

        virtual ~TWndManagerImpl()
        {
        }

        RSCOM_INTF_BEGIN2(TCmdTargetImpl)
           RSCOM_INTF(IID_CMDSETCONT, ICmdSetContainer)
        RSCOM_INTF_END

      public: // ISiteObj
        virtual int RSCOMCALL version() { return 9; }
        virtual const char *RSCOMCALL getAppName() { return appName; }
        virtual const char *RSCOMCALL getHlpName() { return hlpName; }

        virtual void RSCOMCALL clearAction(int flag) 
        { 
         if(m_spDefSiteObj && m_spDefSiteObj->version() > 1) 
           m_spDefSiteObj->clearAction(flag); 
        }

        virtual int RSCOMCALL testBreak(int useDialog) 
        { 
         return (m_spDefSiteObj && m_spDefSiteObj->version() > 1) ? m_spDefSiteObj->testBreak(useDialog) : 0; 
        }

        virtual const char *RSCOMCALL getLbrPath()  { return NULL; }   
        virtual HWND        RSCOMCALL getMainHWnd() { return NULL; }
        virtual void        RSCOMCALL setMainHWnd(HWND) {}

        virtual int RSCOMCALL HandleRsComError(TAppObj *app, TRsStat stat, unsigned long flags, unsigned long uCat)
        { 
         return ::HandleRsComErrorEx(app, stat, "Ошибка RSCOM", flags, true);
        }

        virtual void *RSCOMCALL getWndManager() { return NULL; }
        virtual void  RSCOMCALL setWndManager(void * mgr) {}

        virtual TSrvExtInf *RSCOMCALL getSrvExtIntf() { return NULL; }

        virtual const char *RSCOMCALL getProperty(const char *key)
        {
         if(m_spDefSiteObj && m_spDefSiteObj->version() >= 9)
           return m_spDefSiteObj->getProperty(key);

         return NULL;
        }

        virtual bool RSCOMCALL setProperty(const char *key, const char *value)
        {
         if(m_spDefSiteObj && m_spDefSiteObj->version() >= 9)
           return m_spDefSiteObj->setProperty(key, value);

         return NULL;
        }

        virtual PROPENUM RSCOMCALL enumProperty(PROPENUM id, TPropertyHolder *elem)
        {
         if(m_spDefSiteObj && m_spDefSiteObj->version() >= 9)
           return m_spDefSiteObj->enumProperty(id, elem);

         return NULL;
        }


      public: // ILifeTime
        virtual TRsStat RSCOMCALL finalConstruct(const char *namePtr, IModuleObj *pMod, void *data)
        {
         TParmArray  prm((IParmInfo *)data);


         if(prm[0].getType () == RSCOM_STR)
           appName = (const char *)prm[0];

         if(prm[1].getType () == RSCOM_STR)
           hlpName = (const char *)prm[1];

         InitRSForms();
         TRsStat  stat = TCmdTargetImpl::finalConstruct(namePtr, pMod, data);

         addHandler(EV_ON_CREATEDATASOURCE, RSCOM_EVENT_LEVEL, &OnGetDsEventPtr);
         addHandler(EV_ON_ERROR,            RSCOM_EVENT_LEVEL, &OnErrorPtr);
         addHandler(EV_ON_CREATE,           RSCOM_EVENT_LEVEL, &OnCreateWndPtr);
         addHandler(EV_ON_QUERY_EXIT,       RSCOM_EVENT_LEVEL, &OnQueryExitAppPtr);

         getCmdHandlerFor("system_app")->addHandler(999, RSCOM_EVENT_LEVEL, &OnAboutPtr);

         LoadAddIns();

         return stat;
        }

        virtual void RSCOMCALL finalRelease ()
        {
         TRsError  oldErr;
         TRsError  curErr(app);
         bool  needRestore = false;


         if(curErr.getCount() && fs_getServerObjectA(app, NULL, "$RSERR", NULL, IID_RSERRINFO, 
                                                     &oldErr, RSCOM_LOCAL, NULL, NULL, 1) == RSL_STAT_OK)
           needRestore = true;

         UnloadAddIns();

         remHandler(EV_ON_CREATEDATASOURCE, RSCOM_EVENT_LEVEL, &OnGetDsEventPtr);
         remHandler(EV_ON_ERROR,            RSCOM_EVENT_LEVEL, &OnErrorPtr);
         remHandler(EV_ON_CREATE,           RSCOM_EVENT_LEVEL, &OnCreateWndPtr);
         remHandler(EV_ON_QUERY_EXIT,       RSCOM_EVENT_LEVEL, &OnQueryExitAppPtr);

         getCmdHandlerFor("system_app")->remHandler(999, RSCOM_EVENT_LEVEL, &OnAboutPtr);

         TCmdTargetImpl::finalRelease();

         DoneRSForms();

         if(needRestore)
           curErr.loadFrom(oldErr);
        }

      public: // ICmdSetContainer
        long RSCOMCALL cmdSetCount()
        {
         return m_spCmdSetContImpl->cmdSetCount();
        }

        bool RSCOMCALL addCmdSet(ICmdSet *pSet)
        {
         return m_spCmdSetContImpl->addCmdSet(pSet);
        }

        bool RSCOMCALL remCmdSet(ICmdSet *pSet, bool bUpdate = true)
        {
         return m_spCmdSetContImpl->remCmdSet(pSet, bUpdate);
        }

        void RSCOMCALL getCmdSet(long i, ICmdSet **ppSet)
        {
         m_spCmdSetContImpl->getCmdSet(i, ppSet);
        }

        bool RSCOMCALL loadCmdSet(ICmdSet *pSet, bool bUpdate = true)
        {
         return m_spCmdSetContImpl->loadCmdSet(pSet, bUpdate);
        }

        bool RSCOMCALL findCmdSet(IDispObj *proxy, ICmdSet **ppSet)
        {
         return m_spCmdSetContImpl->findCmdSet(proxy, ppSet);
        }

        bool RSCOMCALL findCommandByName(const char *szProgName, IDispObj **ppItem)
        {
         return m_spCmdSetContImpl->findCommandByName(szProgName, ppItem);
        }

      public: // FormManager
        int Run (IFrameWindow *pMainWnd = NULL)
        {
         return RunImpl(TRsPtrX<IDispObj, IID_RSDISP>(pMainWnd));
        }

        int Run(IControl *pMainWnd)
        {
         return RunImpl(TRsPtrX<IDispObj, IID_RSDISP>(pMainWnd));
        }

        bool SetMainFrame(IFrameWindow *pMainWnd)
        {
         if(m_spSrvModule == NULL || pMainWnd == NULL)
            return false;

         TParmArray  prm(app, 1, true);


         prm[0] = pMainWnd;

         TRsStat  stat = m_spSrvModule->invoke(RTM_SETMAINFRAME, RS_DISP_METHOD, prm, RSCOM_SYS_LEVEL);

         return (stat == RSL_STAT_OK && (bool)prm.retVal() == true);
        }

        bool GetMainFrame(IFrameWindow **ppFrm)
        {
         if(m_spSrvModule == NULL || ppFrm == NULL || (*ppFrm) != NULL)
            return false;

         TParmArray  prm(app, 0, true);
         TRsStat  stat = m_spSrvModule->invoke(RTM_GETMAINFRAME, RS_DISP_METHOD, prm, RSCOM_SYS_LEVEL);

         if(stat == RSL_STAT_OK) 
          {
           (*ppFrm) = TRsPtrX<IFrameWindow, IID_FRAME_WND>(prm.retVal());

           if(*ppFrm)
             (*ppFrm)->addRef();
          }

         return stat == RSL_STAT_OK;
        }

        bool GetFrame(const char *szName, IFrameWindow **ppFrm)
        {
         if(m_spSrvModule == NULL || ppFrm == NULL || (*ppFrm) != NULL)
            return false;

         TParmArray  prm(app, 1, true);

         prm[0] = szName;

         TRsStat  stat = m_spSrvModule->invoke(RTM_GETFRAME, RS_DISP_METHOD, prm, RSCOM_SYS_LEVEL);

         if(stat == RSL_STAT_OK)
          {
           (*ppFrm) = TRsPtrX<IFrameWindow, IID_FRAME_WND>(prm.retVal());

           if(*ppFrm)
             (*ppFrm)->addRef();
          }

         return stat == RSL_STAT_OK;
        }
         
        bool EnterModalState()
        {
         if(m_spSrvModule == NULL)
           return false;

         TRsStat  stat = m_spSrvModule->invoke(RTM_ENTERMODAL, RS_DISP_METHOD, NULL, RSCOM_SYS_LEVEL);

         return stat == RSL_STAT_OK;
        }

        bool LeaveModalState()
        {
         if(m_spSrvModule == NULL)
           return false;

         TRsStat  stat = m_spSrvModule->invoke(RTM_LEAVEMODAL, RS_DISP_METHOD, NULL, RSCOM_SYS_LEVEL);    

         return stat == RSL_STAT_OK;
        }

      RSCOM_INVOKE_BEGIN(TWndManagerImpl, TCmdTargetImpl)
      RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
          RSCOM_DISP_METHOD(RTM_HANDLE_ERROR, RsCracker(this, &TWndManagerImpl::HandleError))
      RSCOM_INVOKE_END
         
      RSCOM_TYPE_BEGIN_IMP(TWndManagerImpl, TCmdTargetImpl, TWndManager)
          RSCOM_METHODEX("HandleError", RTM_HANDLE_ERROR, RSCOM_INT, RV_I4 RV_UI4 RV_UI4, LEVEL_RSFRUNTIME)
      RSCOM_TYPE_END

      protected:
        virtual TRsStat InitRSForms() 
        {
         if(fs_appVersionA(app) >= 12)
          {
           m_spDefSiteObj = fs_getSiteObjA(app);

           fs_setSiteObjA(app, this);
          }

         RsFmSyncWithTerminal(appName);

         TRsStat  stat = fs_getServerObjectA(app, runtime_module, RSMODULECLS, NULL, IID_RSDISP, &m_spSrvModule, true, 0, NULL, 0);

         if(stat == RSL_STAT_OK && m_spSrvModule)
          {
           TParmArray  prm(app, 1);

           prm[0] = static_cast<IStub *>(this);

           m_spSrvModule->invoke(RTM_SETUSEROBJ, RS_DISP_METHOD, prm, RSCOM_SYS_LEVEL);
           m_spSrvModule->getInterface(IID_CMDSETCONT, (void **)&m_spCmdSetContImpl);
          }

         return stat;
        }
         
        virtual void DoneRSForms()
        {             
         if(m_spSrvModule)
          {
           // Деинициализация TOOLS удалит застрявшие окна.
           //TParmArray prm_1 (app, 0);
           //m_spSrvModule->invoke (RTM_REMALLFRAMES, RS_DISP_METHOD, prm_1, RSCOM_SYS_LEVEL);

           TParmArray  prm_2(app, 1);

           prm_2[0] = NULL;

           m_spSrvModule->invoke(RTM_SETUSEROBJ, RS_DISP_METHOD, prm_2, RSCOM_SYS_LEVEL);

           m_spSrvModule.Release();
          }

         if(fs_appVersionA(app) >= 12)
          {
           fs_setSiteObjA(app, m_spDefSiteObj);
          }
        }
         
        virtual int RunImpl(IDispObj *pMainWnd)
        {
         int  iRet = -1;

         if(m_spSrvModule)
          {
           TParmArray  prm(app, 1, true);

           if(pMainWnd)
             prm[0] = pMainWnd;

           TRsStat  stat = m_spSrvModule->invoke(RTM_APPRUN, RS_DISP_METHOD, prm, RSCOM_SYS_LEVEL);

           if(stat == RSL_STAT_OK) 
             iRet = prm.retVal();
          }

         return iRet;
        }

        TRsStat HandleError(TValRef retVal, TRsStat stat, unsigned long flags, unsigned long uCat)
        {
         // Зачем нужна была эта функция?
         // Почему вызов не отправлен в Module?
         retVal = HandleRsComError(app, stat, flags, uCat);

         return RSL_STAT_OK;
        }

        bool RSCOMCALL onCommand(ICmdSet *pSender, unsigned long id, IDispObj *pFocusedObj, const char *parms, IDispObj *pCommandItem)
        {
         if(RslMenuCommand(pSender, id, pFocusedObj, parms))
           return true;

         return TCmdTargetImpl::onCommand(pSender, id, pFocusedObj, parms, pCommandItem);
        }

        MemberFunctionImpl<TWndManagerImpl>  OnGetDsEventPtr;

        void OnGetDsEvent(IParmInfo *pInfo)
        {
         TParmArray  prm(pInfo);
         TRsPtr<IRsObj>  result;

         if(RslGetDsCommand(prm[1], &result))
           prm[2] = result;
        }

        CmdHandlerImpl<TWndManagerImpl>  OnAboutPtr;

        void OnAbout(ICmdSet *pSender, unsigned long id, ICmdTarget *pFocusedObj, IFrameWindow *wnd, const char *parms, IDispObj *pCmdItem, bool &cancel)
        {
         TRsString  msg = appName;

         msg.append(" - приложение RS-Forms|Copyright (C) 2005 R-Style Softlab", NULL);

         RsMsgBox(wnd, msg, "О программе", RSMB_OK | RSMB_ICONINFORMATION, true);

         cancel = true;
        }

        MemberFunctionImpl<TWndManagerImpl>  OnErrorPtr;

        virtual void OnError(IParmInfo *pInfo)
        {
         TParmArray  prm(pInfo);

           //prm[3] = true; // подавляем показ асинхронных ошибок
         prm.retVal() = 0;
        }

        MemberFunctionImpl<TWndManagerImpl>  OnCreateWndPtr;

        virtual void OnCreateWnd(IParmInfo* pInfo)
        {
         TParmArray  prm(pInfo);

         TRsPtrX<IFrameWindow, IID_FRAME_WND>  spWnd(prm[1]);

         if(spWnd && stricmp(spWnd->getName(), "mainwnd") == 0)
           AddInOnCreateWindow(prm[1]);
        }

        MemberFunctionImpl<TWndManagerImpl>  OnQueryExitAppPtr;

        virtual void OnQueryExitApp(IParmInfo *pInfo)
        {
         TParmArray  prm(pInfo);

         prm.retVal() = AddInOnQueryExitApp();
        }


      protected:    
        TRsPtr<IDispObj>         m_spSrvModule;
        TRsPtr<ICmdSetContainer> m_spCmdSetContImpl;
        TRsPtr<ISiteObj>         m_spDefSiteObj;
        TRsString  appName;
        TRsString  hlpName;
     };

 DECLARE_CLASS(TWndManager, TWndManagerImpl);

 /*-----------------------------------------------------------------------*/

 class TPrintJob
     {
      public:
        TPrintJob(TAppObj *_app) : app(_app), aborted(false)
        {
         JobBegin(app);
        }

       ~TPrintJob ()
        {
         if(!aborted)
           JobEnd(app);
        }

        void abort()
        {
         JobAbort(app);

         aborted = true;
        }

      public:
        static void JobBegin(TAppObj *app)
        {
         TParmArray  prm(app, 0);

         fs_getServerObjectA(app, runtime_module, RSMODULECLS, NULL, IID_RSDISP, NULL, true, NULL, prm, RTM_JOB_BEGIN);
        }

        static void JobEnd(TAppObj *app)
        {
         TParmArray  prm(app, 0);

         fs_getServerObjectA(app, runtime_module, RSMODULECLS, NULL, IID_RSDISP, NULL, true, NULL, prm, RTM_JOB_END);
        }

        static void JobAbort(TAppObj *app)
        {
         TParmArray  prm(app, 0);

         fs_getServerObjectA(app, runtime_module, RSMODULECLS, NULL, IID_RSDISP, NULL, true, NULL, prm, RTM_JOB_ABORT);
        }

      private:
        TAppObj  *app;
        bool      aborted;
     };

 /*-----------------------------------------------------------------------*/

 typedef bool (RSCOMCALL *OnCommandType)(ICmdSet *pSender, unsigned long id, IDispObj *pFocusedObj, const char *parms);
 typedef TRsStat (RSCOMCALL *OnEventType)(RSDISPID id, IParmInfo *pInfo, int level);

 struct TSrvExtInf
      {
       int            version;
       OnCommandType  onCommand;
       OnEventType    onEvent;
      };
} // namespace rsforms

/*-----------------------------------------------------------------------*/

#pragma warning( pop )

/*-----------------------------------------------------------------------*/
#endif // __RSFORMS_H__

/* <-- EOF --> */