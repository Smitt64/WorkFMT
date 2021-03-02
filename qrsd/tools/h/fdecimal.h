/*******************************************************************************
 FILE         :   FDECIMAL.H

 COPYRIGHT    :   R-Style Software Lab, 2000

 DESCRIPTION  :   Family of classes FDecimal, FMoney independent of realization.

 PROGRAMMED BY:   A.Dobroslavskiy
                  Alexey Lebedev

 CREATION DATE:   01 Dec 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/fdecimal.h $
                  $Revision: 48 $
                  $Date: 08.05.01 12:15 $
                  $Author: Lebedev $
                  $Modtime: 08.05.01 12:14 $
*******************************************************************************/

#ifndef __FDECIMAL_H
#define __FDECIMAL_H

#include "fdec_std.h"


/////////////////////////////////////////////////////////////////////////////
// The C++ part

#ifdef __cplusplus

#ifdef FDECIMAL_WCHAR
    #include <wchar.h>
#endif

#define  FDECIMAL_POINT   4
#define  FMONEY_POINT     2

// -----------------------------------------------------------------------------
#ifdef RSL_PL_MS

    #ifdef _MSC_VER
        // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
        #pragma warning(disable : 4251)
    #endif

    #include "fixed_i64.h"

    typedef FixedT_i64<FDECIMAL_POINT>  FDecimal_Value;
    typedef FixedT_i64<FMONEY_POINT>    FMoney_Value;
    typedef FixedT_i64<0>               Fixed_Value;

#endif //RSL_PL_MS

/////////////////////////////////////////////////////////////////////////////
// Умножение и деление целых чисел

#define FDECIMAL_INTEGER_BINARY(OpSymb, DecType, IntType)                        \
          inline FDecimal operator OpSymb (DecType decVal, IntType intVal)       \
          {                                                                      \
           return decVal OpSymb (FDecimal)intVal;                                \
          }                                                                      \
          inline FDecimal operator OpSymb (IntType intVal, DecType decVal)       \
          {                                                                      \
           return (FDecimal)intVal OpSymb decVal;                                \
          }

#define DBFDECIMAL_INTEGER_BINARY(OpSymb, DecType, IntType)                      \
          inline FDecimal operator OpSymb (DecType decVal, IntType intVal)       \
          {                                                                      \
           return (FDecimal)decVal OpSymb (FDecimal)intVal;                      \
          }                                                                      \
          inline FDecimal operator OpSymb (IntType intVal, DecType decVal)       \
          {                                                                      \
           return (FDecimal)intVal OpSymb (FDecimal)decVal;                      \
          }


#define FMONEY_INTEGER_1ST_DEC(OpSymb, MonType, IntType, RetType)                \
          inline RetType operator OpSymb (IntType intVal, MonType monVal)        \
          {                                                                      \
           return (FDecimal)intVal OpSymb monVal;                                \
          }

#define FMONEY_INTEGER_1ST_MON(OpSymb, MonType, IntType, RetType)                \
          inline RetType operator OpSymb (IntType intVal, MonType monVal)        \
          {                                                                      \
           return (FMoney)intVal OpSymb monVal;                                  \
          }

#define FMONEY_INTEGER_2ND_DEC(OpSymb, MonType, IntType, RetType)                \
          inline RetType operator OpSymb (MonType monVal, IntType intVal)        \
          {                                                                      \
           return monVal OpSymb (FDecimal)intVal;                                \
          }

#define FMONEY_INTEGER_2ND_MON(OpSymb, MonType, IntType, RetType)                \
          inline RetType operator OpSymb (MonType monVal, IntType intVal)        \
          {                                                                      \
           return monVal OpSymb (FMoney)intVal;                                  \
          }


#define DBFMONEY_INTEGER_1ST_DEC(OpSymb, MonType, IntType, RetType)              \
          inline RetType operator OpSymb (IntType intVal, MonType monVal)        \
          {                                                                      \
           return (FDecimal)intVal OpSymb (FMoney)monVal;                        \
          }

#define DBFMONEY_INTEGER_1ST_MON(OpSymb, MonType, IntType, RetType)              \
          inline RetType operator OpSymb (IntType intVal, MonType monVal)        \
          {                                                                      \
           return (FMoney)intVal OpSymb (FMoney)monVal;                          \
          }

#define DBFMONEY_INTEGER_2ND_DEC(OpSymb, MonType, IntType, RetType)              \
          inline RetType operator OpSymb (MonType monVal, IntType intVal)        \
          {                                                                      \
           return (FMoney)monVal OpSymb (FDecimal)intVal;                        \
          }

#define DBFMONEY_INTEGER_2ND_MON(OpSymb, MonType, IntType, RetType)              \
          inline RetType operator OpSymb (MonType monVal, IntType intVal)        \
          {                                                                      \
           return (FMoney)monVal OpSymb (FMoney)intVal;                          \
          }

/////////////////////////////////////////////////////////////////////////////
// Умножение и деление чисел с плавающей точкой

#define FDECIMAL_FLOAT_BINARY(OpSymb, DecType, FloatType)                               \
          inline long double operator OpSymb (DecType decVal, FloatType fltVal)         \
          {                                                                             \
             long double tempVal = (long double)(FDecimal_Value &)decVal;               \
             return tempVal OpSymb fltVal;                                              \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, DecType decVal)         \
          {                                                                             \
             long double tempVal = (long double)(FDecimal_Value &)decVal;               \
             return fltVal OpSymb tempVal;                                              \
          }

#define FDECIMAL_FLOAT_FIXED(OpSymb, DecType, FloatType, Factor)                        \
          inline long double operator OpSymb (DecType decVal, FloatType fltVal)         \
          {                                                                             \
             long double tempVal = (long double)(Fixed_Value &)decVal;                  \
             return (tempVal OpSymb fltVal) / Factor;                                   \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, DecType decVal)         \
          {                                                                             \
             long double tempVal = (long double)(Fixed_Value &)decVal;                  \
             return (fltVal OpSymb tempVal) / Factor;                                   \
          }


#define DBFDECIMAL_FLOAT_BINARY(OpSymb, DecType, FloatType)                             \
          inline long double operator OpSymb (DecType decVal, FloatType fltVal)         \
          {                                                                             \
             FDecimal_Value fdecVal = (DBBaseValue<FDecimal_Value> &)decVal;            \
             long double tempVal = (long double)fdecVal;                                \
             return tempVal OpSymb fltVal;                                              \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, DecType decVal)         \
          {                                                                             \
             FDecimal_Value fdecVal = (DBBaseValue<FDecimal_Value> &)decVal;            \
             long double tempVal = (long double)fdecVal;                                \
             return fltVal OpSymb tempVal;                                              \
          }

#define DBFDECIMAL_FLOAT_FIXED(OpSymb, DecType, FloatType, Factor)                      \
          inline long double operator OpSymb (DecType decVal, FloatType fltVal)         \
          {                                                                             \
             Fixed_Value fixedVal = (DBBaseValue<Fixed_Value> &)decVal;                 \
             long double tempVal = (long double)fixedVal;                               \
             return (tempVal OpSymb fltVal) / Factor;                                   \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, DecType decVal)         \
          {                                                                             \
             Fixed_Value fixedVal = (DBBaseValue<Fixed_Value> &)decVal;                 \
             long double tempVal = (long double)fixedVal;                               \
             return (fltVal OpSymb tempVal) / Factor;                                   \
          }


#define FMONEY_FLOAT_BINARY(OpSymb, MonType, FloatType)                                 \
          inline long double operator OpSymb (MonType monVal, FloatType fltVal)         \
          {                                                                             \
             long double tempVal = (long double)(FMoney_Value &)monVal;                 \
             return tempVal OpSymb fltVal;                                              \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, MonType monVal)         \
          {                                                                             \
             long double tempVal = (long double)(FMoney_Value &)monVal;                 \
             return fltVal OpSymb tempVal;                                              \
          }

#define DBFMONEY_FLOAT_BINARY(OpSymb, MonType, FloatType)                               \
          inline long double operator OpSymb (MonType monVal, FloatType fltVal)         \
          {                                                                             \
             FMoney_Value fixedVal = (DBBaseValue<FMoney_Value> &)monVal;               \
             long double tempVal = (long double)fixedVal;                               \
             return tempVal OpSymb fltVal;                                              \
          }                                                                             \
          inline long double operator OpSymb (FloatType fltVal, MonType monVal)         \
          {                                                                             \
             FMoney_Value fixedVal = (DBBaseValue<FMoney_Value> &)monVal;               \
             long double tempVal = (long double)fixedVal;                               \
             return fltVal OpSymb tempVal;                                              \
          }


/////////////////////////////////////////////////////////////////////////////
// DBBaseValue

template <class Type>
class DBBaseValue
{
 public:
   // --------------------------------------------
   operator Type () const
   {
/* [!!!:x64]
 #if defined(RSL_PL_WIN64)
        Type  Result;

        memcpy(&Result, m_Value, sizeof(Type));

        return Result;
 #else
*/
        return *(Type *)m_Value;
// #endif
    }

  protected:
    // --------------------------------------------
    // --------------------------------------------
    char  m_Value[sizeof(Type)];
};

/////////////////////////////////////////////////////////////////////////////
// BaseValue

template <class Type>
class BaseValue
{
 public:
   // --------------------------------------------
   operator Type() const
   {
    return *(Type *)m_Value;
   }

 protected:
   // --------------------------------------------
   // --------------------------------------------
   char  m_Value[sizeof(Type)];
};

/////////////////////////////////////////////////////////////////////////////
// DBValue

template <class Type>
class DBValue : public DBBaseValue<Type>
{
 public:
   // --------------------------------------------
   DBValue()
   {
   }

   // --------------------------------------------
   DBValue(const Type &Value)
   {
/* [!!!:x64]
 #if defined(RSL_PL_WIN64)
        memcpy(m_Value, &Value, sizeof(Type));
 #else
*/
    *(Type *)m_Value = Value;
// #endif
   }

   DBValue(const DBBaseValue<Type> &Base)
   {
    *this = (DBValue<Type> &) Base;
   }
};

/////////////////////////////////////////////////////////////////////////////
// FDecimal

class FDECIMAL_DECLSPEC FDecimal
    {
       friend class BaseFDecimal;
       friend class DBBaseFDecimal;
       friend class DBFDecimal;
       friend class FMoney;
       friend class BaseFMoney;

     public:
       // Constructors
       FDECIMAL_INLINE FDecimal();
       FDECIMAL_INLINE FDecimal(int);
       FDECIMAL_INLINE FDecimal(long);
       FDECIMAL_INLINE FDecimal(int64_t);
       FDECIMAL_INLINE FDecimal(short);
       FDECIMAL_INLINE FDecimal(unsigned int);
       FDECIMAL_INLINE FDecimal(unsigned long);
       FDECIMAL_INLINE FDecimal(uint64_t);
       FDECIMAL_INLINE FDecimal(unsigned short);
       FDECIMAL_INLINE FDecimal(float);
       FDECIMAL_INLINE FDecimal(double);
       FDECIMAL_INLINE FDecimal(long double);

       explicit FDECIMAL_INLINE FDecimal(const char *);

 #ifdef FDECIMAL_WCHAR
       explicit FDECIMAL_INLINE FDecimal(const wchar_t *);
 #endif

       explicit FDECIMAL_INLINE FDecimal(const FMoney &);

       // Conversions
       FDECIMAL_INLINE operator    BaseFDecimal()   const;
       FDECIMAL_INLINE operator    DBBaseFDecimal() const;
       FDECIMAL_INLINE double      castDouble()     const;
       FDECIMAL_INLINE long double castLDouble()    const;

       // Operators
       FDECIMAL_INLINE FDecimal &operator += (const FDecimal &);
       FDECIMAL_INLINE FDecimal &operator -= (const FDecimal &);
       FDECIMAL_INLINE FDecimal &operator *= (const FDecimal &);
       FDECIMAL_INLINE FDecimal &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FDecimal operator - () const;
       FDECIMAL_INLINE bool     operator ! () const;

       // Member functions
       FDECIMAL_INLINE FDecimal Abs() const;
       FDECIMAL_INLINE FDecimal Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char    *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char    *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;

 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

 #ifdef FDECIMAL_T
       FDECIMAL_INLINE FDecimal(const FDecimal_t &);
       FDECIMAL_INLINE void get(FDecimal_t *) const;
       FDECIMAL_INLINE operator FDecimal_t() const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE FDecimal &operator *= (int Operand);
       FDECIMAL_INLINE FDecimal &operator *= (long Operand);
       FDECIMAL_INLINE FDecimal &operator *= (int64_t Operand);
       FDECIMAL_INLINE FDecimal &operator *= (short Operand);
       FDECIMAL_INLINE FDecimal &operator *= (unsigned int Operand);
       FDECIMAL_INLINE FDecimal &operator *= (unsigned long Operand);
       FDECIMAL_INLINE FDecimal &operator *= (uint64_t Operand);
       FDECIMAL_INLINE FDecimal &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE FDecimal &operator *= (float Operand);
       FDECIMAL_INLINE FDecimal &operator *= (double Operand);
       FDECIMAL_INLINE FDecimal &operator *= (long double Operand);

       static long double Factor() { return 10000.L; }

     protected:
       FDECIMAL_INLINE FDecimal(const FDecimal_Value &);

       FDecimal_Value m_Value;

       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal FDecimal_MinValue();
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal FDecimal_MaxValue();
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal FDecimal_ZeroValue();

       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator == (const FDecimal &, const FDecimal &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator != (const FDecimal &, const FDecimal &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator <= (const FDecimal &, const FDecimal &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator >= (const FDecimal &, const FDecimal &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator <  (const FDecimal &, const FDecimal &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator >  (const FDecimal &, const FDecimal &);
    };


/////////////////////////////////////////////////////////////////////////////
// BaseFDecimal

class FDECIMAL_DECLSPEC BaseFDecimal
    {
       friend class FDecimal;
       friend class DBBaseFDecimal;
       friend class DBFDecimal;

     public:
       // Conversions
       FDECIMAL_INLINE operator    FDecimal()       const;
       FDECIMAL_INLINE operator    DBBaseFDecimal() const;
       FDECIMAL_INLINE double      castDouble()     const;
       FDECIMAL_INLINE long double castLDouble()    const;

       // Operators
       FDECIMAL_INLINE BaseFDecimal &operator += (const FDecimal &);
       FDECIMAL_INLINE BaseFDecimal &operator -= (const FDecimal &);
       FDECIMAL_INLINE BaseFDecimal &operator *= (const FDecimal &);
       FDECIMAL_INLINE BaseFDecimal &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FDecimal operator - () const;
       FDECIMAL_INLINE bool     operator ! () const;

       // Member functions
       FDECIMAL_INLINE FDecimal Abs() const;
       FDECIMAL_INLINE FDecimal Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char    *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char    *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;

 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE BaseFDecimal &operator *= (int Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (long Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (int64_t Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (short Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (unsigned int Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (unsigned long Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (uint64_t Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE BaseFDecimal &operator *= (float Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (double Operand);
       FDECIMAL_INLINE BaseFDecimal &operator *= (long double Operand);

       static long double Factor() { return 10000.L; }

     protected:
       BaseValue<FDecimal_Value> m_Value;
    };


/////////////////////////////////////////////////////////////////////////////
// DBBaseFDecimal

class FDECIMAL_DECLSPEC DBBaseFDecimal
    {
       friend class FDecimal;
       friend class BaseFDecimal;

     public:
       // Conversions
       FDECIMAL_INLINE operator    FDecimal()     const;
       FDECIMAL_INLINE operator    BaseFDecimal() const;
       FDECIMAL_INLINE double      castDouble()   const;
       FDECIMAL_INLINE long double castLDouble()  const;

       // Operators
       FDECIMAL_INLINE DBBaseFDecimal &operator += (const FDecimal &);
       FDECIMAL_INLINE DBBaseFDecimal &operator -= (const FDecimal &);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (const FDecimal &);
       FDECIMAL_INLINE DBBaseFDecimal &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FDecimal operator - () const;
       FDECIMAL_INLINE bool     operator ! () const;

       // Member functions
       FDECIMAL_INLINE FDecimal Abs() const;
       FDECIMAL_INLINE FDecimal Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char    *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char    *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;

 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (int Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (long Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (int64_t Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (short Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (unsigned int Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (unsigned long Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (uint64_t Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (float Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (double Operand);
       FDECIMAL_INLINE DBBaseFDecimal &operator *= (long double Operand);

       static long double Factor() { return 10000.L; }

     protected:
       DBBaseValue<FDecimal_Value> m_DBValue;
    };


/////////////////////////////////////////////////////////////////////////////
// DBFDecimal

class FDECIMAL_DECLSPEC DBFDecimal : public DBBaseFDecimal
    {
     public:
       // Constructors
       FDECIMAL_INLINE DBFDecimal();
       FDECIMAL_INLINE DBFDecimal(int);
       FDECIMAL_INLINE DBFDecimal(long);
       FDECIMAL_INLINE DBFDecimal(int64_t);
       FDECIMAL_INLINE DBFDecimal(short);
       FDECIMAL_INLINE DBFDecimal(unsigned int);
       FDECIMAL_INLINE DBFDecimal(unsigned long);
       FDECIMAL_INLINE DBFDecimal(uint64_t);
       FDECIMAL_INLINE DBFDecimal(unsigned short);
       FDECIMAL_INLINE DBFDecimal(float);
       FDECIMAL_INLINE DBFDecimal(double);
       FDECIMAL_INLINE DBFDecimal(long double);

       FDECIMAL_INLINE DBFDecimal(const FDecimal &);
       FDECIMAL_INLINE DBFDecimal(const BaseFDecimal &);
       FDECIMAL_INLINE DBFDecimal(const DBBaseFDecimal &);
    };

/////////////////////////////////////////////////////////////////////////////
// FMoney

class FDECIMAL_DECLSPEC FMoney
    {
       friend class BaseFMoney;
       friend class DBBaseFMoney;
       friend class DBFMoney;

     public:
       // Constructors
       FDECIMAL_INLINE FMoney();
       FDECIMAL_INLINE FMoney(int);
       FDECIMAL_INLINE FMoney(long);
       FDECIMAL_INLINE FMoney(int64_t);
       FDECIMAL_INLINE FMoney(short);
       FDECIMAL_INLINE FMoney(unsigned int);
       FDECIMAL_INLINE FMoney(unsigned long);
       FDECIMAL_INLINE FMoney(uint64_t);
       FDECIMAL_INLINE FMoney(unsigned short);
       FDECIMAL_INLINE FMoney(float);
       FDECIMAL_INLINE FMoney(double);
       FDECIMAL_INLINE FMoney(long double);

       explicit FDECIMAL_INLINE FMoney(const char *);

 #ifdef FDECIMAL_WCHAR
       explicit FDECIMAL_INLINE FMoney(const wchar_t *);
 #endif

       explicit FDECIMAL_INLINE FMoney(const FDecimal &);

       // Conversions
       FDECIMAL_INLINE operator    BaseFMoney() const;
       FDECIMAL_INLINE operator    DBBaseFMoney() const;
       FDECIMAL_INLINE double      castDouble() const;
       FDECIMAL_INLINE long double castLDouble() const;

       // Operators
       FDECIMAL_INLINE FMoney &operator += (const FMoney &);
       FDECIMAL_INLINE FMoney &operator -= (const FMoney &);
       FDECIMAL_INLINE FMoney &operator *= (const FDecimal &);
       FDECIMAL_INLINE FMoney &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FMoney operator - () const;
       FDECIMAL_INLINE bool   operator ! () const;

       // Member functions
       FDECIMAL_INLINE FMoney   Abs() const;
       FDECIMAL_INLINE FMoney   Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char    *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char    *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;
 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

 #ifdef FDECIMAL_T
       FDECIMAL_INLINE FMoney(const FDecimal_t &);
       FDECIMAL_INLINE void get(FDecimal_t *) const;
       FDECIMAL_INLINE operator FDecimal_t() const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE FMoney &operator *= (int Operand);
       FDECIMAL_INLINE FMoney &operator *= (long Operand);
       FDECIMAL_INLINE FMoney &operator *= (int64_t Operand);
       FDECIMAL_INLINE FMoney &operator *= (short Operand);
       FDECIMAL_INLINE FMoney &operator *= (unsigned int Operand);
       FDECIMAL_INLINE FMoney &operator *= (unsigned long Operand);
       FDECIMAL_INLINE FMoney &operator *= (uint64_t Operand);
       FDECIMAL_INLINE FMoney &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE FMoney &operator *= (float Operand);
       FDECIMAL_INLINE FMoney &operator *= (double Operand);
       FDECIMAL_INLINE FMoney &operator *= (long double Operand);

     protected:
       // Операция умножения FMoney * FMoney не имеет реализации в рамках класса
       // FMoney, ее реализация возможна в прозводных классах
             FMoney operator *(const FMoney &) const;
             FMoney operator *(const DBBaseFMoney &) const;
       const FMoney &operator *= (const FMoney &);
       const FMoney &operator *= (const DBBaseFMoney &);

     protected:
       FDECIMAL_INLINE FMoney(const FMoney_Value &);

       FMoney_Value m_Value;

       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney FMoney_MinValue();
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney FMoney_MaxValue();
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney FMoney_ZeroValue();

       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator == (const FMoney &, const FMoney &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator != (const FMoney &, const FMoney &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator <= (const FMoney &, const FMoney &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator >= (const FMoney &, const FMoney &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator <  (const FMoney &, const FMoney &);
       friend FDECIMAL_INLINE FDECIMAL_DECLSPEC bool operator >  (const FMoney &, const FMoney &);
    };


/////////////////////////////////////////////////////////////////////////////
// BaseFMoney

class FDECIMAL_DECLSPEC BaseFMoney
    {
       friend class FMoney;
       friend class DBBaseFMoney;
       friend class DBFMoney;

     public:
       // Conversions
       FDECIMAL_INLINE operator    FMoney()       const;
       FDECIMAL_INLINE operator    DBBaseFMoney() const;
       FDECIMAL_INLINE double      castDouble()   const;
       FDECIMAL_INLINE long double castLDouble()  const;

       // Operators
       FDECIMAL_INLINE BaseFMoney &operator += (const FMoney &);
       FDECIMAL_INLINE BaseFMoney &operator -= (const FMoney &);
       FDECIMAL_INLINE BaseFMoney &operator *= (const FDecimal &);
       FDECIMAL_INLINE BaseFMoney &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FMoney operator - () const;
       FDECIMAL_INLINE bool   operator ! () const;

       // Member functions
       FDECIMAL_INLINE FMoney   Abs() const;
       FDECIMAL_INLINE FMoney   Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char    *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char    *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;

 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE BaseFMoney &operator *= (int Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (long Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (int64_t Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (short Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (unsigned int Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (unsigned long Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (uint64_t Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE BaseFMoney &operator *= (float Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (double Operand);
       FDECIMAL_INLINE BaseFMoney &operator *= (long double Operand);

     protected:
       BaseValue<FMoney_Value> m_Value;
    };


/////////////////////////////////////////////////////////////////////////////
// DBBaseFMoney

class FDECIMAL_DECLSPEC DBBaseFMoney
    {
       friend class FMoney;
       friend class BaseFMoney;

     public:
       // Conversions
       FDECIMAL_INLINE operator    FMoney()      const;
       FDECIMAL_INLINE operator    BaseFMoney()  const;
       FDECIMAL_INLINE double      castDouble()  const;
       FDECIMAL_INLINE long double castLDouble() const;

       // Operators
       FDECIMAL_INLINE DBBaseFMoney &operator += (const FMoney &);
       FDECIMAL_INLINE DBBaseFMoney &operator -= (const FMoney &);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (const FDecimal &);
       FDECIMAL_INLINE DBBaseFMoney &operator /= (const FDecimal &);

       // Unary operators
       FDECIMAL_INLINE FMoney operator - () const;
       FDECIMAL_INLINE bool   operator ! () const;

       // Member functions
       FDECIMAL_INLINE FMoney     Abs() const;
       FDECIMAL_INLINE FMoney     Round(int nPos = 0, int Mode = FDecimal_Round) const;

       FDECIMAL_INLINE char      *FormatRawStr(char *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE char      *FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const;

 #ifdef FDECIMAL_WCHAR
       FDECIMAL_INLINE wchar_t   *FormatRawStr(wchar_t *pRawStr, size_t StrLen) const;
       FDECIMAL_INLINE wchar_t   *FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const;
 #endif

       // Integer multiplication
       FDECIMAL_INLINE DBBaseFMoney &operator *= (int Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (long Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (int64_t Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (short Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (unsigned int Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (unsigned long Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (uint64_t Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (unsigned short Operand);

       // Floating point multiplication
       FDECIMAL_INLINE DBBaseFMoney &operator *= (float Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (double Operand);
       FDECIMAL_INLINE DBBaseFMoney &operator *= (long double Operand);

     protected:
       // Операция умножения FMoney * FMoney не имеет реализации в рамках класса
       // FMoney, ее реализация возможна в прозводных классах
       FMoney        operator *  (const FMoney &) const;
       FMoney        operator *  (const DBBaseFMoney &) const;
       const FMoney &operator *= (const FMoney &);
       const FMoney &operator *= (const DBBaseFMoney &);

     protected:
       DBBaseValue<FMoney_Value> m_DBValue;
    };


/////////////////////////////////////////////////////////////////////////////
// DBFMoney

class FDECIMAL_DECLSPEC DBFMoney : public DBBaseFMoney
    {
     public:
       // Constructors
       FDECIMAL_INLINE DBFMoney();
       FDECIMAL_INLINE DBFMoney(int);
       FDECIMAL_INLINE DBFMoney(long);
       FDECIMAL_INLINE DBFMoney(int64_t);
       FDECIMAL_INLINE DBFMoney(short);
       FDECIMAL_INLINE DBFMoney(unsigned int);
       FDECIMAL_INLINE DBFMoney(unsigned long);
       FDECIMAL_INLINE DBFMoney(uint64_t);
       FDECIMAL_INLINE DBFMoney(unsigned short);
       FDECIMAL_INLINE DBFMoney(float);
       FDECIMAL_INLINE DBFMoney(double);
       FDECIMAL_INLINE DBFMoney(long double);

       FDECIMAL_INLINE DBFMoney(const FMoney &);
       FDECIMAL_INLINE DBFMoney(const BaseFMoney &);
       FDECIMAL_INLINE DBFMoney(const DBBaseFMoney &);
    };

/////////////////////////////////////////////////////////////////////////////
// fabs, floor, ceil

#if !defined(__TOS_OS400__) || __OS400_TGTVRM__ > 370
    FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   fabs(const FDecimal &);
#endif

FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   floor (const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   ceil  (const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   fmod  (const FDecimal &, const FDecimal &);

FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   fabsl (const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   floorl(const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   ceill (const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   fmodl (const FDecimal &, const FDecimal &);

#if !defined(__TOS_OS400__) || __OS400_TGTVRM__ > 370
    FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     fabs(const FMoney &);
#endif

FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     floor (const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     ceil  (const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     fmod  (const FMoney &, const FMoney &);

FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     fabsl (const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     floorl(const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     ceill (const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     fmodl (const FMoney &, const FMoney &);

/////////////////////////////////////////////////////////////////////////////
// Min, max and zero values

FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   FDecimal_MinValue ();
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   FDecimal_MaxValue ();
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal   FDecimal_ZeroValue();

FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     FMoney_MinValue   ();
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     FMoney_MaxValue   ();
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney     FMoney_ZeroValue  ();

/////////////////////////////////////////////////////////////////////////////
// Global operators

//
// Operators: == != <= >= < >
// FDecimal <-> FDecimal
//
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator == (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator != (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator <= (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator >= (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator <  (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator >  (const FDecimal &, const FDecimal &);

//
// Operators: == != <= >= < >
// FMoney <-> FMoney
//
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator == (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator != (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator <= (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator >= (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator <  (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC bool     operator >  (const FMoney &, const FMoney &);

//
// Operators: + - * /
// FDecimal <-> FDecimal
//
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal operator + (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal operator - (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal operator * (const FDecimal &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal operator / (const FDecimal &, const FDecimal &);

//
// Operators: + -
// FMoney <-> FMoney
//
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney   operator + (const FMoney &, const FMoney &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney   operator - (const FMoney &, const FMoney &);

//
// Operators: * /
// FMoney <-> FDecimal, FDecimal <-> FMoney
//
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney   operator * (const FMoney   &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney   operator * (const FDecimal &, const FMoney &);

FDECIMAL_INLINE FDECIMAL_DECLSPEC FMoney   operator / (const FMoney &, const FDecimal &);
FDECIMAL_INLINE FDECIMAL_DECLSPEC FDecimal operator / (const FMoney &, const FMoney &);

//
// Operator: *
// FDecimal <-> internal types
//
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, int)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, long)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, int64_t)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, short)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, unsigned int)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, unsigned long)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, uint64_t)
FDECIMAL_INTEGER_BINARY(*, const FDecimal &, unsigned short)
FDECIMAL_FLOAT_FIXED   (*, const FDecimal &, float,         FDecimal::Factor())
FDECIMAL_FLOAT_FIXED   (*, const FDecimal &, double,        FDecimal::Factor())
FDECIMAL_FLOAT_FIXED   (*, const FDecimal &, long double,   FDecimal::Factor())

//
// Operator: /
// FDecimal <-> internal types
//
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, int)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, long)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, int64_t)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, short)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, unsigned int)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, unsigned long)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, uint64_t)
FDECIMAL_INTEGER_BINARY(/, const FDecimal &, unsigned short)
FDECIMAL_FLOAT_BINARY  (/, const FDecimal &, float)
FDECIMAL_FLOAT_BINARY  (/, const FDecimal &, double)
FDECIMAL_FLOAT_BINARY  (/, const FDecimal &, long double)

//
// Operator: *
// BaseFDecimal <-> internal types
//
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, int)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, long)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, int64_t)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, short)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, unsigned int)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, unsigned long)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, uint64_t)
FDECIMAL_INTEGER_BINARY(*, const BaseFDecimal &, unsigned short)
FDECIMAL_FLOAT_FIXED   (*, const BaseFDecimal &, float,        BaseFDecimal::Factor())
FDECIMAL_FLOAT_FIXED   (*, const BaseFDecimal &, double,       BaseFDecimal::Factor())
FDECIMAL_FLOAT_FIXED   (*, const BaseFDecimal &, long double,  BaseFDecimal::Factor())

//
// Operator: /
// BaseFDecimal <-> internal types
//
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, int)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, long)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, int64_t)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, short)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, unsigned int)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, unsigned long)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, uint64_t)
FDECIMAL_INTEGER_BINARY(/, const BaseFDecimal &, unsigned short)
FDECIMAL_FLOAT_BINARY  (/, const BaseFDecimal &, float)
FDECIMAL_FLOAT_BINARY  (/, const BaseFDecimal &, double)
FDECIMAL_FLOAT_BINARY  (/, const BaseFDecimal &, long double)

//
// Operator: *
// DBBaseFDecimal <-> internal types
//
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, int)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, long)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, int64_t)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, short)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, unsigned int)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, unsigned long)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, uint64_t)
DBFDECIMAL_INTEGER_BINARY(*, const DBBaseFDecimal &, unsigned short)
DBFDECIMAL_FLOAT_FIXED   (*, const DBBaseFDecimal &, float,          DBBaseFDecimal::Factor())
DBFDECIMAL_FLOAT_FIXED   (*, const DBBaseFDecimal &, double,         DBBaseFDecimal::Factor())
DBFDECIMAL_FLOAT_FIXED   (*, const DBBaseFDecimal &, long double,    DBBaseFDecimal::Factor())

//
// Operator: /
// DBBaseFDecimal <-> internal types
//
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, int)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, long)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, int64_t)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, short)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, unsigned int)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, unsigned long)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, uint64_t)
DBFDECIMAL_INTEGER_BINARY(/, const DBBaseFDecimal &, unsigned short)
DBFDECIMAL_FLOAT_BINARY  (/, const DBBaseFDecimal &, float)
DBFDECIMAL_FLOAT_BINARY  (/, const DBBaseFDecimal &, double)
DBFDECIMAL_FLOAT_BINARY  (/, const DBBaseFDecimal &, long double)

//
// Operator: *
// FMoney <-> internal types
//
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, int,            FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, long,           FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, int64_t,        FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, short,          FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_1ST_DEC(*, const FMoney &, unsigned short, FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, int,            FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, long,           FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, int64_t,        FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, short,          FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_2ND_DEC(*, const FMoney &, unsigned short, FMoney)
FMONEY_FLOAT_BINARY   (*, const FMoney &, float)
FMONEY_FLOAT_BINARY   (*, const FMoney &, double)
FMONEY_FLOAT_BINARY   (*, const FMoney &, long double)

//
// Operator: /
// FMoney <-> internal types
//
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, int,            FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, long,           FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, int64_t,        FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, short,          FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_2ND_DEC(/, const FMoney &, unsigned short, FMoney)
FMONEY_FLOAT_BINARY   (/, const FMoney &, float)
FMONEY_FLOAT_BINARY   (/, const FMoney &, double)
FMONEY_FLOAT_BINARY   (/, const FMoney &, long double)

//
// Operator: *
// BaseFMoney <-> internal types
//
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, int,            FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, long,           FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, int64_t,        FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, short,          FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_1ST_DEC(*, const BaseFMoney &, unsigned short, FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, int,            FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, long,           FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, int64_t,        FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, short,          FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_2ND_DEC(*, const BaseFMoney &, unsigned short, FMoney)
FMONEY_FLOAT_BINARY   (*, const BaseFMoney &, float)
FMONEY_FLOAT_BINARY   (*, const BaseFMoney &, double)
FMONEY_FLOAT_BINARY   (*, const BaseFMoney &, long double)

//
// Operator: /
// BaseFMoney <-> internal types
//
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, int,            FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, long,           FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, int64_t,        FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, short,          FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, unsigned int,   FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, unsigned long,  FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, uint64_t,       FMoney)
FMONEY_INTEGER_2ND_DEC(/, const BaseFMoney &, unsigned short, FMoney)
FMONEY_FLOAT_BINARY   (/, const BaseFMoney &, float)
FMONEY_FLOAT_BINARY   (/, const BaseFMoney &, double)
FMONEY_FLOAT_BINARY   (/, const BaseFMoney &, long double)

//
// Operator: *
// DBBaseFMoney <-> internal types
//
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, int,            FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, long,           FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, int64_t,        FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, short,          FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, unsigned int,   FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, unsigned long,  FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, uint64_t,       FMoney)
DBFMONEY_INTEGER_1ST_DEC(*, const DBBaseFMoney &, unsigned short, FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, int,            FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, long,           FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, int64_t,        FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, short,          FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, unsigned int,   FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, unsigned long,  FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, uint64_t,       FMoney)
DBFMONEY_INTEGER_2ND_DEC(*, const DBBaseFMoney &, unsigned short, FMoney)
DBFMONEY_FLOAT_BINARY   (*, const DBBaseFMoney &, float)
DBFMONEY_FLOAT_BINARY   (*, const DBBaseFMoney &, double)
DBFMONEY_FLOAT_BINARY   (*, const DBBaseFMoney &, long double)

//
// Operator: /
// DBBaseFMoney <-> internal types
//
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, int,            FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, long,           FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, int64_t,        FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, short,          FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, unsigned int,   FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, unsigned long,  FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, uint64_t,       FMoney)
DBFMONEY_INTEGER_2ND_DEC(/, const DBBaseFMoney &, unsigned short, FMoney)
DBFMONEY_FLOAT_BINARY   (/, const DBBaseFMoney &, float)
DBFMONEY_FLOAT_BINARY   (/, const DBBaseFMoney &, double)
DBFMONEY_FLOAT_BINARY   (/, const DBBaseFMoney &, long double)


/////////////////////////////////////////////////////////////////////////////
// Inline implementation

#ifndef FDECIMAL_NO_INLINE
    #include "fdec_inl.h"
#endif

#endif //__cplusplus

#endif // __FDECIMAL_H

/* <-- EOF --> */