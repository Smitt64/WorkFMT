/*************************************************************************
        File Name               : ipcode_c.h                    25.05.2000
        Programmer              : A.Tokmashev

        Subsystem               : Interpreter RSL
        Description             : C-wrapper for generator P-Code.

        Source                  : ---

        Library(s)              : ---

        Copyright(C) 1991 - 2000 by R-Style Software Lab.
        All Rights Reserved.

*************************************************************************/

#ifndef __IPCODE_C_H
#define __IPCODE_C_H

#ifndef RSL_NO_PCODE

    #include "platform.h"

    #include <packpsh1.h>

    //////////////////////////////////////////////////////////////////////////
    // Begin - Symbol item.
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // End - Symbol item.
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // Begin - PCode Segment item.
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
    // End - PCode Segment item.
    //////////////////////////////////////////////////////////////////////////

    #ifdef __cplusplus
    extern "C" {
    #endif

    #ifdef __cplusplus
    }
    #endif

    #include <packpop.h>

#endif // RSL_NO_PCODE

#endif // __IPCODE_C_H

/* <-- EOF --> */