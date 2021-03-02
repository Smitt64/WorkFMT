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

_TOOLEXP int ParseDate  /* �८�ࠧ����� ��ப� � ���� btrive           */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨                         */
 bdate *d,              /* ��� � �ଠ� btrive                        */
 int delim              /* �������⥫�                                  */
);

_TOOLEXP int Str2bdate  /* �८�ࠧ����� ��ப� � ���� btrive           */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨                         */
 bdate *d,              /* ��� � �ଠ� btrive                        */
 int delim              /* �������⥫�                                  */
);

_TOOLEXP int Str2bdate2           /* �८�ࠧ����� ��ப� � ���� btrive           */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨                         */
 bdate *d,              /* ��� � �ଠ� btrieve                       */
 int delim,
 int sql
);

_TOOLEXP int stobd      /* �८�ࠧ����� ��ப� � ���� btrive     */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨            */
 bdate *d               /* ��� � �ଠ� btrive        */
);

_TOOLEXP int stobtime     /* �८�ࠧ����� ��ப� �� �६� btrive      */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨            */
 btime *d               /* �६� � �ଠ� btrive       */
);


_TOOLEXP int stobst     /* �८�ࠧ����� ��ப� �� �६� btrive      */
(
 char  *s,              /* ��ப� ��� ��ࠡ�⪨            */
 btime *d               /* �६� � �ଠ� btrive       */
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