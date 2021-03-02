/*******************************************************************************
 FILE         :   APPTLDEF.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Macro definitions for APPTOOLS library exports/imports

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   16 Oct 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/apptldef.h $
                  $Revision: 1 $
                  $Date: 19.10.98 19:25 $
                  $Author: Markovitch $
                  $Modtime: 16.10.98 20:54 $
*******************************************************************************/

#ifndef __APPTLDEF_H
#define __APPTLDEF_H

#include "platform.h"

#if defined(__APPTOOLS_BUILD)

    #if defined(RSL_MD_DLL)
        #define  _APPTLEXP   __EXPORT2
        #define  _APPTLFUNC  __EXPORT2
        #define  _APPTLDATA  __EXPORT2
    #else
        #define  _APPTLEXP   __IMPORT2
        #define  _APPTLFUNC  __IMPORT2
        #define  _APPTLDATA  __IMPORT2
    #endif

#else

    #if defined(__RSRTL_DLL) && !defined(__APPTOOLS_DLL)
        #define  __APPTOOLS_DLL
    #endif

    #ifdef __APPTOOLS_DLL
        #define  _APPTLEXP   __IMPORT2
        #define  _APPTLFUNC  __IMPORT2
        #define  _APPTLDATA  __IMPORT2
    #else   // Use Static lib or use WIN16 DLL
        #define  _APPTLEXP
        #define  _APPTLFUNC
        #define  _APPTLDATA
    #endif

#endif

#endif // __APPTLDEF_H

/* <-- EOF --> */