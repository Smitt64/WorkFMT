//-@H------------------------------------------------------------------
//         ��⮬�⨧�஢����� ������᪠� ��⥬� RS-Bank v3.2
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : statvars.h                      January 24,1994
//
//  Description      : ����᪨� ��ଥ��� NA
//
//  Public routines  :
//  Base files       :
//  Programer        : ���ᮢ �.�.
//
//  VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/statvars.h $
//                   $Revision: 4 $
//                   $Date: 1.08.00 16:18 $
//                   $Author: Lebedev $
//                   $Modtime: 1.08.00 16:14 $
//
//                History
//  January 24,1994           : ������
//  April   26,1994           : ����ᮢ �.�. - ������ ����� � ���ᥭ��
//                              ����� ᮯ���
//                              (� ��⥭���� �� 㭨���ᠫ쭮���)
//
//------------------------------------------------------------------@H-

#ifndef __STATVARS_H
#define __STATVARS_H

#include <stdlib.h>

#include <platform.h>
#include <rscdefs.h>
#include "btrv.h"
#include "res.h"

#include "rsl/common.h"

#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern _APPTLDATA char RSL_FAR  MsgFile[_MAX_PATH];      // ���� ᮮ�饭��
extern _APPTLDATA char RSL_FAR  MsgTool[_MAX_PATH];      // ���� ᮮ�饭��
extern _APPTLDATA char RSL_FAR  DicFile[_MAX_PATH];      // ���� ᫮����
extern _APPTLDATA char RSL_FAR  LbrName[_MAX_PATH];      // ���� ���짮��⥫�᪮� ������⥪� ��������
extern _APPTLDATA char RSL_FAR  HelpFile[_MAX_PATH];     // ����
extern _APPTLDATA char RSL_FAR  BtrMacName[_MAX_PATH];   // ����

//......................................................... ��४�ਨ
extern _APPTLDATA char RSL_FAR  openfileDir[_MAX_PATH];  // ��४�਩ ������ 䠩���
extern _APPTLDATA char RSL_FAR  objfileDir[_MAX_PATH];   // ��४�਩ �믮��塞�� ���㫥�
extern _APPTLDATA char RSL_FAR  toolsfileDir[_MAX_PATH]; // ��४�਩ 䠩��� �����㬥��
extern _APPTLDATA char RSL_FAR  txtfileDir[_MAX_PATH];   // ��४�਩ �६����� 䠩���
extern _APPTLDATA char RSL_FAR  macroDir[1024];          // ��४�਩ RSL 䠩���
extern _APPTLDATA char RSL_FAR  dbfileDir[_MAX_PATH];    // ��४�਩ dbt 䠩���
extern _APPTLDATA char RSL_FAR  wrkfileDir[_MAX_PATH];   // ��४�਩ ࠡ��� 䠩���
extern _APPTLDATA char RSL_FAR  poolDir[_MAX_PATH];      // ��४�਩ 䠩��� ��� ��ࠢ�� �� �ନ���

//......................................................... ��६����

extern _APPTLDATA uchar RSL_FAR  szIdentProgram[51];
extern _APPTLDATA uchar RSL_FAR  cIdentProgram;
extern _APPTLDATA char  RSL_FAR  cTypePerson;

extern _APPTLDATA int  adm;                          // �ਧ��� ����樮����-�����������
extern _APPTLDATA int  oper;                         // ����� ����樮����
extern _APPTLDATA int  Numb_ActivArh;
extern _APPTLDATA int  GroupOperF;
extern _APPTLDATA int  GroupOperL;

extern _APPTLDATA ResFile  ToolRes;
extern _APPTLDATA char RSL_FAR  InParol[9];

extern _APPTLDATA BTRVFILE  *FileSItem; // ���� �ࠢ�筨� ��⥬��� ���㫥�
extern _APPTLDATA BTRVFILE  *FileUItem; // ���� �ࠢ�筨� ���짮��⥫�᪨� ���㫥�
extern _APPTLDATA BTRVFILE  *FileNA;    // ���� �����⬮�
extern _APPTLDATA BTRVFILE  *FileOM;
extern _APPTLDATA BTRVFILE  *FileOP;
extern _APPTLDATA BTRVFILE  *FileO;
extern _APPTLDATA BTRVFILE  *FileTA;    // ���� ⨯�� ��楢�� ��⮢
extern _APPTLDATA BTRVFILE  *FileCL;

// ��뫪� �� ���짮��⥫���� ��楤���
extern _APPTLDATA TRslMesProc  User_MsgProc;

//
// �㭪樨 c��࠭����/����⠭������� ������ ���짮��⥫�
//
typedef int (TUserParmSaveProc)(
                                const char *ParmType,      // ��� ��ࠬ��஢, ��ப� ������ �� ����� 31 ᨬ����
                                const char *Context,       // ���⥪��, ��ப� ������ �� ����� 8 ᨬ�����
                                const void *UserParm,      // �����⥫� �� �������� ��ࠬ��஢
                                size_t      UserParmSize   // ������ ��������
                               );

typedef int (TUserParmRestProc)(
                                const char *ParmType,      // ��� ��ࠬ��஢, ��ப� ������ �� ����� 31 ᨬ����
                                const char *Context,       // ���⥪��, ��ப� ������ �� ����� 8 ᨬ�����
                                                           // ����� ParmType == szLastScreenParmType, Context
                                                           // ��।����� �� �ਪ������� ���� � ��� ���ᨬ��쭠�
                                                           // ����� �������⭠
                                void       *UserParm,      // �����⥫� �� �������� ��ࠬ��஢
                                size_t     *UserParmSize   // ������ �������� (�����頥� ࠧ��� ॠ�쭮
                                                           // ���⠭��� ������)
                               );

//
// ���� ������ �� �㭪権 ��࠭����/����⠭�������
//
enum
   {
    UserParm_OK       = 0,  // ������ �믮����� ��� �訡��
    UserParm_Failed   = -1, // ������ �� �믮�����
    UserParm_NotFound = 1,  // ����� �� �������
    UserParm_LData    = 2   // �����뢠��� ����� �ॢ���� ���ᨬ��쭮 �������� ࠧ���.
                            // ��� c�⠭�� ����� �� 㬥����� � ��।���� ����. � �⮬
                            // ��砥 UserParmSize ᮤ�ন� ࠧ��� ॠ�쭮 ���⠭��� ������.
   };

// �㭪�� ��࠭���� ��ࠬ��஢ ���짮��⥫�
extern _APPTLDATA TUserParmSaveProc  *SaveUserParmProc;

// �㭪�� ��࠭���� ��ࠬ��஢ ���짮��⥫� ��� ��� ���짮��⥫��, � ������
// ��� ���ᮭ����� ����஥�
extern _APPTLDATA TUserParmSaveProc  *SaveUserParmForAllProc;

// �㭪�� ����⠭������� ��ࠬ��஢ ���짮��⥫�
extern _APPTLDATA TUserParmRestProc  *RestoreUserParmProc;

//
// ��ப�, �ᯮ��㥬� ��� �����䨪�樨 ���짮��⥫�᪨� ��ࠬ��஢
//
extern _APPTLDATA char RSL_FAR szFiltrParmType[32];
extern _APPTLDATA char RSL_FAR szColumnWidthParmType[32];
extern _APPTLDATA char RSL_FAR szLastScreenParmType[32];
extern _APPTLDATA char RSL_FAR szSysFiltrParmType[32];
extern _APPTLDATA char RSL_FAR szSysSortParmType[32];


// -----------------------------------------------------------------------------
// ����� � ��ࠬ��ࠬ� ����஥筮�� 䠩�� rstools.ini

// -----------------------------------------------------------------------------
// ���祭�� ��� TSToolsConfigData::addToolsIconToScroll

#define  dTOOLS_ADDICON_SELECT    1             // ��������� �����㬥�⠫�� ������ � ������ �롮� 䠩���
#define  dTOOLS_SHOW_WORKMODE     1             // �뢮���� � �����-��ப� ⥪�騩 ०�� ࠡ���
#define  dTOOLS_SHOW_TRACEMODE    2             // �뢮���� � �����-��ப� ⥪�騩 ०�� ����஢��
#define  dTOOLS_SHOW_NOTIFY       3             // �뢮���� � �����-��ப� ������� 㢥��������
#define  dTOOLS_SELECTFF_DLGMODE  1             // �ᯮ�짮���� ����஫����� � ������� �롮� 䠩��/��⠫���

// -----------------------------------------------
typedef struct
             {
              int   addToolsIconToScroll,       // ����⠭�� ⨯� dTOOLS_ADDICON_
                    showStatusBarWorkMode,      // ����⠭�� ⨯� dTOOLS_SHOW_
                    showStatusBarTraceMode,     // ����⠭�� ⨯� dTOOLS_SHOW_
                    selectFileFolderDlgMode;    // ����⠭�� ⨯� dTOOLS_SELECTFF_

              // ----- ����� 㢥��������
              int   showStatusBarNotify,        // ����⠭�� ⨯� dTOOLS_SHOW_ (�ᯮ�짮����� ०��� 㢥��������)
                    useNotifyHistory,           // �ᯮ�짮���� ����� 㢥��������
                    useNotifySilentMode,        // �����஢��� �ᯫ뢠�騥 ���� 㢥��������
                    defaultNotifyTimeOut,       // �६� �⮡ࠦ���� �ᯫ뢠�饣� ���� ��-㬮�砭��
                    maxNotifyHistorySize;       // ���ᨬ���� ࠧ��� ���ਨ 㢥��������

              // ----- ����� 䠩���
              int   useFileHistory,             // �ᯮ�짮���� ����� 䠩���
                    maxFileHistorySize,         // ���ᨬ���� ࠧ��� ���ਨ 䠩���
                    useHomeDirMode;             // ����� ��⠫���� � ��墥���

              char  resFileDir[_MAX_PATH];
              char  helpFileDir[_MAX_PATH];

              int   useDelayedTree;

              // ----- ���� PICTURE
              int   maxPictureSize,             // ���ᨬ���� ࠧ��� (� ���������) 䠩�� � ���⨭��� ��� ���� PICTURE
                    maxPictureCount;            // ���ᨬ��쭮� ������⢮ ���⨭�� � ᯨ᪥ ��� ���� PICTURE

              char  externalViewer[_MAX_PATH];  // �ணࠬ�� ��� ���譥�� ��ᬮ�� ����ࠦ����
              int   externalMode;               // ����� ࠡ��� �� ���譥� ��ᬮ��
             } TSToolsConfigData;


int RsToolsConnect(const char *iniFile);
int RsToolsDisconnect(void);

TSToolsConfigData *RsToolsGetConfig(void);

void RsToolsReadConfigData(const char *iniFile, TSToolsConfigData *data);

// -----------------------------------------------------------------------------
extern _APPTLDATA int Bank55OperLogVerNum;

#ifdef __cplusplus
}
#endif

#endif // __STATVARS_H

/* <-- EOF --> */