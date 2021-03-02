#ifndef __RSSRCEDIT_HPP
#define __RSSRCEDIT_HPP
/*************************************************************************
   File Name      : rssrcedit.hpp
   Programmer     : Alexey Lebedev


   Creation       : 18 Oct 2002
   Description    : RSSourceEditor class.

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __PLATFORM_H
#include "platform.h"
#endif // __PLATFORM_H

#ifdef RSL_PL_WIN32

#ifndef _INC_WINDOWS
#define STRICT
#include <windows.h>
#endif

// -----------------------------------------------------------------------------
#ifndef RSL_RSSED_DEFS
#define RSL_RSSED_DEFS

    #ifdef __cplusplus
        interface IRSMenu;
    #else
        struct IRSMenu;
    #endif

    #include "packpshn.h"

    typedef struct tagRSOpenFileData
          {
           int           iSize;
           const char   *szFileName;
           bool          bSaveAs;
           bool          bReadOnly;
           bool          bAlowMultiSelect;
          } RSOpenFileData;

    #include "packpop.h"

    typedef bool (RSOpenFileProc)(RSOpenFileData *, void *);

    typedef long (RSSed_NotifyProc)(unsigned, long, void *, void *);
    typedef bool (RSSed_CommandProc)(IRSMenu *, unsigned, void *);
    typedef bool (RSSed_UpdateStateProc)(IRSMenu *, unsigned, void *);

#endif // RSL_RSSED_DEFS


///////////////////////////////////////////////////////////////////////////////
// RSSourceEditor class

class RSSourceEditor
    {
     public:
       enum Commands
          {
           CmdFileNew          = 1001,
           CmdFileOpen,
           CmdFileClose,
           CmdFileCloseAll,
           CmdFileSave,
           CmdFileSaveAs,
           CmdFileSaveAll,
           CmdFileReload,
           CmdFileReloadAll,

           CmdEditUndo         = 2001,
           CmdEditRedo,
           CmdEditCut,
           CmdEditCopy,
           CmdEditPaste,
           CmdEditClear,
           CmdEditSelectAll,
           CmdToggleOemMode,
           CmdEditFind,
           CmdEditReplace,
           CmdEditRepeat,

           CmdFrameNew         = 3001,
           CmdFrameClose,
           CmdFrameNext,
           CmdFramePrevious,

           CmdViewOutput       = 4001,
           CmdToggleStatusBar,
           CmdToggleSwitchBar,

           CmdWindowClose      = 5001,
           CmdWindowCloseAll,
           CmdWindowNext,
           CmdWindowPrevious,

           CmdErrorNext        = 9001,
           CmdErrorPrevious,

           CmdAppExit          = 9999,
          };

       enum Messages
          {
           OnFileSaved   = 101,
           OnFileClosed,

           OnFrameOpened = 201,
           OnFrameClosed,
          };

     // Construction
     public:
       inline RSSourceEditor();

     // Operations
     public:
       inline bool InitInstance();
       inline void ExitInstance();

       inline void SetAppTitle(const char *szAppTitle);
       inline bool SetOemMode(bool bOemMode);

       inline void SetOpenFileProc(RSOpenFileProc *pProc, void *pParam);
       inline void SetNotifyProc(RSSed_NotifyProc *pProc, void *pParam);
       inline void SetCommandProc(RSSed_CommandProc *pProc, void *pParam);
       inline void SetUpdateStateProc(RSSed_UpdateStateProc *pProc, void *pParam);

       inline int  RunMsgLoop();
       inline void EndMsgLoop(int iRetCode);

       inline int  Run();

       inline bool LoadFile(const char *szFileName, bool bReadOnly = false);
       inline bool SaveAll();
       inline bool ReloadAll();

       inline void BringToFront();
       inline bool ViewOutput();
       inline const char* CurrentFileName();

       inline HANDLE NewFrame(const char *szFrameTitle = NULL);
       inline int  GetFrameCount();

       inline bool EnableAllFrames();
       inline bool DisableAllFrames();

       inline bool PreModal(HANDLE hFrame = NULL);
       inline bool PostModal(HANDLE hFrame = NULL);

       inline bool AddMenu(const char *szMenuName, IRSMenu *);
       inline bool DelMenu(IRSMenu *);
       inline bool GetMenu(const char *szMenuName, IRSMenu **);

       inline bool LoadMenu(const char *szLibName, const char *szResName, const char *szMenuName);
       inline bool LoadMenuFromModule(HINSTANCE hInstance, const char *szResName, const char *szMenuName);

       inline bool EnableMenuItem(const char *szMenuName, unsigned nCmdId, bool bEnabled);
       inline bool SetCheckMenuItem(const char *szMenuName, unsigned nCmdId, bool bChecked);

       inline bool AddError(const char *szFileName, int nLine, int nPosition, const char *szErrorText);
       inline bool RemoveAllErrors();
       inline bool GotoError();
       inline bool GotoNextError();
       inline bool GotoPreviousError();
       inline int  GetErrorCount();

       inline bool PreTranslateMessage(MSG *pMsg);
       inline bool OnIdle(int nIdleCount);

       inline char* GetTagLineFromFile (const char* fileName, const char* prefix);
       inline void  FreeTagLine (char* tag);

     // Implementation
     public:
       inline ~RSSourceEditor();

     private:
       inline bool GetProcAddr(const char *szNameProc, FARPROC *pProcAddr);
       inline bool LoadEditor(HANDLE *hEditor);
       inline void UnloadEditor(HANDLE hEditor);

     // Attributes
     private:
       HINSTANCE m_hInst;
       HANDLE m_hEditor;

       bool (*m_pLoadEditor) (HANDLE*);
       void (*m_pUnloadEditor) (HANDLE);

       bool (*m_pInitInstance) (HANDLE);
       void (*m_pExitInstance) (HANDLE);

       void (*m_pSetAppTitle) (HANDLE, const char* szAppTitle);
       bool (*m_pSetOemMode) (HANDLE, bool bOemMode);

       void (*m_pSetOpenFileProc) (HANDLE, RSOpenFileProc* pProc, void* pParam);
       void (*m_pSetNotifyProc) (HANDLE, RSSed_NotifyProc* pProc, void* pParam);
       void (*m_pSetCommandProc) (HANDLE, RSSed_CommandProc *pProc, void* pParam);
       void (*m_pSetUpdateStateProc) (HANDLE, RSSed_UpdateStateProc* pProc, void* pParam);

       int  (*m_pRunMsgLoop) (HANDLE);
       void (*m_pEndMsgLoop) (HANDLE, int iRetCode);

       int  (*m_pRun) (HANDLE);

       bool (*m_pLoadFile) (HANDLE, const char* szFileName, bool bReadOnly);
       bool (*m_pSaveAllFiles) (HANDLE);
       bool (*m_pReloadAllFiles) (HANDLE);

       void (*m_pBringToFront) (HANDLE);
       bool (*m_pViewOutput) (HANDLE);
       const char* (*m_pCurrentFileName) (HANDLE);

       bool (*m_pNewFrame) (HANDLE, const char* szFrameTitle, HANDLE*);
       int  (*m_pGetFrameCount) (HANDLE);

       bool (*m_pEnableAllFrames) (HANDLE);
       bool (*m_pDisableAllFrames) (HANDLE);

       bool (*m_pPreModal) (HANDLE, HANDLE);
       bool (*m_pPostModal) (HANDLE, HANDLE);

       bool (*m_pAddMenu) (HANDLE, const char* szMenuName, IRSMenu*);
       bool (*m_pDelMenu) (HANDLE, IRSMenu*);
       bool (*m_pGetMenu) (HANDLE, const char* szMenuName, IRSMenu**);

       bool (*m_pLoadMenu) (HANDLE, const char* szLibName, const char* szResName, const char* szMenuName);
       bool (*m_pLoadMenuFromModule) (HANDLE, HINSTANCE hInstance, const char* szResName, const char* szMenuName);

       bool (*m_pEnableMenuItem) (HANDLE, const char* szMenuName, unsigned nCmdId, bool bEnabled);
       bool (*m_pSetCheckMenuItem) (HANDLE, const char* szMenuName, unsigned nCmdId, bool bChecked);

       bool (*m_pAddError) (HANDLE, const char* szFileName, int nLine, int nPosition, const char* szErrorText);
       bool (*m_pRemoveAllErrors) (HANDLE);
       bool (*m_pGotoError) (HANDLE);
       bool (*m_pGotoNextError) (HANDLE);
       bool (*m_pGotoPreviousError) (HANDLE);
       int  (*m_pGetErrorCount) (HANDLE);

       bool (*m_pMsgFilter) (HANDLE, MSG* pMsg);
       bool (*m_pMsgIdle) (HANDLE, int nIdleCount);

       char* (*m_pGetTagLineFromFile) (HANDLE, const char* szFileName, const char* szPrefix);
       void  (*m_FreeTagLine) (HANDLE, char* tag);
    };

///////////////////////////////////////////////////////////////////////////////
inline RSSourceEditor::RSSourceEditor()
{
 m_hEditor = NULL;

 // Загружаем DLL
 m_hInst = ::LoadLibrary("SrcEdit.dll");

 if(!m_hInst)
   return;

 // Получаем адреса экспортируемых процедур
 if(!GetProcAddr("RSSed_LoadEditor",         (FARPROC *)&m_pLoadEditor)         ||
    !GetProcAddr("RSSed_UnloadEditor",       (FARPROC *)&m_pUnloadEditor)       ||

    !GetProcAddr("RSSed_SetAppTitle",        (FARPROC *)&m_pSetAppTitle)        ||
    !GetProcAddr("RSSed_SetOemMode",         (FARPROC *)&m_pSetOemMode)         ||

    !GetProcAddr("RSSed_SetOpenFileProc",    (FARPROC *)&m_pSetOpenFileProc)    ||
    !GetProcAddr("RSSed_SetNotifyProc",      (FARPROC *)&m_pSetNotifyProc)      ||
    !GetProcAddr("RSSed_SetCommandProc",     (FARPROC *)&m_pSetCommandProc)     ||
    !GetProcAddr("RSSed_SetUpdateStateProc", (FARPROC *)&m_pSetUpdateStateProc) ||

    !GetProcAddr("RSSed_InitInstance",       (FARPROC *)&m_pInitInstance)       ||
    !GetProcAddr("RSSed_ExitInstance",       (FARPROC *)&m_pExitInstance)       ||

    !GetProcAddr("RSSed_RunMsgLoop",         (FARPROC *)&m_pRunMsgLoop)         ||
    !GetProcAddr("RSSed_EndMsgLoop",         (FARPROC *)&m_pEndMsgLoop)         ||

    !GetProcAddr("RSSed_Run",                (FARPROC *)&m_pRun)                ||

    !GetProcAddr("RSSed_LoadFile",           (FARPROC *)&m_pLoadFile)           ||
    !GetProcAddr("RSSed_SaveAllFiles",       (FARPROC *)&m_pSaveAllFiles)       ||
    !GetProcAddr("RSSed_ReloadAllFiles",     (FARPROC *)&m_pReloadAllFiles)     ||

    !GetProcAddr("RSSed_BringToFront",       (FARPROC *)&m_pBringToFront)       ||
    !GetProcAddr("RSSed_ViewOutput",         (FARPROC *)&m_pViewOutput)         ||
    !GetProcAddr("RSSed_CurrentFileName",    (FARPROC *)&m_pCurrentFileName)    ||

    !GetProcAddr("RSSed_NewFrame",           (FARPROC *)&m_pNewFrame)           ||
    !GetProcAddr("RSSed_GetFrameCount",      (FARPROC *)&m_pGetFrameCount)      ||

    !GetProcAddr("RSSed_EnableAllFrames",    (FARPROC *)&m_pEnableAllFrames)    ||
    !GetProcAddr("RSSed_DisableAllFrames",   (FARPROC *)&m_pDisableAllFrames)   ||

    !GetProcAddr("RSSed_PreModal",           (FARPROC *)&m_pPreModal)           ||
    !GetProcAddr("RSSed_PostModal",          (FARPROC *)&m_pPostModal)          ||

    !GetProcAddr("RSSed_AddMenu",            (FARPROC *)&m_pAddMenu)            ||
    !GetProcAddr("RSSed_DelMenu",            (FARPROC *)&m_pDelMenu)            ||
    !GetProcAddr("RSSed_GetMenu",            (FARPROC *)&m_pGetMenu)            ||

    !GetProcAddr("RSSed_LoadMenu",           (FARPROC *)&m_pLoadMenu)           ||
    !GetProcAddr("RSSed_LoadMenuFromModule", (FARPROC *)&m_pLoadMenuFromModule) ||

    !GetProcAddr("RSSed_EnableMenuItem",     (FARPROC *)&m_pEnableMenuItem)     ||
    !GetProcAddr("RSSed_SetCheckMenuItem",   (FARPROC *)&m_pSetCheckMenuItem)   ||

    !GetProcAddr("RSSed_AddError",           (FARPROC *)&m_pAddError)           ||
    !GetProcAddr("RSSed_RemoveAllErrors",    (FARPROC *)&m_pRemoveAllErrors)    ||
    !GetProcAddr("RSSed_GotoError",          (FARPROC *)&m_pGotoError)          ||
    !GetProcAddr("RSSed_GotoNextError",      (FARPROC *)&m_pGotoNextError)      ||
    !GetProcAddr("RSSed_GotoPreviousError",  (FARPROC *)&m_pGotoPreviousError)  ||
    !GetProcAddr("RSSed_GetErrorCount",      (FARPROC *)&m_pGetErrorCount)      ||

    !GetProcAddr("RSSed_MsgFilter",          (FARPROC *)&m_pMsgFilter)          ||
    !GetProcAddr("RSSed_MsgIdle",            (FARPROC *)&m_pMsgIdle)            ||

    !LoadEditor(&m_hEditor))
  {
   ::FreeLibrary(m_hInst);
   m_hInst = NULL;
  }

 // Опциональные методы
 GetProcAddr("RSSed_GetTagLineFromFile", (FARPROC *)&m_pGetTagLineFromFile);

 if(!GetProcAddr("RSSed_FreeTagLine", (FARPROC *)&m_FreeTagLine))
   m_pGetTagLineFromFile = NULL;
}

// -----------------------------------------------------------------------------
inline RSSourceEditor::~RSSourceEditor()
{
 if(m_hEditor)
   UnloadEditor(m_hEditor);

 if(m_hInst)
   ::FreeLibrary(m_hInst);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::GetProcAddr(const char *szNameProc, FARPROC *pProcAddr)
{
 FARPROC  ProcAddr = ::GetProcAddress(m_hInst, szNameProc);

 if(!ProcAddr)
   return false;

 *pProcAddr = ProcAddr;

 return true;
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::LoadEditor(HANDLE *hEditor)
{
 if(!m_hInst)
   return false;

 return m_pLoadEditor(hEditor);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::UnloadEditor(HANDLE hEditor)
{
 if(!m_hInst)
   return;

 m_pUnloadEditor(hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::InitInstance()
{
 if(!m_hInst)
   return false;

 return m_pInitInstance(m_hEditor);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::ExitInstance()
{
 if(!m_hInst)
   return;

 m_pExitInstance(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline void RSSourceEditor::SetAppTitle(const char *szAppTitle)
{
 if(!m_hInst)
   return;

 m_pSetAppTitle(m_hEditor, szAppTitle);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::SetOemMode(bool bOemMode)
{
 if(!m_hInst)
   return false;

 return m_pSetOemMode(m_hEditor, bOemMode);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::SetOpenFileProc(RSOpenFileProc *pProc, void *pParam)
{
 if(!m_hInst)
   return;

 m_pSetOpenFileProc(m_hEditor, pProc, pParam);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::SetNotifyProc(RSSed_NotifyProc *pProc, void *pParam)
{
 if(!m_hInst)
   return;

 m_pSetNotifyProc(m_hEditor, pProc, pParam);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::SetCommandProc(RSSed_CommandProc *pProc, void *pParam)
{
 if(!m_hInst)
   return;

 m_pSetCommandProc(m_hEditor, pProc, pParam);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::SetUpdateStateProc(RSSed_UpdateStateProc *pProc, void *pParam)
{
 if(!m_hInst)
   return;

 m_pSetUpdateStateProc(m_hEditor, pProc, pParam);
}

///////////////////////////////////////////////////////////////////////////////
inline int RSSourceEditor::RunMsgLoop()
{
 if(!m_hInst)
   return -1;

 return m_pRunMsgLoop(m_hEditor);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::EndMsgLoop(int iRetCode)
{
 if(!m_hInst)
   return;

 m_pEndMsgLoop(m_hEditor, iRetCode);
}

// -----------------------------------------------------------------------------
inline int RSSourceEditor::Run()
{
 if(!m_hInst)
   return -1;

 return m_pRun(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::LoadFile(const char *szFileName, bool bReadOnly)
{
 if(!m_hInst)
   return false;

 return m_pLoadFile(m_hEditor, szFileName, bReadOnly);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::SaveAll()
{
 if(!m_hInst)
   return false;

 return m_pSaveAllFiles(m_hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::ReloadAll()
{
 if(!m_hInst)
   return false;

 return m_pReloadAllFiles(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline void RSSourceEditor::BringToFront()
{
 if(!m_hInst)
   return;

 m_pBringToFront(m_hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::ViewOutput()
{
 if(!m_hInst)
   return false;

 return m_pViewOutput(m_hEditor);
}

// -----------------------------------------------------------------------------
inline const char *RSSourceEditor::CurrentFileName()
{
 if(!m_hInst)
   return NULL;

 return m_pCurrentFileName(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline HANDLE RSSourceEditor::NewFrame(const char *szFrameTitle)
{
 if(!m_hInst)
   return NULL;

 HANDLE  hFrame;

 if(!m_pNewFrame(m_hEditor, szFrameTitle, &hFrame))
   return NULL;

 return hFrame;
}

// -----------------------------------------------------------------------------
inline int RSSourceEditor::GetFrameCount()
{
 if(!m_hInst)
   return -1;

 return m_pGetFrameCount(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::EnableAllFrames()
{
 if(!m_hInst)
   return false;

 return m_pEnableAllFrames(m_hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::DisableAllFrames()
{
 if(!m_hInst)
   return false;

 return m_pDisableAllFrames(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::PreModal(HANDLE hFrame)
{
 if(!m_hInst)
   return false;

 return m_pPreModal(m_hEditor, hFrame);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::PostModal(HANDLE hFrame)
{
 if(!m_hInst)
   return false;

 return m_pPostModal(m_hEditor, hFrame);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::AddMenu(const char *szMenuName, IRSMenu *pMenu)
{
 if(!m_hInst)
   return false;

 return m_pAddMenu(m_hEditor, szMenuName, pMenu);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::DelMenu(IRSMenu *pMenu)
{
 if(!m_hInst)
   return false;

 return m_pDelMenu(m_hEditor, pMenu);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::GetMenu(const char *szMenuName, IRSMenu **ppMenu)
{
 if(!m_hInst)
   return false;

 return m_pGetMenu(m_hEditor, szMenuName, ppMenu);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::LoadMenu(const char *szLibName, const char *szResName, const char *szMenuName)
{
 if(!m_hInst)
   return false;

 return m_pLoadMenu(m_hEditor, szLibName, szResName, szMenuName);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::LoadMenuFromModule(HINSTANCE hInstance, const char *szResName, const char *szMenuName)
{
 if(!m_hInst)
   return false;

 return m_pLoadMenuFromModule(m_hEditor, hInstance, szResName, szMenuName);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::EnableMenuItem(const char *szMenuName, unsigned nCmdId, bool bEnabled)
{
 if(!m_hInst)
   return false;

 return m_pEnableMenuItem(m_hEditor, szMenuName, nCmdId, bEnabled);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::SetCheckMenuItem(const char *szMenuName, unsigned nCmdId, bool bChecked)
{
 if(!m_hInst)
   return false;

 return m_pSetCheckMenuItem(m_hEditor, szMenuName, nCmdId, bChecked);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::AddError(const char *szFileName, int nLine, int nPosition, const char *szErrorText)
{
 if(!m_hInst)
   return false;

 return m_pAddError(m_hEditor, szFileName, nLine, nPosition, szErrorText);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::RemoveAllErrors()
{
 if(!m_hInst)
   return false;

 return m_pRemoveAllErrors(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::GotoError()
{
 if(!m_hInst)
   return false;

 return m_pGotoError(m_hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::GotoNextError()
{
 if(!m_hInst)
   return false;

 return m_pGotoNextError(m_hEditor);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::GotoPreviousError()
{
 if(!m_hInst)
   return false;

 return m_pGotoPreviousError(m_hEditor);
}

// -----------------------------------------------------------------------------
inline int RSSourceEditor::GetErrorCount()
{
 if(!m_hInst)
   return false;

 return m_pGetErrorCount(m_hEditor);
}

///////////////////////////////////////////////////////////////////////////////
inline bool RSSourceEditor::PreTranslateMessage(MSG *pMsg)
{
 if(!m_hInst)
   return false;

 return m_pMsgFilter(m_hEditor, pMsg);
}

// -----------------------------------------------------------------------------
inline bool RSSourceEditor::OnIdle(int nIdleCount)
{
 if(!m_hInst)
   return false;

 return m_pMsgIdle(m_hEditor, nIdleCount);
}

// -----------------------------------------------------------------------------
inline char* RSSourceEditor::GetTagLineFromFile (const char *fileName, const char *prefix)
{
 if(!m_hInst || !m_pGetTagLineFromFile)
   return NULL;

 return m_pGetTagLineFromFile(m_hEditor, fileName, prefix);
}

// -----------------------------------------------------------------------------
inline void RSSourceEditor::FreeTagLine (char* tag)
{
 if(!m_hInst || !m_FreeTagLine || !tag)
   return;

 m_FreeTagLine(m_hEditor, tag);
}

#endif // RSL_PL_WIN32

#endif // __RSSRCEDIT_HPP

/* <-- EOF --> */