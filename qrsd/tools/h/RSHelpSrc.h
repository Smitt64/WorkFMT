// -------------------------------------------------------------------------------------------------
// 2013-11-14  Новый механизм справочной системы
//             Работа с требуемыми dbt-файлами

#ifndef _RSHELPSRC_H_
#define _RSHELPSRC_H_

#include "nwbtrv.h"

#include "menu.h"

#include "rstypes.h"

#include "RSHelpDef.h"


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Работа с файлом help_src.dbt (Источники данных и файлы справки)

// -----------------------------------------------------------------------------
#include <packpsh1.h>

typedef struct
      {
       db_int32  ID;                    // Автоинкрементный уникальный идентификатор записи
       db_int32  Type;                  // Тип записи (1 - источник, 2 - файл справки)
       db_int32  SrcType;               // Тип источника (1 - ресурс, 2 - msg-файл, 3 - itemsyst.dbt)
       char      File[dHELP_SRC_FILE];  // Наименование файла-источника/файла справки
       char      Reserv[300];           // Резерв
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
// сохранить/восстановить ключевую информацию
void RKF_Help_Src(char SaveFlag, bool flag = true);

// -----------------------------------------------------------------------------
// установить ключевой фильтр на файл help_src.dbt
void SKF_Help_Src
   (
    enum eRSH_KEYS       nKey,     // Номер ключа
         int32           id,       // Идентификатор
         int32           type,     // Тип записи
         int32           srcType,  // Тип источника
         const char     *name      // Имя файла
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