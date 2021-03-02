/*******************************************************************************
 FILE         :   RSSTRPOS.H

 COPYRIGHT    :   R-Style Software Lab, 1999

 DESCRIPTION  :   Get character index in the string

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   15 Apr 1999

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/rsstrpos.h $
                  $Revision: 1 $
                  $Date: 15.04.99 21:18 $
                  $Author: Markovitch $
                  $Modtime: 15.04.99 21:16 $
*******************************************************************************/

#ifndef __RSSTRPOS_H
#define __RSSTRPOS_H

#include <tooldef.h>

// strchrpos() - get position of character in string
//
// Returns position of the first occurence of the character c in the string str.
// The null-terminator is considered to be part of the string.
// Parameters:
//    str   -  pointer to the string
//    c     -  character to search
// Return Value
//    strchrpos returns an index (beginning from 0) or -1 if c is not in str.
//

RSL_CFUNC _TOOLFUNC int strchrpos(const char *str, int c);


#endif // __RSSTRPOS_H

/* <-- EOF --> */