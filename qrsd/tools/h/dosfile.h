//***********************************************************************
//                    Инструментарий для работы с dos-файлами
//                            VECsoft 29.10.1992
//***********************************************************************
// October 25,1995   Serg Kubrin (K78)
//     Add CopyFileLong
// November 2,1995   Serg Kubrin (K78)
//     Add chunkNumber to CopyFileLong

#ifndef __DOSFILE_H
#define __DOSFILE_H

#include "bdate.h"
#include "apptldef.h"
#include "rsfname.h"

#ifdef __cplusplus
 extern "C" {
#endif

// Получить имя файла из даты
_APPTLFUNC void get_namefile(
                             char  *In,        // Путь к файлу
                             char  *Out,       // Имя файла
                             bdate *Date       // Дата
                            );

// Получить имя файла с расширением в сети
_APPTLFUNC void get_extfile(
                            char *In,          // Входное имя файла без расширения
                            char *Out          // Выходное имя файла с расширением
                           );

// Получить имя рабочего файла
_APPTLFUNC char *get_wrkfile(
                             const char *In,   // Входное имя файла без расширения и пути
                             char *Out         // Полный путь к выходному файлу
                            );

// Копировать файл в файл
_APPTLFUNC int copyfile(
                        char *NameIn,     // Имя исходного файла
                        char *NameOut,    // Имя приемного файла
                        int   sure        // Признак обязательного копирования
                       );


// -----------------------------------------------------------------------------
typedef enum
      {
       CP_NEXT_TARGET_FILE,
       CP_NEXT_SOURCE_FILE
      } TCopyFileOperation;

typedef struct
      {
       TCopyFileOperation  oper;
       char               *NameIn;         // Имя исходного файла
       char               *NameOut;        // Имя файла назначения
       unsigned long       targetFreeSize; // Размер свободного пространства для файла
                                           // назначения или 0 если контроль не выполнять
       unsigned long       bytesWritten;   // Количество записанных байт
       int                 chunkNumberIn;  // Номер сегмента исходного файла
       int                 chunkNumberOut; // Номер сегмента файла назначения
      } TFileCopyInfo;


_APPTLFUNC unsigned long CopyFileLong(
                                      char *nameIn,                 // Имя исходного файла
                                      char *nameOut,                // Имя файла назначения
                                      unsigned long sourceSize,     // Размер всех сегментов исходного фала или 0
                                                                    // если сегмент один
                                      unsigned long targetFreeSize, // Размер свободного пространства для файла
                                                                    // назначения если useFreeSize == True
                                      int (*userProc)(TFileCopyInfo *), // Процедура запроса на смену дискеты
                                                                    // при успехе должна возвращать 0
                                      int useFreeSize               // Если True использовать targetFreeSize
                                   );

#ifdef __cplusplus
 } ;
#endif

#endif // __DOSFILE_H

/* <-- EOF --> */