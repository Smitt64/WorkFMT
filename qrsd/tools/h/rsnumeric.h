//-*--------------------------------------------------------------------------*-
//
// File Name   : rsnumeric.h
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 15, 2004 Alexey Lebedev - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSNUMERIC_H
#define __RSNUMERIC_H

#include "int64.h"

//
// Классы для десятичной арифметики с плавающей точкой.
//

typedef struct Numeric96_t
      {
       unsigned char  unused_;
       unsigned char  state_;
       short          exponent_;
       signed long    leadDigit_;
       unsigned long  trailingDigits_[2];
      } Numeric96_t;


typedef struct BtrDecimal_t
      {
       unsigned char data [15];
      } BtrDecimal_t;


#ifdef __cplusplus

#include <stddef.h>

#include "tooldef.h"
#include "platform.h"
#include "rsdbtype.hpp"

#if !defined(RSL_RESTRICT_NUMERIC)

    #include <string>
    #include "rsiostrm.h"

#endif //!defined(RSL_RESTRICT_NUMERIC)

class _TOOLEXP Numeric96InexactErr;
class _TOOLEXP Numeric96OverflowErr;
class _TOOLEXP Numeric96DivByZeroErr;

class DBNumeric96;

/*******************************************************************************
 class Numeric96
*******************************************************************************/
class _TOOLEXP Numeric96 : public Numeric96_t
{
 public:
   enum RoundingMethod { plain = 0, up = 1, down = 2, bankers = 3, round = 0, truncate = 2 };
   enum State          { normal = 0, nullstate = 1, NaNstate = 2, ctorError = 4, missingstate = 8 };
   enum Op             { assign, add, sub, mult, div, powop, pow10op, conversion };

// Special values ----------------------------------------------------------------------------
   static Numeric96 nullValue();
   static Numeric96 missingValue();
   static Numeric96 NaNValue();

// Public constructors -----------------------------------------------------------------------
   // default
   Numeric96();

   // Construct from int
   Numeric96(int i);

   // Construct from long int
   Numeric96(long int i);

   // Construct from int64_t
   Numeric96(int64_t i);

   // Construct for integer and exponent.
   Numeric96(int m, int e);

   // Construct for long integer and exponent.
   Numeric96(long int m, int e);

   // Construct from an ascii decimal string
   Numeric96(const char *str);

   // Construct from Numeric96_t
   Numeric96(const Numeric96_t &t)
   {
    *this = *(Numeric96 *)&t;
   }

   // Convert to DBNumeric96
   inline operator DBNumeric96() const;

   // Construct from double
   Numeric96(double d)
   {
    *this = Numeric96::from(d);
   }

   // Construct from long double
   Numeric96(long double d)
   {
    *this = Numeric96::from(d);
   }

#ifdef FDECIMAL_T
   // Convert to FDecimal_t
   operator FDecimal_t() const
   {
    long  digits[2];


    Numeric96::toDigits(*this, 4, digits, 2U);

    FDecimal_t  t;

    t.m_Hi = digits[0];
    t.m_Lo = digits[1];

    return t;
   }

   // Convert from FDecimal_t
   Numeric96(const FDecimal_t &t)
   {
    long  digits[2];

    digits[0] = t.m_Hi;
    digits[1] = t.m_Lo;

    Numeric96::fromDigits(*this, 4, digits, 2U);
   }
#endif

// Error handlers ----------------------------------------------------------------------------
   typedef void (InexactHandler)(const Numeric96InexactErr &);
   typedef void (OverflowHandler)(const Numeric96OverflowErr &);
   typedef void (DivByZeroHandler)(const Numeric96DivByZeroErr &);

   static InexactHandler   *setInexactHandler(InexactHandler *eh);
   static OverflowHandler  *setOverflowHandler(OverflowHandler *eh);
   static DivByZeroHandler *setDivByZeroHandler(DivByZeroHandler *eh);

   static void defaultInexactHandler(const Numeric96InexactErr &err);
   static void defaultOverflowHandler(const Numeric96OverflowErr &err);
   static void defaultDivByZeroHandler(const Numeric96DivByZeroErr &err);

// Arithmetic assignment operators -----------------------------------------------------------
   inline Numeric96& operator += (const Numeric96 &a);
   inline Numeric96& operator -= (const Numeric96 &a);
   inline Numeric96& operator *= (const Numeric96 &a);
   inline Numeric96& operator /= (const Numeric96 &a);

// Utility functions -------------------------------------------------------------------------
   static int       maxDigits();
   static Numeric96 maxValue();
   static Numeric96 minValue();
   static Numeric96 zeroValue();

   int isNumber()      const { return (state_ == normal); }
   int decimalPlaces() const { return (state_ == normal ? exponent_ : 0); }
   int isZero()        const
   {
    return (state_ == normal ? !trailingDigits_[1] && !trailingDigits_[0] && !leadDigit_ : state_ == nullstate);
   }

// Narrowing conversion functions ------------------------------------------------------------
   static Numeric96 from(double);
   static Numeric96 from(long double);

// Arithmetic functions ----------------------------------------------------------------------
   static Numeric96 plus    (const Numeric96 &a, const Numeric96 &b);
   static Numeric96 minus   (const Numeric96 &a, const Numeric96 &b);
   static Numeric96 multiply(const Numeric96 &a, const Numeric96 &b);
   static Numeric96 divide  (const Numeric96 &a, const Numeric96 &b);

// Unary arithmetic functions ----------------------------------------------------------------
   static Numeric96 plus (const Numeric96 &);
   static Numeric96 minus(const Numeric96 &);

// Comparison functions ----------------------------------------------------------------------
   static int equal   (const Numeric96 &a, const Numeric96 &b);
   static int lessThan(const Numeric96 &a, const Numeric96 &b);

// Math functions ----------------------------------------------------------------------------
   static Numeric96 power         (const Numeric96 &, int);
   static Numeric96 absoluteValue (const Numeric96 &);
   static Numeric96 roundDecimal  (const Numeric96 &, int, RoundingMethod m, unsigned int roundDigit);
   static Numeric96 convertDecimal(const Numeric96 &, int, RoundingMethod m, unsigned int roundDigit);

// Conversion functions ----------------------------------------------------------------------
   static int         convertToInt       (const Numeric96 &d, RoundingMethod m, unsigned int roundDigit);
   static long int    convertToLong      (const Numeric96 &d, RoundingMethod m, unsigned int roundDigit);
   static int64_t     convertToBigInt    (const Numeric96 &d, RoundingMethod m, unsigned int roundDigit);

   static double      convertToDouble    (const Numeric96 &);
   static long double convertToLongDouble(const Numeric96 &);

#if !defined(RSL_RESTRICT_NUMERIC)
   static RSL_STD::string  convertToString(const Numeric96&);

// I/O functions -----------------------------------------------------------------------------
   static RSL_STD::istream &shift(RSL_STD::istream &, Numeric96 &);
   static RSL_STD::ostream &shift(RSL_STD::ostream &, const Numeric96 &);
#endif //!RSL_RESTRICT_NUMERIC

protected:
// Conversion to/from array of integers ------------------------------------------------------
   static void toDigits(const Numeric96& d, int p, long *digits, size_t n);
   static void fromDigits(Numeric96& d, int p, const long *digits, size_t n);

public:
// Public utility functions ---------------------------------------------------------------
   static void inexact  (Op oper, const Numeric96 &a, const Numeric96 &b, const char *msg);
   static void overflow (Op oper, const Numeric96 &a, const Numeric96 &b, const char *msg);
   static void divByZero(Op oper, const Numeric96 &a, const Numeric96 &b, const char *msg);

private:
   static void (*inexactHandler_)  (const Numeric96InexactErr &);
   static void (*overflowHandler_) (const Numeric96OverflowErr &);
   static void (*divByZeroHandler_)(const Numeric96DivByZeroErr &);
};

// Unary arithmetic operators ----------------------------------------------------------------
inline _TOOLEXP Numeric96 operator - (const Numeric96 &d)
{
 return Numeric96::minus(d);
}

inline _TOOLEXP Numeric96 operator + (const Numeric96 &d)
{
 return Numeric96::plus(d);
}

inline _TOOLEXP int operator ! (const Numeric96 &d)
{
 return d.isZero();
}

// Arithmetic operators ----------------------------------------------------------------------
inline _TOOLEXP Numeric96 operator + (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::plus(a, b);
}

inline _TOOLEXP Numeric96 operator - (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::minus(a, b);
}

inline _TOOLEXP Numeric96 operator * (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::multiply(a, b);
}

inline _TOOLEXP Numeric96 operator / (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::divide(a, b);
}

// Assignment operators ----------------------------------------------------------------------
inline Numeric96 &Numeric96::operator += (const Numeric96 &a)
{
 Numeric96  sum = *this + a;

 return (*this = sum);
}

inline Numeric96 &Numeric96::operator -= (const Numeric96 &a)
{
 Numeric96  diff = *this - a;
 
 return (*this = diff);
}

inline Numeric96 &Numeric96::operator *= (const Numeric96 &a)
{
 Numeric96  prod = *this * a;
 
 return (*this = prod);
}

inline Numeric96 &Numeric96::operator /= (const Numeric96 &a)
{
 Numeric96  result = *this / a;
 
 return (*this = result);
}

// Comparison operators ----------------------------------------------------------------------
inline _TOOLEXP bool operator == (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::equal(a, b) != 0;
}

inline _TOOLEXP bool operator < (const Numeric96 &a, const Numeric96 &b)
{
 return Numeric96::lessThan(a, b) != 0;
}

#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))

    inline _TOOLEXP bool operator != (const Numeric96 &a, const Numeric96 &b)
    {
     return !Numeric96::equal(a, b);
    }

    inline _TOOLEXP bool operator > (const Numeric96 &a, const Numeric96 &b)
    {
     return Numeric96::lessThan(b, a) != 0;
    }

    inline _TOOLEXP bool operator >= (const Numeric96 &a, const Numeric96 &b)
    {
     return Numeric96::lessThan(a, b) == 0;
    }

    inline _TOOLEXP bool operator <= (const Numeric96 &a, const Numeric96 &b)
    {
     return Numeric96::lessThan(b, a) == 0;
    }

#endif //#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))

// Math functions ----------------------------------------------------------------------------
inline _TOOLEXP Numeric96 power(const Numeric96 &d, int e)
{
 return Numeric96::power(d, e);
}

inline _TOOLEXP Numeric96 abs(const Numeric96 &d)
{
 return Numeric96::absoluteValue(d);
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d, int digits, Numeric96::RoundingMethod m, unsigned int roundDigit)
{
 return Numeric96::roundDecimal(d, digits, m, roundDigit);
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d, int digits, Numeric96::RoundingMethod m) 
{
 return Numeric96::roundDecimal(d, digits, m, 4U);
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d, Numeric96::RoundingMethod m, unsigned int roundDigit)
{
 return Numeric96::roundDecimal(d, 0, m, roundDigit);
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d, Numeric96::RoundingMethod m)
{
 return Numeric96::roundDecimal(d, 0, m, 4U);
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d, int digits)
{
 return Numeric96::roundDecimal(d, digits, Numeric96::plain, 4U);
}

// -----------------------------------------------------------------------------
inline _TOOLEXP Numeric96 getIntegerPart(const Numeric96 &value)    
{
 return round(value, value.leadDigit_ > 0 ? Numeric96::down : Numeric96::up);
}

inline _TOOLEXP Numeric96 getFractionalPart(const Numeric96 &value) 
{
 return value - getIntegerPart(value);
}

inline _TOOLEXP bool isInteger(const Numeric96 &value) 
{
 return getFractionalPart(value) == Numeric96::zeroValue();
}

inline _TOOLEXP Numeric96 round(const Numeric96 &d)
{
 return Numeric96::roundDecimal(d, 0, Numeric96::plain, 4U);
}

// -----------------------------------------------------------------------------
inline _TOOLEXP int toInt(const Numeric96 &d, Numeric96::RoundingMethod m, unsigned int roundDigit)
{
 return Numeric96::convertToInt(d, m, roundDigit);
}

inline _TOOLEXP int toInt(const Numeric96 &d, Numeric96::RoundingMethod m)
{
 return Numeric96::convertToInt(d, m, 4U);
}

inline _TOOLEXP int toInt(const Numeric96 &d)
{
 return Numeric96::convertToInt(d, Numeric96::plain, 4U);
}

// -----------------------------------------------------------------------------
inline _TOOLEXP long int toLong(const Numeric96 &d, Numeric96::RoundingMethod m, unsigned int roundDigit)
{
 return Numeric96::convertToLong(d, m, roundDigit);
}

inline _TOOLEXP long int toLong(const Numeric96 &d, Numeric96::RoundingMethod m)
{
 return Numeric96::convertToLong(d, m, 4U);
}

inline _TOOLEXP long int toLong(const Numeric96 &d)
{
 return Numeric96::convertToLong(d, Numeric96::plain, 4U);
}

// -----------------------------------------------------------------------------
inline _TOOLEXP int64_t toBigInt(const Numeric96 &d, Numeric96::RoundingMethod m, unsigned int roundDigit)
{
 return Numeric96::convertToBigInt(d, m, roundDigit);
}

inline _TOOLEXP int64_t toBigInt(const Numeric96 &d, Numeric96::RoundingMethod m)
{
 return Numeric96::convertToBigInt(d, m, 4U);
}

inline _TOOLEXP int64_t toBigInt(const Numeric96 &d)
{
 return Numeric96::convertToBigInt(d, Numeric96::plain, 4U);
}

// -----------------------------------------------------------------------------
inline _TOOLEXP double toDouble(const Numeric96 &d)
{
 return Numeric96::convertToDouble(d);
}

inline _TOOLEXP long double toLongDouble(const Numeric96 &d)
{
 return Numeric96::convertToLongDouble(d);
}

#if !defined(RSL_RESTRICT_NUMERIC)
    inline _TOOLEXP RSL_STD::string toString(const Numeric96 &d)
    {
     return Numeric96::convertToString(d);
    }

    // I/O operators -----------------------------------------------------------------------------
    inline _TOOLEXP RSL_STD::ostream &operator << (RSL_STD::ostream &strm, const Numeric96 &d)
    {
     return Numeric96::shift(strm, d);
    }

    inline _TOOLEXP RSL_STD::istream &operator >> (RSL_STD::istream &strm, Numeric96 &d)
    {
     return Numeric96::shift(strm, d);
    }
#endif //!RSL_RESTRICT_NUMERIC


/*******************************************************************************
 class DBNumeric96
*******************************************************************************/
class DBNumeric96 : public db_2reftype<Numeric96, Numeric96_t>
    {
     enum { fixedPrecision = 12 };

     public:
       // Assignment operators ----------------------------------------------------------------------
       DBNumeric96 &operator = (const Numeric96 &n)
       {
        db_2reftype<Numeric96, Numeric96_t>::operator = (Numeric96::convertDecimal(n, fixedPrecision, Numeric96::plain, 4U));

#if defined (RSL_PL_WIN32) // [!!!:x64] && !defined (RSL_PL_WIN64)
        data.unused_ = '\0';
#else
        data[0] = '\0';
#endif
        return *this;
       }

       // Arithmetic assignment operators -----------------------------------------------------------
       DBNumeric96 &operator += (const Numeric96 &n)
       {
        *this = *this + n;

        return *this;
       }

       DBNumeric96 &operator -= (const Numeric96 &n)
       {
        *this = *this - n;

        return *this;
       }

       DBNumeric96 &operator *= (const Numeric96 &n)
       {
        *this = *this * n;

        return *this;
       }

       DBNumeric96 &operator /= (const Numeric96 &n)
       {
        *this = *this / n;

        return *this;
       }

       // Utility functions -------------------------------------------------------------------------
       static _TOOLEXP DBNumeric96 maxValue();
       static _TOOLEXP DBNumeric96 minValue();
       static _TOOLEXP DBNumeric96 zeroValue();
    };

// Convert to DBNumeric96
inline Numeric96::operator DBNumeric96() const
{
 DBNumeric96  result;

 return result.operator = (*this);
}

// -----------------------------------------------------------------------------
// Какой алогоритм конвертации Numeric <-> DBNumeric использовать - новый или старый
void SetNumericConvertNew(bool flag);
bool GetNumericConvertNew(void);

#endif //__cplusplus

#endif //__RSNUMERIC_H

/* <-- EOF --> */