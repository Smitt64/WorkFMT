/*******************************************************************************
 FILE         :   FIXED_I64.H

 COPYRIGHT    :   R-Style Software Lab, 1998, 2000

 DESCRIPTION  :    лассы чисел с фиксированной точкой, реализованные
                  на базе int64: FixedP_i64, FixedT_i64.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   01 Dec 2000

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/fixed_i64.h $
                  $Revision: 8 $
                  $Date: 12.02.01 20:32 $
                  $Author: Lebedev $
                  $Modtime: 12.02.01 17:12 $
*******************************************************************************/

#ifndef __FIXED_I64_H
#define __FIXED_I64_H

#include "fdec_std.h"
#include "int64.h"
#include "tooldef.h"

// -----------------------------------------------------------------------------
class FixedParm
{
 public:
   long Multiplier()
   {
    return _mul;
   }

   short Maxbits()
   {
    return _bits;
   }

   // --------------------------------------------
   // --------------------------------------------
   long   _mul;
   short  _bits;
};

// -----------------------------------------------------------------------------
class _TOOLEXP FixedP_i64
    {
     public:
       FixedP_i64();
       FixedP_i64(FixedParm, int);
       FixedP_i64(FixedParm, long);
       FixedP_i64(FixedParm, int64_t);
       FixedP_i64(FixedParm, short);
       FixedP_i64(FixedParm, unsigned int);
       FixedP_i64(FixedParm, unsigned long);
       FixedP_i64(FixedParm, uint64_t);
       FixedP_i64(FixedParm, unsigned short);
       FixedP_i64(FixedParm, float);
       FixedP_i64(FixedParm, double);
       FixedP_i64(FixedParm, long double);

       explicit FixedP_i64(int Point, FixedParm, const char *);

 #ifdef FDECIMAL_WCHAR
       explicit FixedP_i64(int Point, FixedParm, const wchar_t *);
 #endif

       double      ToDouble (FixedParm) const;
       long double ToLDouble(FixedParm) const;

       void Add(const FixedP_i64 &);
       void Sub(const FixedP_i64 &);
       void Mul(FixedParm, const FixedP_i64 &);
       void Div(FixedParm, const FixedP_i64 &);

       FixedP_i64 operator - () const;

       bool IsZero() const;
       bool Equal(const FixedP_i64 &Operand) const;
       bool LessThan(const FixedP_i64 &Operand) const;

       FixedP_i64 Abs() const;
       FixedP_i64 Round(int Point, int nPos = 0, int Mode = FDecimal_Round) const;

       char    *FormatRawStr(int Point, int Precision, char *, size_t) const;
 #ifdef FDECIMAL_WCHAR
       wchar_t *FormatRawStr(int Point, int Precision, wchar_t *, size_t) const;
 #endif

       static const FixedP_i64 *MaxValue();
       static const FixedP_i64 *MinValue();
       static const FixedP_i64 *ZeroValue();

       // Old version
       double      cvt2dbl (FixedParm) const;
       long double cvt2ldbl(FixedParm) const;
       char       *cvt2str (int Point, char *, size_t) const;
 #ifdef FDECIMAL_WCHAR
       wchar_t    *cvt2str (int Point, wchar_t *, size_t) const;
 #endif
       void add(const FixedP_i64 &);
       void sub(const FixedP_i64 &);
       void mul(FixedParm, const FixedP_i64 &);
       void div(FixedParm, const FixedP_i64 &);

     protected:
       FixedP_i64(const int64 &);

     private:
       int64  m_i64;
    };

// -----------------------------------------------------------------------------
template <int Point>
class FixedT_i64
   {
    public:
      inline FixedT_i64();
      inline FixedT_i64(int);
      inline FixedT_i64(long);
      inline FixedT_i64(int64_t);
      inline FixedT_i64(short);
      inline FixedT_i64(unsigned int);
      inline FixedT_i64(unsigned long);
      inline FixedT_i64(uint64_t);
      inline FixedT_i64(unsigned short);
      inline FixedT_i64(float);
      inline FixedT_i64(double);
      inline FixedT_i64(long double);

      explicit inline FixedT_i64(const char *);

#ifdef FDECIMAL_WCHAR
      explicit inline FixedT_i64(const wchar_t *);
#endif

      inline operator double () const;
      inline operator long double () const;

      inline const FixedT_i64 &operator += (const FixedT_i64 &);
      inline const FixedT_i64 &operator -= (const FixedT_i64 &);
      inline const FixedT_i64 &operator *= (const FixedT_i64 &);
      inline const FixedT_i64 &operator /= (const FixedT_i64 &);

      inline bool operator == (const FixedT_i64 &) const;
      inline bool operator != (const FixedT_i64 &) const;
      inline bool operator <= (const FixedT_i64 &) const;
      inline bool operator >= (const FixedT_i64 &) const;
      inline bool operator <  (const FixedT_i64 &) const;
      inline bool operator >  (const FixedT_i64 &) const;

      inline FixedT_i64 operator - () const;
      inline bool operator ! () const;

      inline FixedT_i64 Abs() const;
      inline FixedT_i64 Round(int nPos = 0, int Mode = FDecimal_Round) const;

      inline char *FormatRawStr(char *, size_t) const;
      inline char *FormatRawStr(int Precision, char *, size_t) const;
#ifdef FDECIMAL_WCHAR
      inline wchar_t *FormatRawStr(wchar_t *, size_t) const;
      inline wchar_t *FormatRawStr(int Precision, wchar_t *, size_t) const;
#endif

      inline static FixedT_i64 MaxValue();
      inline static FixedT_i64 MinValue();
      inline static FixedT_i64 ZeroValue();

    protected:
      inline FixedT_i64(const FixedP_i64 &);

    private:
      FixedP_i64  m_Value;
      static FixedParm _TOOLDATA  m_Parm;
};

template <int Point>
inline FixedT_i64<Point>::FixedT_i64() : m_Value()
{
}

template <int Point>
inline FixedT_i64<Point>::FixedT_i64(int Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(long Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(int64_t Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(short Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(unsigned int Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(unsigned long Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(uint64_t Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(unsigned short Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(float Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(double Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(long double Src) : m_Value(m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
template <int Point>
inline FixedT_i64<Point>::FixedT_i64(const char *Src) : m_Value(Point, m_Parm, Src)
{
}

// -----------------------------------------------------------------------------
#ifdef FDECIMAL_WCHAR
// -----------------------------------------------------------------------------
    template <int Point>
    inline FixedT_i64<Point>::FixedT_i64(const wchar_t *Src) : m_Value(Point, m_Parm, Src)
    {
    }
#endif

template <int Point>
inline FixedT_i64<Point>::FixedT_i64(const FixedP_i64 &Src) : m_Value(Src)
{
}

template <int Point>
inline FixedT_i64<Point>::operator double () const
{
 return m_Value.ToDouble(m_Parm);
}


template <int Point>
inline FixedT_i64<Point>::operator long double () const
{
 return m_Value.ToLDouble(m_Parm);
}

template <int Point>
inline const FixedT_i64<Point> &FixedT_i64<Point>::operator += (const FixedT_i64 &Operand)
{
 m_Value.Add(Operand.m_Value);
 return *this;
}

template <int Point>
inline const FixedT_i64<Point> &FixedT_i64<Point>::operator -= (const FixedT_i64 &Operand)
{
 m_Value.Sub(Operand.m_Value);
 return *this;
}

template <int Point>
inline const FixedT_i64<Point> &FixedT_i64<Point>::operator *= (const FixedT_i64 &Operand)
{
 m_Value.Mul(m_Parm, Operand.m_Value);
 return *this;
}

template <int Point>
inline const FixedT_i64<Point> &FixedT_i64<Point>::operator /= (const FixedT_i64 &Operand)
{
 m_Value.Div(m_Parm, Operand.m_Value);
 return *this;
}

template <int Point>
inline bool FixedT_i64<Point>::operator == (const FixedT_i64<Point> &Operand) const
{
 return m_Value.Equal(Operand.m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator != (const FixedT_i64<Point> &Operand) const
{
 return !m_Value.Equal(Operand.m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator < (const FixedT_i64<Point> &Operand) const
{
 return m_Value.LessThan(Operand.m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator >= (const FixedT_i64<Point> &Operand) const
{
 return !m_Value.LessThan(Operand.m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator > (const FixedT_i64<Point> &Operand) const
{
 return Operand.m_Value.LessThan(m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator <= (const FixedT_i64<Point> &Operand) const
{
 return !Operand.m_Value.LessThan(m_Value);
}

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::operator - () const
{
 return FixedT_i64<Point>(-m_Value);
}

template <int Point>
inline bool FixedT_i64<Point>::operator ! () const
{
 return m_Value.IsZero();
}

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::Abs() const
{
 return FixedT_i64<Point>(m_Value.Abs());
}

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::Round(int nPos, int Mode) const
{
 return FixedT_i64<Point>(m_Value.Round(Point, nPos, Mode));
}

template <int Point>
inline char *FixedT_i64<Point>::FormatRawStr(char *pRawStr, size_t StrLen) const
{
 return m_Value.FormatRawStr(Point, Point, pRawStr, StrLen);
}

template <int Point>
inline char *FixedT_i64<Point>::FormatRawStr(int Precision, char *pRawStr, size_t StrLen) const
{
 return m_Value.FormatRawStr(Point, Precision, pRawStr, StrLen);
}

#ifdef FDECIMAL_WCHAR
    template <int Point>
    inline wchar_t *FixedT_i64<Point>::FormatRawStr(wchar_t *pRawStr, size_t StrLen) const
    {
     return m_Value.FormatRawStr(Point, Point, pRawStr, StrLen);
    }

    template <int Point>
    inline wchar_t *FixedT_i64<Point>::FormatRawStr(int Precision, wchar_t *pRawStr, size_t StrLen) const
    {
     return m_Value.FormatRawStr(Point, Precision, pRawStr, StrLen);
    }
#endif

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::MaxValue()
{
 return *(FixedT_i64<Point> *)FixedP_i64::MaxValue();
}

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::MinValue()
{
 return *(FixedT_i64<Point> *)FixedP_i64::MinValue();
}

template <int Point>
inline FixedT_i64<Point> FixedT_i64<Point>::ZeroValue()
{
 return *(FixedT_i64<Point> *)FixedP_i64::ZeroValue();
}

#endif // __FIXED_I64_H

/* <-- EOF --> */