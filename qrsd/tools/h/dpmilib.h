/*-----------------------------------------------------------------------*-

 File Name   : dpmilib.h 

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 December 4,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __DPMILIB_H
#define __DPMILIB_H

#include "tooldef.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef RSL_MD_DPMI32

    _TOOLEXP void forceLinkRslDllMain (void);

    _TOOLEXP HINSTANCE LoadLibraryDPMI32(const char *name);
    _TOOLEXP BOOL      FreeLibraryDPMI32(HMODULE mod);

#else

    #define  forceLinkRslDllMain()
    #define  LoadLibraryDPMI32      LoadLibrary
    #define  FreeLibraryDPMI32      FreeLibrary

#endif

#ifdef __cplusplus
}
#endif

#endif  // __DPMILIB_H

/* <-- EOF --> */