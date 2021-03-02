//-*--------------------------------------------------------------------------*-
//
// File Name   : rslimits.h
//
// Copyright (c) 1991 - 2006 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// September 5,2007  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSLIMITS_H
#define __RSLIMITS_H

#if defined(_MSC_VER) && defined(__cplusplus)

    #ifndef _LIMITS_
        #include <limits>
    #endif

    #ifndef  __RSTYPES_H
        #include "rstypes.h"
    #endif

    _STD_BEGIN

    #ifdef USE_NUMERIC

        template<> class numeric_limits<Numeric> : public _Num_int_base
        {
        public:
          typedef Numeric  _Ty;

          static _Ty(min) () _THROW0()
          {
           return NUMERIC_MIN;
          }

          static _Ty (max) () _THROW0()
          {
           return NUMERIC_MAX;
          }

          static _Ty epsilon() _THROW0()
          {
           return 0;
          }

          static _Ty round_error() _THROW0()
          {
           return 0;
          }

          static _Ty denorm_min() _THROW0()
          {
           return 0;
          }

          static _Ty infinity() _THROW0()
          {
           return 0;
          }

          static _Ty quiet_NaN() _THROW0()
          {
           return 0;
          }

          static _Ty signaling_NaN() _THROW0()
          {
           return 0;
          }

          _STCONS(bool, is_signed, true);
          _STCONS(int, digits10, RSL_NUMERIC_DIGITS);
          _STCONS(int, max_exponent10, RSL_NUMERIC_DIGITS);
          _STCONS(int, min_exponent10, 0);
        };

        _STCONSDEF(numeric_limits<Numeric>, int, digits10)
        _STCONSDEF(numeric_limits<Numeric>, int, max_exponent10)
        _STCONSDEF(numeric_limits<Numeric>, int, min_exponent10)

    #endif // USE_NUMERIC

    #ifdef USE_FDECIMAL

        template<> class numeric_limits<FMoney> : public _Num_int_base
        {
         public:
           typedef FMoney  _Ty;

           static _Ty(min) () _THROW0()
           {
            return FMoney_MinValue();
           }

           static _Ty(max) () _THROW0()
           {
            return FMoney_MaxValue();
           }

           static _Ty epsilon() _THROW0()
           {
            return 0;
           }

           static _Ty round_error() _THROW0()
           {       
            return 0;
           }

           static _Ty denorm_min() _THROW0()
           {
            return 0;
           }

           static _Ty infinity() _THROW0()
           {
            return 0;
           }

           static _Ty quiet_NaN() _THROW0()
           {
            return 0;
           }

           static _Ty signaling_NaN() _THROW0()
           {
            return 0;
           }

           _STCONS(bool, is_signed, true);
           _STCONS(int, digits10, 16);
           _STCONS(int, max_exponent10, 4);
           _STCONS(int, min_exponent10, 0);
        };

        _STCONSDEF(numeric_limits<FMoney>, int, digits10)
        _STCONSDEF(numeric_limits<FMoney>, int, max_exponent10)
        _STCONSDEF(numeric_limits<FMoney>, int, min_exponent10)

    #endif // USE_FDECIMAL

    _STD_END

#endif  // _MSC_VER
#endif  // __RSLIMITS_H

/* <-- EOF --> */