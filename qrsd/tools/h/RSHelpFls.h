// -------------------------------------------------------------------------------------------------
// 2013-11-14  Новый механизм справочной системы
//             Работа с требуемыми dbt-файлами

#ifndef _RSHELPFLS_H_
#define _RSHELPFLS_H_

#include "rstypes.h"

#include "RSHelpDef.h"


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Работа с файлом help_fls.dbt (Основной файл данных)

// -----------------------------------------------------------------------------
#include <packpsh1.h>

typedef struct
      {
       db_int32  SrcType;                     // Тип источника (1 - ресурс, 2 - msg-файл, 3 - itemsyst.dbt)
       db_int32  SrcID;                       // Идентификатор источника (файл help_src.dbt)
       db_int32  PageID;                      // Номер страницы помощи
       db_int32  HelpID;                      // Идентификатор файла справки (файл help_src.dbt)
       uint8     System;                      // Идентификатор подсистемы
       db_int32  ItemID;                      // Общий идентификатор
       db_int32  ResType;                     // Тип ресурса
       char      ResName[dHELP_FLS_RESNAME];  // Наименование ресурса
       char      Reserv[300];                 // Резерв
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
// сохранить/восстановить ключевую информацию
void RKF_Help_Fls(char SaveFlag, bool flag = true);

// -----------------------------------------------------------------------------
// установить ключевой фильтр на файл help_fls.dbt
// Для ключа 3 в itemID передается helpID !
void SKF_Help_Fls
   (
    enum eRSH_KEYS       nKey,     // Номер ключа
         int32           srcType,  // Тип источника
         int32           srcId,    // Идентификатор источника
         int32           itemId,   // Общий идентификатор
         uint8           system,   // Идентификатор подсистемы
         int32           resType,  // Тип ресурса
         const char     *resName   // Наименование ресурса
   );

// -----------------------------------------------------------------------------
extern _APPTLDATA BTRVFILE  *Fhelp_fls;

int OpenHelp_Fls(int mode);

// -----------------------------------------------------------------------------
int GetHelp_Fls(uint8 system, int32 srcType, int32 srcId, int32 resType, const char *resName, int32 itemId, int32 helpID, THELP_FLS *buff = NULL);

#endif // _RSHELPFLS_H_

/* <-- EOF --> */