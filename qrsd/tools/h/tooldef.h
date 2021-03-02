/*******************************************************************************
 FILE         :   TOOLDEF.H

 COPYRIGHT    :   R-Style, 1996

 DESCRIPTION  :   Macro definitions for TOOLS library exports/imports

 PROGRAMMED BY:   Yakov E. Markovitch (JNS)

 CREATION DATE:   27/11/1996

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/tooldef.h $
                  $Revision: 5 $
                  $Date: 8.09.98 18:12 $
                  $Author: Markovitch $
                  $Modtime: 7.09.98 19:10 $
*******************************************************************************/

#ifndef __TOOLDEF_H
#define __TOOLDEF_H

#include "platform.h"

#ifndef __DLMDEF_H
    #include "dlmdef.h"
#endif

// -----------------------------------------------------------------------------
#if defined(__TOOLS_BUILD)

    #if defined(RSL_MD_DLL)

        #define  _TOOLEXP   __EXPORT2
        #define  _TOOLFUNC  __EXPORT2
        #define  _TOOLDATA  __EXPORT2

    #else

        #define  _TOOLEXP   __IMPORT2
        #define  _TOOLFUNC  __IMPORT2
        #define  _TOOLDATA  __IMPORT2

    #endif

#else

    #if defined(__RSRTL_DLL) && !defined(__TOOLS_DLL)
        #define __TOOLS_DLL
    #endif

    // -----------------------------------------------
    #ifdef __TOOLS_DLL
        #define  _TOOLEXP   __IMPORT2
        #define  _TOOLFUNC  __IMPORT2
        #define  _TOOLDATA  __IMPORT2
    #else   // Use Static lib or use WIN16 DLL
        #define  _TOOLEXP
        #define  _TOOLFUNC
        #define  _TOOLDATA
    #endif

#endif

// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
    #define  TOOLDLM      DLMAPI
    #define  TOOLDLMC     DLMAPIC
    #define  TOOLDLMEXP 
#else
    #define  TOOLDLM      
    #define  TOOLDLMC     
    #define  TOOLDLMEXP   _TOOLEXP
#endif

#endif // __TOOLDEF_H

/* <-- EOF --> */