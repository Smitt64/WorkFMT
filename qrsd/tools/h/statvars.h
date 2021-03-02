//-@H------------------------------------------------------------------
//         Автоматизированная банковская система RS-Bank v3.2
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : statvars.h                      January 24,1994
//
//  Description      : Статические перменные NA
//
//  Public routines  :
//  Base files       :
//  Programer        : Чаусов В.И.
//
//  VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/statvars.h $
//                   $Revision: 4 $
//                   $Date: 1.08.00 16:18 $
//                   $Author: Lebedev $
//                   $Modtime: 1.08.00 16:14 $
//
//                History
//  January 24,1994           : Создан
//  April   26,1994           : Денисов М.Ю. - подбор старых и внесение
//                              новых соплей
//                              (с претензией на универсальность)
//
//------------------------------------------------------------------@H-

#ifndef __STATVARS_H
#define __STATVARS_H

#include <stdlib.h>

#include <platform.h>
#include <rscdefs.h>
#include "btrv.h"
#include "res.h"

#include "rsl/common.h"

#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

extern _APPTLDATA char RSL_FAR  MsgFile[_MAX_PATH];      // Файл сообщений
extern _APPTLDATA char RSL_FAR  MsgTool[_MAX_PATH];      // Файл сообщений
extern _APPTLDATA char RSL_FAR  DicFile[_MAX_PATH];      // Файл словаря
extern _APPTLDATA char RSL_FAR  LbrName[_MAX_PATH];      // Файл пользовательской библиотеки диалогов
extern _APPTLDATA char RSL_FAR  HelpFile[_MAX_PATH];     // Файл
extern _APPTLDATA char RSL_FAR  BtrMacName[_MAX_PATH];   // Файл

//......................................................... директории
extern _APPTLDATA char RSL_FAR  openfileDir[_MAX_PATH];  // Директорий открытия файлов
extern _APPTLDATA char RSL_FAR  objfileDir[_MAX_PATH];   // Директорий выполняемых модулей
extern _APPTLDATA char RSL_FAR  toolsfileDir[_MAX_PATH]; // Директорий файлов инструмента
extern _APPTLDATA char RSL_FAR  txtfileDir[_MAX_PATH];   // Директорий временных файлов
extern _APPTLDATA char RSL_FAR  macroDir[1024];          // Директорий RSL файлов
extern _APPTLDATA char RSL_FAR  dbfileDir[_MAX_PATH];    // Директорий dbt файлов
extern _APPTLDATA char RSL_FAR  wrkfileDir[_MAX_PATH];   // Директорий рабочих файлов
extern _APPTLDATA char RSL_FAR  poolDir[_MAX_PATH];      // Директорий файлов для отправки на терминал

//......................................................... переменные

extern _APPTLDATA uchar RSL_FAR  szIdentProgram[51];
extern _APPTLDATA uchar RSL_FAR  cIdentProgram;
extern _APPTLDATA char  RSL_FAR  cTypePerson;

extern _APPTLDATA int  adm;                          // Признак операциониста-администратора
extern _APPTLDATA int  oper;                         // Номер операциониста
extern _APPTLDATA int  Numb_ActivArh;
extern _APPTLDATA int  GroupOperF;
extern _APPTLDATA int  GroupOperL;

extern _APPTLDATA ResFile  ToolRes;
extern _APPTLDATA char RSL_FAR  InParol[9];

extern _APPTLDATA BTRVFILE  *FileSItem; // Файл справочник системных модулей
extern _APPTLDATA BTRVFILE  *FileUItem; // Файл справочник пользовательских модулей
extern _APPTLDATA BTRVFILE  *FileNA;    // Файл алгоритмов
extern _APPTLDATA BTRVFILE  *FileOM;
extern _APPTLDATA BTRVFILE  *FileOP;
extern _APPTLDATA BTRVFILE  *FileO;
extern _APPTLDATA BTRVFILE  *FileTA;    // Файл типов лицевых счетов
extern _APPTLDATA BTRVFILE  *FileCL;

// Ссылка на пользовательскую процедуру
extern _APPTLDATA TRslMesProc  User_MsgProc;

//
// Функции cохранения/восстановления данных пользователя
//
typedef int (TUserParmSaveProc)(
                                const char *ParmType,      // Тип параметров, строка длиной не более 31 символа
                                const char *Context,       // Контекст, строка длиной не более 8 символов
                                const void *UserParm,      // Указатель на структуру параметров
                                size_t      UserParmSize   // Размер структуры
                               );

typedef int (TUserParmRestProc)(
                                const char *ParmType,      // Тип параметров, строка длиной не более 31 символа
                                const char *Context,       // Контекст, строка длиной не более 8 символов
                                                           // Когда ParmType == szLastScreenParmType, Context
                                                           // передается из прикладного кода и его максимальная
                                                           // длина неизвестна
                                void       *UserParm,      // Указатель на структуру параметров
                                size_t     *UserParmSize   // Размер структуры (возвращает размер реально
                                                           // прочитанных данных)
                               );

//
// Коды возврата от функций сохранения/восстановления
//
enum
   {
    UserParm_OK       = 0,  // Операция выполнена без ошибок
    UserParm_Failed   = -1, // Операция не выполнена
    UserParm_NotFound = 1,  // Данные не найдены
    UserParm_LData    = 2   // Записываемые данные превышают максимально возможный размер.
                            // Или cчитанные данные не умещаются в переданный буфер. В этом
                            // случае UserParmSize содержит размер реально прочитанных данных.
   };

// Функция сохранения параметров пользователя
extern _APPTLDATA TUserParmSaveProc  *SaveUserParmProc;

// Функция сохранения параметров пользователя для всех пользователей, у которых
// нет персональных настроек
extern _APPTLDATA TUserParmSaveProc  *SaveUserParmForAllProc;

// Функция восстановления параметров пользователя
extern _APPTLDATA TUserParmRestProc  *RestoreUserParmProc;

//
// Строки, используемые для идентификации пользовательских параметров
//
extern _APPTLDATA char RSL_FAR szFiltrParmType[32];
extern _APPTLDATA char RSL_FAR szColumnWidthParmType[32];
extern _APPTLDATA char RSL_FAR szLastScreenParmType[32];
extern _APPTLDATA char RSL_FAR szSysFiltrParmType[32];
extern _APPTLDATA char RSL_FAR szSysSortParmType[32];


// -----------------------------------------------------------------------------
// Работа с параметрами настроечного файла rstools.ini

// -----------------------------------------------------------------------------
// Значения для TSToolsConfigData::addToolsIconToScroll

#define  dTOOLS_ADDICON_SELECT    1             // Добавлять инструментальные иконки в диалог выбора файлов
#define  dTOOLS_SHOW_WORKMODE     1             // Выводить в статус-строку текущий режим работы
#define  dTOOLS_SHOW_TRACEMODE    2             // Выводить в статус-строку текущий режим трассировки
#define  dTOOLS_SHOW_NOTIFY       3             // Выводить в статус-строку область уведомлений
#define  dTOOLS_SELECTFF_DLGMODE  1             // Использовать автоскроллинги в диалоге выбора файла/каталога

// -----------------------------------------------
typedef struct
             {
              int   addToolsIconToScroll,       // Константы типа dTOOLS_ADDICON_
                    showStatusBarWorkMode,      // Константы типа dTOOLS_SHOW_
                    showStatusBarTraceMode,     // Константы типа dTOOLS_SHOW_
                    selectFileFolderDlgMode;    // Константы типа dTOOLS_SELECTFF_

              // ----- Режим уведомлений
              int   showStatusBarNotify,        // Константы типа dTOOLS_SHOW_ (использование режима уведомлений)
                    useNotifyHistory,           // Использовать историю уведомлений
                    useNotifySilentMode,        // Блокировать всплывающие окна уведомлений
                    defaultNotifyTimeOut,       // Время отображения всплывающего окна по-умолчанию
                    maxNotifyHistorySize;       // Максимальный размер истории уведомлений

              // ----- История файлов
              int   useFileHistory,             // Использовать историю файлов
                    maxFileHistorySize,         // Максимальный размер истории файлов
                    useHomeDirMode;             // Режим каталогов в трехвенке

              char  resFileDir[_MAX_PATH];
              char  helpFileDir[_MAX_PATH];

              // ----- Поле PICTURE
              int   maxPictureSize,             // Максимальный размер (в килобайтах) файла с картинкой для поля PICTURE
                    maxPictureCount;            // Максимальное количество картинок в списке для поля PICTURE

              char  externalViewer[_MAX_PATH];  // Программа для внешнего просмотра изображений
              int   externalMode;               // Режим работы при внешнем просмотре
             } TSToolsConfigData;


int RsToolsConnect(const char *iniFile);
int RsToolsDisconnect(void);

TSToolsConfigData *RsToolsGetConfig(void);

void RsToolsReadConfigData(const char *iniFile, TSToolsConfigData *data);

#ifdef __cplusplus
}
#endif

#endif // __STATVARS_H

/* <-- EOF --> */