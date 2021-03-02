// -------------------------------------------------------------------------------------------------
// 2013-11-14  ���� ��堭��� �ࠢ�筮� ��⥬�
//             ����� � �ॡ㥬묨 dbt-䠩����

#ifndef _RSHELPFLS_H_
#define _RSHELPFLS_H_

#include "rstypes.h"

#include "RSHelpDef.h"


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ����� � 䠩��� help_fls.dbt (�᭮���� 䠩� ������)

// -----------------------------------------------------------------------------
#include <packpsh1.h>

typedef struct
      {
       db_int32  SrcType;                     // ��� ���筨�� (1 - �����, 2 - msg-䠩�, 3 - itemsyst.dbt)
       db_int32  SrcID;                       // �����䨪��� ���筨�� (䠩� help_src.dbt)
       db_int32  PageID;                      // ����� ��࠭��� �����
       db_int32  HelpID;                      // �����䨪��� 䠩�� �ࠢ�� (䠩� help_src.dbt)
       uint8     System;                      // �����䨪��� �����⥬�
       db_int32  ItemID;                      // ��騩 �����䨪���
       db_int32  ResType;                     // ��� �����
       char      ResName[dHELP_FLS_RESNAME];  // ������������ �����
       char      Reserv[300];                 // �����
      } THELP_FLS;


// -----
typedef union
      {
       struct
            {
             db_baseint32  SrcType;
             db_baseint32  SrcID;
             db_baseint32  ResType;
             char          ResName[dHELP_FLS_RESNAME];
             db_ubaseint8  System;
            } KEY_0;

       struct
            {
             db_baseint32  ItemID;
             db_baseint32  SrcType;
             db_baseint32  SrcID;
             db_baseint32  ResType;
             char          ResName[dHELP_FLS_RESNAME];
             db_ubaseint8  System;
            } KEY_1;

       struct
            {
             db_baseint32  ItemID;
             db_baseint32  SrcType;
             db_baseint32  SrcID;
             db_ubaseint8  System;
            } KEY_2;

       struct
            {
             db_baseint32  HelpID;
            } KEY_3;
      } THELP_FLS_KEY_BUF;

#include <packpop.h>

// -----------------------------------------------------------------------------
// ��࠭���/����⠭����� ���祢�� ���ଠ��
void RKF_Help_Fls(char SaveFlag, bool flag = true);

// -----------------------------------------------------------------------------
// ��⠭����� ���祢�� 䨫��� �� 䠩� help_fls.dbt
// ��� ���� 3 � itemID ��।����� helpID !
void SKF_Help_Fls
   (
    enum eRSH_KEYS       nKey,     // ����� ����
         int32           srcType,  // ��� ���筨��
         int32           srcId,    // �����䨪��� ���筨��
         int32           itemId,   // ��騩 �����䨪���
         uint8           system,   // �����䨪��� �����⥬�
         int32           resType,  // ��� �����
         const char     *resName   // ������������ �����
   );

// -----------------------------------------------------------------------------
extern _APPTLDATA BTRVFILE  *Fhelp_fls;

int OpenHelp_Fls(int mode);

// -----------------------------------------------------------------------------
int GetHelp_Fls(uint8 system, int32 srcType, int32 srcId, int32 resType, const char *resName, int32 itemId, int32 helpID, THELP_FLS *buff = NULL);

#endif // _RSHELPFLS_H_

/* <-- EOF --> */