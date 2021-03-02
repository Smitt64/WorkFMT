/*-----------------------------------------------------------------------*-

 File Name   : common.hpp

 Sybsystem   : RSERVER  
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 April 16,1996  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __COMMON_HPP
#define __COMMON_HPP

#include "platform.h"

#ifndef STRICT
#define STRICT
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "smartlst.hpp"

#ifdef RSL_MD_FLAT
    #ifdef RSL_USE_WINSOCK2 
        #include <winsock2.h>
    #else
        #include <winsock.h>
    #endif

    #include <nb30.h>

    #ifndef _WINSPOOL_
        #include <winspool.h>
    #endif

    #include "hcom\misc.hpp"

    #ifdef RSL_MD_MT
        #include "hcom\threadex.hpp"
        #include "hcom\safelist.hpp"
    #endif
#endif

#endif // __COMMON_HPP

/* <-- EOF --> */