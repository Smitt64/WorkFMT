//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naalg.h                        January 24,1994
//
//  Description      : Ведение меню
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
// 02.03.99 Мол.
//   В OPERMENU часть резерва задействована под хранение параметров
//
//------------------------------------------------------------------@H-

#ifndef __NAMENU_H
#define __NAMENU_H

#define _NAMENU_H_

#include "na/naitem.h"
#include "na/nadef.h"
#include "rstypes.h"

#ifdef __cplusplus
 extern "C" {
#endif

#include <packpshn.h>

// -----------------------------------------------------------------------------
#define  PPSZ_StrParm  81 // Длина строки параметров

// -----------------------------------------------------------------------------
typedef struct // Параметры системного модуля
      {
       char   szNameProc[81];    // Название модуля в списке
       char   szNameDirTxt[81];  // Полное имя текстового файла вывода
       char   szNameFile[81];    // Название файла для интерпр.отчетов и вып.модулей
       char   szStrParm[PPSZ_StrParm]; // Строка параметров
       char   szParmReport[250]; // Список параметров
       char   szSeq[160];        // Строка управляющих символов принтера
       int16  iPageSize;         // Длина страницы
       int16  iDbl;              // Количество отчетов в ширину
       int16  iGetParm;          // Признак для ввода параметров
       int16  iKindMethod;       // Вид выполнения
       int16  iKindProgram;      // Вид программы
       char   cContext;          // Признак контекста отчета
      } PARMPLAY;

#include <packpop.h>

// -----------------------------------------------------------------------------
typedef int (* na_execproc_t)(int iCaseItem, PARMPLAY *);  // Обработчик меню

//*******************************************************************

#include <packpsh1.h>

// -----------------------------------------------
// Список выборов программ операциониста
typedef struct
      {
       db_int16  iOper;            // Номер операциониста
       db_int16  iIdentProgram;    // Идентификатор приложения
       db_int16  iNumberPoint;     // Уникальный номер узла меню
       db_int16  iNumberFather;    // Номер вышестоящего пункта в меню
       db_int16  iNumberLine;      // Номер в подменю
       db_int16  iCaseItem;        // Системный номер выбора
       char      cSystemItem;      // Признак системного модуля
       char      szNameItem[81];   // Название выбора в списке
       char      szNamePrompt[81]; // Подсказка к названию выбора
       db_int16  iHelp;            // Номер страницы помощи
       char      szParam[32];      // Параметры пункта меню (добавляются к параметрам ITEMSYST)
       db_int16  iProgItem;        // Приложение из которого взят модуль
       char      cNoBorrow;        // Признак запрета заимствования
       char      szReserve[3];     // Резерв
      } OPERMENU;

// -----------------------------------------------
enum OMKEY
   {
    OMSCRL = 0,                    // iOper, iIdentProgram, iInumberPoint
    OMMENU                         // iOper, iIdentProgram, iNumberFather, iNumberLine
   };

// -----------------------------------------------
typedef union // Ключи справочника выборов
      {
       struct
            {
             db_baseint16  iOper;        // Номер операциониста
             db_baseint16  iIdentProgram;// Идентификатор приложения
             db_baseint16  iNumberPoint; // Уникальный номер узла меню
            } scrl;

       struct
            {
             db_baseint16  iOper;        // Номер операциониста
             db_baseint16  iIdentProgram;// Идентификатор приложения
             db_baseint16  iNumberFather;// Номер вышестоящего пункта в меню
             db_baseint16  iNumberLine;  // Номер в подменю
            } menu;
      } OPERMENUKEY;

#include <packpop.h>

_NALGFUNC void  MKF_omenu(void);
_NALGFUNC void  RKF_omenu(void);
_NALGFUNC void  SKF_omenu           // Установка ключей и фильтров файла выборов
                         (
                          int  numkey,                   // Номер ключа
                          int  iOper,                    // Номер операциониста
                          int  iIdentProgram,            // Идентификатор приложения
                          int  iNumber                   // Номер узла
                         );

_NALGFUNC int  iOpenOM(int iOpenMode);  // opermenu.dbt

// Выполение программ из списка
_NALGFUNC int  iListMenuOper(na_execproc_t exec_proc);

// Создание меню
_NALGFUNC int  MenuFileCnstr(MENU **mHor, int iOper, uchar cIdentProgram);

// Освобождение меню
_NALGFUNC int  MenuFileDestr(MENU *mHor, int iN);

// Параметры из структуры OPERMENU
typedef struct
      {
       int    iProgItem;    // Приложение из которого взят модуль
       char  *szParam;      // Параметры пункта меню (добавляются к параметрам ITEMSYST)
      } OMENUPARM;

_NALGFUNC OMENUPARM *FindOMenuParm(MENU *addr);

//*******************************************************************

#include <packpsh1.h>

// -----------------------------------------------
// Параметры индивидуальной настройки
typedef union
      {
       char  Reserve[300];

       struct                        // Настройка отчета
            {
             char          szParmItem[250];    // Список параметров выбора
             char          szNmSeq[15];        // Название строки с параметрами принтера
             db_baseint16  iPageSize;          // Длина страницы
             db_baseint16  iDbl;               // Количество отчетов в ширину
            } rept;

       struct                        // Настройка процедуры
            {
             char          szParmItem[250];    // Список параметров выбора
            } proc;
      } PROGPARM;

// -----------------------------------------------
// Список индивидуальных настроек
typedef struct
      {
       db_int16  iOper;               // Номер операциониста
       uchar     cIdentProgram;       // Идентификатор приложения
       db_int16  iCaseItem;           // Системный номер выбора
       char      cSystemItem;         // Признак системного модуля
       PROGPARM  parm;                // Параметры настройки
      } OPERPARM;

// -----------------------------------------------
// Ключи справочника настройки
typedef union
      {
       struct
            {
             db_baseint16  iOper;          // Номер операциониста
             uchar         cIdentProgram;  // Идентификатор приложения
             db_baseint16  iCaseItem;      // Системный номер выбора
             char          cSystemItem;    // Признак системного модуля
            } ident;
      } OPERPARMKEY;

#include <packpop.h>

// -----------------------------------------------------------------------------
_NALGFUNC void SKF_oparm           // Установка ключей и фильтров файла настройки
                        (
                         int   numkey,                  // Номер ключа
                         int   iOper,                   // Номер операциониста
                         uchar cIdentProgram,           // Идентификатор приложения
                         int   iCaseItem,               // Системный номер выбора
                         char  cSystemItem              // Признак системного модуля
                        );

_NALGFUNC int iOpenOP(int iOpenMode);  // operparm.dbt

// Подготовка к редактированию параметров
_NALGFUNC int iPreEditParm(OPERMENU *om);

_NALGFUNC int iPreEditParmEx(OPERMENU *om, PROGPARM *parm);

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */