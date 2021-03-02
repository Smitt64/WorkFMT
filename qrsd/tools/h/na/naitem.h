//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naitem.h                        January 24,1994
//
//  Description      : ������� �ࠢ�筨�� ��⥬��� ���㫥�
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
//------------------------------------------------------------------@H-

#ifndef __NAITEM_H
#define __NAITEM_H

#define _NAITEM_H_

#include <systypes.h>

#include "desktop.h"
#include "na/nadef.h"


// -----------------------------------------------------------------------------
#include <packpsh1.h>

// ��ࠬ���� ������쭮� ����ன�� �롮�
typedef union
      {
       char  Reserve[400];

       struct                         // ����ன�� ����
            {
             char          szParmReport[250];   // ���᮪ ��ࠬ��஢ �롮�
             char          szNameTxt[9];        // �������� ⥪�⮢��� 䠩�� �뢮��
             char          szNameFile[81];      // �������� 䠩�� ��� �����.���⮢ � �믮��.���㫥�
             char          szNmSeq[15];         // �������� ��ப� � ��ࠬ��ࠬ� �ਭ��
             db_baseint16  iPageSize;           // ����� ��࠭���
             db_baseint16  iDbl;                // ������⢮ ���⮢ � �ਭ�
             char          cContext;            // ���⥪�� �ணࠬ��
             db_baseint16  iHelp;               // ����� ��࠭��� �����
            } rept;

       struct                         // ����ன�� ����⥬��� �ணࠬ��
            {
             char          szNameFile[81];      // �������� 䠩�� ��� �����.���⮢ � �믮��.���㫥�
             char          szStrParm[81];       // ��ப� ��ࠬ��஢
             db_baseint16  iHelp;               // ����� ��࠭��� �����
            } exec;

      // 20.2.95 VC ���ᥭ� ��࠭�� help
       struct                         // ����ன�� ��楤���
            {
             char          szParmItem[250];     // ���᮪ ��ࠬ��஢ �롮�
             db_baseint16  iHelp;               // ����� ��࠭��� �����
            } proc;

      } ITEMPARM;

// -----------------------------------------------
typedef struct                  // ��ࠢ�筨� �ணࠬ���� ���㫥� �����
      {
       uchar     cIdentProgram;        // �����䨪��� �ணࠬ��
       db_int16  iCaseItem;            // ���⥬�� ����� �롮�
       db_int16  iKindMethod;          // ��� �믮������
       db_int16  iKindProgram;         // ��� �ணࠬ��
       db_int16  iHelp;                // ����� ��࠭��� �����
       char      Reserve[13];          // �����
       char      szNameItem[61];       // �������� �롮�
       ITEMPARM  parm;                 // ��ࠬ���� �롮�
      } ITEM;

// -----------------------------------------------
typedef union                   // ���� �ࠢ�筨�� ���㫥�
      {
       struct
            {
             uchar         cIdentProgram;   // �����䨪��� �ਫ������
             db_baseint16  iCaseItem;       // ���⥬�� ����� �롮�
            } item;
      } ITEMKEY;


#ifdef __cplusplus
 extern "C" {
#endif

// ��⠭���� ���祩 � 䨫��஢ 䠩�� ��⥬��� ���㫥�
_NALGFUNC void SKF_sitem(
                         int   numkey,                  // ����� ����
                         uchar cIdentProgram            // �����䨪��� �ணࠬ��
                        );

_NALGFUNC int iOpenSItem(int iOpenMode); // itemsyst.dbt

// ��⠭���� ���祩 � 䨫��஢ 䠩�� ��⥬��� ���㫥�
_NALGFUNC void SKF_uitem(
                         int   numkey,                  // ����� ����
                         uchar cIdentProgram            // �����䨪��� �ணࠬ��
                        );

_NALGFUNC int iOpenUItem(int iOpenMode); // itemuser.dbt

_NALGFUNC int iFindSItem(uchar cIdentProgram, int iCaseItem, void *buff);
_NALGFUNC int iFindUItem(uchar cIdentProgram, int iCaseItem, void *buff);

_NALGFUNC int iListUItem(
                         coord x,                       // ���न���� ������ � ������⢮ �����
                         coord y,
                         coord l,
                         void *buff                     // ���� ��� ����� ����������
                        );

_NALGFUNC int iListSItem(
                         coord x,                       // ���न���� ������ � ������⢮ �����
                         coord y,
                         coord l,
                         void *buff                     // ���� ��� ����� ����������
                        );

_NALGFUNC void vNameMethod(char *Name, int iKindMethod);

typedef int (* na_edititem_t)(ITEM *);    // ������஢���� ��ࠬ��஢

// �ணࠬ�� ।���஢���� ᯨ᪠ ���㫥�
_NALGFUNC int edit_item(
                        int           system,
                        na_edititem_t editparm,                 // ������஢���� ��ࠬ��஢
                        int           user_edit = 1
                       );

#ifdef __cplusplus
 } ;
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */