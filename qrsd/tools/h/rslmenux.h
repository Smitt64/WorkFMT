/*-----------------------------------------------------------------------*-
 File Name   : rslmenu.h

 Sybsystem   :
 Description :

 Copyright (c) 1991 - 1994 by R-Style
 All Rights Reserved.
------------------------------------------------------------------------*/

#ifndef _RSLMENUX_H
#define _RSLMENUX_H

#ifdef __cplusplus
extern "C" {
#endif

int MenuList(VALUE *array, int nElem, int width, int height, char *prompt, char *head,
             int x, int y, int curPos, TGenObject *obj);

#ifdef __cplusplus
}
#endif

#endif // _RSLMENUX_H

/* <-- EOF --> */