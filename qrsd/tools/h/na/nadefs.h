/*******************************************************************************
 FILE         :   NADEFS.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Common definitions for NA library

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   3 Oct 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/na/nadefs.h $
                  $Revision: 2 $
                  $Date: 4.02.00 16:01 $
                  $Author: Lebedev $
                  $Modtime: 3.02.00 16:28 $
*******************************************************************************/

#ifndef __NADEFS_H
#define __NADEFS_H

#include <platform.h>

#ifndef NAFAR
    #if defined(RSL_MD_FLAT) || !defined(RSL_PL_MS)
        #define  NAFAR
        #define  NANEAR
    #else
        #define  NAFAR   _far
        #define  NANEAR  _near
    #endif
#endif

#endif // __NADEFS_H

/* <-- EOF --> */