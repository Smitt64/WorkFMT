/*******************************************************************************
 FILE         :   RSAPPDIR.H

 COPYRIGHT    :   R-Style Software Lab, 1994, 1998

 DESCRIPTION  :

 PROGRAMMED BY:   Emil Dovidovitch

 CREATION DATE:   14 Mar 1994

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/rsappdir.h $
                  $Revision: 1 $
                  $Date: 19.10.98 19:26 $
                  $Author: Markovitch $
                  $Modtime: 16.10.98 21:28 $
*******************************************************************************/

#ifndef __RSAPPDIR_H
#define __RSAPPDIR_H

#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

// Получение имени файла в текущей директирии
_APPTLFUNC void getFileName(
                            char       *DirFileName,    // Имя файла в директории
                            const char *FileName        // Имя файла
                           );

// Формирование полного имени файла в рабочем каталоге (WORKFILE)
_APPTLFUNC void getWorkFileName(char *FullFileName, const char *SmallFileName);

// Формирование полного имени файла в каталоге <TXTFILE>
_APPTLFUNC void getTxtFileName(char *FullFileName, const char *SmallFileName);

// Формирование полного имени файла в каталоге <HELP>
void getHelpFileName(char *outfname, const char *fname);

// Формирование полного имени файла в каталоге <RES>
//void getResFileName(char *outfname, const char *fname);


_APPTLFUNC void MemFileDir(void);       // Сохранить предыдущий Директорий
_APPTLFUNC void RestFileDir(void);      // Восстановить предыдущий Директорий
_APPTLFUNC void SetTxtDir(void);        // Установить Директорий текстовых файлов
_APPTLFUNC void SetWorkDir(void);       // Установить Директорий текстовых файлов
_APPTLFUNC void SetObjDir(void);        // Установить Директорий exe файлов
_APPTLFUNC void SetToolsDir(void);      // Установить Директорий tools файлов
_APPTLFUNC void SetDBfileDir(void);
_APPTLFUNC void SetResFileDir(const char* resFileDir);
_APPTLFUNC void SetHelpFileDir(const char* helpFileDir);

#ifdef __cplusplus
}
#endif

#endif // __RSAPPDIR_H

/* <-- EOF --> */