//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naalg.h                        January 24,1994
//
//  Description      : ������� �ࠢ�筨�� �����⬮�
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
//------------------------------------------------------------------@H-

#ifndef __NAALG_H
#define __NAALG_H

#define _NAALG_H_

#include <systypes.h>

#include "platform.h"
#include "na/nadef.h"

#include <packpsh1.h>

// �������
enum NAKINDALG
   {
    TYPEMETHOD  = 19,              // 19 ���� �믮������
    KINDPROGRAM = 20               // 20 ���� �ணࠬ�
   };

// ��ࠢ�筨� �����⬮�
typedef struct
      {
       db_int16   iTypeAlg;           // ��� �����⬠
       db_int16   iNumberAlg;         // ����� �����⬠
       char       szNameAlg[41];      // �������� �����⬠
       db_int16   iLenName;           // ����� ��������
       db_int16   iQuantAlg;          // ������⢮ �����⬮� ⨯�
       char       Reserve[8];         // �����
      } NAMEALG;

// ���� ���� ⨯�
typedef union
      {
       struct                         // 0 - ����
            {
             db_baseint16  iTypeAlg;        // ��� �����⬠
             db_baseint16  iNumberAlg;      // ����� �����⬠
            } type;
      } NAMEALGKEY;


#ifdef __cplusplus
 extern "C" {
#endif


_NALGFUNC int  iOpenNA(int iOpenMode);

// ��⠭���� ���祩 � 䨫��஢ 䠩�� �����⬮�
_NALGFUNC void  SKF_namealg(
                            int  numkey,                   // ����� ����
                            int  type                      // ����� ⨯�
                           );

// ����� ������
_NALGFUNC int  list_alg(
                        int   kind,                    // ��� �����⬠
                        int   item,                    // ⥪�騩 �롮� �� ᯨ᪠
                        char *nameitem,                // �������� �롮�
                        coord x,                       // ���न���� ᯨ᪠
                        coord y
                       );

// ������� �������� �����⬠
_NALGFUNC char *name_alg(
                         int  kind,                     // ��� �����⬠
                         int  item                      // ⥪�騩 �롮� �� ᯨ᪠
                        );

//    ����� ⨯��
enum NANUMTYPES
   {
   #ifdef RSL_MD_FLAT
       TA_PROGRAM = 22,               // 22 32-� ࠧ�來� �ਫ������
   #else
       TA_PROGRAM = 12,               // 12 ���� �ਫ������
   #endif

    TA_PERSON     = 13,               // 13 �஢�� ����㯮�
    TA_DLLPROG    = 32                // 32 DLL-�ਫ������
   };

// ��ࠢ�筨� ⨯��
typedef struct
      {
       db_int16 iNumType;             // ����� ⨯�
       char     Type;                 // ���
       char     Name_Type[18];        // �������� ⨯�
       char     Contens[61];          // ��ࠪ���⨪�
      } TYPEAC;

// ���� ���� ⨯�
typedef union
      {
       struct                       // 0 - ����
            {
             db_baseint16 iNumType;        // ����� �����
             char         Type;            // ����� �����ᮢ��� ���
            } nt;

       struct                       // 1 - ����
            {
             db_baseint16 iNumType;        // ����� �����
             char         Name_Type[18];   // �������� ⨯�
            } nn;
      } TYPEACKEY;

_NALGFUNC int  iOpenTA(int iOpenMode);

// ��⠭���� ���祩 � 䨫��஢ 䠩�� ⨯��
_NALGFUNC void  SKF_typeac(
                           int  numkey,                   // ����� ����
                           int  num                       // ����� ⨯�
                          );

_NALGFUNC int  iFindType(
                         int   iNumType,                // ����� ⨯�
                         char  Type,                    // ���
                         void *buff                     // ���� ��� ����� ����������
                        );

// �ணࠬ�� �롮� ⨯�
_NALGFUNC int  iChsType(
                        int   iNumType,                // ����� ⨯�
                        char  *Type,                   // �������㥬� ⨯
                        coord x,                       // ���न���� ������ � ������⢮ �����
                        coord y,
                        coord l,
                        void *buff                     // ���� ��� ����� ����������
                       );

// �ணࠬ�� �롮� ⨯�
_NALGFUNC int  iChsTypeM(
                         int   iNumType,                // ����� ⨯�
                         char  *Type,                   // �������㥬� ⨯
                         coord x,                       // ���न���� ������ � ������⢮ �����
                         coord y,
                         coord l,
                         void *buff                     // ���� ��� ����� ����������
                        );

// �ணࠬ�� ��ᬮ�� ⨯�
_NALGFUNC int  iViewType(
                         int   iNumType,                // ����� ⨯�
                         char  *Type,                   // ��ᬠ�ਢ���� ⨯
                         coord x,                       // ���न���� ������ � ������⢮ �����
                         coord y,
                         coord l
                        );

// �롮� ⨯��
_NALGFUNC int  iListType(
                         int      iNumType,             // ����� ⨯�
                         char    *Type,                 // �������㥬� ⨯
                         int      iMaxType,             // ������⢮ ��������� ⨯��
                         coord    x,                    // ���न���� ���孥�� ������ 㣫� ����
                         coord    y,
                         coord    l
                        );

// ��।������ ������ ⨯�
_NALGFUNC int  iIsType(
                       const char *ListType,            // ��ப� ⨯��
                       char        type                 // �᪮�� ⨯
                      );

// ������஢���� ⨯��
_NALGFUNC int  edit_typeac(
                           int iNumType,                 // ����� ⨯�
                           char *Name                    // �������� ⨯� (��������� �஫����)
                          );

// ������஢���� ⨯��
_NALGFUNC int  edit_typeacEx(
                             int iNumType,                  // ����� ⨯�
                             char *Name,                    // �������� ⨯� (��������� �஫����)
                             int RO_Mode                    // ReadOnly Mode
                            );

#ifdef __cplusplus
 } ;
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */