/*-----------------------------------------------------------------------*-
 File Name      : int64.h
 Date           : 2000
 Programmer     : A.Dobroslavskiy

 $Revision: 8 $

 Subsystem      : 
 Description    : ������ ������ � ����� 64-������ ������

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
// int64 ��� ����� :-(
typedef __int64           int64_t;
typedef unsigned __int64  uint64_t;


#ifdef __cplusplus

// -----------------------------------------------------------------------------
#if defined(_MSC_VER)
    #define INT64_WCHAR

    #include <wchar.h>
#endif

// -----------------------------------------------------------------------------
// ���������� ������ int64
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
// ������������ � ����������� ����� ������. 
#define  MAX_INT64  (int64("9223372036854775807"))
#define  MIN_INT64  (int64("-9223372036854775808"))

// -----------------------------------------------------------------------------
// ����� 64-������� ������ ��������� �����.
class _TOOLEXP int64
{
   friend class int64_cast;

   // --------------------------------------------
   // ��������� 64-������� �����

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
   // ������� ������ �����. 
   // ���������� 1, ���� ����� �������������, -1 - ���� �������������
   long GetSign() const;

   // ���������� ���������� ��������
   int64 Abs() const;

   //////////////////
   // ������������ //
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
   // ������� ���������� � ����� '������' //
   //////////////////////////////////////////////////////////////////////////////

   // ������� ��������� ������.
   // ����� ��� ������ ������ ���� ������ ���������� ��������
   char *GetString(char *, short) const;
#ifdef INT64_WCHAR
   wchar_t *GetString(wchar_t *, short) const;
#endif
   // ������� ��������� ����� ������ ��� �����
   short GetStrLen() const;
   // ������� ������������� ������ �������
   void InitFromString(const char *);
#ifdef INT64_WCHAR
   void InitFromString(const wchar_t *);
#endif

   //////////////////////
   // ��������� ������ //
   //////////////////////////////////////////////////////////////////////////////

   // ������������ �������� ��� ������: +/- 64 ����.
   // ������� �������� ������ �� �����
   int64        operator >>  (long) const;
   int64        operator <<  (long) const;
   const int64 &operator >>= (long);
   const int64 &operator <<= (long);

   ///////////////////////
   // ������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������� ����� - ����� �����
   int64 operator - () const;
   // �������� ���������� - ��������
   int64 operator ~ () const;

   ////////////////////////
   // ��������� �������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������� ��������
   int64        operator +  (const int64 &) const;
   // �������� � �������������
   const int64 &operator += (const int64 &);
   // ���������� ���������� �������
   int64        operator ++ ();
   // ����������� ���������� �������
   int64        operator ++ (int);

   /////////////////////////
   // ��������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������� ���������
   int64        operator -  (const int64 &) const;
   // ��������� � �������������
   const int64 &operator -= (const int64 &);
   // ���������� ��������� �������
   int64        operator -- ();
   // ����������� ��������� �������
   int64        operator -- (int);

   /////////////////////////
   // ��������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������� ���������
   int64        operator * (const int64 &) const;
   // ��������� � �������������
   const int64 &operator *= (const int64 &);

   ///////////////////////
   // ��������� ������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������� �������
   int64        operator / (const int64 &) const;
   // ������� � �������������
   const int64 &operator /= (const int64 &);

   //////////////////////////////////////
   // ��������� ������ ������� ������� //
   //////////////////////////////////////////////////////////////////////////////

   // ������ ������� �������
   int64        operator % (const int64 &) const;
   // ������ ������� ������� � �������������
   const int64 &operator %= (const int64 &);

   /////////////////////////
   // ��������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   bool operator <  (const int64 &) const;
   bool operator <= (const int64 &) const;
   bool operator >  (const int64 &) const;
   bool operator >= (const int64 &) const;
   bool operator != (const int64 &) const;
   bool operator == (const int64 &) const;

   ///////////////////////
   // ������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   // ��������� '�'
   int64        operator & (const int64 &) const;
   // ��������� '�' � �������������
   const int64 &operator &= (const int64 &);
   // ��������� '���'
   int64        operator | (const int64 &) const;
   // ��������� '���' � �������������
   const int64 &operator |= (const int64 &);
   // ��������� ����������� '���'
   int64        operator ^ (const int64 &) const;
   // ��������� ����������� '���' � �������������
   const int64 &operator ^= (const int64 &);

   //////////////////////////
   // ���������� ��������� //
   //////////////////////////////////////////////////////////////////////////////

   // ���������. ���������� 'true', ���� ������ ����� 0, 
   // 'false' - � ��������� ������.
   bool operator ! () const;
   // ���������� '�'
   bool operator && (const int64 &) const;
   // ���������� '���'
   bool operator || (const int64 &) const;

   // ����� �������� �������� � �������� ����� 
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
// ���������� ������ ��� ������������ int64 � ������� �����
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