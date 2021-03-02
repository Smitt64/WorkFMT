/*******************************************************************************
 FILE         :   NADEF.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Macro definitions for NA library exports/imports

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   16 Oct 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/na/nadef.h $
                  $Revision: 1 $
                  $Date: 19.10.98 19:26 $
                  $Author: Markovitch $
                  $Modtime: 19.10.98 18:30 $
*******************************************************************************/

#ifndef __NADEF_H
#define __NADEF_H

#include "platform.h"

#if defined(__RSNALG_BUILD)

    #if defined(RSL_MD_DLL)
        #define  _NALGEXP   __EXPORT2
        #define  _NALGFUNC  __EXPORT2
        #define  _NALGDATA  __EXPORT2
    #else
        #define  _NALGEXP   __IMPORT2
        #define  _NALGFUNC  __IMPORT2
        #define  _NALGDATA  __IMPORT2
    #endif

#else

    #if defined(__RSRTL_DLL) && !defined(__RSNALG_DLL)
        #define  __RSNALG_DLL
    #endif

    #ifdef __RSNALG_DLL
        #define  _NALGEXP   __IMPORT2
        #define  _NALGFUNC  __IMPORT2
        #define  _NALGDATA  __IMPORT2
    #else   // Use Static lib or use WIN16 DLL
        #define  _NALGEXP
        #define  _NALGFUNC
        #define  _NALGDATA
    #endif

#endif

#endif // __NADEF_H

/* <-- EOF --> */