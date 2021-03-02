/*******************************************************************************
 FILE         :   DICT.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   Словарь файлов

 PROGRAMMED BY:   Чаусов  В.И.

 CREATION DATE:   12.04.94

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/dict.h $
                  $Revision: 3 $
                  $Date: 30.10.98 19:03 $
                  $Author: Lebedev $
                  $Modtime: 22.10.98 18:03 $
*******************************************************************************/

#ifndef __DICT_H
#define __DICT_H

#include "apptldef.h"

#include <rsl/bshstruc.h>


//**************************** Глобализм *********************************

#ifdef __cplusplus
 extern "C" {
#endif

extern _APPTLDATA BTRVFILE *FilePrj;       // Файл проекта

// Ключи и фильтры на проект
_APPTLFUNC extern void SKF_Prj(
                               BTRVFILE      *bf,              // Файл проекта
                               enum BRECKEY   numkey,          // Номер ключа
                               unsigned char  Group,           // Признак группы
                               long           code             // Код файла/поля
                              );

#ifdef __cplusplus
 } ;
#endif

//**************************** Конец файла *******************************

#endif // __DICT_H

/* <-- EOF --> */