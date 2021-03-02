/*-----------------------------------------------------------------------*-

 File Name   : rsassert.h

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 February 11,2002  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef RSASSERT

#include "platform.h"

/*
При запуске в 3-х режиме выдача диалоговых окон блокирована. Информация об assert выводится 
в лог-файл rsassert.log в каталоге запуска программы.
*/

#ifdef DEBUG

    #ifdef _MSC_VER
        #ifndef _INC_CRTDBG
            #include <crtdbg.h>
        #endif

        #define  RSASSERT(expr)  _ASSERTE(expr)
    #else
        #include <assert.h>

        #define  RSASSERT(expr)  assert(expr)
    #endif

#else  // DEBUG

    #define  RSASSERT(expr)  ((void)0)

#endif // DEBUG


#ifdef __cplusplus
extern "C" {
#endif

void RsForceLinkRsAssertRedirect(int force);

#ifdef __cplusplus
}
#endif

// 
// Add this macro to any source file of dll module to check if it need rederect 
// assert to a file
//
#if (!defined (_RTLDLL)  && !defined (__RSRTL_DLL)) || defined (__TOOLS_BUILD)
    #define  RS_CHECK_ASSERT_REDIRECT  void RsAssertDummy1(void) { RsForceLinkRsAssertRedirect(0); }
#else
    #define  RS_CHECK_ASSERT_REDIRECT
#endif

#endif  // RSASSERT

/* <-- EOF --> */