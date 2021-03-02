//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naitem.h                        January 24,1994
//
//  Description      : ������� ������ ����㯠 New Access (NA)
//
//  Public routines  :
//  Base files       :
//  Programer        : ���ᮢ �.�.
//
//                History
//  January 24,1994           : ������
//  April   26,1994           : ����ᮢ �.�. - ������ ����� � ���ᥭ��
//                              ����� ᮯ���
//                              (� ��⥭���� �� 㭨���ᠫ쭮���)
//
// March 31,1995   Serg Kubrin (K78)
//            ������ int ���16
// 02.03.99 ���. �������� ��ࠬ��� �㭪樨 iReadParmProc()
//------------------------------------------------------------------@H-

#ifndef __NA_H
#define __NA_H

#define _NA_H_

#include <platform.h>
#include "na/nadef.h"
#include "na/nadefs.h"
#include "rstypes.h"
#include "bedit.h"

#include "na/namenu.h"
#include "na/naalg.h"
#include "na/naoper.h"
#include "na/navars.h"
#include "na/nafname.h"

#include "rsl/common.h"

#ifdef __cplusplus
 extern "C" {
#endif


// -----------------------------------------------------------------------------
typedef int (* na_appproc_t)(void);    // ����, ���樠������ � 䨭�� �ਫ������
typedef int (* na_menuproc_t)(void);   // ��楤�� ����᪮�� ����

// -----------------------------------------------------------------------------
// ��ୠ������ ����権
typedef int (* na_oplogrec_t)(int bop, BTRVFILE *opfile, void *saveb, int sizeb, int objType, const char *objID);
typedef int (* na_oplogproc_t)(int opCode, const char *szDescription, int objType, const char *objID);


_NALGFUNC int iPlayExec            // �믮������ exe ��� bat �����
(
 PARMPLAY *PP                   // ��ࠬ���� �믮������
);

_NALGFUNC int  iPlayExternProc      // �믮������ ���譥�� �����
(
 int       method,              // ���⥬�� ����� �롮�
 PARMPLAY *PP,                  // ��ࠬ���� �믮������
 TRslActionProc ProcFun,        // �ணࠬ�� �믮������ ���⥪��
 void     *data,                // ����� ���짮��⥫�
 int      iView                 // �ਧ��� ��������樨 ����
);

_NALGFUNC int iPlayProc            // �믮������ �����
(
 int    iCaseItem,              // ���⥬�� ����� �롮�
 int    iView,                  // �ਧ��� ��������樨 ����
 na_execproc_t proc             // �ணࠬ�� �믮������
);

_NALGFUNC int iPlayModule          // �믮������ �����
(
  int    iCaseItem,     // ���⥬�� ����� �롮�
  int    iView,         // �ਧ��� ��������樨 ����
  PARMPLAY *PP,         // ��ࠬ���� �믮������
  na_execproc_t proc    // �ணࠬ�� �믮������
);

_NALGFUNC void iPlayModuleProc(long iCaseItem, const char *parms, void *data);

// �������� � ��ப� ��ࠬ��஢ ������ ��ப� (�१ �஡�� � ����� ��ப� ��ࠬ��஢)
// �����頥� 㪠��⥫� �� ��ப� ��ࠬ��஢ (१����)
_NALGFUNC char *szAddParamToParamStr
(
  PARMPLAY *PP, // ��ࠬ���� �����
  char *str     // ��ப� ��ࠬ�஢.
);

// �������� � ��ப� ��ࠬ��஢ ��ࠬ���� �㭪� ����
// �����頥� 㪠��⥫� �� ��ப� ��ࠬ��஢ (१����)
_NALGFUNC char *szAddMenuItemParm
(
  PARMPLAY *PP, // ��ࠬ���� �����
  MENU     *nm  // ��࠭�� �㭪� ����
);

_NALGFUNC int iReadParmProc(int iCaseItem, PARMPLAY *PP);

_NALGFUNC int iReadParmProc2 // �⥭�� � �����⮢�� ��ࠬ��஢ �����
(
 int      iCaseItem, // ���⥬�� ����� �롮�
 PARMPLAY *PP,       // ��ࠬ���� �����
 MENU     *nm        // ��࠭�� �㭪� ����
);

_NALGFUNC int  iStdItem             // �믮������ �⠭������ ��楤��
(
 int        iCaseItem,
 PARMPLAY  *parm,
 na_execproc_t proc                        // ��ࠡ��稪 ����
);

_NALGFUNC int view_report(void); // ��ᬮ�� �����⮢������ ���⮢
_NALGFUNC int edit_macro(void);
_NALGFUNC int edit_macro2(PARMPLAY *parm);

_NALGFUNC int listFile             // ���⠫�� 䠩��� ��� �롮� �����
(
 BSCROL *LC,         // ������ �஫��� �롮�
 coord x,         // ���न���� ������ � ������⢮ �����
 coord y,
 coord l,
 void *buff          // ���� ��� ����� ����������
);

// ���� ���㫥�
enum METHODKIND
   {
    KM_SYSTINTR = -1,              // ���⥬�� ����� + ���������
    KM_SYST     =  0,              // ���⥬�� �����
    KM_INTR     =  1,              // ���������
    KM_EXEC     =  2,              // �믮��塞� �����
    KM_VIEW     =  3               // ��ᬮ�� ⥪�⮢��� 䠩��
   };

// ���� �ணࠬ�
enum PROGRAMKIND
   {
    KP_PROC = 1,                   // ��楤��
    KP_REPT                        // ����
   };

_NALGFUNC void writeARM(char *name);

/************************* ����� BSCROL *********************************/

_NALGFUNC int PrintBs   (BSCROL *bs, const char *PrDir); // ����� BScrol'a � 䠩�
_NALGFUNC int PrintBsXml(BSCROL *bs, const char *PrDir); // ����� BScrol'a � 䠩�

_NALGFUNC int PrintS   (SCROL *bs, const char *PrDir); // ����� Scrol'a � 䠩�
_NALGFUNC int PrintSXml(SCROL *bs, const char *PrDir); // ��ᯮ�� � XML

_NALGFUNC void SetPrintDefault(void); // ��⠭����� �㭪樨 ���� ��� �஫������
                                      // BSCROL, SCROL � LSCROL, ���஥��� � �����㬥��

_NALGFUNC void SetPrintDir(const char *dir); // ��⠭����� ���., ��� 䠩��� c �ᯥ�⪮�
                                             // BSCROL, SCROL � LSCROL

// -----------------------------------------------------------------------------
// � ������� ������ �ਫ������ ��ᬠ�ਢ��� xml-�����
enum
   {
    eRR_ToExcel = 0,  // Excel (MS Office)
    eRR_ToSCalc,      // SCalc (OpenOffice)
    // -------------------------------------------
    eRR_End           // ��ନ����. ������騥 �����䨪���� ��������� ⮫쪮 ��। ��� !
   };

// -----------------------------------------------------------------------------
// ��⠭���� �ணࠬ�� ��ᬮ�� xml-�����
// [in]  to - ��� �ணࠬ�� �⮡ࠦ���� (eRR_)
// [ret]    - �����頥� �।��騩 ��� �ணࠬ��
_NALGFUNC int SetReportRedirect(int to);

// -----------------------------------------------------------------------------
// ������� �ਫ������ ��� �⮡ࠦ���� ����� (���ᯮ���㥬��)
// [in]  isPrg - true  - ������ ��� 䠩�� �ணࠬ��
//               false - ������ �������� �ணࠬ��
const char *GetReportXmlViewer(bool isPrg = true);

// ����� 蠯�� � 䠩�
int PrintHead        (FILE *fpr, PANEL *pm, int *ind_array, int *len_array, int isHeader);

// ����� ����� � 䠩�
void PrintFld        (FILE *fpr, PANEL *pm, coord x, int lens, int *ind_array, int *len_array);
void PrintFldXml     (FILE *fpr, PANEL *pm, coord x, int *ind_array, int *len_array, bool bFooter, int maxLevel);
void PrintColumnStyle(FILE *fpr, PANEL *pm, int *ind_array, int *len_array, int maxLevel);

// ������� �ଠ� ���� ��� �뢮�� � xml
const char *GetExcelStyleFormat(char *buff, int type, int point);

// -----------------------------------------------------------------------------
// ��� ������ �㬬 �� ���� ��������� ⨯�
typedef struct fmVal
      {
       void  *val;  // ���祭�� �㬬�
       int    ind;  // ������ ��������� ���� � ��ப�
      } fmVal;

void PrintFoot(FILE *fpr, PANEL *p, fmVal *sarray, int nfmoney, coord x, int lens,
               int *ind_array, int *len_array);

fmVal *MakeSumArray(PANEL *p, int *n);
void AddToSumArray(PANEL *p, fmVal *sarray, int n);
void FreeSumArray(fmVal *sarray, int n);

void GetEWColInfo(PANEL *p, int *len_arr, int *ind_arr);

/************************************************************************/


_NALGFUNC int appStart          // ������ �ਪ������ ���㫥�
(
 const char   *szAppName,       // ��� �ਫ������
 const char   *szWorkPlaceName, // �������� ࠡ�祣� ����
 const char   *szCopyRight,     // JV RStyle
 const char   *szGName,         // ��饥 ��� ����ᮢ �ਫ������
 const char   *szGroup,         // ��� �⥢�� ��㯯�
 uchar         cIdent,          // �����䨪��� �ணࠬ��
 na_openoper_t pOpenOper,       // ��楤�� ������ 䠩�� ���짮��⥫��
 na_execproc_t exec_proc,       // ��ࠡ��稪 ����
 na_appproc_t  start_proc,      // �।���⥫쭠� ����. (�� inoper)
 na_appproc_t  beg_log,         // ��楤�� �믮��塞�� �� ��⨢���樨 ����
 na_appproc_t  end_log          // ��楤�� �믮��塞�� ��᫥ �᢮�������� ����
);

_NALGFUNC int appStartProc      // ������ �ਪ������ ���㫥�
(
 const char   *szAppName,       // ��� �ਫ������
 const char   *szWorkPlaceName, // �������� ࠡ�祣� ����
 const char   *szCopyRight,     // JV RStyle
 const char   *szGName,         // ��饥 ��� ����ᮢ �ਫ������
 const char   *szGroup,         // ��� �⥢�� ��㯯�
 uchar         cIdent,          // �����䨪��� �ணࠬ��
 na_openoper_t pOpenOper,       // ��楤�� ������ 䠩�� ���짮��⥫��
 na_execproc_t exec_proc,       // ��ࠡ��稪 ����
 na_appproc_t  start_proc,      // �।���⥫쭠� ����. (�� inoper)
 na_appproc_t  beg_log,         // ��楤�� �믮��塞�� �� ��⨢���樨 ����
 na_appproc_t  end_log,         // ��楤�� �믮��塞�� ��᫥ �᢮�������� ����
 na_menuproc_t menuproc         // ��楤�� ����᪮�� ����
);

/************************************************************************/

_NALGFUNC void SetSystEdit(bool);


_NALGFUNC int SaveRestScrlParm(
                               PANEL    *mp,            // �����⥫� �� ������ ����� �஫�����
                               void     *UserParm,      // �����⥫� �� ���� � ��ࠬ��ࠬ�
                               size_t   *UserParmSize,  // ������ ����
                               int       IsSave         // 1 - ��࠭���, 0 - ����⠭�����
                              );

_NALGFUNC void EnableColWidthSave(bool);

/************************************************************************/

_NALGFUNC na_oplogrec_t  SetOpLogRec(na_oplogrec_t proc);   // ��⠭����� �㭪�� ����� � ��ୠ�
_NALGFUNC na_oplogproc_t SetOpLogProc(na_oplogproc_t proc); // ��⠭����� �㭪�� ����� � ��ୠ�

_NALGFUNC const char    *SetOperIdFormat(const char *fmt);  // ��⠭����� �ଠ� objectID ��� ��ୠ����樨
                                                            // ����⢨� ��� ���짮��⥫��

void BuildRemoteFileName(char* remoteName, const char *fl);
void BuildLocalFileName(char* localName, const char *prDir, const char *fl);

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */