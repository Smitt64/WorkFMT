/*************************************************************************
   File Name      : rsdll.h
   Programmer     : Alexey Lebedev

   Creation       : 28 Feb 2002
   Description    : LoadLibrary, FreeLibrary and GetProcAddress

   Copyright(C) 2002 by R-Style Software Lab.
   All Rights Reserved.

*************************************************************************/

#ifndef __RSDLL_H
#define __RSDLL_H

#include "platform.h"


// -----------------------------------------------------------------------------
#ifdef RSL_PL_WIN32

    #ifndef _INC_WINDOWS
        #define STRICT

        #include <windows.h>
    #endif

    // -----------------------------------------------
    #ifdef RSL_MD_DPMI32

        #include "dpmilib.h"

        #ifdef LoadLibrary
            #undef LoadLibrary
        #endif

        #ifdef FreeLibrary
            #undef FreeLibrary
        #endif

        #define  LoadLibrary  LoadLibraryDPMI32
        #define  FreeLibrary  FreeLibraryDPMI32

    #endif //RSL_MD_DPMI32

    #define  MAKEDLLNAME(name)  (TEXT(name".dll"))

#endif //RSL_PL_WIN32

// -----------------------------------------------------------
#ifdef RSL_MD_SUN

    #include <dlfcn.h>
    #include <string.h>

    typedef void *HMODULE;
    typedef void (*FARPROC)();
    typedef int BOOL;

    #define  LoadLibrary(name)                 (dlopen(name, RTLD_LAZY|RTLD_WORLD))
    #define  FreeLibrary(hmod)                 (!dlclose(hmod))
    #define  GetProcAddress(hmod, func)        (dlsym(hmod, func))
    #define  GetModuleFileName(hmod, buf, sz)  (dlinfo(hmod, RTLD_DI_ORIGIN, buf), strlen(buf))

    #define  MAKEDLLNAME(name)                 (name".so")

#endif //RSL_MD_SUN

#endif // __RSDLL_H

/* <-- EOF --> */