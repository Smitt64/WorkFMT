//***********************************************************************
//                    Инструментарий для работы с датами
//                            VECsoft 13.11.1992
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/days.h $
//                  $Revision: 4 $
//                  $Date: 20.10.98 20:29 $
//                  $Author: Stepankov $
//                  $Modtime: 20.10.98 18:48 $
//***********************************************************************

#ifndef __DAYS_H
#define __DAYS_H

#include "rsdates.h"
#include "menu.h"
#include "apptldef.h"

#ifdef __cplusplus
 extern "C" {
#endif

// Выбрать месяц (квартал) из списка
_APPTLFUNC int list_date(
                         char  kind,                     // вид  списка  ( М / К )
                         int   item,                     // текущий выбор из списка
                         char  *maneitem,                // название выбора
                         coord  x,                       // координаты списка
                         coord  y
                        );

// Выбрать название месяца (квартала)
_APPTLFUNC char *name_date(
                           char kind,                    // вид  списка  ( М / К )
                           int  item                     // текущий выбор из списка
                          );

// Перевести дату в строку
_APPTLFUNC void date_to_str(
                            int    kind,                 // вид даты  0 - дд.Месяц.гггг
                                                         //           1 - День.Месяц
                            bdate *dt,                   // дата
                            char  *str                   // строка названия (макс.длина 40 символов)
                           );

#ifdef __cplusplus
 } ;
#endif

#endif // __DAYS_H

/* <-- EOF --> */