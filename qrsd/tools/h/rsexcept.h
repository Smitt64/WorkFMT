/*******************************************************************************
 FILE         :   RSEXCEPT.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   class RSException, class RSSystemException,
                  typedef RSFatalHandler and function SetFatalHandler,
                  typedef RSFatalFilter and function SetFatalFilter.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   28 Nov 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsdigit.h $
                  $Revision: 3 $
                  $Date: 13.03.00 19:58 $
                  $Author: Lebedev $
                  $Modtime: 13.03.00 18:19 $
*******************************************************************************/

#ifndef __RSEXCEPT_H
#define __RSEXCEPT_H

#include <string.h>

#include "platform.h"
#include "tooldef.h"

#ifdef _MSC_VER
    #pragma warning(disable:4290)
    #pragma warning(disable:4786)
#endif

#ifdef __cplusplus
/*
   �� ��������� <stdexcept> �� ��������, �� ����� �� ����� ���� �������...
#include <stdexcept>
*/

#if defined(_MSC_VER)

    #include <eh.h>

#elif defined(__IBMCPP__)

    #include <terminat.h>

    #ifdef __TOS_OS400__
        typedef void (*terminate_handler)();
    #else
        typedef _PFV terminate_handler;
    #endif

#else

    #include <exception>

    using std::set_terminate;
    using std::set_unexpected;

    using std::terminate_handler;
    using std::unexpected_handler;

#endif

// -----------------------------------------------------------------------------
class _TOOLEXP RSException
    {
     public:
       RSException(const char *str = "Unknown exception")
       {
        if(str)
         {
          strncpy(_buf, str, sizeof(_buf));

          *(_buf + sizeof(_buf) - 1) = '\0';
         }
        else
          strcpy(_buf, "NULL exception");
       }

       virtual ~RSException() {}

       virtual const char *what() const
       {
        return _buf;
       }

       RSException(const RSException &arg)
       {
        if(this != &arg)
          strcpy(_buf, arg._buf);
       }

       RSException &operator = (const RSException &arg)
       {
        if(this != &arg)
          strcpy(_buf, arg._buf);

        return *this;
       }

     protected:
       char  _buf[256];
    };

// -----------------------------------------------------------------------------
class _TOOLEXP RSSystemException : public RSException
    {
     public:
       enum
          {
           // Illegal instruction
           Illegal_Operation = 1,
           Privileged_Operation,
           Stack_Overflow,

           // Unmapped, protected, or bad memory
           Access_Violation,
           Missing_Alignment,
           Address_Error,

           // Arithmetic exception
           Float_DivideByZero,
           Float_Overflow,
           Float_InexactResult,
           Float_Underflow,
           Float_InvalidOperation,
           Float_PlatformSpecific,
           Binary_DivideByZero,
           Binary_Overflow,


           Illegal_Execution = Illegal_Operation | Privileged_Operation | Stack_Overflow,

           Memory_Exception = Access_Violation | Missing_Alignment | Address_Error,

           Arithmetic_Exception = Float_DivideByZero | Float_Overflow | Float_InexactResult |
                                   Float_Underflow | Float_InvalidOperation | Float_PlatformSpecific |
                                   Binary_DivideByZero | Binary_Overflow,

           OSSpecific_Exception = 0x9999
          };

       int error() const
       {
        return _code;
       }

       int nativeError() const
       {
        return _nativeCode;
       }

       virtual const char *what() const
       {
        return _str;
       }

       virtual const void *address() const
       {
        return _address;
       }

       RSSystemException(int code, int nativeCode);

       virtual ~RSSystemException() {}

       void executeHandler() const;

       static void setDumpName(const char *);
       static void setDumpDir(const char *);

       static void disableDump();
       static void enableDump();

     protected:
       int  _code;
       int  _nativeCode;

       const char  *_str;
       const void  *_address;

       static char  _dumpName[16];  //��� dump-�����
       static char  _dumpDir[256];  //������� ��� dump-������

       static bool  _dumpDisabled;
    };

#endif // __cplusplus


typedef void (*RSFatalHandler)(int code, int nativeCode, const char *str);

// �������-������, ��������� ������������� ����������� ���������� ��� ���������
// ��������� �������� �� ��������� �����. �������� info ��������� �� ���������,
// ������� �������� ����������-��������� ���������� � ����������:
//    Win32    - EXCEPTION_POINTERS
//    AS/400   - _INTRPT_Hndlr_Parms_T
//    Sun      - siginfo_t
//
// ����������:
//    0, ������� �������� ����������
//   -1, ���������� ����� ������� (������ ��� Win32).
//
typedef int (*RSFatalFilter)(int code, int nativeCode, const void *info);

RSL_CFUNC RSFatalHandler _TOOLEXP SetFatalHandler(RSFatalHandler);
RSL_CFUNC RSFatalFilter  _TOOLEXP SetFatalFilter(RSFatalFilter);

#endif // __RSEXCEPT_H

/* <-- EOF --> */