/*-----------------------------------------------------------------------*-

 File Name   : iexcept.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 19,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __IEXCEPT_H
#define __IEXCEPT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef bool (*TSafeProc)(void *data);

bool RslSafeCall(TSafeProc proc, void *data);

#ifdef __cplusplus
}
#endif

#endif // __IEXCEPT_H

/* <-- EOF --> */