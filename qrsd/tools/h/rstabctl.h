/*************************************************************************
   File Name      : rstabctl.h
   Programmer     : Alexey Lebedev

   Creation       : 13 Mar 2003
   Description    : TabControl.

   Copyright(C) 2003 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

//
//  RS-Bank 5.1
//
//  Попытка сделать что-нибудь среднее между Tab control и
//  многооконностью, да еще и в текстовой консоли
//  с использованием существующего инструмента :-)
//
//  13.09.99 AS
//

#ifndef __RSTABCTL_H
#define __RSTABCTL_H

#include "apptldef.h"

// Виды ресурсов, поддерживаемые в LoopTabCtrl
enum
   {
    DLGTYPE_PANEL,

    // скроллинг всегда запускается через BRunScrol
    DLGTYPE_BSCROL,
    DLGTYPE_BSCROL_READONLY = DLGTYPE_BSCROL // оставим для обратной совместимости
   };


// -----------------------------------------------------------------------------
// Режимы работы TabControl'а
enum
   {
    eTC_Mode_0 = 0,  // 0: не отображать кнопки
    eTC_Mode_1,      // 1: отображать кнопки
    eTC_Mode_2       // 2: не отображать заголовок в EW-режиме (в консоли аналогичен режиму 1)
   };

// Максимально возможное число закладок
// Более не используется - оставлена для совместимости.
#define  MAX_TABCTRL_SCREENS  20


#include "packpshn.h"

// Параметры для LoopTabCtrl
typedef struct TABCTRL_PARMS
      {
       void **dlg;                  // Массив ресурсов
       int   *types;                // Массив типов ресурсов (или NULL)
       int    num_screens;          // Число экранов
       int    start_screen;         // С какого начать (1=10)
       int    ret_screen;           // Из какого вышли
       int    showtabs;             // показывать "закладки" (см. eTC_Mode_)
       char **tabhead;              // названия "закладок"
       char  *RegKeyName;           // название ключа в настройках или NULL
       void (*UserSetCurrentScreen)(int *CurScr, void *UserParm); // IL 25.06.02 возможность пробросить экран
       char  *Phead;
      } TABCTRL_PARMS;

// для внутреннего использования
typedef struct TABCTRL
      {
       PANEL            tabs1;
       PANEL            tabs2;

       int              current_screen;
       int              goto_screen;
       int              goto_field;

       void           **panel_procs;
       void           **user_parms;
       TABCTRL_PARMS   *parms;

       const char      *Phead;
       const char      *Pstatus;

       char            *save;
       int             *tabs_xx;

       bool             canResize;  // TabCtrl может менять размер
      } TABCTRL;

#include "packpop.h"

#ifdef __cplusplus
extern "C" {
#endif

// Создать TabCtrl
_APPTLFUNC TABCTRL *CreateTabCtrl(TABCTRL_PARMS *parms);

// Удалить TabCtrl
_APPTLFUNC void FreeTabCtrl(TABCTRL *tab);

// Вывести TabCtrl
_APPTLFUNC int PutTabCtrl(TABCTRL *tab);

// Убрать TabCtrl
_APPTLFUNC void RemTabCtrl(TABCTRL *tab);

// Запустить цикл обработки для TabCtrl
_APPTLFUNC int UseTabCtrl(TABCTRL *tab);

// Все вышеописанное вместе взятое
_APPTLFUNC int LoopTabCtrl(TABCTRL_PARMS *prm);

// Перейти на закладку в TabCtrl
_APPTLFUNC void GotoTabCtrl(TABCTRL *tab, int ntab, int nfield);

#ifdef __cplusplus
};
#endif

#endif // __RSTABCTL_H

/* <-- EOF --> */