/*******************************************************************************
 FILE         :   FDEC_STD.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Standard components of classes FDecimal, FMoney.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   07 Dec 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/fdec_std.h $
                  $Revision: 2 $
                  $Date: 13.12.00 20:23 $
                  $Author: Lebedev $
                  $Modtime: 13.12.00 17:40 $
*******************************************************************************/

#ifndef __FDEC_STD_H
#define __FDEC_STD_H

#include "rsexcept.h"
#include "platform.h"
#include "tooldef.h"


#if defined(_MSC_VER)
    #define FDECIMAL_NO_INLINE
#endif

/////////////////////////////////////////////////////////////////////////////
// Enable wchar_t support

#if defined(_MSC_VER)
    #define FDECIMAL_WCHAR
#endif


/////////////////////////////////////////////////////////////////////////////
// Inline configuration

#ifndef FDECIMAL_NO_INLINE
    #define  FDECIMAL_INLINE     inline
    #define  FDECIMAL_DECLSPEC
#else
    #define  FDECIMAL_INLINE
    #define  FDECIMAL_DECLSPEC   _TOOLEXP
#endif

#ifndef FDECIMAL_T
#define FDECIMAL_T

    #ifdef RSL_PL_MS
        // -----------------------------------------------------------------------------
        typedef struct FDecimal_t
              {
               unsigned long  m_Lo;
               unsigned long  m_Hi;
              } FDecimal_t;

        // -----------------------------------------------------------------------------
        typedef FDecimal_t  DBFDecimal_t;
    #endif // RSL_PL_MS
#endif // FDECIMAL_T

enum FDecimal_Error
   {
    FDecimal_Overflow  = 0x0001,
    FDecimal_DivByZero = 0x0002,
    FDecimal_DataError = 0x0003
   };

enum FDecimal_RoundMode
   {
    FDecimal_Round = 0,
    FDecimal_Truncate,
    FDecimal_Floor,
    FDecimal_Ceil
   };

/////////////////////////////////////////////////////////////////////////////
// FDecimal_Error_Proc

typedef void (*FDecimal_Error_Proc)(int, const char *);

RSL_CFUNC FDecimal_Error_Proc _TOOLEXP Set_FDecimal_Error_Proc(FDecimal_Error_Proc);

/////////////////////////////////////////////////////////////////////////////
// The C++ part

#ifdef __cplusplus
/////////////////////////////////////////////////////////////////////////////
// FDecimal_Exception
class _TOOLEXP FDecimal_Exception : public RSException
    {
     public:
       // --------------------------------------------
       FDecimal_Exception(int Status) : RSException(Message(Status)), m_Status(Status)
       {
       }
       // --------------------------------------------
       void raise();

       // --------------------------------------------
       int error() const
       {
        return m_Status;
       }

     protected:
       // --------------------------------------------
       int m_Status;

       // --------------------------------------------
       static const char *Message(int Status);
    };

/////////////////////////////////////////////////////////////////////////////
// FDecimal_Status
class _TOOLEXP FDecimal_Status
    {
     public:
       // -------------------------------------------
       enum
          {
           stRound = 0x0001
          };

       // -------------------------------------------
       // Default Status
       static void ResetStatus()
       {
        m_Stat = stRound;
       }

       // -------------------------------------------
       static void SetStatus(int Status)
       {
        m_Stat = Status;
       }

       // -------------------------------------------
       static int  GetStatus()
       {
        return m_Stat;
       }

     protected:
       // -------------------------------------------
       static int  m_Stat;
    };

/////////////////////////////////////////////////////////////////////////////
// FMoney_Mode

class FMoney_Mode
    {
     public:
       static void set1to1cvt(bool cvt1to1)
       {
        m_1to1cvt = cvt1to1 ? 1 : 0;
       }

       static bool is1to1cvt()
       {
        return m_1to1cvt ? true : false;
       }

     private:
       static int  m_1to1cvt;
    };

#endif //__cplusplus

#endif // __FDEC_STD_H

/* <-- EOF --> */