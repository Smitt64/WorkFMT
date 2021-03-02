//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naoper.h                        January 24,1994
//
//  Description      : ������� �ࠢ�筨�� ����樮���⮢
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
//  October 25, 1994          : ���� �.�. - ���ᥭ�� ���짮��⥫�᪮��
//                              ���७�� � �������� ����� OPERMAN
// March 31,1995   Serg Kubrin (K78)
//            ������ int �� int16
//------------------------------------------------------------------@H-

#ifndef __NAOPER_H
#define __NAOPER_H

#define _NAOPER_H_

#include "na/namenu.h"
#include "na/nadef.h"
#include "rstypes.h"

/*
 * �᫨ ������� OPERMAN ������ ᮤ�ঠ�� ���⠭���⭮� ���७��,
 * � ��� �⮣� ����室���:
 *
 * 1)

 * ����� ������祭��� H-䠩�� "na/naoper.h" ��।����� ��������
 * ᢮��� ���७��:
 *
 * typedef struct {
 *                 <user-field-1>;
 *                 ...
 *                 <user-field-N>;
 *                }
 *                 NAOPER_VPART;
 *
 * ��� �� ��।����� ��६����� _NAOPER_VPART_
 * (����� �������� �� ����� ����ન�����):
 *
 * #define _NAOPER_VPART_
 *
 *
 * 2)
 *
 * ��� ���饭�� � ���� ���७�� ᫥��� �ᯮ�짮���� ᨭ⠪��:
 *
 * (OPERMAN *)pOper->Reserv.vpart.<user-field-name>
 *                      ^     ^
 *                      |     |
 *                      |     +-- 䨪�஢����� ��� ���짮��⥫�᪮�� ���७��
 *                      |
 *                      +-- 䨪�. ��� union'�, ᮤ�ঠ饣� �������� १�ࢠ
 *                          � ����� OPERMAN
 *
 */

// -----------------------------------------------------------------------------
#define  RESERVELEN        80

#include <packpsh1.h>

// -----------------------------------------------------------------------------
#if !defined ( _NAOPER_VPART_ )
    typedef struct
          {
           char  UserType[RESERVELEN];
          } NAOPER_VPART;
    //#error "NAOPER_VPART: Variable part not defined!"
    // Kireev 3.7.00 ��� � �� �� �訡�� (#error ���������७�), � ����
    // ��ᨣ������஢���, �� NAOPER_VPART ��।�����, � ���� ��।����� _NAOPER_VPART_
    #define _NAOPER_VPART_

#else
    //
    // �஢�ઠ ����� �� ࠡ�⠥� ��� *.C
    // (⮫쪮 ��� *.CPP?)
    //
    //#   if sizeof(NAOPER_VPART) > RESERVLEN
    //#      error "NAOPER_VPART: User defined part of OPERMAN structure is too long"
    //#   endif
#endif


// -----------------------------------------------------------------------------
// ��ࠢ�筨� ����樮���⮢
typedef struct
      {
       db_int16  Oper;               // ����樮����
       char      Name_Oper[36];      // ��� ����樮����
       char      Parol[9];           // ��஫� ����樮����
       db_int16  Seek;               // �����
       db_int16  SeekE;              // ����७�� �����
       char      cTypePerson;        // �஢��� ����㯠
       uchar     szIdentProgram[17]; // ���᮪ �ਫ������
       db_int16  AhrCarryDay;        // �ਧ��� �஢���� � ��娢�
       db_int16  GroupOperF;         // ��砫� ����஢ ��㯯� ���஢
       db_int16  GroupOperL;         // ����砭�� ����஢ ��㯯� ���஢
       db_int16  GroupCredF;         // ��砫� ����஢ ��㯯� �।�⮢
       db_int16  GroupCredL;         // ����砭�� ����஢ ��㯯� �।�⮢

       union                               // ����� � ���짮��⥫�᪨�� ���������
           {
            char          _reserved_[RESERVELEN];
            NAOPER_VPART  vpart;
           } Reserv;
      } OPERMAN;

#include <packpop.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef int (* na_openoper_t)(int OpenMode); // �㭪�� ������ 䠩�� ����樮���⮢
typedef int (* na_editoper_t)(void);         // ������஢���� ����
typedef int (* na_editparm_t)(OPERMENU *);   // ������஢���� ��ࠬ��஢
typedef int (* na_printmenu_t)(void);        // ������஢���� ��ࠬ��஢

// ����⨥ 䠩��
_NALGFUNC int iOpenUser(int iOpenMode, const char *szFile, const char *szPassword);

// ��������� ���짮��⥫�
_NALGFUNC int inoper(
                     int O,                         // �����
                     char *P,                       // ��஫�
                     na_openoper_t    OpenOper      // �㭪�� ������ 䠩�� ����樮���⮢
                    );

// ����� ��஫� �� �६� ᥠ��
_NALGFUNC int chparol(na_openoper_t OpenOper);

_NALGFUNC int edit_oper(
                        na_openoper_t    OpenOper,     // �㭪�� ������ 䠩�� ����樮���⮢
                        na_editoper_t    EditOper,     // �㭪�� ।���஢���� ����樮����
                        na_editparm_t    EditParm      // �㭪�� ।���஢���� ��ࠬ��஢ ����樮����
                       );

// ������஢���� ���� ���짮��⥫�
_NALGFUNC int edit_menuoper(
                            OPERMAN       *Oper,
                            na_editparm_t  EditParm      // �㭪�� ।���஢���� ��ࠬ��஢ ����樮����
                           );

// ����� ०�� ।���஢���� ���� ���짮��⥫�:
//    0 - �஫����
//    1 - ��ॢ�
_NALGFUNC int SetMenuOperMode(int mode);

_NALGFUNC int edit_oper2(
                         na_openoper_t    OpenOper,     // �㭪�� ������ 䠩�� ����樮���⮢
                         na_editoper_t    EditOper,     // �㭪�� ।���஢���� ����樮����
                         na_editparm_t    EditParm,     // �㭪�� ।���஢���� ��ࠬ��஢ ����樮����
                         na_printmenu_t   PrintMenu     // �㭪�� ���� ���� ����樮����
                        );

// ������஢���� ���� ���짮��⥫�
_NALGFUNC int edit_menuoper2(
                             OPERMAN         *Oper,
                             na_editparm_t    EditParm,     // �㭪�� ।���஢���� ��ࠬ��஢ ����樮����
                             na_printmenu_t   PrintMenu     // �㭪�� ���� ���� ����樮����
                            );

_NALGFUNC int iListOper(
                        coord x,                       // ���न���� ������ � ������⢮ �����
                        coord y,
                        coord l,
                        na_openoper_t    OpenOper,     // �㭪�� ������ 䠩�� ����樮���⮢
                        void *buff                     // ���� ��� ����� ����������
                       );

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */