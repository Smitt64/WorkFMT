/*-----------------------------------------------------------------------*-
 File Name      : int64.h
 Date           : 2000
 Programmer     : A.Dobroslavskiy

 $Revision: 8 $

 Subsystem      : 
 Description    : Классы работы с целым 64-битным числом

 Classes        : int64
                  int64_cast
              int64_exception

 Implementation : int64.cpp

 Copyright (c) 2000 R-Style Software Lab.
 All Rights Reserved.
-*-----------------------------------------------------------------------*-
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/FDecimal.h $
//                  $Revision: 26 $
//                  $Date: 19.09.00 16:48 $
//                  $Author: Lebedev $
//                  $Modtime: 19.09.00 16:21 $
-*-----------------------------------------------------------------------*/

#ifndef __INT64_H
#define __INT64_H

#include "platform.h"
#include "tooldef.h"


// -----------------------------------------------------------------------------
// int64 уже занят :-(
typedef __int64           int64_t;
typedef unsigned __int64  uint64_t;


#ifdef __cplusplus

// -----------------------------------------------------------------------------
#if defined(_MSC_VER)
    #define INT64_WCHAR

    #include <wchar.h>
#endif

// -----------------------------------------------------------------------------
// Исключения класса int64
class int64_exception
    {
   // --------------------------------------------
   // --------------------------------------------
     short  ex;

     public:
   // --------------------------------------------
       enum
          {
           exOverflow  = 1,
           exDivByZero = 2
          };

     public:
       // ----------------------------------------
       int64_exception(short e)
       {
        ex = e;
       }

       // ----------------------------------------
       short GetEx()
       {
        return ex;
       }
    };

// -----------------------------------------------------------------------------
// Максимальное и минимальное числа класса. 
#define  MAX_INT64  (int64("9223372036854775807"))
#define  MIN_INT64  (int64("-9223372036854775808"))

// -----------------------------------------------------------------------------
// Класс 64-битного целого знакового числа.
class _TOOLEXP int64
{
   friend class int64_cast;

   // --------------------------------------------
   // Хранилище 64-битного числа

#ifdef RSL_PRC_LITTLE_ENDIAN
   unsigned long  m_Lo;
   unsigned long  m_Hi;
#endif

#ifdef RSL_PRC_BIG_ENDIAN
   unsigned long  m_Hi;
   unsigned long  m_Lo;
#endif

 public:
   // --------------------------------------------
   // Функция взятия знака. 
   // Возвращает 1, если число положительное, -1 - если отрицательное
   long GetSign() const;

   // Возвращает абсолютное значение
   int64 Abs() const;

   //////////////////
   // Конструкторы //
   //////////////////////////////////////////////////////////////////////////////

   int64()
   {
    m_Lo = m_Hi = 0UL;
   }

   int64(const char *);
#ifdef INT64_WCHAR
   int64(const wchar_t *);
#endif
   int64(long, long);
   int64(signed char);
   int64(unsigned char);
   int64(short);
   int64(unsigned short);
   int64(int);
   int64(unsigned int);
   int64(long);
   int64(unsigned long);
   int64(int64_t);
   int64(uint64_t);
   int64(float);
   int64(double);
   int64(long double);

#if defined(_MSC_VER) || defined(__IBMCPP__)
   void fromLongDouble(void *d);
#endif
   /////////////////////////////////////////
   // Функции интерфейса с типом 'строка' //
   //////////////////////////////////////////////////////////////////////////////

   // Функция получения строки.
   // Буфер для строки должен быть создан вызывающей функцией
   char *GetString(char *, short) const;
#ifdef INT64_WCHAR
   wchar_t *GetString(wchar_t *, short) const;
#endif
   // Функция получения длины строки для числа
   short GetStrLen() const;
   // Функция инициализации класса строкой
   void InitFromString(const char *);
#ifdef INT64_WCHAR
   void InitFromString(const wchar_t *);
#endif

   //////////////////////
   // Операторы сдвига //
   //////////////////////////////////////////////////////////////////////////////

   // Максимальное значение для сдвига: +/- 64 бита.
   // Большее значение смысла не имеет
   int64        operator >>  (long) const;
   int64        operator <<  (long) const;
   const int64 &operator >>= (long);
   const int64 &operator <<= (long);

   ///////////////////////
   // Унарные операторы //
   //////////////////////////////////////////////////////////////////////////////

   // унарный минус - смена знака
   int64 operator - () const;
   // двоичное дополнение - инверсия
   int64 operator ~ () const;

   ////////////////////////
   // Операторы сложения //
   //////////////////////////////////////////////////////////////////////////////

   // простое сложение
   int64        operator +  (const int64 &) const;
   // сложение с присваиванием
   const int64 &operator += (const int64 &);
   // префиксное добавление единицы
   int64        operator ++ ();
   // постфиксное добавление единицы
   int64        operator ++ (int);

   /////////////////////////
   // Операторы вычитания //
   //////////////////////////////////////////////////////////////////////////////

   // простое вычитание
   int64        operator -  (const int64 &) const;
   // вычитание с присваиванием
   const int64 &operator -= (const int64 &);
   // префиксное вычитание единицы
   int64        operator -- ();
   // постфиксное вычитание единицы
   int64        operator -- (int);

   /////////////////////////
   // Операторы умножения //
   //////////////////////////////////////////////////////////////////////////////

   // простое умножение
   int64        operator * (const int64 &) const;
   // умножение с присваиванием
   const int64 &operator *= (const int64 &);

   ///////////////////////
   // Операторы деления //
   //////////////////////////////////////////////////////////////////////////////

   // простое деление
   int64        operator / (const int64 &) const;
   // деление с присваиванием
   const int64 &operator /= (const int64 &);

   //////////////////////////////////////
   // Операторы взятия остатка деления //
   //////////////////////////////////////////////////////////////////////////////

   // взятие остатка деления
   int64        operator % (const int64 &) const;
   // взятие остатка деления с присваиванием
   const int64 &operator %= (const int64 &);

   /////////////////////////
   // Операторы сравнения //
   //////////////////////////////////////////////////////////////////////////////

   bool operator <  (const int64 &) const;
   bool operator <= (const int64 &) const;
   bool operator >  (const int64 &) const;
   bool operator >= (const int64 &) const;
   bool operator != (const int64 &) const;
   bool operator == (const int64 &) const;

   ///////////////////////
   // Битовые операторы //
   //////////////////////////////////////////////////////////////////////////////

   // Побитовое 'и'
   int64        operator & (const int64 &) const;
   // Побитовое 'и' с присваиванием
   const int64 &operator &= (const int64 &);
   // Побитовое 'или'
   int64        operator | (const int64 &) const;
   // Побитовое 'или' с присваиванием
   const int64 &operator |= (const int64 &);
   // Побитовое исключающее 'или'
   int64        operator ^ (const int64 &) const;
   // Побитовое исключающее 'или' с присваиванием
   const int64 &operator ^= (const int64 &);

   //////////////////////////
   // Логические операторы //
   //////////////////////////////////////////////////////////////////////////////

   // отрицание. Возвращает 'true', если объект равен 0, 
   // 'false' - в противном случае.
   bool operator ! () const;
   // Логическое 'и'
   bool operator && (const int64 &) const;
   // Логическое 'или'
   bool operator || (const int64 &) const;

   // Взять значение старшего и младшего байта 
   const unsigned long HiDWORD() const
   {
    return m_Hi;
   }

   const unsigned long LoDWORD() const
   {
    return m_Lo;
   }
};

// -----------------------------------------------------------------------------
// Реализация класса для кастирования int64 к простым типам
class int64_cast
{
   // --------------------------------------------
   // --------------------------------------------
  unsigned long  m_Lo;
  unsigned long  m_Hi;

 public:
   // --------------------------------------------
   int64_cast(const int64 &i64)
   {
    m_Hi = i64.m_Hi;
    m_Lo = i64.m_Lo;
   }

   // --------------------------------------------
   operator bool() const
   {
    return m_Lo != 0 || m_Hi != 0;
   }

   // --------------------------------------------
   operator char() const
   {
    return (char)m_Lo;
   }

   // --------------------------------------------
   operator signed char() const
   {
    return (signed char)m_Lo;
   }

   // --------------------------------------------
   operator unsigned char() const
   {
    return (unsigned char)m_Lo;
   }

   // --------------------------------------------
   operator short() const
   {
    return (short)m_Lo;
   }

   // --------------------------------------------
   operator unsigned short() const
   {
    return (unsigned short)m_Lo;
   }

   // --------------------------------------------
   operator int() const
   {
    return (int)m_Lo;
   }

   // --------------------------------------------
   operator unsigned int() const
   {
    return (unsigned int)m_Lo;
   }

   // --------------------------------------------
   operator long() const
   {
    return (long)m_Lo;
   }

   // --------------------------------------------
   operator int64_t() const
   {
    int64_t  v = 0i64;


    v  = (int64_t)m_Hi << 32;
    v |= (m_Lo % 0xffffffff);

    return v;
   }

   // --------------------------------------------
   operator unsigned long() const
   {
    return (unsigned long)m_Lo;
   }

   // --------------------------------------------
   operator uint64_t() const
   {
    uint64_t  v = 0i64;


    v  = (uint64_t)m_Hi << 32;
    v |= (m_Lo % 0xffffffff);

    return v;
   }

   // --------------------------------------------
   operator float() const
   {
    return (float)operator double();
   }

   // --------------------------------------------
   operator double() const
   {
    int64_cast  temp     = *this;
    double      max_long = 0xFFFFFFFFL + 1.0L;


    if(m_Hi & 0x80000000L)
     {
      temp.m_Hi = ~m_Hi;
      temp.m_Lo = ~m_Lo;

      if(temp.m_Lo == 0xFFFFFFFFL)
        temp.m_Hi++;

      temp.m_Lo++;

      return -(temp.m_Hi*max_long + temp.m_Lo);
     }
    else
      return temp.m_Hi*max_long + temp.m_Lo;
   }

   // --------------------------------------------
   operator long double() const
   {
    int64_cast   temp     = *this;
    long double  max_long = 0xFFFFFFFFL + 1.0L;


    if(m_Hi & 0x80000000L)
     {
      temp.m_Hi = ~m_Hi;
      temp.m_Lo = ~m_Lo;

      if(temp.m_Lo == 0xFFFFFFFFL)
        temp.m_Hi++;

      temp.m_Lo++;

      return -(temp.m_Hi*max_long + temp.m_Lo);
     }
    else
      return temp.m_Hi*max_long + temp.m_Lo;
   }
};

#endif 

#ifdef __cplusplus
extern "C" {
#endif

void MakeInt64FromLDouble(long double val, char *out);
void MakeInt64FromDouble(double val, char *out);
void MakeDoubleFromInt64(double *out, const char *in);


#ifdef __cplusplus
}
#endif

#endif // __INT64_H

/* <-- EOF --> */