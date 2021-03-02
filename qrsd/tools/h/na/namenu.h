//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naalg.h                        January 24,1994
//
//  Description      : ������� ����
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
//            ������ int �� int16
// 02.03.99 ���.
//   � OPERMENU ���� १�ࢠ ������⢮���� ��� �࠭���� ��ࠬ��஢
//
//------------------------------------------------------------------@H-

#ifndef __NAMENU_H
#define __NAMENU_H

#define _NAMENU_H_

#include "na/naitem.h"
#include "na/nadef.h"
#include "rstypes.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include <packpshn.h>

// -----------------------------------------------------------------------------
#define  PPSZ_StrParm  81 // ����� ��ப� ��ࠬ��஢

// -----------------------------------------------------------------------------
typedef struct // ��ࠬ���� ��⥬���� �����
      {
       char   szNameProc[81];    // �������� ����� � ᯨ᪥
       char   szNameDirTxt[81];  // ������ ��� ⥪�⮢��� 䠩�� �뢮��
       char   szNameFile[81];    // �������� 䠩�� ��� �����.���⮢ � ��.���㫥�
       char   szStrParm[PPSZ_StrParm]; // ��ப� ��ࠬ��஢
       char   szParmReport[250]; // ���᮪ ��ࠬ��஢
       char   szSeq[160];        // ��ப� �ࠢ����� ᨬ����� �ਭ��
       int16  iPageSize;         // ����� ��࠭���
       int16  iDbl;              // ������⢮ ���⮢ � �ਭ�
       int16  iGetParm;          // �ਧ��� ��� ����� ��ࠬ��஢
       int16  iKindMethod;       // ��� �믮������
       int16  iKindProgram;      // ��� �ணࠬ��
       char   cContext;          // �ਧ��� ���⥪�� ����
      } PARMPLAY;

#include <packpop.h>

// -----------------------------------------------------------------------------
typedef int (* na_execproc_t)(int iCaseItem, PARMPLAY *);  // ��ࠡ��稪 ����

//*******************************************************************

#include <packpsh1.h>

// -----------------------------------------------
// ���᮪ �롮஢ �ணࠬ� ����樮����
typedef struct
      {
       db_int16  iOper;            // ����� ����樮����
       db_int16  iIdentProgram;    // �����䨪��� �ਫ������
       db_int16  iNumberPoint;     // �������� ����� 㧫� ����
       db_int16  iNumberFather;    // ����� ������饣� �㭪� � ����
       db_int16  iNumberLine;      // ����� � �������
       db_int16  iCaseItem;        // ���⥬�� ����� �롮�
       char      cSystemItem;      // �ਧ��� ��⥬���� �����
       char      szNameItem[81];   // �������� �롮� � ᯨ᪥
       char      szNamePrompt[81]; // ���᪠��� � �������� �롮�
       db_int16  iHelp;            // ����� ��࠭��� �����
       char      szParam[32];      // ��ࠬ���� �㭪� ���� (����������� � ��ࠬ��ࠬ ITEMSYST)
       db_int16  iProgItem;        // �ਫ������ �� ���ண� ���� �����
       char      cNoBorrow;        // �ਧ��� ����� �����⢮�����
       char      szReserve[3];     // �����
      } OPERMENU;

// -----------------------------------------------
enum OMKEY
   {
    OMSCRL = 0,                    // iOper, iIdentProgram, iInumberPoint
    OMMENU                         // iOper, iIdentProgram, iNumberFather, iNumberLine
   };

// -----------------------------------------------
typedef union // ���� �ࠢ�筨�� �롮஢
      {
       struct
            {
             db_baseint16  iOper;        // ����� ����樮����
             db_baseint16  iIdentProgram;// �����䨪��� �ਫ������
             db_baseint16  iNumberPoint; // �������� ����� 㧫� ����
            } scrl;

       struct
            {
             db_baseint16  iOper;        // ����� ����樮����
             db_baseint16  iIdentProgram;// �����䨪��� �ਫ������
             db_baseint16  iNumberFather;// ����� ������饣� �㭪� � ����
             db_baseint16  iNumberLine;  // ����� � �������
            } menu;
      } OPERMENUKEY;

#include <packpop.h>

_NALGFUNC void  MKF_omenu(void);
_NALGFUNC void  RKF_omenu(void);
_NALGFUNC void  SKF_omenu           // ��⠭���� ���祩 � 䨫��஢ 䠩�� �롮஢
                         (
                          int  numkey,                   // ����� ����
                          int  iOper,                    // ����� ����樮����
                          int  iIdentProgram,            // �����䨪��� �ਫ������
                          int  iNumber                   // ����� 㧫�
                         );

_NALGFUNC int  iOpenOM(int iOpenMode);  // opermenu.dbt

// �믮����� �ணࠬ� �� ᯨ᪠
_NALGFUNC int  iListMenuOper(na_execproc_t exec_proc);

// �������� ����
_NALGFUNC int  MenuFileCnstr(MENU **mHor, int iOper, uchar cIdentProgram);

// �᢮�������� ����
_NALGFUNC int  MenuFileDestr(MENU *mHor, int iN);

// ��ࠬ���� �� �������� OPERMENU
typedef struct
      {
       int    iProgItem;    // �ਫ������ �� ���ண� ���� �����
       char  *szParam;      // ��ࠬ���� �㭪� ���� (����������� � ��ࠬ��ࠬ ITEMSYST)
      } OMENUPARM;

_NALGFUNC OMENUPARM *FindOMenuParm(MENU *addr);

//*******************************************************************

#include <packpsh1.h>

// -----------------------------------------------
// ��ࠬ���� �������㠫쭮� ����ன��
typedef union
      {
       char  Reserve[300];

       struct                        // ����ன�� ����
            {
             char          szParmItem[250];    // ���᮪ ��ࠬ��஢ �롮�
             char          szNmSeq[15];        // �������� ��ப� � ��ࠬ��ࠬ� �ਭ��
             db_baseint16  iPageSize;          // ����� ��࠭���
             db_baseint16  iDbl;               // ������⢮ ���⮢ � �ਭ�
            } rept;

       struct                        // ����ன�� ��楤���
            {
             char          szParmItem[250];    // ���᮪ ��ࠬ��஢ �롮�
            } proc;
      } PROGPARM;

// -----------------------------------------------
// ���᮪ �������㠫��� ����஥�
typedef struct
      {
       db_int16  iOper;               // ����� ����樮����
       uchar     cIdentProgram;       // �����䨪��� �ਫ������
       db_int16  iCaseItem;           // ���⥬�� ����� �롮�
       char      cSystemItem;         // �ਧ��� ��⥬���� �����
       PROGPARM  parm;                // ��ࠬ���� ����ன��
      } OPERPARM;

// -----------------------------------------------
// ���� �ࠢ�筨�� ����ன��
typedef union
      {
       struct
            {
             db_baseint16  iOper;          // ����� ����樮����
             uchar         cIdentProgram;  // �����䨪��� �ਫ������
             db_baseint16  iCaseItem;      // ���⥬�� ����� �롮�
             char          cSystemItem;    // �ਧ��� ��⥬���� �����
            } ident;
      } OPERPARMKEY;

#include <packpop.h>

// -----------------------------------------------------------------------------
_NALGFUNC void SKF_oparm           // ��⠭���� ���祩 � 䨫��஢ 䠩�� ����ன��
                        (
                         int   numkey,                  // ����� ����
                         int   iOper,                   // ����� ����樮����
                         uchar cIdentProgram,           // �����䨪��� �ਫ������
                         int   iCaseItem,               // ���⥬�� ����� �롮�
                         char  cSystemItem              // �ਧ��� ��⥬���� �����
                        );

_NALGFUNC int iOpenOP(int iOpenMode);  // operparm.dbt

// �����⮢�� � ।���஢���� ��ࠬ��஢
_NALGFUNC int iPreEditParm(OPERMENU *om);

_NALGFUNC int iPreEditParmEx(OPERMENU *om, PROGPARM *parm);

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */