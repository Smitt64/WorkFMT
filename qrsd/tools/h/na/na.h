//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naitem.h                        January 24,1994
//
//  Description      : Ведение нового доступа New Access (NA)
//
//  Public routines  :
//  Base files       :
//  Programer        : Чаусов В.И.
//
//                History
//  January 24,1994           : Создан
//  April   26,1994           : Денисов М.Ю. - подбор старых и внесение
//                              новых соплей
//                              (с претензией на универсальность)
//
// March 31,1995   Serg Kubrin (K78)
//            Замена int инт16
// 02.03.99 Мол. Добавлен параметр функции iReadParmProc()
//------------------------------------------------------------------@H-

#ifndef __NA_H
#define __NA_H

#define _NA_H_

#include <platform.h>
#include "na/nadef.h"
#include "na/nadefs.h"
#include "rstypes.h"
#include "bedit.h"

#include "na/namenu.h"
#include "na/naalg.h"
#include "na/naoper.h"
#include "na/navars.h"
#include "na/nafname.h"

#include "rsl/common.h"

#ifdef __cplusplus
 extern "C" {
#endif


// -----------------------------------------------------------------------------
typedef int (* na_appproc_t)(void);    // Старт, инициализация и финиш приложения
typedef int (* na_menuproc_t)(void);   // Процедура статического меню

// -----------------------------------------------------------------------------
// Журнализация операций
typedef int (* na_oplogrec_t)(int bop, BTRVFILE *opfile, void *saveb, int sizeb, int objType, const char *objID);
typedef int (* na_oplogproc_t)(int opCode, const char *szDescription, int objType, const char *objID);


_NALGFUNC int iPlayExec            // Выполнение exe или bat модуля
(
 PARMPLAY *PP                   // Параметры выполнения
);

_NALGFUNC int  iPlayExternProc      // Выполнение внешнего модуля
(
 int       method,              // Системный номер выбора
 PARMPLAY *PP,                  // Параметры выполнения
 TRslActionProc ProcFun,        // Программа выполнения контекста
 void     *data,                // Данные пользователя
 int      iView                 // Признак демонстрации отчета
);

_NALGFUNC int iPlayProc            // Выполнение модуля
(
 int    iCaseItem,              // Системный номер выбора
 int    iView,                  // Признак демонстрации отчета
 na_execproc_t proc             // Программа выполнения
);

_NALGFUNC int iPlayModule          // Выполнение модуля
(
  int    iCaseItem,     // Системный номер выбора
  int    iView,         // Признак демонстрации отчета
  PARMPLAY *PP,         // Параметры выполнения
  na_execproc_t proc    // Программа выполнения
);

_NALGFUNC void iPlayModuleProc(long iCaseItem, const char *parms, void *data);

// Добавить в строку параметров данную строку (через пробел в конец строки параметров)
// Возвращает указатель на строку параметров (результат)
_NALGFUNC char *szAddParamToParamStr
(
  PARMPLAY *PP, // Параметры модуля
  char *str     // Строка парамеров.
);

// Добавить в строку параметров параметры пункта меню
// Возвращает указатель на строку параметров (результат)
_NALGFUNC char *szAddMenuItemParm
(
  PARMPLAY *PP, // Параметры модуля
  MENU     *nm  // Выбранный пункт меню
);

_NALGFUNC int iReadParmProc(int iCaseItem, PARMPLAY *PP);

_NALGFUNC int iReadParmProc2 // Чтение и подготовка параметров модуля
(
 int      iCaseItem, // Системный номер выбора
 PARMPLAY *PP,       // Параметры модуля
 MENU     *nm        // Выбранный пункт меню
);

_NALGFUNC int  iStdItem             // Выполнение стандартных процедур
(
 int        iCaseItem,
 PARMPLAY  *parm,
 na_execproc_t proc                        // Обработчик меню
);

_NALGFUNC int view_report(void); // Просмотр подготовленных отчетов
_NALGFUNC int edit_macro(void);
_NALGFUNC int edit_macro2(PARMPLAY *parm);

_NALGFUNC int listFile             // Листалка файлов для выбора элемента
(
 BSCROL *LC,         // Открытый скролинг выбора
 coord x,         // Координаты панели и количество линий
 coord y,
 coord l,
 void *buff          // Буфер для записи найденного
);

// Виды модулей
enum METHODKIND
   {
    KM_SYSTINTR = -1,              // Системный модуль + МАКРОЯЗЫК
    KM_SYST     =  0,              // Системный модуль
    KM_INTR     =  1,              // МАКРОЯЗЫК
    KM_EXEC     =  2,              // Выполняемый модуль
    KM_VIEW     =  3               // Просмотр текстового файла
   };

// Виды программ
enum PROGRAMKIND
   {
    KP_PROC = 1,                   // процедура
    KP_REPT                        // отчет
   };

_NALGFUNC void writeARM(char *name);

/************************* Печать BSCROL *********************************/

_NALGFUNC int PrintBs   (BSCROL *bs, const char *PrDir); // Печать BScrol'a в файл
_NALGFUNC int PrintBsXml(BSCROL *bs, const char *PrDir); // Печать BScrol'a в файл

_NALGFUNC int PrintS   (SCROL *bs, const char *PrDir); // Печать Scrol'a в файл
_NALGFUNC int PrintSXml(SCROL *bs, const char *PrDir); // Экспорт в XML

_NALGFUNC void SetPrintDefault(void); // установить функции печати для скроллингов
                                      // BSCROL, SCROL и LSCROL, встроенные в инструмент

_NALGFUNC void SetPrintDir(const char *dir); // установить дир., для файлов c распечаткой
                                             // BSCROL, SCROL и LSCROL

// -----------------------------------------------------------------------------
// С помощью какого приложения просматривать xml-отчёт
enum
   {
    eRR_ToExcel = 0,  // Excel (MS Office)
    eRR_ToSCalc,      // SCalc (OpenOffice)
    eRR_ToR7Office,
    // -------------------------------------------
    eRR_End           // Терминатор. Следующие идентификаторы добавлять только перед ним !
   };

// -----------------------------------------------------------------------------
// Установка программы просмотра xml-отчёт
// [in]  to - вид программы отображения (eRR_)
// [ret]    - возвращает предыдущий вид программы
_NALGFUNC int SetReportRedirect(int to);

// -----------------------------------------------------------------------------
// Получить приложение для отображения отчёта (неэкспортируемая)
// [in]  isPrg - true  - вернуть имя файла программы
//               false - вернуть название программы
const char *GetReportXmlViewer(bool isPrg = true);

// Печать шапки в файл
int PrintHead        (FILE *fpr, PANEL *pm, int *ind_array, int *len_array, int isHeader);

// Печать полей в файл
void PrintFld        (FILE *fpr, PANEL *pm, coord x, int lens, int *ind_array, int *len_array);
void PrintFldXml     (FILE *fpr, PANEL *pm, coord x, int *ind_array, int *len_array, bool bFooter, int maxLevel);
void PrintColumnStyle(FILE *fpr, PANEL *pm, int *ind_array, int *len_array, int maxLevel);

// Получить формат поля для вывода в xml
const char *GetExcelStyleFormat(char *buff, int type, int point);

// -----------------------------------------------------------------------------
// Для подсчета сумм по полям денежного типа
typedef struct fmVal
      {
       void  *val;  // Значение суммы
       int    ind;  // Индекс денежного поля в строке
      } fmVal;

void PrintFoot(FILE *fpr, PANEL *p, fmVal *sarray, int nfmoney, coord x, int lens,
               int *ind_array, int *len_array);

fmVal *MakeSumArray(PANEL *p, int *n);
void AddToSumArray(PANEL *p, fmVal *sarray, int n);
void FreeSumArray(fmVal *sarray, int n);

void GetEWColInfo(PANEL *p, int *len_arr, int *ind_arr);

/************************************************************************/


_NALGFUNC int appStart          // Монитор прикладных модулей
(
 const char   *szAppName,       // Имя приложения
 const char   *szWorkPlaceName, // Название рабочего места
 const char   *szCopyRight,     // JV RStyle
 const char   *szGName,         // Общее имя ресурсов приложения
 const char   *szGroup,         // Имя сетевой группы
 uchar         cIdent,          // Идентификатор программы
 na_openoper_t pOpenOper,       // Процедура открытия файла пользователей
 na_execproc_t exec_proc,       // Обработчик меню
 na_appproc_t  start_proc,      // Предварительная иниц. (до inoper)
 na_appproc_t  beg_log,         // Процедура выполняемая до активизации меню
 na_appproc_t  end_log          // Процедура выполняемая после освобождения меню
);

_NALGFUNC int appStartProc      // Монитор прикладных модулей
(
 const char   *szAppName,       // Имя приложения
 const char   *szWorkPlaceName, // Название рабочего места
 const char   *szCopyRight,     // JV RStyle
 const char   *szGName,         // Общее имя ресурсов приложения
 const char   *szGroup,         // Имя сетевой группы
 uchar         cIdent,          // Идентификатор программы
 na_openoper_t pOpenOper,       // Процедура открытия файла пользователей
 na_execproc_t exec_proc,       // Обработчик меню
 na_appproc_t  start_proc,      // Предварительная иниц. (до inoper)
 na_appproc_t  beg_log,         // Процедура выполняемая до активизации меню
 na_appproc_t  end_log,         // Процедура выполняемая после освобождения меню
 na_menuproc_t menuproc         // Процедура статического меню
);

/************************************************************************/

_NALGFUNC void SetSystEdit(bool);


_NALGFUNC int SaveRestScrlParm(
                               PANEL    *mp,            // Указатель на панель записи скроллинга
                               void     *UserParm,      // Указатель на буфер с параметрами
                               size_t   *UserParmSize,  // Размер буфера
                               int       IsSave         // 1 - сохранить, 0 - восстановить
                              );

_NALGFUNC void EnableColWidthSave(bool);

/************************************************************************/

_NALGFUNC na_oplogrec_t  SetOpLogRec(na_oplogrec_t proc);   // Установить функцию записи в журнал
_NALGFUNC na_oplogproc_t SetOpLogProc(na_oplogproc_t proc); // Установить функцию записи в журнал

_NALGFUNC const char    *SetOperIdFormat(const char *fmt);  // Установить формат objectID для журнализации
                                                            // действий над пользователем

void BuildRemoteFileName(char* remoteName, const char *fl);
void BuildLocalFileName(char* localName, const char *prDir, const char *fl);

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */