/*-----------------------------------------------------------------------*-

 File Name   : strtobd.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/strtobd.h $
                  $Revision: 6 $
                  $Date: 15.09.99 13:28 $
                  $Author: Kubrin $
                  $Modtime: 15.09.99 11:07 $

 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 February 17,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef  __STRTOBD_H
#define __STRTOBD_H

#include "tooldef.h"
#include <bdate.h>

#ifdef __cplusplus
extern "C" {
#endif

_TOOLEXP int ParseDate  /* Преобразовать строку в дату btrive           */
(
 char  *s,              /* Строка для обработки                         */
 bdate *d,              /* Дата в формате btrive                        */
 int delim              /* Разделитель                                  */
);

_TOOLEXP int Str2bdate  /* Преобразовать строку в дату btrive           */
(
 char  *s,              /* Строка для обработки                         */
 bdate *d,              /* Дата в формате btrive                        */
 int delim              /* Разделитель                                  */
);

_TOOLEXP int Str2bdate2           /* Преобразовать строку в дату btrive           */
(
 char  *s,              /* Строка для обработки                         */
 bdate *d,              /* Дата в формате btrieve                       */
 int delim,
 int sql
);

_TOOLEXP int stobd      /* Преобразовать строку в дату btrive     */
(
 char  *s,              /* Строка для обработки            */
 bdate *d               /* Дата в формате btrive        */
);

_TOOLEXP int stobtime     /* Преобразовать строку во время btrive      */
(
 char  *s,              /* Строка для обработки            */
 btime *d               /* Время в формате btrive       */
);


_TOOLEXP int stobst     /* Преобразовать строку во время btrive      */
(
 char  *s,              /* Строка для обработки            */
 btime *d               /* Время в формате btrive       */
);

int IsValidDate2 (bdate *d,int isSQL);

_TOOLEXP int IsValidDate(bdate *d);
_TOOLEXP int RslSetBreakYear(int newVal);
_TOOLEXP int RslEnable32December(int enable);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */