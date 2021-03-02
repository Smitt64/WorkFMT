/**************************************************************************
*                                                                         *
* - bcd.h                                                                 *
*                                                                         *
* IBM(R) VisualAge(TM) for C++ for AS/400(R), V3R6M0, V3R7M0              *
*                                                                         *
* (C) Copyright IBM Corporation, 1995, 1996 - All Rights Reserved         *
*                                                                         *
* 5716-CX4, 5716-CX5                                                      *
*                                                                         *
* Licensed Materials - Property of IBM                                    *
*                                                                         *
* US Government Users Restricted Rights - Use, duplication or disclosure  *
* restricted by GSA ADP Schedule Contract with IBM Corp.                  *
*                                                                         *
**************************************************************************/

#ifndef __FIXED_400_H
#define __FIXED_400_H

// *************************************************
// *** Functions to perform basic BCD operations ***
// *************************************************
extern int __padd(char *, int, int, const char *, int, int, const char *, int, int);
extern int __psub(char *, int, int, const char *, int, int, const char *, int, int);
extern int __pmpy(char *, int, int, const char *, int, const char *, int);
extern int __pdiv(char *, int, int, const char *, int, int, const char *, int, int);
extern int __pcmp(const char *, int, int, const char *, int, int, int);

// ********************************************************
// *** Function to convert BCD from one type to another ***
// ********************************************************
extern int  __dectodec(int, int, int, int, const char *, char *, int);

// ***********************************************************
// *** Functions to convert BCD to/from builtin data types ***
// ***********************************************************
extern void   __dtodec(unsigned char *, int, int, double); // double to BCD
extern void   __itodec(unsigned char *, int, int, int);    // int to BCD
extern double __dectod(unsigned char *, int, int);         // BCD to double
extern int    __dectoi(unsigned char *, int, int);         // BCD to int

#ifndef FALSE
    #define FALSE  ((int)0)
    #define TRUE   (!FALSE)
#endif

#define  DEC_MIN         __D("-9999999999999999999999999999999")
#define  DEC_MAX         __D("+9999999999999999999999999999999")
#define  DEC_EPSILON     __D(".0000000000000000000000000000001")

#define  DEC_DIG         31
#define  DEC_PRECISION   31

#define  DEC_INT_DIG     10
#define  DEC_INT_PREC    0

#define  MAX_BUF_SIZE    ((DEC_DIG >> 1) + 1)

#define  digitsof    __digitsof
#define  precisionof __precisionof

#define  __digitsof(DecName)     (DecName).DigitsOf()
#define  __precisionof(DecName)  (DecName).PrecisionOf()

// **********************************
// *** enum type used by __pcmp() ***
// **********************************
typedef enum CmpType
      {
       Equ = 1,
       NotEqu,
       Grt,
       GrtEqu,
       Sml,
       SmlEqu
      };


// *********************
// *** class _DecErr ***
// *********************
class _DecErr { };

// ************************************
// *** classes derived from _DecErr ***
// ************************************
class _DecErrInvalidConst : public _DecErr { }; // Invalid BCD constant
class _DecErrInvalidParam : public _DecErr { }; // Invalid BCD parameters
class _DecErrDivByZero    : public _DecErr { }; // Divided by zero
class _DecErrOverflow     : public _DecErr { }; // Overflow
class _DecErrDigTruncated : public _DecErr { }; // Truncation in BCD digit
class _DecErrInternal     : public _DecErr { }; // Internal error


/*******************************************************************************
 FILE         :   FIXED_400.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   01 Dec 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/fixed_400.h $
                  $Revision: 7 $
                  $Date: 14.03.01 17:39 $
                  $Author: Lebedev $
                  $Modtime: 14.03.01 17:39 $
*******************************************************************************/

#include <limits.h>
#include <stdio.h>

#include "fdec_std.h"

#define  RSL_BCD_DIGITS     19

RSL_CFUNC const char *fixdec_zero_dump(unsigned dig);
RSL_CFUNC const char *fixdec_min_dump(unsigned dig);
RSL_CFUNC const char *fixdec_max_dump(unsigned dig);
RSL_CFUNC char *fixdec_round(char *n, int d, int p, int mode);
RSL_CFUNC char *fixdec_to_i64(const char *n, int d, long *hi, long *lo);
RSL_CFUNC char *fixdec_from_i64(char *n, int d, long hi, long lo);

// -----------------------------------------------------------------------------
inline char *dec_abs(char *n, int d)
{
 d = d/2 ;
 int  sig = n[d] & 0x0F;

 if(sig == 0x0D || sig == 0x0B)
   n[d] |= 0x0F;

 return n;
}

// -----------------------------------------------------------------------------
inline char *dec_negative(char *n, int d)
{
 d = d/2 ;
 int  sig = n[d] & 0x0F;

 if(sig == 0x0D || sig == 0x0B)
   n[d] |= 0x0F;
 else if(sig >= 0x0A)
  {
   if((n[d] & 0xF0) != 0)
     n[d] = (n[d] & 0xF0) | 0x0D;
   else
     for(int i = 0; i < d; i++)
        if(n[i] != 0)
         {
          n[d] = (n[d] & 0xF0) | 0x0D;
          break;
         }
  }

 return n;
}

// -----------------------------------------------------------------------------
template <int Point>
class FixedT_400
    {
     public:
       inline FixedT_400();
       inline FixedT_400(int);
       inline FixedT_400(long);
       inline FixedT_400(short);
       inline FixedT_400(unsigned int);
       inline FixedT_400(unsigned long);
       inline FixedT_400(unsigned short);
       inline FixedT_400(float);
       inline FixedT_400(double);
       inline FixedT_400(long double);

       explicit inline FixedT_400(const char *);

 #ifdef FDECIMAL_T
       inline FixedT_400(const PCFDecimal_t &);
       inline void get(PCFDecimal_t *) const;
 #endif

       inline operator double () const;
       inline operator long double () const;

       inline const FixedT_400<Point> &operator += (const FixedT_400<Point> &);
       inline const FixedT_400<Point> &operator -= (const FixedT_400<Point> &);
       inline const FixedT_400<Point> &operator *= (const FixedT_400<Point> &);
       inline const FixedT_400<Point> &operator /= (const FixedT_400<Point> &);

       inline bool operator == (const FixedT_400<Point> &) const;
       inline bool operator != (const FixedT_400<Point> &) const;
       inline bool operator <= (const FixedT_400<Point> &) const;
       inline bool operator >= (const FixedT_400<Point> &) const;
       inline bool operator <  (const FixedT_400<Point> &) const;
       inline bool operator >  (const FixedT_400<Point> &) const;

       inline FixedT_400<Point> operator - () const;
       inline bool operator ! () const;

       inline FixedT_400<Point> Round(int nPos = 0, int Mode = FDecimal_Round) const;
       inline FixedT_400<Point> Abs() const;

       inline char *FormatRawStr(char *, size_t) const;
       inline char *FormatRawStr(int Precision, char *, size_t) const;

       inline static FixedT_400<Point> MinValue();
       inline static FixedT_400<Point> MaxValue();
       inline static FixedT_400<Point> ZeroValue();

     private:
       char data[RSL_BCD_DIGITS/2 + 1];
       static const char zerobuf[RSL_BCD_DIGITS/2 + 1];
    };

// -----------------------------------------------------------------------------
template <int Point>
const char FixedT_400<Point>::zerobuf[RSL_BCD_DIGITS/2 + 1] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0','\x0F'};

template <int Point>
inline FixedT_400<Point>::FixedT_400()
{
 memcpy(data, zerobuf, sizeof(data));
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(int Src)
{
 try
   {
    __itodec ((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(long Src)
{
 try
   {
    __itodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(short Src)
{
 try
   {
    __itodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(unsigned int Src)
{
 try
   {
    __itodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(unsigned long Src)
{
 try
   {
    __itodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(unsigned short Src)
{
 try
   {
    __itodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(float Src)
{
 try
   {
    __dtodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(double Src)
{
 try
   {
    __dtodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(long double Src)
{
 try
   {
    __dtodec((unsigned char *)data, RSL_BCD_DIGITS, Point, Src);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

template <int Point>
inline FixedT_400<Point>::FixedT_400(const char *Src)
{
 try
   {
    sscanf(Src, "%D(*,*)", RSL_BCD_DIGITS, Point, data);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
}

#ifdef FDECIMAL_T
    template <int Point>
    inline FixedT_400<Point>::FixedT_400(const PCFDecimal_t &t)
    {
     try
       {
        fixdec_from_i64(data, RSL_BCD_DIGITS, (long)t.m_Hi, (long)t.m_Lo);
       }
     catch(_DecErrOverflow)
       {
        FDecimal_Exception(FDecimal_Overflow).raise();
       }
     catch(_DecErrDigTruncated)
       {
        FDecimal_Exception(FDecimal_Overflow).raise();
       }
    }

    template <int Point>
    inline void FixedT_400<Point>::get(PCFDecimal_t *t) const
    {
     if(!fixdec_to_i64(data, RSL_BCD_DIGITS, (long *)&t->m_Hi, (long *)&t->m_Lo))
       FDecimal_Exception(FDecimal_Overflow).raise();
    }
#endif

template <int Point>
inline FixedT_400<Point>::operator double () const
{
 return __dectod ((unsigned char *)data, RSL_BCD_DIGITS, Point);
}


template <int Point>
inline FixedT_400<Point>::operator long double () const
{
 return __dectod ((unsigned char *)data, RSL_BCD_DIGITS, Point);
}

// -----------------------------------------------------------------------------
template <int Point>
inline const FixedT_400<Point> &FixedT_400<Point>::operator += (const FixedT_400<Point> &Operand)
{
 try
   {
    char  buf[MAX_BUF_SIZE];
    int  bufSize = (RSL_BCD_DIGITS >> 1) + 1;

    __padd((char *)buf, RSL_BCD_DIGITS, -Point, (char *)data, RSL_BCD_DIGITS, 0,
           (char *)Operand.data, RSL_BCD_DIGITS, 0);

    memcpy(data, buf + MAX_BUF_SIZE - bufSize, bufSize);
   }
 catch (_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch (_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }

 return *this;
}

// -----------------------------------------------------------------------------
template <int Point>
inline const FixedT_400<Point> &FixedT_400<Point>::operator -= (const FixedT_400<Point> &Operand)
{
 try
   {
    char  buf[MAX_BUF_SIZE];
    int  bufSize = (RSL_BCD_DIGITS >> 1) + 1;

    __psub((char *)buf, RSL_BCD_DIGITS, -Point, (char *)data, RSL_BCD_DIGITS, 0,
           (char *)Operand.data, RSL_BCD_DIGITS, 0);

    memcpy(data, buf + MAX_BUF_SIZE - bufSize, bufSize);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }

 return *this;
}

// -----------------------------------------------------------------------------
template <int Point>
inline const FixedT_400<Point> &FixedT_400<Point>::operator *= (const FixedT_400<Point> &Operand)
{
 try
   {
    char  buf[MAX_BUF_SIZE];
    int  bufSize = (RSL_BCD_DIGITS >> 1) + 1;

    __pmpy((char *)buf, RSL_BCD_DIGITS, -Point, (char *)data, RSL_BCD_DIGITS,
           (char *)Operand.data, RSL_BCD_DIGITS);

    memcpy(data, buf + MAX_BUF_SIZE - bufSize, bufSize);
   }
 catch (_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch (_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }

   return *this;
}

// -----------------------------------------------------------------------------
template <int Point>
inline const FixedT_400<Point> &FixedT_400<Point>::operator /= (const FixedT_400<Point> &Operand)
{
 try
   {
    char  buf[MAX_BUF_SIZE];
    int  bufSize = (RSL_BCD_DIGITS >> 1) + 1;


    __pdiv((char *)buf, RSL_BCD_DIGITS, 0, (char *)data, RSL_BCD_DIGITS, -Point,
                                           (char *)Operand.data, RSL_BCD_DIGITS, 0);

    memcpy(data, buf + MAX_BUF_SIZE - bufSize, bufSize);
   }
 catch(_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch(_DecErrDivByZero)
   {
    FDecimal_Exception(FDecimal_DivByZero).raise();
   }

 return *this;
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator == ( const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, Equ);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator != ( const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, NotEqu);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator <= (const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, SmlEqu);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator >= (const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, GrtEqu);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator < (const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, Sml);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator > (const FixedT_400<Point> &Operand) const
{
 return __pcmp((char *)data, RSL_BCD_DIGITS, 0, (char *)Operand.data, RSL_BCD_DIGITS, 0, Grt);
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::operator - () const
{
 char  buf[MAX_BUF_SIZE];

 memcpy(buf, data, sizeof data);

 return *(FixedT_400<Point> *)dec_negative((char *)buf, RSL_BCD_DIGITS);
}

// -----------------------------------------------------------------------------
template <int Point>
inline bool FixedT_400<Point>::operator ! () const
{
 return memcmp(data, zerobuf, sizeof data) == 0;
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::Abs () const
{
 char  buf[MAX_BUF_SIZE];

 memcpy(buf, data, sizeof data);

 return *(FixedT_400<Point> *)dec_abs((char *)buf, RSL_BCD_DIGITS);
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::Round ( int nPos, int Mode ) const
{
 char  buf[MAX_BUF_SIZE];
 int  p = (nPos < Point ? Point - nPos : 0);

 try
   {
    memcpy(buf, data, sizeof data);
    return *(FixedT_400<Point> *)fixdec_round((char *)buf, RSL_BCD_DIGITS, p, Mode);
   }
 catch (_DecErrOverflow)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }
 catch (_DecErrDigTruncated)
   {
    FDecimal_Exception(FDecimal_Overflow).raise();
   }

 return *this;
}

// -----------------------------------------------------------------------------
template <int Point>
inline char *FixedT_400<Point>::FormatRawStr( char * pRawStr, size_t StrLen ) const
{
 char  buf[DEC_DIG + 1];

 sprintf(buf, "%D(*,*)", RSL_BCD_DIGITS, Point, *this);

 return strlen(buf) < StrLen ? strcpy(pRawStr, buf) : NULL;
}

// -----------------------------------------------------------------------------
template <int Point>
inline char *FixedT_400<Point>::FormatRawStr( int Precision, char * pRawStr, size_t StrLen ) const
{
 char  buf[DEC_DIG + 1];

 sprintf(buf, "%.*D(*,*)", Precision, RSL_BCD_DIGITS, Point, *this);

 return strlen(buf) < StrLen ? strcpy(pRawStr, buf) : NULL;
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::MinValue()
{
 return *(FixedT_400<Point> *)fixdec_min_dump(RSL_BCD_DIGITS);
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::MaxValue()
{
 return *(FixedT_400<Point> *)fixdec_max_dump(RSL_BCD_DIGITS);
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_400<Point> FixedT_400<Point>::ZeroValue()
{
 return *(FixedT_400<Point> *)fixdec_zero_dump(RSL_BCD_DIGITS);
}

#endif // __FIXED_400_H

/* <-- EOF --> */