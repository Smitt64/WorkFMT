//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naitem.h                        January 24,1994
//
//  Description      : Ведение справочника системных модулей
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
//            Замена int на int16
//------------------------------------------------------------------@H-

#ifndef __NAITEM_H
#define __NAITEM_H

#define _NAITEM_H_

#include <systypes.h>

#include "desktop.h"
#include "na/nadef.h"


// -----------------------------------------------------------------------------
#include <packpsh1.h>

// Параметры глобальной настройки выбора
typedef union
      {
       char  Reserve[400];

       struct                         // Настройка отчета
            {
             char          szParmReport[250];   // Список параметров выбора
             char          szNameTxt[9];        // Название текстового файла вывода
             char          szNameFile[81];      // Название файла для интерпр.отчетов и выполн.модулей
             char          szNmSeq[15];         // Название строки с параметрами принтера
             db_baseint16  iPageSize;           // Длина страницы
             db_baseint16  iDbl;                // Количество отчетов в ширину
             char          cContext;            // Контекст программы
             db_baseint16  iHelp;               // Номер страницы помощи
            } rept;

       struct                         // Настройка несистемной программы
            {
             char          szNameFile[81];      // Название файла для интерпр.отчетов и выполн.модулей
             char          szStrParm[81];       // Строка параметров
             db_baseint16  iHelp;               // Номер страницы помощи
            } exec;

      // 20.2.95 VC Внесена страница help
       struct                         // Настройка процедуры
            {
             char          szParmItem[250];     // Список параметров выбора
             db_baseint16  iHelp;               // Номер страницы помощи
            } proc;

      } ITEMPARM;

// -----------------------------------------------
typedef struct                  // Справочник программных модулей банка
      {
       uchar     cIdentProgram;        // Идентификатор программы
       db_int16  iCaseItem;            // Системный номер выбора
       db_int16  iKindMethod;          // Вид выполнения
       db_int16  iKindProgram;         // Вид программы
       db_int16  iHelp;                // Номер страницы помощи
       char      Reserve[13];          // Резерв
       char      szNameItem[61];       // Название выбора
       ITEMPARM  parm;                 // Параметры выбора
      } ITEM;

// -----------------------------------------------
typedef union                   // Ключи справочника модулей
      {
       struct
            {
             uchar         cIdentProgram;   // Идентификатор приложения
             db_baseint16  iCaseItem;       // Системный номер выбора
            } item;
      } ITEMKEY;


#ifdef __cplusplus
 extern "C" {
#endif

// Установка ключей и фильтров файла системных модулей
_NALGFUNC void SKF_sitem(
                         int   numkey,                  // Номер ключа
                         uchar cIdentProgram            // Идентификатор программы
                        );

_NALGFUNC int iOpenSItem(int iOpenMode); // itemsyst.dbt

// Установка ключей и фильтров файла системных модулей
_NALGFUNC void SKF_uitem(
                         int   numkey,                  // Номер ключа
                         uchar cIdentProgram            // Идентификатор программы
                        );

_NALGFUNC int iOpenUItem(int iOpenMode); // itemuser.dbt

_NALGFUNC int iFindSItem(uchar cIdentProgram, int iCaseItem, void *buff);
_NALGFUNC int iFindUItem(uchar cIdentProgram, int iCaseItem, void *buff);

_NALGFUNC int iListUItem(
                         coord x,                       // Координаты панели и количество линий
                         coord y,
                         coord l,
                         void *buff                     // Буфер для записи найденного
                        );

_NALGFUNC int iListSItem(
                         coord x,                       // Координаты панели и количество линий
                         coord y,
                         coord l,
                         void *buff                     // Буфер для записи найденного
                        );

_NALGFUNC void vNameMethod(char *Name, int iKindMethod);

typedef int (* na_edititem_t)(ITEM *);    // Редактирование параметров

// Программа редактирования списка модулей
_NALGFUNC int edit_item(
                        int           system,
                        na_edititem_t editparm,                 // Редактирование параметров
                        int           user_edit = 1
                       );

#ifdef __cplusplus
 } ;
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */