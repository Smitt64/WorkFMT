/*******************************************************************************
 FILE         :   RSFFIND.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Семейство функций для поиска файлов по маске

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   16 Mar 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsffind.h $
                  $Revision: 2 $
                  $Date: 25.04.00 11:08 $
                  $Author: Lebedev $
                  $Modtime: 22.04.00 19:06 $
*******************************************************************************/

#ifndef __RSFFIND_H
#define __RSFFIND_H

#include <stdlib.h>
#include <sys/stat.h>

#include "platform.h"
#include "rscdefs.h"
#include "tooldef.h"

#include "rstypes.h"


// -----------------------------------------------------------------------------
#ifndef S_IFDIR
    #define  S_IFDIR         0x4000  /* directory */
#endif

#ifndef S_ISDIR
    #define  S_ISDIR(mode)   ((mode) & S_IFDIR)
#endif


// -----------------------------------------------------------------------------
#ifdef __cplusplus
    struct RslFindBlock;
#endif

typedef struct RslFindData
      {
       #ifdef __cplusplus
           struct RslFindBlock  *fndblk;
       #else
           void                 *fndblk;
       #endif

       char                      fname[_MAX_FNAME + _MAX_EXT];
       char                      path[_MAX_PATH];

       unsigned short            mode;
       unsigned long             size;
       int64_t                   sizeFull;      // Полный размер файла (64-битное целое знаковое)

       time_t                    atime;
       time_t                    ctime;
       time_t                    mtime;
      } RslFindData;

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
//
// Флаги поиска
//
enum
   {
    FF_Direc   = 01,    // каталоги, соответствующие маске
    FF_File    = 02,    // файлы, соответствующие маске

    FF_AnyDir  = 010,   // все каталоги, игнорирую маску
    FF_AnyFile = 020,   // все файлы, игнорирую маску

    FF_Default = (FF_AnyDir | FF_File)
   };

//
// Функция-фильтр
// Возвращает 0, файл подходит
//           -1, файл не подходит
//           >0, зарезервировано для ошибок
//
typedef int (* RslFindFiltr)(const RslFindData *);



/*******************************************************************************
 Последовательный поиск файлов, удовлетворяющих маске
 
 RslFindFirst - найти первый файл, удовлетворяющий условиям поиска.
 ~~~~~~~~~~~~
   Параметры:
        fmask - маска файлов, включая путь;
        flags - флаги поиска, не может быть 0;
        filtr - функцию-фильтр, может быть NULL;

   Возвращает указатель на структуру RslFindData, если первый файл найден,
   или NULL, если файлов с заданными условиями нет или произошла ошибка.
   Глобальная переменная errno содержит код ошибки.

 RslFindNext  - найти следующий файл, удовлетворяющий условиям поиска.
 ~~~~~~~~~~~
   Параметры:
        data  - указатель на структуру RslFindData;

   Возвращает 0, если следующий файл найден, Иначе возвращает ненулевое значение
   и устанавливает код ошибки в глобальной переменной errno.

 RslFindClose - освободить память и используемые ресурсы.
 ~~~~~~~~~~~
   Параметры:
        data  - указатель на структуру RslFindData;

   Возвращаемого значения нет.

*******************************************************************************/

_TOOLEXP RslFindData *RslFindFirst(const char *fmask, long flags, RslFindFiltr filtr);
_TOOLEXP int          RslFindNext (RslFindData *data);
_TOOLEXP void         RslFindClose(RslFindData *data);



/*******************************************************************************
 Семейство функций для поиска Btrieve-файлов
*******************************************************************************/

#define  BtFindFirst(fmask, flags, filtr)  RslFindFirst(fmask, flags, filtr)
#define  BtFindNext(data)                  RslFindNext(data)
#define  BtFindClose(data)                 RslFindClose(data)

// Old names
#define  BtrvFindFirst  BtFindFirst
#define  BtrvFindNext   BtFindNext
#define  BtrvFindClose  BtFindClose

/*******************************************************************************

 RslMatchFile - проверить соответствие имени файла маске
 ~~~~~~~~~~~~
   Параметры:
      pattern - маска, может содержать * и ?;
        fname - имя файла;

   Возвращает 0, если имя файла соответствует маске.

*******************************************************************************/

_TOOLEXP int RslMatchFile(const char *pattern, const char *fname);

#ifdef __cplusplus
};
#endif

#endif /* __RSFFIND_H */

/* <-- EOF --> */