//-@H------------------------------------------------------------------
//                 Copyright (c) JV R-Style 1993,94
//
//  File Name        : naoper.h                        January 24,1994
//
//  Description      : Ведение справочника операционистов
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
//  October 25, 1994          : Любимов А.Ю. - внесение пользовательского
//                              расширения в структуру записи OPERMAN
// March 31,1995   Serg Kubrin (K78)
//            Замена int на int16
//------------------------------------------------------------------@H-

#ifndef __NAOPER_H
#define __NAOPER_H

#define _NAOPER_H_

#include "na/namenu.h"
#include "na/nadef.h"
#include "rstypes.h"

/*
 * Если структура OPERMAN должна содержать нестандартное расширение,
 * то для этого необходимо:
 *
 * 1)

 * ПЕРЕД подключением H-файла "na/naoper.h" определить структуру
 * своего расширения:
 *
 * typedef struct {
 *                 <user-field-1>;
 *                 ...
 *                 <user-field-N>;
 *                }
 *                 NAOPER_VPART;
 *
 * Тут же определить переменную _NAOPER_VPART_
 * (обратите внимание на знаки подчеркивания):
 *
 * #define _NAOPER_VPART_
 *
 *
 * 2)
 *
 * Для обращения к полям расширения следует использовать синтаксис:
 *
 * (OPERMAN *)pOper->Reserv.vpart.<user-field-name>
 *                      ^     ^
 *                      |     |
 *                      |     +-- фиксированное имя пользовательского расширения
 *                      |
 *                      +-- фикс. имя union'а, содержащего структуру резерва
 *                          в записи OPERMAN
 *
 */

// -----------------------------------------------------------------------------
#define  RESERVELEN       230   // По запросу 2944 увеличено с 130 до 230
#define  IDENTPROGRAMLEN   51

#define  D_MAX_NAME_OPER   71   // По запросу 3246 увеличено с 36 до 71
#define  D_MAX_PAROL        9


#include <packpsh1.h>

// -----------------------------------------------------------------------------
#if !defined (_NAOPER_VPART_)
    typedef struct
          {
           char  UserType[RESERVELEN];
          } NAOPER_VPART;
    //#error "NAOPER_VPART: Variable part not defined!"
    // Kireev 3.7.00 Раз уж это не ошибка (#error закомментарено), то надо
    // Просигнализировать, что NAOPER_VPART определена, то есть определить _NAOPER_VPART_
    #define _NAOPER_VPART_

#else
    //
    // Проверка длины не работает для *.C
    // (только для *.CPP?)
    //
    //#   if sizeof(NAOPER_VPART) > RESERVLEN
    //#      error "NAOPER_VPART: User defined part of OPERMAN structure is too long"
    //#   endif
#endif


// -----------------------------------------------------------------------------
// Справочник операционистов
typedef struct
      {
       db_int16  Oper;                          // Операционист
       char      Name_Oper[D_MAX_NAME_OPER];    // ФИО операциониста
       char      Parol[D_MAX_PAROL];            // Пароль операциониста
       db_int16  Seek;                          // Доступ
       db_int16  SeekE;                         // Расширенный доступ
       char      cTypePerson;                   // Уровень доступа
       uchar     szIdentProgram[IDENTPROGRAMLEN]; // Список приложений
       db_int16  AhrCarryDay;                   // Признак проводок в архиве
       db_int16  GroupOperF;                    // Начало номеров группы оперов
       db_int16  GroupOperL;                    // Окончание номеров группы оперов
       db_int16  GroupCredF;                    // Начало номеров группы кредитов
       db_int16  GroupCredL;                    // Окончание номеров группы кредитов

       union                                    // Резерв с пользовательскими добавками
           {
            char          _reserved_[RESERVELEN];
            NAOPER_VPART  vpart;
           } Reserv;
      } OPERMAN;

#include <packpop.h>

#ifdef __cplusplus
 extern "C" {
#endif

typedef int (* na_openoper_t)(int OpenMode); // Функция открытия файла операционистов
typedef int (* na_editoper_t)(void);         // Редактирование опера
typedef int (* na_editparm_t)(OPERMENU *);   // Редактирование параметров
typedef int (* na_printmenu_t)(void);        // Редактирование параметров

// Открытие файла
_NALGFUNC int iOpenUser(int iOpenMode, const char *szFile, const char *szPassword);

// Регистрация пользователя
_NALGFUNC int inoper(
                     int O,                         // Номер
                     char *P,                       // Пароль
                     na_openoper_t    OpenOper      // Функция открытия файла операционистов
                    );

// Смена пароля во время сеанса
_NALGFUNC int chparol(na_openoper_t OpenOper);

_NALGFUNC int edit_oper(
                        na_openoper_t    OpenOper,     // Функция открытия файла операционистов
                        na_editoper_t    EditOper,     // Функция редактирования операциониста
                        na_editparm_t    EditParm      // Функция редактирования параметров операциониста
                       );

// Редактирование меню пользователя
_NALGFUNC int edit_menuoper(
                            OPERMAN       *Oper,
                            na_editparm_t  EditParm      // Функция редактирования параметров операциониста
                           );

// Выбрать режим редактирования меню пользователя:
//    0 - скроллинг
//    1 - дерево
_NALGFUNC int SetMenuOperMode(int mode);

_NALGFUNC int edit_oper2(
                         na_openoper_t    OpenOper,     // Функция открытия файла операционистов
                         na_editoper_t    EditOper,     // Функция редактирования операциониста
                         na_editparm_t    EditParm,     // Функция редактирования параметров операциониста
                         na_printmenu_t   PrintMenu     // Функция печати меню операциониста
                        );

// Редактирование меню пользователя
_NALGFUNC int edit_menuoper2(
                             OPERMAN         *Oper,
                             na_editparm_t    EditParm,     // Функция редактирования параметров операциониста
                             na_printmenu_t   PrintMenu     // Функция печати меню операциониста
                            );

_NALGFUNC int iListOper(
                        coord x,                       // Координаты панели и количество линий
                        coord y,
                        coord l,
                        na_openoper_t    OpenOper,     // Функция открытия файла операционистов
                        void *buff                     // Буфер для записи найденного
                       );

#ifdef __cplusplus
 };
#endif

#endif

/* <-- EOF --> */
