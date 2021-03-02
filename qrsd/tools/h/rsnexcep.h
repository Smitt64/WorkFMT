/*******************************************************************************
 FILE         :   RSNEXCEP.H

 COPYRIGHT    :   R-Style Software Lab, 2001

 DESCRIPTION  :   Класс native-исключения, содержит платформно-зависимую
                  информацию об ошибке.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   05 Jun 2001

 VERSION INFO :   $Archive: $
                  $Revision: $
                  $Date: $
                  $Author: $
                  $Modtime: $
*******************************************************************************/

#ifndef __RSNEXCEP_H
#define __RSNEXCEP_H

#include "rsexcept.h"
#include "platform.h"

#ifdef RSL_PL_WIN32

    #ifndef _INC_WINDOWS
        #define STRICT
        #include <windows.h>
    #endif

    #include "tooldef.h"

    // -----------------------------------------------------------------------------
    class _TOOLEXP RSNativeException : public RSSystemException
        {
         public:
           static int ExceptionFilter(DWORD ExceptionCode, EXCEPTION_POINTERS *ExceptionInfo);

           const EXCEPTION_RECORD *ExceptionRecord() const
           {
            return &_ExceptionRecord;
           }

           RSNativeException(const RSNativeException &);

         private:
           EXCEPTION_RECORD  _ExceptionRecord;

           RSNativeException(int code, int nativeCode, const EXCEPTION_RECORD *ExceptionRecord);
        };

    #ifdef _MSC_VER
        #define  RSL_EXCEPTION_HANDLER  __try {
    #endif

    #define  RSL_DISABLE_HANDLER  } \
                __except (RSNativeException::ExceptionFilter(GetExceptionCode(),GetExceptionInformation())) \
                { \
                }

#endif //RSL_PL_WIN32

// -----------------------------------------------------------------------------
#ifdef RSL_MD_SUN

    #include <signal.h>

    class RSNativeException : public RSSystemException
        {
         public:
           const siginfo_t *sigInfo() const
           {
            return &_siginfo;
           }

           static void signalCatcher(int signo, siginfo_t *info, void *);

         private:
           siginfo_t  _siginfo;

           RSNativeException(int code, int nativeCode, const siginfo_t *info);
        };

#endif //RSL_MD_SUN

// -----------------------------------------------------------------------------
#if defined(RSL_MD_SUN)

    class RSSignalHandler;

    class RSSetSignalHandler
        {
         public:
           RSSetSignalHandler();
          ~RSSetSignalHandler();

         private:
           RSSignalHandler *_signalHandler;
        };

    #define  RSL_EXCEPTION_HANDLER  { RSSetSignalHandler  _signalHandler;

    #define  RSL_DISABLE_HANDLER    }

#endif //RSL_MD_SUN

#endif // __RSNEXCEP_H

/* <-- EOF --> */