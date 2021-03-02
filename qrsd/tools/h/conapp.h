/*-----------------------------------------------------------------------*-

 File Name   : conapp.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 February 15,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __CONAPP_H
#define __CONAPP_H

#include "rstypes.h"
#include "bexpdef.h"
#include "apptldef.h"
#include "traceinit.h"


// -----------------------------------------------------------------------------
// ����ਨ����㬥�⠫�� �����䨪����
#define  dRST_ID_HOTKEY_DEBUG_ON     -10101
#define  dRST_ID_HOTKEY_DEBUG_OFF    -10102

#define  dRST_ID_HOTKEY_DEBUG2_ON    -10103
#define  dRST_ID_HOTKEY_DEBUG2_OFF   -10104

// -----------------------------------------------------------------------------
typedef void (waitProgram_t)(intptr_t programID, void *data);
typedef void (execProcedure_t)(int32 commandID, const char *parms, void *data);

#ifdef __cplusplus
extern "C" {
#endif


_APPTLFUNC void RSConApp(const char *Systema, const char *Desk, const char *Copr, const char *Res,
                         const char *Helpfile, const char *Bdir);

_APPTLFUNC int  DisablePanelHandler(int disable);

_APPTLFUNC void InitTextEditor(void);
_APPTLFUNC void CloseTextEditor(void);

_APPTLFUNC bool AddWaitProgram(intptr_t programID, bool fRemote, bool stopOnExit, waitProgram_t *execProgram, void *data);
_APPTLFUNC void CheckWaitProgram(void);

_APPTLFUNC int  RSRTLInit(void);
_APPTLFUNC void RSRTLDone(void);
_APPTLFUNC void RSRTLDoneDll(unsigned hDll);
           void RSRTLDoneExe(void);

_APPTLFUNC void InitEasyWin(void);
_APPTLFUNC void DoneEasyWin(void);
_APPTLFUNC bool IsEasyWinMode(void);

#if defined(RSL_MD_DLL)

    // ������� ���������� ����� dll'�, �� ������� ��� ���� �������.
    unsigned GetUserDll(void);

    #define  RSRTLDone()     RSRTLDoneDll(GetUserDll())

#endif

/*  Need to do something usefull */
typedef int (*RslNotifyProc)(void);
_APPTLFUNC void RSRTLSetNotifySink(RslNotifyProc proc);

int  RslAttachConsole(long procId);
long RslGetProcessId(void);
const char *RslGetRTLFileName(char *buff, int size);
unsigned RslGetRTLModuleHandle(void);

void ShowFatalError(const char *mesg);

void InitTimeZone(void);

#if !defined(RSL_PL_DOS) && defined(RSL_PL_MS)
    void InitConsoleBacksForDLM (void);
#else
    #define  InitConsoleBacksForDLM()
#endif

_APPTLFUNC void *OpenContext(const char *title, int numcols, int numrows);
_APPTLFUNC void  CloseContext(void *);

_APPTLFUNC void CloseMainWindow(void);
_APPTLFUNC void RestoreMainWindow(void);

_APPTLFUNC void SetOverWriteMode (int overMode);

_APPTLFUNC const char *RslGetUserName(char *buff);
_APPTLFUNC const char *RslGetHomeDir(char *buff);
_APPTLFUNC const char *RslGetAppName(void);

_APPTLFUNC char *RslEditFileBuff(void);
_APPTLFUNC void  StartRsDesigner(void);
_APPTLFUNC int   EditorCallBack  (int cmd, const char *fname, int mode);
           int   EditorCallBackEx(int cmd, void *msgData);

int HandleInGlobalEditor(int cmd, const char *szFileName, int mode);

_APPTLFUNC void  StartRsDesignerResource(const char *resId);
_APPTLFUNC void  SetRsDesignedId(uint32 p);

// EditManager
_APPTLFUNC bool  ConnectToEditor(bool bWinEditor);
_APPTLFUNC void  DisconnectFromEditor(void);
_APPTLFUNC unsigned long GetEditorCookie(void);

_APPTLFUNC void  AddExecProcedure(execProcedure_t *proc, void *data);
_APPTLFUNC void  DelExecProcedure();

struct tagPANEL;

_APPTLFUNC int   TestHorMenu(struct tagPANEL *p, execProcedure_t *proc, void *data);

// SourceEditor
_APPTLFUNC bool  EnableSourceEditor(bool enabled);

// ����⭮� ���������� 䠩��� �� �ନ����
_APPTLFUNC void BatchSynchronize(const char *szIniFile);
void AppSynchronize(const void *pInfo);

// -----------------------------------------------------------------------------
// ������� ���� � ��⠫��� ������ �ਫ������ ()
// ����� ������ NULL !
const char *GetAppDataDir(void);

// -----------------------------------------------------------------------------
// ��ࠢ����� ०���� ����祩 �⫠��� ����䠩���
// [in] mode - 0 - �ᯮ�짮����� �������権 Ctrl-Alt-[/Ctrl-Alt-] ������㯭� (��⠭������ ��-㬮�砭��)
//             1 - �ᯮ�짮����� �������権 Ctrl-Alt-[/Ctrl-Alt-] ����㯭�
// [ret]     - �����頥� �।��饥 ���祭�� 䫠��
_APPTLFUNC int SetHotkeyDebugMode(int mode);
_APPTLFUNC int GetHotkeyDebugMode(void);

#ifdef __cplusplus
}
#endif

extern        HTRACE  hToolsTrace;
extern _BTEXP HTRACE  hMdbTrace;

#endif  // __CONAPP_H

/* <-- EOF --> */