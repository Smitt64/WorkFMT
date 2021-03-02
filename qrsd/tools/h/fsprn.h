/*-----------------------------------------------------------------------*-

 File Name   : fsprn.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 April 29,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __FSPRN_H
#define __FSPRN_H

#include "platform.h"
#include "pexpdef.h"

RSL_MAKE_HANDLE(RSLPRNHANDLE);

#ifdef __cplusplus
extern "C" {
#endif

_RSPNLEXP RSLPRNHANDLE fs_openPrinter(const char *prnName);
_RSPNLEXP int          fs_closePrinter(RSLPRNHANDLE hd);
_RSPNLEXP int          fs_putPrnChar  (RSLPRNHANDLE hd, int ch);
_RSPNLEXP int          fs_putPrnBuff  (RSLPRNHANDLE hd, const char *buff, int sz);

#ifdef __cplusplus
}
#endif

#endif // __FSPRN_H

/* <-- EOF --> */