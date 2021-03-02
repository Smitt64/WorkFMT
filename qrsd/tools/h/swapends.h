/*******************************************************************************
 FILE         :   SWAPENDS.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Little-to-Big-endian and vice versa conversions

 PROGRAMMED BY:   Yakov E. Markovitch
                  Alexey Lebedev

 CREATION DATE:   10 Sep 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/swapends.h $
                  $Revision: 2 $
                  $Date: 16.02.00 11:45 $
                  $Author: Lebedev $
                  $Modtime: 16.02.00 11:44 $
*******************************************************************************/

#ifndef __SWAPENDS_H
#define __SWAPENDS_H

#include "packpsh1.h"

// -----------------------------------------------------------------------------
typedef union
      {
       unsigned short  val;

       struct
            {
             unsigned char  a;
             unsigned char  b;
            } half;
      } R2byte;

// -----------------------------------------------------------------------------
typedef union
      {
       unsigned long  val;

       struct
            {
             unsigned char  a;
             unsigned char  b;
             unsigned char  c;
             unsigned char  d;
            } quarter;
      } R4byte;

// -----------------------------------------------------------------------------
#define swapShort(val)                                                    \
        {                                                                 \
         unsigned char tmp          = ((R2byte *)&(val))->half.a;         \
         ((R2byte *)&(val))->half.a = ((R2byte *)&(val))->half.b;         \
         ((R2byte *)&(val))->half.b = tmp;                                \
        }

#define swapLong(val)                                                     \
        {                                                                 \
           unsigned char             tmp = ((R4byte *)&(val))->quarter.a; \
           ((R4byte *)&(val))->quarter.a = ((R4byte *)&(val))->quarter.d; \
           ((R4byte *)&(val))->quarter.d = tmp;                           \
                                     tmp = ((R4byte *)&(val))->quarter.b; \
           ((R4byte *)&(val))->quarter.b = ((R4byte *)&(val))->quarter.c; \
           ((R4byte *)&(val))->quarter.c = tmp;                           \
        }

#define swapShort2(dest, src)                                             \
        {                                                                 \
         ((R2byte *)&(dest))->half.a = ((R2byte *)&(src))->half.b;        \
         ((R2byte *)&(dest))->half.b = ((R2byte *)&(src))->half.a;        \
        }

#define swapLong2(dest, src)                                              \
        {                                                                 \
         ((R4byte *)&(dest))->quarter.a = ((R4byte *)&(src))->quarter.d;  \
         ((R4byte *)&(dest))->quarter.b = ((R4byte *)&(src))->quarter.c;  \
         ((R4byte *)&(dest))->quarter.c = ((R4byte *)&(src))->quarter.b;  \
         ((R4byte *)&(dest))->quarter.d = ((R4byte *)&(src))->quarter.a;  \
        }

#ifdef __cplusplus

// -----------------------------------------------------------------------------
class _p_endiannes_cvt_
    {
     friend void swapEnds(unsigned short &val);
     friend void swapEnds(unsigned long &val);

  #ifdef _LONG_LONG
     friend void swapEnds(unsigned long long &val);
  #endif

     friend void swapEnds(unsigned short &dest, const unsigned short &src);
     friend void swapEnds(unsigned long &dest, const unsigned long &src);

  #ifdef _LONG_LONG
     friend void swapEnds(unsigned long long &dest, const unsigned long long &src);
  #endif

     union R2byte
         {
          unsigned short  val;

          struct
               {
                unsigned char  a;
                unsigned char  b;
               } half;
         };

     union R4byte
         {
          unsigned long  val;

          struct
               {
                unsigned char  a;
                unsigned char  b;
                unsigned char  c;
                unsigned char  d;
               } quarter;
         };

     union R8byte
         {
    #ifdef _LONG_LONG
          unsigned long long  val;
    #endif
          struct
               {
                unsigned char  a;
                unsigned char  b;
                unsigned char  c;
                unsigned char  d;
                unsigned char  e;
                unsigned char  f;
                unsigned char  g;
                unsigned char  h;
               } eighth;
         };
    };

// -----------------------------------------------------------------------------
inline void swapEnds(unsigned short &val)
{
 unsigned char  tmp = ((_p_endiannes_cvt_::R2byte *)&val)->half.a;

 ((_p_endiannes_cvt_::R2byte *)&val)->half.a = ((_p_endiannes_cvt_::R2byte *)&val)->half.b;
 ((_p_endiannes_cvt_::R2byte *)&val)->half.b = tmp;
}

// -----------------------------------------------------------------------------
inline void swapEnds(unsigned long &val)
{
 unsigned char  tmp = ((_p_endiannes_cvt_::R4byte *)&val)->quarter.a;

 ((_p_endiannes_cvt_::R4byte *)&val)->quarter.a = ((_p_endiannes_cvt_::R4byte *)&val)->quarter.d;
 ((_p_endiannes_cvt_::R4byte *)&val)->quarter.d = tmp;
                                            tmp = ((_p_endiannes_cvt_::R4byte *)&val)->quarter.b;
 ((_p_endiannes_cvt_::R4byte *)&val)->quarter.b = ((_p_endiannes_cvt_::R4byte *)&val)->quarter.c;
 ((_p_endiannes_cvt_::R4byte *)&val)->quarter.c = tmp;
}

// -----------------------------------------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds(unsigned long long &val)
    {
     unsigned char  tmp = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.a;

     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.a = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.h;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.h = tmp;
                                               tmp = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.b;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.b = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.g;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.g = tmp;
                                               tmp = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.c;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.c = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.f;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.f = tmp;
                                               tmp = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.d;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.d = ((_p_endiannes_cvt_::R8byte *)&val)->eighth.e;
     ((_p_endiannes_cvt_::R8byte *)&val)->eighth.e = tmp;
    }
#endif

// -----------------------------------------------------------------------------
inline void swapEnds(unsigned short &dest, const unsigned short &src)
{
 ((_p_endiannes_cvt_::R2byte *)&dest)->half.b = ((_p_endiannes_cvt_::R2byte *)&src)->half.a;
 ((_p_endiannes_cvt_::R2byte *)&dest)->half.a = ((_p_endiannes_cvt_::R2byte *)&src)->half.b;
}

// -----------------------------------------------------------------------------
inline void swapEnds (unsigned long &dest, const unsigned long &src)
{
 ((_p_endiannes_cvt_::R4byte *)&dest)->quarter.a = ((_p_endiannes_cvt_::R4byte *)&src)->quarter.d;
 ((_p_endiannes_cvt_::R4byte *)&dest)->quarter.b = ((_p_endiannes_cvt_::R4byte *)&src)->quarter.c;
 ((_p_endiannes_cvt_::R4byte *)&dest)->quarter.c = ((_p_endiannes_cvt_::R4byte *)&src)->quarter.b;
 ((_p_endiannes_cvt_::R4byte *)&dest)->quarter.d = ((_p_endiannes_cvt_::R4byte *)&src)->quarter.a;
}

// -----------------------------------------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds (unsigned long long &dest, const unsigned long long &src)
    {
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.a = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.h;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.b = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.g;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.c = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.f;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.d = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.e;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.e = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.d;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.f = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.c;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.g = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.b;
     ((_p_endiannes_cvt_::R8byte *)&dest)->eighth.h = ((_p_endiannes_cvt_::R8byte *)&src)->eighth.a;
    }
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------
inline void swapEnds(short &val)
{
 swapEnds((unsigned short &)val);
}

// -----------------------------------------------
inline void swapEnds(int &val)
{
 swapEnds((unsigned long &)val);
}

// -----------------------------------------------
inline void swapEnds(long &val)
{
 swapEnds((unsigned long &)val);
}

// -----------------------------------------------
inline void swapEnds(unsigned &val)
{
 swapEnds((unsigned long &)val);
}

// -----------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds(long long &val)
    {
     swapEnds((unsigned long long &)val);
    }
#endif

// -----------------------------------------------
inline void swapEnds(float &val)
{
 swapEnds((unsigned long &)val);
}

// -----------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds(double &val)
    {
     swapEnds((unsigned long long &)val);
    }
#endif

// -----------------------------------------------
inline void swapEnds(short &dest, const short &src)
{
 swapEnds((unsigned short &)dest, (unsigned short &)src);
}

// -----------------------------------------------
inline void swapEnds(int &dest, const int &src)
{
 swapEnds((unsigned long &)dest, (unsigned long &)src);
}

// -----------------------------------------------
inline void swapEnds(long &dest, const long &src)
{
 swapEnds((unsigned long &)dest, (unsigned long &)src);
}

// -----------------------------------------------
inline void swapEnds(unsigned &dest, const unsigned &src)
{
 swapEnds((unsigned long &)dest, (unsigned long &)src);
}

// -----------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds(long long &dest, const long long &src)
    {
     swapEnds((unsigned long long &)dest, (unsigned long long &)src);
    }
#endif

// -----------------------------------------------
inline void swapEnds(float &dest, const float &src)
{
 swapEnds((unsigned long &)dest, (unsigned long &)src);
}

// -----------------------------------------------
#ifdef _LONG_LONG
    inline void swapEnds(double &dest, const double &src)
    {
     swapEnds((unsigned long long &)dest, (unsigned long long &)src);
    }
#endif

#endif //__cplusplus

#include "packpop.h"

#endif // __SWAPENDS_H

/* <-- EOF --> */