/*******************************************************************************
 FILE         :   CRITSECT.H

 COPYRIGHT    :   R-Style, 1996

 DESCRIPTION  :   Critical section declaration for C (not C++) code

 PROGRAMMED BY:   Yakov E. Markovitch (JNS)

 CREATION DATE:   18/12/1996

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/critsect.h $
                  $Revision: 4 $
                  $Date: 16.09.98 14:56 $
                  $Author: Markovitch $
                  $Modtime: 15.09.98 20:03 $
*******************************************************************************/

#ifndef __CRITSECT_H
#define __CRITSECT_H

#include "platform.h"

// -----------------------------------------------------------------------------
#if defined(RSL_PL_WIN32)
    #include <windows.h>

    #define  RSL_DECL_CRITICAL_SECTION(name)   CRITICAL_SECTION  name
    #define  RSL_DECL_CRITICAL_SECTION2(name)  CRITICAL_SECTION  name;
    #define  RSL_INIT_CRITICAL_SECTION(name)   (InitializeCriticalSection(&name))
    #define  RSL_DONE_CRITICAL_SECTION(name)   (DeleteCriticalSection(&name))
    #define  RSL_ENTER_CRITICAL_SECTION(name)  (EnterCriticalSection(&name))
    #define  RSL_LEAVE_CRITICAL_SECTION(name)  (LeaveCriticalSection(&name))

#elif defined(RSL_MD_SUN)
    #include <pthread.h>

    #define  RSL_DECL_CRITICAL_SECTION(name)   pthread_mutex_t  name
    #define  RSL_DECL_CRITICAL_SECTION2(name)  pthread_mutex_t  name;
    #define  RSL_INIT_CRITICAL_SECTION(name)   (pthread_mutex_init(&name, NULL))
    #define  RSL_DONE_CRITICAL_SECTION(name)   (pthread_mutex_destroy(&name))
    #define  RSL_ENTER_CRITICAL_SECTION(name)  (pthread_mutex_lock(&name))
    #define  RSL_LEAVE_CRITICAL_SECTION(name)  (pthread_mutex_unlock(&name))

#else

    #define  RSL_DECL_CRITICAL_SECTION(name)   int  name
    #define  RSL_DECL_CRITICAL_SECTION2(name)
    #define  RSL_INIT_CRITICAL_SECTION(name)   ((void)0)
    #define  RSL_DONE_CRITICAL_SECTION(name)   ((void)0)
    #define  RSL_ENTER_CRITICAL_SECTION(name)  ((void)0)
    #define  RSL_LEAVE_CRITICAL_SECTION(name)  ((void)0)

#endif

// -----------------------------------------------------------------------------
#if defined(RSL_MD_MT)

    #define  DECL_CRITICAL_SECTION(name)       RSL_DECL_CRITICAL_SECTION(name)
    #define  DECL_CRITICAL_SECTION2(name)      RSL_DECL_CRITICAL_SECTION2(name)
    #define  INIT_CRITICAL_SECTION(name)       RSL_INIT_CRITICAL_SECTION(name)
    #define  DONE_CRITICAL_SECTION(name)       RSL_DONE_CRITICAL_SECTION(name)
    #define  ENTER_CRITICAL_SECTION(name)      RSL_ENTER_CRITICAL_SECTION(name)
    #define  LEAVE_CRITICAL_SECTION(name)      RSL_LEAVE_CRITICAL_SECTION(name)

#else //   RSL_MD_MT

    #define  DECL_CRITICAL_SECTION(name)       int  name
    #define  DECL_CRITICAL_SECTION2(name)
    #define  INIT_CRITICAL_SECTION(name)       ((void)0)
    #define  DONE_CRITICAL_SECTION(name)       ((void)0)
    #define  ENTER_CRITICAL_SECTION(name)      ((void)0)
    #define  LEAVE_CRITICAL_SECTION(name)      ((void)0)

#endif //  RSL_MD_MT

#define  RS_CRITICAL_SECTION(name)   DECL_CRITICAL_SECTION(name)
#define  RS_CRITICAL_SECTION2(name)  DECL_CRITICAL_SECTION2(name)
#define  DEL_CRITICAL_SECTION(name)  DONE_CRITICAL_SECTION(name)

#endif // __CRITSECT_H

/* <-- EOF --> */