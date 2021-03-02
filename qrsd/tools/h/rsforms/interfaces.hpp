/*-----------------------------------------------------------------------*-

 File Name   : rsforms\interfaces.hpp
 Programmer  : Leonid S. Pavlov
 Description : RSForms C++ library header

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Feb 26, 2002  Leonid S. Pavlov (LSP) - Create file
 Jun 21, 2002  Nechaev Pavel D. (NPD) - Modify file

-*-----------------------------------------------------------------------*/

#ifndef __RSFSRV_I_H__
#define __RSFSRV_I_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef IDispObj MemberFunction;

/*-----------------------------------------------------------------------*/

struct IDynamicDispObj : public IDispObj
{
    virtual void RSCOMCALL AddMemberFunction(RSDISPID id, int level, MemberFunction* ptr) = 0;
    virtual void RSCOMCALL RemMemberFunction(RSDISPID id, int level, MemberFunction* ptr) = 0;
    virtual long RSCOMCALL FunctionCount() = 0;
    virtual bool RSCOMCALL GetInfoByIndex(int index, RSDISPID* pId, int* pLevel, MemberFunction** ppf) = 0;
};

/*---------------------------------------------------------------------------*/

struct IHostObject;
struct IHostObject2;
struct IEventHandler;
struct IEventHandler2;

struct IAxObject;
struct IDesignedObject;

struct ICmdSet;
struct ICmdSetContainer;
struct ICmdTarget;

struct IWindow;
struct IFrameWindow;
struct IFrameWindow2;
struct IFrameView;

struct IControl;
struct IControl2;
struct IContainer;
struct IForm;
struct ITab;

struct IRollForm;
struct IRollSection;

// устаревшие интерфейсы
struct IReportsGroup;
struct IReportsGroup2;
struct IReport;
struct IReport2;

/*-----------------------------------------------------------------------*/

IFrameWindow* const activeWnd = (IFrameWindow*)(intptr_t)(int_activeWnd);
IFrameWindow* const mainWnd = (IFrameWindow*)(intptr_t)(int_mainWnd);

/*---------------------------------------------------------------------------*/

struct IDispStubImpl : public IRsObj
{
    virtual TRsStat RSCOMCALL invokeBase(RSDISPID id, int cmdType, IParmInfo* pInfo, int level) = 0;
    virtual TRsStat RSCOMCALL raiseEventBase(RSDISPID id, int cmdType, IParmInfo* pInfo, int level) = 0;
};

/** \class IHostObject interfaces.hpp "interfaces.hpp"
    Базовый интерфейс RS-Forms.
    Описывает функции для работы с управляемым объектом.
*/
struct IHostObject : public IRsObj
{
    virtual void RSCOMCALL attach(IDispObj* pRemote) = 0;
    virtual void RSCOMCALL detach() = 0;
    virtual IDispObj* RSCOMCALL getControlObj() = 0;
    virtual IDispObj* RSCOMCALL getSink() = 0;
};

struct IHostObject2 : public IHostObject
{
    virtual bool RSCOMCALL init(IDispStubImpl* pThisCallback, IParmInfo* pParams) = 0;
    virtual void RSCOMCALL done() = 0;

    virtual TRsStat RSCOMCALL raiseEventEx(RSDISPID id, int cmdType, IParmInfo* pInfo, int level) = 0;
};

/**
    \class IEventHandler interfaces.hpp "interfaces.hpp"
    Интерфейс приёмника событий.
    Формализует способ приёма событий от управляемого объекта.
*/
struct IEventHandler : public IRsObj
{
    virtual TRsStat RSCOMCALL onEvent(RSDISPID id, IParmInfo* pInfo, int level) = 0;
};

/**
    Расширенный интерфейс приёмника событий.
    Позволяет добавлять функции - обработчики для конкретного вида событий.
*/
struct IEventHandler2 : public IEventHandler
{
    virtual void RSCOMCALL addHandler(RSDISPID id, int level, MemberFunction* ptr) = 0;
    virtual void RSCOMCALL remHandler(RSDISPID id, int level, MemberFunction* ptr) = 0;
    virtual long RSCOMCALL getHandlersCount() = 0;
    virtual bool RSCOMCALL getHandlerByIndex(int index, RSDISPID* pId, int* pLevel, MemberFunction** ppf) = 0;
};

struct IErrorHandler : public IRsObj
{
    virtual unsigned long RSCOMCALL onError(IRsErrorInfo* pErrorInfo, unsigned long uType) = 0;
};

/*---------------------------------------------------------------------------*/

struct IAxObject : public IRsObj
{
    virtual TRsStat RSCOMCALL CreateComObject(const char* szProgID, const char* szEvType, bool bUseActive, bool bRemote) = 0;
    virtual void    RSCOMCALL Unsubscribe() = 0;
};

/*---------------------------------------------------------------------------*/

/**
    \class IDesignedObject interfaces.hpp "interfaces.hpp"
    Интерфейс объекта, настроенного заранее в дизайнере
*/
struct IDesignedObject : public IRsObj
{
    virtual void RSCOMCALL setTemplate (const char* szLib, const char* szRes) = 0;
    virtual const char* RSCOMCALL getLibName () = 0;
    virtual const char* RSCOMCALL getResName () = 0;
};

/**
    \class IClone interfaces.hpp "interfaces.hpp"
    Интерфейс объекта - клона
*/
struct IClone : public IRsObj
{
    virtual IRsObj* RSCOMCALL getCloneData() = 0;
};

/*---------------------------------------------------------------------------*/

/**
    \class ICmdSet interfaces.hpp "interfaces.hpp"
    Интерфейс источника команд (представитель меню на сервере)
*/
struct ICmdSet : public IRsObj
{
    virtual bool RSCOMCALL setOwner(ICmdSetContainer* pOwner) = 0;
    virtual const char* RSCOMCALL getName() = 0;
    //virtual void RSCOMCALL alterPath(const char* szPath) = 0;
};

/*---------------------------------------------------------------------------*/

/**
    \class ICmdSetContainer interfaces.hpp "interfaces.hpp"
    Поддерживает коллекцию собственных источников команд.
*/
struct ICmdSetContainer : public IRsObj
{
    virtual long RSCOMCALL cmdSetCount() = 0;
    virtual bool RSCOMCALL addCmdSet(ICmdSet* pSet) = 0;
    virtual bool RSCOMCALL remCmdSet(ICmdSet* pSet, bool bUpdate = true) = 0;
    virtual void RSCOMCALL getCmdSet(long i, ICmdSet** ppSet) = 0;

    virtual bool RSCOMCALL loadCmdSet(ICmdSet* pSet, bool bUpdate = true) = 0;
    virtual bool RSCOMCALL findCmdSet(IDispObj* proxy, ICmdSet** ppSet) = 0;
    virtual bool RSCOMCALL findCommandByName(const char* szProgName, IDispObj** ppItem) = 0;
};

/*---------------------------------------------------------------------------*/

typedef IEventHandler2 ICmdHandler;

/**
    \class ICmdTarget interfaces.hpp "interfaces.hpp"
    Интерфейс объекта - приёмника командных сообщений.
    Поддерживает коллекцию собственных источников команд.
*/
struct ICmdTarget : public IRsObj
{
    virtual void RSCOMCALL setParentTarget(ICmdTarget* pParent) = 0;
    virtual ICmdTarget* RSCOMCALL getParentTarget() = 0;
    virtual ICmdHandler* RSCOMCALL getCmdHandlerFor(const char* szSetName) = 0;
    virtual bool RSCOMCALL onCommand(ICmdSet* pSender, unsigned long id, IDispObj* pFocusedObj, const char* szParams, IDispObj* pCommandItem) = 0;
    
    virtual ICmdHandler* RSCOMCALL getCmdHandler(ICmdSet* pSet) = 0;
};

/*---------------------------------------------------------------------------*/

struct IWindow : public IRsObj
{
    virtual HWND RSCOMCALL getHandle() = 0;
};

struct IFrameWindow : public IWindow
{
    virtual bool RSCOMCALL create(const char* szName, unsigned long dwStyle, IWindow* pOwner = NULL) = 0;
    virtual bool RSCOMCALL create_ex(const char* szName, IFrameView* pView, IWindow* pOwner = NULL) = 0;
    virtual bool RSCOMCALL destroy() = 0;
    virtual bool RSCOMCALL show(ShowWindowFlags nCmdShow, ModalType nModal = MT_NONE) = 0;
    virtual int  RSCOMCALL showModal(ModalType nModal = MT_THREAD) = 0;
    virtual bool RSCOMCALL close(int exitCode) = 0;
    
    virtual bool RSCOMCALL openView(IFrameView* pView, bool bActivate = true) = 0;
    virtual bool RSCOMCALL closeView(IFrameView* pView) = 0;
    virtual bool RSCOMCALL showView(IFrameView* pView) = 0;
    virtual bool RSCOMCALL remView(IFrameView* pView) = 0;

    virtual IFrameView* RSCOMCALL getActiveView() = 0;
    virtual IFrameView* RSCOMCALL getView(const char* szName) = 0;

    virtual const char* RSCOMCALL getName() = 0;
    virtual bool RSCOMCALL isCreated() = 0;
    virtual bool RSCOMCALL isVisible() = 0;
    
    virtual bool RSCOMCALL setCmdFilter(const char* szFilter, const char* szExtParams) = 0;
    //virtual bool RSCOMCALL switchFrameMode(const char* szNewMode) = 0;

    virtual bool RSCOMCALL centerWindow(IWindow* pAltOwner = NULL) = 0;
    virtual bool RSCOMCALL moveWindow(int x, int y, int w, int h, bool bRedraw) = 0;

    virtual IRsObj* RSCOMCALL getStatusBar() = 0;
    virtual int RSCOMCALL getExitCode() = 0;

    virtual bool RSCOMCALL setHelpInfo(const char* szFile, HelpType hn, const char* szKeyWord) = 0;

    virtual bool RSCOMCALL openDockView (IFrameView* pView, 
        unsigned long dockType, bool bHideOnClose = false, unsigned short accelFlag = 0, unsigned short accelKey = 0, 
        bool bActivate = true) = 0;

    virtual bool RSCOMCALL showStatusBar (bool bShow) = 0;
};

struct IFrameWindow2 : public IFrameWindow
{
    virtual bool RSCOMCALL showModalImpl(ModalType nModal, int* piRetCode) = 0;
};

struct ITrayIcon : public IWindow
{
   virtual bool RSCOMCALL show () = 0;
   virtual bool RSCOMCALL hide () = 0;
   virtual bool RSCOMCALL setIcon (const char* szResLib, const char* szResName, unsigned long dwIndex) = 0;
   virtual bool RSCOMCALL setTooltip (const char* szText) = 0;
};

/*---------------------------------------------------------------------------*/

struct IFrameView : public IRsObj
   {
   virtual bool RSCOMCALL create (IFrameWindow* pWndInTo, IWindow* pWndOwner, 
      unsigned long dockType = 0, unsigned long dockStyle = 0, unsigned short accelFlag = 0, unsigned short accelKey = 0) = 0;

   virtual bool RSCOMCALL close(int exitCode) = 0;
   virtual IFrameWindow* RSCOMCALL getFrame() = 0;
   virtual const char* RSCOMCALL getName() = 0;

   virtual void RSCOMCALL lock() = 0;
   virtual void RSCOMCALL unlock() = 0;

   virtual bool RSCOMCALL isVisible() = 0;
   };

/*---------------------------------------------------------------------------*/

struct IControlLight : public IRsObj
   {
   virtual const char*  RSCOMCALL getName() = 0;
   virtual const char*  RSCOMCALL getControlType() = 0;
   virtual evFilterType RSCOMCALL getEventFilterType() = 0;
   virtual IContainer*  RSCOMCALL getContainer() = 0;

   virtual bool RSCOMCALL isLoaded() = 0;
   virtual void RSCOMCALL setName(const char* szName) = 0;
   virtual void RSCOMCALL setOwner(IContainer* pOwner) = 0;
   };

struct IControl : public IControlLight
   {
   virtual IFrameWindow* RSCOMCALL getFrame(bool bRecursive = false) = 0;

   virtual int  RSCOMCALL doModal(IWindow* pOwner = NULL, ModalType nModal = MT_THREAD) = 0;
   virtual bool RSCOMCALL showPopup(ShowWindowFlags nCmdChow = SW_SHOW, IWindow* pOwner = NULL, ModalType nModal = MT_NONE) = 0;
   virtual bool RSCOMCALL activate() = 0;

   virtual bool RSCOMCALL print(bool bPreview = true) = 0;
   virtual void RSCOMCALL setPrintCopies (long nCopies) = 0;

   //virtual void RSCOMCALL alterPath(const char* szPath) = 0;
   virtual bool RSCOMCALL setContextMenu (ICmdSet* pSet, SetContextMenuBehaviour scmb) = 0;
   };

struct IControl2 : public IControl
{
    virtual IFrameView* RSCOMCALL getFrameView() = 0;
    virtual bool RSCOMCALL isJobAborted() = 0;
};

/*---------------------------------------------------------------------------*/

struct IContainer : public IRsObj
{
    virtual long RSCOMCALL count() = 0;
    virtual bool RSCOMCALL addItem(IControlLight* pControl) = 0;
    virtual bool RSCOMCALL remItem(IControlLight* pControl) = 0;
    virtual void RSCOMCALL item(long i, IControlLight** ppCtrl) = 0;
    virtual bool RSCOMCALL findItem(IDispObj* proxy, IControlLight** ppCtrl) = 0;
    virtual void RSCOMCALL clear() = 0;
};

struct ISlotContainer : public IRsObj
{
    virtual bool RSCOMCALL bindSlot (IControl* pCtrl, const char* szName) = 0;
    
    virtual long RSCOMCALL slotsCount () = 0;
    virtual void RSCOMCALL getSlotInfo (long i, const char** pszName, IParmInfo** ppInf) = 0;
};

/*---------------------------------------------------------------------------*/

struct IForm : public IRsObj
{
    virtual bool RSCOMCALL addControl (IControl* pCtrl, const char* szName = 0, bool bVisible = true) = 0;
    virtual bool RSCOMCALL addForm (IForm* pFrm, const char* szName = 0, bool bVisible = true) = 0;
    virtual void RSCOMCALL alterBaseName (const char* szName) = 0;
    
    virtual bool RSCOMCALL remControl (IControl* pCtrl) = 0;
};

struct ITab : public IRsObj
{
    virtual bool RSCOMCALL addPage (IForm* pFrm, const char* szName, long index, const char* szCaption) = 0;
};

struct IRollForm : public IRsObj
{
    virtual bool RSCOMCALL addSection (IRollSection* pFrm, const char* szName, long index, const char* szCaption) = 0;
    virtual bool RSCOMCALL save (const char* szFileName) = 0;

    virtual void RSCOMCALL setAsyncMode () = 0;
};

struct IRollSection : public IRsObj
{
    virtual bool RSCOMCALL addControl (IControl* pCtrl, const char* szName = 0) = 0;
    virtual bool RSCOMCALL addForm (IForm* pFrm, const char* szName = 0, IDispObj* pData = NULL) = 0;
};

/*---------------------------------------------------------------------------*/

struct IReportsGroup : public IRsObj
{
    virtual void RSCOMCALL RefUserObj(IRsObj* pObj) = 0;

    virtual void RSCOMCALL SetPreviewPagesLimit( long limit = -1 ) = 0;
    virtual bool RSCOMCALL Print(bool preview = false, printmode mode = PM_NORMAL, bool edit = false) = 0;    
    virtual void RSCOMCALL SetReportCopies( long copies = 1 ) = 0;
};

struct IReportsGroup2 : public IReportsGroup
{
    virtual bool RSCOMCALL init(IDispObj * pThis) = 0;
    virtual void RSCOMCALL deinit() = 0;
};

struct IReport : public IRsObj
{
    virtual void RSCOMCALL setTemplate(const char* szLib, const char* szRes) = 0;
    virtual bool RSCOMCALL putElement(const char * p_name, IHostObject* p_pObj = NULL, bool* p_aborted = NULL) = 0;
    virtual bool RSCOMCALL putElement(IControl* pCtl, IHostObject* p_pObj = NULL, bool* p_aborted = NULL) = 0;

    virtual bool RSCOMCALL putFormByRecordset(const char * pFormName, const char* szBindName, IRsObj* p_ds, bool* p_aborted = NULL) = 0;
    virtual bool RSCOMCALL putFormByRecordset(IForm* pForm, const char* szBindName, IRsObj* p_ds, bool* p_aborted = NULL) = 0;

    virtual void RSCOMCALL SetPreviewPagesLimit( long limit = -1 ) = 0;
    virtual bool RSCOMCALL Print(bool preview = false, printmode mode = PM_NORMAL, bool edit = false) = 0;
    virtual void RSCOMCALL SetReportCopies( long copies = 1 ) = 0;
};

struct IReport2 : public IReport
{
    virtual bool RSCOMCALL init(IReportsGroup* pRepsGroup, IRsObj* p_userRefObj, bool bOptimize = false) = 0;
    virtual void RSCOMCALL deinit() = 0;
    virtual void RSCOMCALL chkevtprm(IParmInfo* pInfo) = 0;

    virtual bool RSCOMCALL testElement(const char * p_name, long* p_outSize, bool* p_aborted = NULL) = 0;
    virtual bool RSCOMCALL testElement(IControl* pCtl, long* p_outSize, bool* p_aborted = NULL) = 0;

    virtual void RSCOMCALL construct (IDispObj* pThis, IEventHandler* pBaseHandler ) = 0;
};

/*---------------------------------------------------------------------------*/
#endif // __RSFSRV_I_H__
