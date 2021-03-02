// -------------------------------------------------------------------------------------------------
// 2013-05-16  Утилитные функции для работы с каталогами

#ifndef _RSUTILDIR_H_
#define _RSUTILDIR_H_

#include <string>
#include <vector>


// -----------------------------------------------------------------------------
namespace RSUDIR
        {
         // --------------------------------------------------------------------
         // Массив масок файлов
         typedef std::vector<std::string>  TVFileMask;


         // --------------------------------------------------------------------
         // Получаем маску (или набор масок)
         // [out] maskOnly - маски отбора ("*.mac,*.lst")
         // [in]  fullPath - полный путь с масками ("D:\Test\*.mac,*.lst")
         // [in]  path     - каталог из fullPath ("\Test\")
         void ExtractMask(char *maskOnly, const char *fullPath, const char *path);

         // --------------------------------------------------------------------
         // Заполняем массив масками
         // [out] lst    - массив строк (string) с масками
         // [in]  masks  - строка с масками ("*.mac,*.lst")
         // [in]  delims - символы-разделители между масками в masks
         void CreateMaskArray(TVFileMask &lst, const char *masks, const char *delims = ",;");
        };

#endif _RSUTILDIR_H_

/* <-- EOF --> */