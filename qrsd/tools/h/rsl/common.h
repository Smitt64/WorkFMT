/*-----------------------------------------------------------------------*-

 File Name   : common.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsl/common.h $
                  $Revision: 17 $
                  $Date: 9/06/00 2:49p $
                  $Author: Tokmashev $
                  $Modtime: 9/06/00 1:01p $

 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 10,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __RSLCOMMON_H
#define __RSLCOMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "platform.h"

#ifndef __EXT_H
    #include "dlm.h"
#endif

#include "listobj.h"  // Common tools
#include "rstring.h"

#ifndef RSL_NO_PCODE
    #define RSL_PCODE2
#endif

#ifdef RSL_PL_WIN64
    #define USE_DBGHANDLE32
#endif


#include "rsl/expdef.h"
#include "rsl/itypes.h"  // Common RSL types
#include "rsl/msg.h"
#include "rsl/ierror.h"

#include "rsl/rslcatch.h"
#include "rsl/iutil.h"


#include "rsl/iscaner.h"
#include "rsl/rsgenobj.h"

#ifndef NO_BTR
    #include "rsl/ibfile.h"
#endif

#include "rsl/isymbol.h"

#include "rsl/rslobj.h"

#include "rsl/osfun.h"
#include <signal.h>

#ifdef USE_FDECIMAL
    #include "rsl/idecimal.hpp"
#endif

#include "traceinit.h"

#include "rsl/idata.h"
#include "rsl/krnlintf.h"

#endif // __RSLCOMMON_H

/* <-- EOF --> */