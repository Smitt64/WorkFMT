/*-----------------------------------------------------------------------*-

 File Name   : bdstrm.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: /RS-Tools/Old_tools/tools/h/bdstrm.h $
               $Revision: 2 $
               $Date: 7.09.00 12:22 $
               $Author: Kormukhin $
               $Modtime: 7.09.00 12:05 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 September 5,2000  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __BDSTRM_H
#define __BDSTRM_H

#include <stdio.h>
#include <platform.h>
#include <rsiostrm.h>
#include <bdate.h>

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const bdate &d)
{
 char  buf[14];


 sprintf(buf, "%04hu-%02hu-%02hu", (short)d.year, (short)d.mon, (short)d.day);

 return os << buf;
}

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const btime &t)
{
 char  buf[16];


 sprintf(buf, "%02hu.%02hu.%02hu.%02hu", (short)t.hour, (short)t.min, (short)t.sec, (short)t.hundr);

 return os << buf;
}

// -----------------------------------------------------------------------------
inline RSL_STD::ostream &operator << (RSL_STD::ostream &os, const btimestamp &ts)
{
 return os << ts.date << '-' << ts.time;
}

#endif // __BDSTRM_H

/* <-- EOF --> */