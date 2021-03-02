/*******************************************************************************
 FILE         :   JDELAY.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   JDELAY (portable delay function) prototype

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   15 Sep 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/jdelay.h $
                  $Revision: 1 $
                  $Date: 15.09.98 14:35 $
                  $Author: Markovitch $
                  $Modtime: 15.09.98 14:30 $
*******************************************************************************/

#ifndef __JDELAY_H
#define __JDELAY_H

#include <tooldef.h>

#ifdef __cplusplus
extern "C" {
#endif

//
// Wait by given amount of time (in milliseconds)
//
_TOOLFUNC void JDELAY(int milliseconds);

#ifdef __cplusplus
}
#endif

#endif // __JDELAY_H

/* <-- EOF --> */