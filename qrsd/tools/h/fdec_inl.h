/*******************************************************************************
 FILE         :   FDEC_INL.H

 COPYRIGHT    :   R-Style Software Lab, 2001

 DESCRIPTION  :   Inline functions for family of classes FDecimal and FMoney.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   15 May 2001

 VERSION INFO :   $Archive:     $
                  $Revision:    $
                  $Date:        $
                  $Author:      $
                  $Modtime:     $
*******************************************************************************/

#ifndef __FDEC_INL_H
#define __FDEC_INL_H

#include "fdecimal.h"

#ifdef FDECIMAL_WCHAR
#include <wchar.h>
#endif


#if defined(_MSC_VER)

    template <class Type>
    const FMoney_Value *InitMoneyValue(FMoney_Value *pValue, Type Operand)
    {
     if(FMoney_Mode::is1to1cvt())
       *(FDecimal_Value*)pValue = Operand;
     else
       *pValue = Operand;

     return pValue;
    }

    inline double ConvertToDouble(const FMoney_Value &Value)
    {
     if(FMoney_Mode::is1to1cvt ())
       return (double)(const FDecimal_Value &)Value;
     else
       return (double)Value;
    }

    inline long double ConvertToLDouble(const FMoney_Value &Value)
    {
     if(FMoney_Mode::is1to1cvt())
       return (long double)(const FDecimal_Value &)Value;
     else
       return (long double)Value;
    }

#endif //#if defined(_MSC_VER)

/*******************************************************************************
   FDecimal
   Constructors
*******************************************************************************/

FDECIMAL_INLINE FDecimal::FDecimal()
{
}

FDECIMAL_INLINE FDecimal::FDecimal(int Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(long Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(int64_t Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(short Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(unsigned int Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(unsigned long Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(uint64_t Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(unsigned short Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(float Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(double Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(long double Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(const char *pRawStr) : m_Value(pRawStr)
{
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE FDecimal::FDecimal(const wchar_t *pRawStr ) : m_Value(pRawStr)
    {
    }
#endif

FDECIMAL_INLINE FDecimal::FDecimal(const FDecimal_Value &Operand) : m_Value(Operand)
{
}

FDECIMAL_INLINE FDecimal::FDecimal(const FDecimal &other) 
{
 if(this != &other)
   *this = other;
}

/*******************************************************************************
   FDecimal
   Conversions
*******************************************************************************/

FDECIMAL_INLINE FDecimal::operator BaseFDecimal() const
{
 BaseFDecimal  Result;

 (FDecimal_Value &)Result.m_Value = m_Value;

 return Result;
}

FDECIMAL_INLINE FDecimal::operator DBBaseFDecimal() const
{
 DBBaseFDecimal  Result;

 Result.m_DBValue = (DBValue<FDecimal_Value>)m_Value;

 return Result;
}

double FDECIMAL_INLINE FDecimal::castDouble() const
{
 return (double)m_Value;
}

long double FDECIMAL_INLINE FDecimal::castLDouble() const
{
 return (long double)m_Value;
}

/*******************************************************************************
   FDecimal
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE FDecimal &FDecimal::operator += (const FDecimal &Operand)
{
 m_Value += Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator -= (const FDecimal &Operand)
{
 m_Value -= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (const FDecimal &Operand)
{
 m_Value *= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator /= (const FDecimal &Operand)
{
 m_Value /= Operand.m_Value;

 return *this;
}

/*******************************************************************************
   FDecimal
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FDecimal FDecimal::operator - () const
{
 return -m_Value;
}

FDECIMAL_INLINE bool FDecimal::operator ! () const
{
 return !m_Value;
}

/*******************************************************************************
   FDecimal
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FDecimal FDecimal::Abs() const
{
 return m_Value.Abs();
}

FDECIMAL_INLINE FDecimal FDecimal::Round(int nPos, int Mode) const
{
 return m_Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *FDecimal::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 return m_Value.FormatRawStr(pRawStr, StrLen);
}

FDECIMAL_INLINE char *FDecimal::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 return m_Value.FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *FDecimal::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     return m_Value.FormatRawStr(pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *FDecimal::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     return m_Value.FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   FDecimal
   Interface for struct FDecimal_t
*******************************************************************************/

#ifdef FDECIMAL_T

    FDECIMAL_INLINE FDecimal::FDecimal(const FDecimal_t &t) : m_Value((const FDecimal_Value &)t)
    {
    }

    FDECIMAL_INLINE void FDecimal::get(FDecimal_t *t) const
    {
     *t = *(FDecimal_t *)&m_Value;
    }

    FDECIMAL_INLINE FDecimal::operator FDecimal_t() const
    {
     return *(FDecimal_t *)this;
    }

#endif //FDECIMAL_T

/*******************************************************************************
   FDecimal
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   FDecimal
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (float Operand)
{
 long double  tempVal = (long double)(Fixed_Value &)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (double Operand)
{
 long double  tempVal = (long double)(Fixed_Value&)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE FDecimal &FDecimal::operator *= (long double Operand)
{
 long double  tempVal = (long double)(Fixed_Value &)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

/*******************************************************************************
   BaseFDecimal
   Conversions
*******************************************************************************/

FDECIMAL_INLINE BaseFDecimal::operator FDecimal() const
{
 FDecimal_Value  Value = m_Value;

 return Value;
}

FDECIMAL_INLINE BaseFDecimal::operator DBBaseFDecimal() const
{
 DBBaseFDecimal  Result;

 Result.m_DBValue = (DBValue<FDecimal_Value>)m_Value;

 return Result;
}

double FDECIMAL_INLINE BaseFDecimal::castDouble() const
{
 FDecimal_Value  Value = m_Value;

 return (double)Value;
}

long double FDECIMAL_INLINE BaseFDecimal::castLDouble() const
{
 FDecimal_Value  Value = m_Value;

 return (long double)Value;
}

/*******************************************************************************
   BaseFDecimal
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator += (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value += Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator -= (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value -= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value *= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator /= (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value /= Operand.m_Value;

 return *this;
}

/*******************************************************************************
   BaseFDecimal
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FDecimal BaseFDecimal::operator - () const
{
 FDecimal_Value  Value = m_Value;

 return -Value;
}

FDECIMAL_INLINE bool BaseFDecimal::operator ! () const
{
 FDecimal_Value  Value = m_Value;

 return !Value;
}

/*******************************************************************************
   BaseFDecimal
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FDecimal BaseFDecimal::Abs() const
{
 FDecimal_Value  Value = m_Value;

 return Value.Abs();
}

FDECIMAL_INLINE FDecimal BaseFDecimal::Round(int nPos, int Mode) const
{
 FDecimal_Value  Value = m_Value;

 return Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *BaseFDecimal::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 FDecimal_Value  Value = m_Value;

 return Value.FormatRawStr(pRawStr, StrLen);
}

FDECIMAL_INLINE char *BaseFDecimal::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 FDecimal_Value  Value = m_Value;

 return Value.FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *BaseFDecimal::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     FDecimal_Value Value = m_Value;

     return Value.FormatRawStr(pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *BaseFDecimal::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     FDecimal_Value  Value = m_Value;

     return Value.FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   BaseFDecimal
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   BaseFDecimal
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (float Operand)
{
 long double  tempVal = (long double)(Fixed_Value &)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (double Operand)
{
 long double  tempVal = (long double)(Fixed_Value &)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE BaseFDecimal &BaseFDecimal::operator *= (long double Operand)
{
 long double  tempVal = (long double)(Fixed_Value &)m_Value;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

/*******************************************************************************
   DBBaseFDecimal
   Conversions
*******************************************************************************/

FDECIMAL_INLINE DBBaseFDecimal::operator FDecimal() const
{
   FDecimal_Value  Value = m_DBValue;

   return Value;
}

FDECIMAL_INLINE DBBaseFDecimal::operator BaseFDecimal() const
{
   BaseFDecimal  Result;

   (FDecimal_Value &)Result.m_Value = m_DBValue;

   return Result;
}

double FDECIMAL_INLINE DBBaseFDecimal::castDouble() const
{
   FDecimal_Value  Value = m_DBValue;

   return (double)Value;
}

long double FDECIMAL_INLINE DBBaseFDecimal::castLDouble() const
{
   FDecimal_Value  Value = m_DBValue;

   return (long double)Value;
}

/*******************************************************************************
   DBBaseFDecimal
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator += (const FDecimal &Operand)
{
 FDecimal  Result = *this;

 Result += Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator -= (const FDecimal &Operand)
{
 FDecimal  Result = *this;

 Result -= Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (const FDecimal &Operand)
{
 FDecimal  Result = *this;

 Result *= Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator /= (const FDecimal &Operand)
{
 FDecimal  Result = *this;

 Result /= Operand;
 *this = Result;

 return *this;
}

/*******************************************************************************
   DBBaseFDecimal
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FDecimal DBBaseFDecimal::operator - () const
{
 FDecimal_Value  Value = m_DBValue;

 return -Value;
}

FDECIMAL_INLINE bool DBBaseFDecimal::operator ! () const
{
 FDecimal_Value  Value = m_DBValue;

 return !Value;
}

/*******************************************************************************
   DBBaseFDecimal
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FDecimal DBBaseFDecimal::Abs() const
{
 FDecimal_Value  Value = m_DBValue;

 return Value.Abs();
}

FDECIMAL_INLINE FDecimal DBBaseFDecimal::Round(int nPos, int Mode) const
{
 FDecimal_Value  Value = m_DBValue;

 return Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *DBBaseFDecimal::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 FDecimal_Value  Value = m_DBValue;

 return Value.FormatRawStr(pRawStr, StrLen);
}

FDECIMAL_INLINE char *DBBaseFDecimal::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 FDecimal_Value  Value = m_DBValue;

 return Value.FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *DBBaseFDecimal::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     FDecimal_Value  Value = m_DBValue;

     return Value.FormatRawStr(pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *DBBaseFDecimal::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     FDecimal_Value  Value = m_DBValue;

     return Value.FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   DBBaseFDecimal
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   DBBaseFDecimal
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (float Operand)
{
 long double  tempVal = (long double)(Fixed_Value)(DBBaseValue<Fixed_Value> &)m_DBValue;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (double Operand)
{
 long double  tempVal = (long double)(Fixed_Value)(DBBaseValue<Fixed_Value> &)m_DBValue;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

FDECIMAL_INLINE DBBaseFDecimal &DBBaseFDecimal::operator *= (long double Operand)
{
 long double  tempVal = (long double)(Fixed_Value)(DBBaseValue<Fixed_Value> &)m_DBValue;

 *this = (FDecimal)((tempVal * Operand) / Factor());

 return *this;
}

/*******************************************************************************
   DBFDecimal
   Constructors
*******************************************************************************/

FDECIMAL_INLINE DBFDecimal::DBFDecimal()
{
 m_DBValue = (DBValue<FDecimal_Value>)FDecimal_Value::ZeroValue();
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(int Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(long Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>) Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(int64_t Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>) Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(short Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(unsigned int Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(unsigned long Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(uint64_t Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(unsigned short Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(float Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(double Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(long double Operand)
{
 FDecimal_Value  Value = Operand;

 m_DBValue = (DBValue<FDecimal_Value>)Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(const FDecimal& Operand)
{
 m_DBValue = (DBValue<FDecimal_Value>)Operand.m_Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(const BaseFDecimal& Operand)
{
 m_DBValue = (DBValue<FDecimal_Value>)Operand.m_Value;
}

FDECIMAL_INLINE DBFDecimal::DBFDecimal(const DBBaseFDecimal& Operand)
{
 m_DBValue = ((DBFDecimal &)Operand).m_DBValue;
}

/*******************************************************************************
   FMoney
   Constructors
*******************************************************************************/

FDECIMAL_INLINE FMoney::FMoney ()
{
}

#if defined(_MSC_VER)

    FDECIMAL_INLINE FMoney::FMoney(int Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(long Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(int64_t Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(short Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned int Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned long Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(uint64_t Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned short Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(float Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(double Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(long double Operand) : m_Value(*InitMoneyValue(&m_Value, Operand))
    {
    }

#else //#if defined(_MSC_VER)

    FDECIMAL_INLINE FMoney::FMoney(int Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(long Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(int64_t Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(short Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned int Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned long Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(uint64_t Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(unsigned short Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(float Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(double Operand) : m_Value(Operand)
    {
    }

    FDECIMAL_INLINE FMoney::FMoney(long double Operand) : m_Value(Operand)
    {
    }

#endif //#if defined(_MSC_VER)

FDECIMAL_INLINE FMoney::FMoney(const char *pRawStr) : m_Value((const FMoney_Value &)FDecimal_Value(pRawStr))
{
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE FMoney::FMoney(const wchar_t *pRawStr) : m_Value((const FMoney_Value &)FDecimal_Value(pRawStr))
    {
    }
#endif

FDECIMAL_INLINE FMoney::FMoney(const FMoney_Value &Value) : m_Value(Value)
{
}

FDECIMAL_INLINE FMoney::FMoney(const FMoney &other) 
{
 if(this != &other)
   *this = other;  
}

/*******************************************************************************
   FMoney
   Conversions
*******************************************************************************/

FDECIMAL_INLINE FMoney::operator BaseFMoney() const
{
 BaseFMoney  Result;

 (FMoney_Value &)Result.m_Value = m_Value;

 return Result;
}

FDECIMAL_INLINE FMoney::operator DBBaseFMoney() const
{
 DBBaseFMoney  Result;

 Result.m_DBValue = (DBValue<FMoney_Value>)m_Value;

 return Result;
}

#if defined(_MSC_VER)
    double FDECIMAL_INLINE FMoney::castDouble() const
    {
     return ConvertToDouble(m_Value);
    }

    long double FDECIMAL_INLINE FMoney::castLDouble() const
    {
     return ConvertToLDouble(m_Value);
    }
#else //#if defined(_MSC_VER)
    double FDECIMAL_INLINE FMoney::castDouble() const
    {
     return (double)m_Value;
    }

    long double FDECIMAL_INLINE FMoney::castLDouble() const
    {
     return (long double)m_Value;
    }
#endif //#if defined(_MSC_VER)

/*******************************************************************************
   FMoney
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE FMoney &FMoney::operator += (const FMoney &Operand)
{
 m_Value += Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FMoney &FMoney::operator -= (const FMoney &Operand)
{
 m_Value -= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (const FDecimal &Operand)
{
 (FDecimal_Value &) m_Value *= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE FMoney &FMoney::operator /= (const FDecimal &Operand)
{
 (FDecimal_Value &) m_Value /= Operand.m_Value;

 return *this;
}

/*******************************************************************************
   FMoney
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FMoney FMoney::operator - () const
{
 return -m_Value;
}

FDECIMAL_INLINE bool FMoney::operator ! () const
{
 return !m_Value;
}

/*******************************************************************************
   FMoney
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FMoney FMoney::Abs() const
{
 return m_Value.Abs();
}

FDECIMAL_INLINE FMoney FMoney::Round(int nPos, int Mode) const
{
 return m_Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *FMoney::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 return ((const FDecimal_Value &)m_Value).FormatRawStr(2, pRawStr, StrLen);
}

FDECIMAL_INLINE char *FMoney::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 return ((const FDecimal_Value &)m_Value).FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *FMoney::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     return ((const FDecimal_Value &)m_Value).FormatRawStr(2, pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *FMoney::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     return ((const FDecimal_Value &)m_Value).FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   FMoney
   Interface for struct FDecimal_t
*******************************************************************************/

#ifdef FDECIMAL_T
    FDECIMAL_INLINE FMoney::FMoney(const FDecimal_t &t) : m_Value((const FMoney_Value &)t)
    {
    }

    FDECIMAL_INLINE void FMoney::get(FDecimal_t *t) const
    {
     *t = *(FDecimal_t *)&m_Value;
    }

    FDECIMAL_INLINE FMoney::operator FDecimal_t() const
    {
     return *(FDecimal_t *)this;
    }
#endif //FDECIMAL_T

/*******************************************************************************
   FMoney
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE FMoney &FMoney::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   FMoney
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE FMoney &FMoney::operator *= (float Operand)
{
 long double  tempVal = (long double)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (double Operand)
{
 long double  tempVal = (long double)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE FMoney &FMoney::operator *= (long double Operand)
{
 long double  tempVal = (long double)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

/*******************************************************************************
   BaseFMoney
   Conversions
*******************************************************************************/

FDECIMAL_INLINE BaseFMoney::operator FMoney() const
{
 FMoney_Value  Value = m_Value;

 return Value;
}

FDECIMAL_INLINE BaseFMoney::operator DBBaseFMoney() const
{
 DBBaseFMoney  Result;

 Result.m_DBValue = (DBValue<FMoney_Value>)m_Value;

 return Result;
}

#if defined(_MSC_VER)
    double FDECIMAL_INLINE BaseFMoney::castDouble() const
    {
     FMoney_Value  Value = m_Value;

     return ConvertToDouble(Value);
    }

    long double FDECIMAL_INLINE BaseFMoney::castLDouble() const
    {
     FMoney_Value  Value = m_Value;

     return ConvertToLDouble(Value);
    }
#else //#if defined(_MSC_VER)
    double FDECIMAL_INLINE BaseFMoney::castDouble() const
    {
     FMoney_Value  Value = m_Value;

     return (double)Value;
    }

    long double FDECIMAL_INLINE BaseFMoney::castLDouble() const
    {
     FMoney_Value  Value = m_Value;

     return (long double)Value;
    }
#endif //#if defined(_MSC_VER)

/*******************************************************************************
   BaseFMoney
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator += (const FMoney &Operand)
{
 (FMoney_Value &)m_Value += Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator -= (const FMoney &Operand)
{
 (FMoney_Value &)m_Value -= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value *= Operand.m_Value;

 return *this;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator /= (const FDecimal &Operand)
{
 (FDecimal_Value &)m_Value /= Operand.m_Value;

 return *this;
}

/*******************************************************************************
   BaseFMoney
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FMoney BaseFMoney::operator - () const
{
 FMoney_Value  Value = m_Value;

 return -Value;
}

FDECIMAL_INLINE bool BaseFMoney::operator ! () const
{
 FMoney_Value  Value = m_Value;

 return !Value;
}


/*******************************************************************************
   BaseFMoney
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FMoney BaseFMoney::Abs() const
{
 FMoney_Value  Value = m_Value;

 return Value.Abs();
}

FDECIMAL_INLINE FMoney BaseFMoney::Round(int nPos, int Mode) const
{
 FMoney_Value  Value = m_Value;

 return Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *BaseFMoney::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 FMoney_Value  Value = m_Value;

 return ((const FDecimal_Value &)Value).FormatRawStr(2, pRawStr, StrLen);
}

FDECIMAL_INLINE char *BaseFMoney::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 FMoney_Value  Value = m_Value;

 return ((const FDecimal_Value &)Value).FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *BaseFMoney::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     FMoney_Value  Value = m_Value;

     return ((const FDecimal_Value &)Value).FormatRawStr(2, pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *BaseFMoney::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     FMoney_Value  Value = m_Value;

     return ((const FDecimal_Value &)Value).FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   BaseFMoney
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   BaseFMoney
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (float Operand)
{
 long double  tempVal = (long double)(FMoney_Value&)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (double Operand)
{
 long double  tempVal = (long double)(FMoney_Value&)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE BaseFMoney &BaseFMoney::operator *= (long double Operand)
{
 long double  tempVal = (long double)(FMoney_Value&)m_Value;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

/*******************************************************************************
   DBBaseFMoney
   Conversions
*******************************************************************************/

FDECIMAL_INLINE DBBaseFMoney::operator FMoney() const
{
 FMoney_Value  Value = m_DBValue;

 return Value;
}

FDECIMAL_INLINE DBBaseFMoney::operator BaseFMoney() const
{
 BaseFMoney  Result;

 (FMoney_Value &)Result.m_Value = (FMoney_Value)m_DBValue;

 return Result;
}

#if defined(_MSC_VER)
    double FDECIMAL_INLINE DBBaseFMoney::castDouble() const
    {
     FMoney_Value  Value = m_DBValue;

     return ConvertToDouble(Value);
    }

    long double FDECIMAL_INLINE DBBaseFMoney::castLDouble() const
    {
     FMoney_Value  Value = m_DBValue;

     return ConvertToLDouble(Value);
    }
#else //#if defined(_MSC_VER)
    double FDECIMAL_INLINE DBBaseFMoney::castDouble() const
    {
     FMoney_Value  Value = m_DBValue;

     return (double)Value;
    }

    long double FDECIMAL_INLINE DBBaseFMoney::castLDouble() const
    {
       FMoney_Value  Value = m_DBValue;

       return (long double)Value;
    }
#endif //#if defined(_MSC_VER)

/*******************************************************************************
   DBBaseFMoney
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator += (const FMoney &Operand)
{
 FMoney  Result = *this;

 Result += Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator -= (const FMoney &Operand)
{
 FMoney  Result = *this;

 Result -= Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (const FDecimal &Operand)
{
 FMoney  Result = *this;

 Result *= Operand;
 *this = Result;

 return *this;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator /= (const FDecimal &Operand)
{
 FMoney  Result = *this;

 Result /= Operand;
 *this = Result;

 return *this;
}

/*******************************************************************************
   DBBaseFMoney
   Unary operators
*******************************************************************************/

FDECIMAL_INLINE FMoney DBBaseFMoney::operator - () const
{
 FMoney_Value  Value = m_DBValue;

 return -Value;
}

FDECIMAL_INLINE bool DBBaseFMoney::operator ! () const
{
 FMoney_Value  Value = m_DBValue;

 return !Value;
}


/*******************************************************************************
   DBBaseFMoney
   Member functions
*******************************************************************************/

FDECIMAL_INLINE FMoney DBBaseFMoney::Abs() const
{
 FMoney_Value  Value = m_DBValue;

 return Value.Abs();
}

FDECIMAL_INLINE FMoney DBBaseFMoney::Round(int nPos, int Mode) const
{
 FMoney_Value  Value = m_DBValue;

 return Value.Round(nPos, Mode);
}

FDECIMAL_INLINE char *DBBaseFMoney::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 FMoney_Value  Value = m_DBValue;

 return ((const FDecimal_Value &)Value).FormatRawStr(2, pRawStr, StrLen);
}

FDECIMAL_INLINE char *DBBaseFMoney::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 FMoney_Value  Value = m_DBValue;

 return ((const FDecimal_Value &)Value).FormatRawStr(Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    FDECIMAL_INLINE wchar_t *DBBaseFMoney::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     FMoney_Value  Value = m_DBValue;

     return ((const FDecimal_Value &) Value).FormatRawStr(2, pRawStr, StrLen);
    }

    FDECIMAL_INLINE wchar_t *DBBaseFMoney::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     FMoney_Value  Value = m_DBValue;

     return ((const FDecimal_Value &)Value).FormatRawStr(Precision, pRawStr, StrLen);
    }
#endif

/*******************************************************************************
   DBBaseFMoney
   Integer multiplication
*******************************************************************************/

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (int64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (short Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (unsigned int Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (unsigned long Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (uint64_t Operand)
{
 return *this *= (FDecimal)Operand;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (unsigned short Operand)
{
 return *this *= (FDecimal)Operand;
}

/*******************************************************************************
   DBBaseFMoney
   Floating point multiplication
*******************************************************************************/

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (float Operand)
{
 long double  tempVal = (long double)(FMoney_Value)(DBBaseValue<FMoney_Value> &)m_DBValue;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (double Operand)
{
 long double  tempVal = (long double)(FMoney_Value)(DBBaseValue<FMoney_Value> &)m_DBValue;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

FDECIMAL_INLINE DBBaseFMoney &DBBaseFMoney::operator *= (long double Operand)
{
 long double  tempVal = (long double)(FMoney_Value)(DBBaseValue<FMoney_Value> &)m_DBValue;

 *this = (FMoney)(tempVal * Operand);

 return *this;
}

/*******************************************************************************
   DBFMoney
   Constructors
*******************************************************************************/

FDECIMAL_INLINE DBFMoney::DBFMoney()
{
 m_DBValue = (DBValue<FMoney_Value>)FMoney_Value::ZeroValue();
}

#if defined(_MSC_VER)

    FDECIMAL_INLINE DBFMoney::DBFMoney(int Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(long Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(int64_t Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(short Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned int Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned long Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(uint64_t Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned short Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(float Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(double Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(long double Operand)
    {
     FMoney_Value  Value;

     InitMoneyValue(&Value, Operand);

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

#else //#if defined(_MSC_VER)

    FDECIMAL_INLINE DBFMoney::DBFMoney(int Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(long Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(int64_t Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(short Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned int Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned long Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(uint64_t Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(unsigned short Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(float Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(double Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

    FDECIMAL_INLINE DBFMoney::DBFMoney(long double Operand)
    {
     FMoney_Value  Value = Operand;

     m_DBValue = (DBValue<FMoney_Value>)Value;
    }

#endif //#if defined(_MSC_VER)

FDECIMAL_INLINE DBFMoney::DBFMoney(const FMoney &Operand)
{
 m_DBValue = (DBValue<FMoney_Value>)Operand.m_Value;
}

FDECIMAL_INLINE DBFMoney::DBFMoney(const BaseFMoney &Operand)
{
 FMoney_Value  Value = Operand.m_Value;

 m_DBValue = (DBValue<FMoney_Value>)Value;
}

FDECIMAL_INLINE DBFMoney::DBFMoney(const DBBaseFMoney &Operand)
{
 m_DBValue = ((DBFMoney &)Operand).m_DBValue;
}

/*******************************************************************************
   FDecimal <-> FMoney
   Conversions
*******************************************************************************/

FDECIMAL_INLINE FDecimal::FDecimal(const FMoney &Operand) : m_Value((const FDecimal_Value &) Operand)
{
#if defined(_MSC_VER)
   if(!FMoney_Mode::is1to1cvt())
#endif
    {
     m_Value *= 100;
    }
}

FDECIMAL_INLINE FMoney::FMoney(const FDecimal &Operand) : m_Value((const FMoney_Value &) Operand)
{
#if defined(_MSC_VER)
   if(!FMoney_Mode::is1to1cvt())
#endif
    {
     m_Value /= 100;
    }
}

/*******************************************************************************
   FDecimal
   fabs, floor, ceil, fmod
   fabsl, floorl, ceill, fmodl
*******************************************************************************/

#if !defined(__TOS_OS400__) || __OS400_TGTVRM__ > 370
    FDECIMAL_INLINE FDecimal fabs(const FDecimal &Operand)
    {
     return Operand.Abs();
    }
#endif

FDECIMAL_INLINE FDecimal floor(const FDecimal &Operand)
{
 return Operand.Round(0, FDecimal_Floor);
}

FDECIMAL_INLINE FDecimal ceil(const FDecimal &Operand)
{
 return Operand.Round(0, FDecimal_Ceil);
}

FDECIMAL_INLINE FDecimal fmod(const FDecimal &x, const FDecimal &y)
{
 FDecimal  d = x/y;

 return x - y*d.Round(0, FDecimal_Round);
}

FDECIMAL_INLINE FDecimal fabsl(const FDecimal &Operand)
{
 return Operand.Abs();
}

FDECIMAL_INLINE FDecimal floorl(const FDecimal &Operand)
{
 return Operand.Round(0, FDecimal_Floor);
}

FDECIMAL_INLINE FDecimal ceill(const FDecimal &Operand)
{
 return Operand.Round(0, FDecimal_Ceil);
}

FDECIMAL_INLINE FDecimal fmodl(const FDecimal &x, const FDecimal &y)
{
 FDecimal  d = x/y;

 return x - y*d.Round(0, FDecimal_Round);
}

/*******************************************************************************
   FMoney
   fabs, floor, ceil, fmod
   fabsl, floorl, ceill, fmodl
*******************************************************************************/

#if !defined(__TOS_OS400__) || __OS400_TGTVRM__ > 370
    FDECIMAL_INLINE FMoney fabs(const FMoney &Operand)
    {
     return Operand.Abs();
    }
#endif

FDECIMAL_INLINE FMoney floor(const FMoney &Operand)
{
 return Operand.Round(0, FDecimal_Floor);
}

FDECIMAL_INLINE FMoney ceil(const FMoney &Operand)
{
 return Operand.Round(0, FDecimal_Ceil);
}

FDECIMAL_INLINE FMoney fmod(const FMoney &x, const FMoney &y)
{
 FDecimal  d = x/y;

 return x - y*d.Round(0, FDecimal_Round);
}

FDECIMAL_INLINE FMoney fabsl(const FMoney &Operand)
{
 return Operand.Abs();
}

FDECIMAL_INLINE FMoney floorl(const FMoney &Operand)
{
 return Operand.Round(0, FDecimal_Floor);
}

FDECIMAL_INLINE FMoney ceill(const FMoney &Operand)
{
 return Operand.Round(0, FDecimal_Ceil);
}

FDECIMAL_INLINE FMoney fmodl(const FMoney &x, const FMoney &y)
{
 FDecimal  d = x/y;

 return x - y*d.Round(0, FDecimal_Round);
}

/*******************************************************************************
   FDecimal
   Min, max and zero values
*******************************************************************************/

FDECIMAL_INLINE FDecimal FDecimal_MinValue()
{
 return FDecimal_Value::MinValue();
}

FDECIMAL_INLINE FDecimal FDecimal_MaxValue()
{
 return FDecimal_Value::MaxValue();
}

FDECIMAL_INLINE FDecimal FDecimal_ZeroValue()
{
 return FDecimal_Value::ZeroValue();
}

/*******************************************************************************
   FMoney
   Min, max and zero values
*******************************************************************************/

FDECIMAL_INLINE FMoney FMoney_MinValue()
{
 return FMoney_Value::MinValue();
}

FDECIMAL_INLINE FMoney FMoney_MaxValue()
{
 return FMoney_Value::MaxValue();
}

FDECIMAL_INLINE FMoney FMoney_ZeroValue()
{
 return FMoney_Value::ZeroValue();
}

/*******************************************************************************
   FDecimal global
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE FDecimal operator + (const FDecimal &Operand, const FDecimal &Operand2)
{
 FDecimal  Result = Operand;

 Result += Operand2;

 return Result;
}

FDECIMAL_INLINE FDecimal operator - (const FDecimal &Operand, const FDecimal &Operand2)
{
 FDecimal  Result = Operand;

 Result -= Operand2;

 return Result;
}

FDECIMAL_INLINE FDecimal operator * (const FDecimal &Operand, const FDecimal &Operand2)
{
 FDecimal  Result = Operand;

 Result *= Operand2;

 return Result;
}

FDECIMAL_INLINE FDecimal operator / (const FDecimal &Operand, const FDecimal &Operand2)
{
 FDecimal  Result = Operand;

 Result /= Operand2;

 return Result;
}

/*******************************************************************************
   FMoney global
   Arithmetical operators
*******************************************************************************/

FDECIMAL_INLINE FMoney operator + (const FMoney &Operand, const FMoney &Operand2)
{
 FMoney  Result = Operand;

 Result += Operand2;

 return Result;
}

FDECIMAL_INLINE FMoney operator - (const FMoney &Operand, const FMoney &Operand2)
{
 FMoney  Result = Operand;

 Result -= Operand2;

 return Result;
}

FDECIMAL_INLINE FMoney operator * (const FMoney &Operand, const FDecimal &Operand2)
{
 FMoney  Result = Operand;

 Result *= Operand2;

 return Result;
}

FDECIMAL_INLINE FMoney operator * (const FDecimal &Operand, const FMoney &Operand2)
{
 FMoney  Result = Operand2;

 Result *= Operand;

 return Result;
}

FDECIMAL_INLINE FMoney operator / (const FMoney &Operand, const FDecimal &Operand2)
{
 FMoney  Result = Operand;

 Result /= Operand2;

 return Result;
}

FDECIMAL_INLINE FDecimal operator / (const FMoney &Operand, const FMoney &Operand2)
{
 FDecimal  Result = (FDecimal &)Operand;

 Result /= (FDecimal &)Operand2;

 return Result;
}

/*******************************************************************************
   FDecimal global
   Logical operators
*******************************************************************************/

FDECIMAL_INLINE bool operator == (const FDecimal &Operand, const FDecimal &Operand2)
{
 return Operand.m_Value == Operand2.m_Value;
}

FDECIMAL_INLINE bool operator < (const FDecimal &Operand, const FDecimal &Operand2)
{
 return Operand.m_Value < Operand2.m_Value;
}

#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))
    FDECIMAL_INLINE bool operator != (const FDecimal &Operand, const FDecimal &Operand2)
    {
     return Operand.m_Value != Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator > (const FDecimal &Operand, const FDecimal &Operand2)
    {
     return Operand.m_Value > Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator <= (const FDecimal &Operand, const FDecimal &Operand2)
    {
     return Operand.m_Value <= Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator >= (const FDecimal &Operand, const FDecimal &Operand2)
    {
     return Operand.m_Value >= Operand2.m_Value;
    }
#endif //#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))

/*******************************************************************************
   FMoney global
   Logical operators
*******************************************************************************/

FDECIMAL_INLINE bool operator == (const FMoney &Operand, const FMoney &Operand2)
{
 return Operand.m_Value == Operand2.m_Value;
}

FDECIMAL_INLINE bool operator < (const FMoney &Operand, const FMoney &Operand2)
{
 return Operand.m_Value < Operand2.m_Value;
}

#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))
    FDECIMAL_INLINE bool operator != (const FMoney &Operand, const FMoney &Operand2)
    {
     return Operand.m_Value != Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator > (const FMoney &Operand, const FMoney &Operand2)
    {
     return Operand.m_Value >  Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator <= (const FMoney &Operand, const FMoney &Operand2)
    {
     return Operand.m_Value <= Operand2.m_Value;
    }

    FDECIMAL_INLINE bool operator >= (const FMoney &Operand, const FMoney &Operand2)
    {
     return Operand.m_Value >= Operand2.m_Value;
    }
#endif //#if !(defined(RWSTD_NO_NAMESPACE) && defined(__STD_UTILITY__))

#endif // __FDEC_INL_H

/* <-- EOF --> */