//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naalg.h                        January 24,1994
//
//  Description      : Ведение справочника алгоритмов
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
//------------------------------------------------------------------@H-

#ifndef __NAALG_H
#define __NAALG_H

#define _NAALG_H_

#include <systypes.h>

#include "platform.h"
#include "na/nadef.h"

#include <packpsh1.h>

// Алгоритмы
enum NAKINDALG
   {
    TYPEMETHOD  = 19,              // 19 Типы выполнения
    KINDPROGRAM = 20               // 20 Виды программ
   };

// Справочник алгоритмов
typedef struct
      {
       db_int16   iTypeAlg;           // Тип алгоритма
       db_int16   iNumberAlg;         // Номер алгоритма
       char       szNameAlg[41];      // Название алгоритма
       db_int16   iLenName;           // Длина названия
       db_int16   iQuantAlg;          // Количество алгоритмов типа
       char       Reserve[8];         // Резерв
      } NAMEALG;

// Буфер ключа типа
typedef union
      {
       struct                         // 0 - ключ
            {
             db_baseint16  iTypeAlg;        // Тип алгоритма
             db_baseint16  iNumberAlg;      // Номер алгоритма
            } type;
      } NAMEALGKEY;


#ifdef __cplusplus
 extern "C" {
#endif


_NALGFUNC int  iOpenNA(int iOpenMode);

// Установка ключей и фильтров файла алгоритмов
_NALGFUNC void  SKF_namealg(
                            int  numkey,                   // Номер ключа
                            int  type                      // Номер типа
                           );

// Выбрать алгоритм
_NALGFUNC int  list_alg(
                        int   kind,                    // Вид алгоритма
                        int   item,                    // текущий выбор из списка
                        char *nameitem,                // название выбора
                        coord x,                       // координаты списка
                        coord y
                       );

// Получить название алгоритма
_NALGFUNC char *name_alg(
                         int  kind,                     // Вид алгоритма
                         int  item                      // текущий выбор из списка
                        );

//    Номера типов
enum NANUMTYPES
   {
   #ifdef RSL_MD_FLAT
       TA_PROGRAM = 22,               // 22 32-х разрядные приложения
   #else
       TA_PROGRAM = 12,               // 12 Типы приложений
   #endif

    TA_PERSON     = 13,               // 13 Уровни доступов
    TA_DLLPROG    = 32                // 32 DLL-приложения
   };

// Справочник типов
typedef struct
      {
       db_int16 iNumType;             // Номер типа
       char     Type;                 // Тип
       char     Name_Type[18];        // Название типа
       char     Contens[61];          // Характеристика
      } TYPEAC;

// Буфер ключа типа
typedef union
      {
       struct                       // 0 - ключ
            {
             db_baseint16 iNumType;        // Номер плана
             char         Type;            // Номер балансового счета
            } nt;

       struct                       // 1 - ключ
            {
             db_baseint16 iNumType;        // Номер плана
             char         Name_Type[18];   // Название типа
            } nn;
      } TYPEACKEY;

_NALGFUNC int  iOpenTA(int iOpenMode);

// Установка ключей и фильтров файла типов
_NALGFUNC void  SKF_typeac(
                           int  numkey,                   // Номер ключа
                           int  num                       // Номер типа
                          );

_NALGFUNC int  iFindType(
                         int   iNumType,                // Номер типа
                         char  Type,                    // Тип
                         void *buff                     // Буфер для записи найденного
                        );

// Программа выбора типа
_NALGFUNC int  iChsType(
                        int   iNumType,                // Номер типа
                        char  *Type,                   // Редактируемый тип
                        coord x,                       // Координаты панели и количество линий
                        coord y,
                        coord l,
                        void *buff                     // Буфер для записи найденного
                       );

// Программа выбора типа
_NALGFUNC int  iChsTypeM(
                         int   iNumType,                // Номер типа
                         char  *Type,                   // Редактируемый тип
                         coord x,                       // Координаты панели и количество линий
                         coord y,
                         coord l,
                         void *buff                     // Буфер для записи найденного
                        );

// Программа просмотра типа
_NALGFUNC int  iViewType(
                         int   iNumType,                // Номер типа
                         char  *Type,                   // Просматриваемый тип
                         coord x,                       // Координаты панели и количество линий
                         coord y,
                         coord l
                        );

// Выбор типов
_NALGFUNC int  iListType(
                         int      iNumType,             // Номер типа
                         char    *Type,                 // Редактируемый тип
                         int      iMaxType,             // Количество возможных типов
                         coord    x,                    // Координаты верхнего левого угла меню
                         coord    y,
                         coord    l
                        );

// Определение наличия типа
_NALGFUNC int  iIsType(
                       const char *ListType,            // Строка типов
                       char        type                 // Искомый тип
                      );

// Редактирование типов
_NALGFUNC int  edit_typeac(
                           int iNumType,                 // Номер типа
                           char *Name                    // Название типа (заголовок скролинга)
                          );

// Редактирование типов
_NALGFUNC int  edit_typeacEx(
                             int iNumType,                  // Номер типа
                             char *Name,                    // Название типа (заголовок скролинга)
                             int RO_Mode                    // ReadOnly Mode
                            );

#ifdef __cplusplus
 } ;
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */