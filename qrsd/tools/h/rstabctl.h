/*************************************************************************
   File Name      : rstabctl.h
   Programmer     : Alexey Lebedev

   Creation       : 13 Mar 2003
   Description    : TabControl.

   Copyright(C) 2003 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

//
//  RS-Bank 5.1
//
//  ����⪠ ᤥ���� ��-����� �।��� ����� Tab control �
//  ���������������, �� �� � � ⥪�⮢�� ���᮫�
//  � �ᯮ�짮������ �������饣� �����㬥�� :-)
//
//  13.09.99 AS
//

#ifndef __RSTABCTL_H
#define __RSTABCTL_H

#include "apptldef.h"

// ���� ����ᮢ, �����ন����� � LoopTabCtrl
enum
   {
    DLGTYPE_PANEL,

    // �஫���� �ᥣ�� ����᪠���� �१ BRunScrol
    DLGTYPE_BSCROL,
    DLGTYPE_BSCROL_READONLY = DLGTYPE_BSCROL // ��⠢�� ��� ���⭮� ᮢ���⨬���
   };


// -----------------------------------------------------------------------------
// ������ ࠡ��� TabControl'�
enum
   {
    eTC_Mode_0 = 0,  // 0: �� �⮡ࠦ��� ������
    eTC_Mode_1,      // 1: �⮡ࠦ��� ������
    eTC_Mode_2       // 2: �� �⮡ࠦ��� ��������� � EW-०��� (� ���᮫� �������祭 ०��� 1)
   };

// ���ᨬ��쭮 ��������� �᫮ ��������
// ����� �� �ᯮ������ - ��⠢���� ��� ᮢ���⨬���.
#define  MAX_TABCTRL_SCREENS  20


#include "packpshn.h"

// ��ࠬ���� ��� LoopTabCtrl
typedef struct TABCTRL_PARMS
      {
       void **dlg;                  // ���ᨢ ����ᮢ
       int   *types;                // ���ᨢ ⨯�� ����ᮢ (��� NULL)
       int    num_screens;          // ��᫮ �࠭��
       int    start_screen;         // � ������ ����� (1=10)
       int    ret_screen;           // �� ������ ��諨
       int    showtabs;             // �����뢠�� "��������" (�. eTC_Mode_)
       char **tabhead;              // �������� "��������"
       char  *RegKeyName;           // �������� ���� � ����ன��� ��� NULL
       void (*UserSetCurrentScreen)(int *CurScr, void *UserParm); // IL 25.06.02 ����������� �஡���� �࠭
       char  *Phead;
      } TABCTRL_PARMS;

// ��� ����७���� �ᯮ�짮�����
typedef struct TABCTRL
      {
       PANEL            tabs1;
       PANEL            tabs2;

       int              current_screen;
       int              goto_screen;
       int              goto_field;

       void           **panel_procs;
       void           **user_parms;
       TABCTRL_PARMS   *parms;

       const char      *Phead;
       const char      *Pstatus;

       char            *save;
       int             *tabs_xx;

       bool             canResize;  // TabCtrl ����� ������ ࠧ���
      } TABCTRL;

#include "packpop.h"

#ifdef __cplusplus
extern "C" {
#endif

// ������� TabCtrl
_APPTLFUNC TABCTRL *CreateTabCtrl(TABCTRL_PARMS *parms);

// ������� TabCtrl
_APPTLFUNC void FreeTabCtrl(TABCTRL *tab);

// �뢥�� TabCtrl
_APPTLFUNC int PutTabCtrl(TABCTRL *tab);

// ����� TabCtrl
_APPTLFUNC void RemTabCtrl(TABCTRL *tab);

// �������� 横� ��ࠡ�⪨ ��� TabCtrl
_APPTLFUNC int UseTabCtrl(TABCTRL *tab);

// �� ��襮��ᠭ��� ����� ���⮥
_APPTLFUNC int LoopTabCtrl(TABCTRL_PARMS *prm);

// ��३� �� �������� � TabCtrl
_APPTLFUNC void GotoTabCtrl(TABCTRL *tab, int ntab, int nfield);

#ifdef __cplusplus
};
#endif

#endif // __RSTABCTL_H

/* <-- EOF --> */