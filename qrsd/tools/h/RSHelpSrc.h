// -------------------------------------------------------------------------------------------------
// 2013-11-14  ���� ��堭��� �ࠢ�筮� ��⥬�
//             ����� � �ॡ㥬묨 dbt-䠩����

#ifndef _RSHELPSRC_H_
#define _RSHELPSRC_H_

#include "nwbtrv.h"

#include "menu.h"

#include "rstypes.h"

#include "RSHelpDef.h"


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// ����� � 䠩��� help_src.dbt (���筨�� ������ � 䠩�� �ࠢ��)

// -----------------------------------------------------------------------------
#include <packpsh1.h>

typedef struct
      {
       db_int32  ID;                    // ��⮨��६���� 㭨����� �����䨪��� �����
       db_int32  Type;                  // ��� ����� (1 - ���筨�, 2 - 䠩� �ࠢ��)
       db_int32  SrcType;               // ��� ���筨�� (1 - �����, 2 - msg-䠩�, 3 - itemsyst.dbt)
       char      File[dHELP_SRC_FILE];  // ������������ 䠩��-���筨��/䠩�� �ࠢ��
       char      Reserv[300];           // �����
      } THELP_SRC;


// -----
typedef union
      {
       struct
            {
             db_baseint32  ID;
            } KEY_0;

       struct
            {
             db_baseint32  Type;
             db_baseint32  SrcType;
             char          File[dHELP_SRC_FILE];
            } KEY_1;
      } THELP_SRC_KEY_BUF;

#include <packpop.h>


// -----------------------------------------------------------------------------
// ��࠭���/����⠭����� ���祢�� ���ଠ��
void RKF_Help_Src(char SaveFlag, bool flag = true);

// -----------------------------------------------------------------------------
// ��⠭����� ���祢�� 䨫��� �� 䠩� help_src.dbt
void SKF_Help_Src
   (
    enum eRSH_KEYS       nKey,     // ����� ����
         int32           id,       // �����䨪���
         int32           type,     // ��� �����
         int32           srcType,  // ��� ���筨��
         const char     *name      // ��� 䠩��
   );

// -----------------------------------------------------------------------------
extern _APPTLDATA BTRVFILE  *Fhelp_src;

int OpenHelp_Src(int mode = ReadOnly);

// -----------------------------------------------------------------------------
int GetHelp_Src(int32 id, THELP_SRC *buff = NULL);

// -----------------------------------------------------------------------------
int GetHelp_Src(int32 type, int32 srcType, const char *srcName, THELP_SRC *buff = NULL);

#endif // _RSHELPSRC_H_

/* <-- EOF --> */