/*--------------------------------------------------------------
 *
 * Implementation of template-based utility functions used 
 * by RWMultiPrecisionInt.
 *
 * (c) Copyright 1993, 1996-1997 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 *--------------------------------------------------------------*/

#ifndef __RWINTUTIL_CC__
#define __RWINTUTIL_CC__

#include "rw/money/mpint.h"
#include "rw/rstream.h"

#if defined(__SUNPRO_CC) && RWSTDLIBTYPE == 1
#include <math.h> //for rint
#endif

static inline long rw_rint(double x)
#if defined(__SUNPRO_CC)
// SunPro has a function rint that rounds to nearest decimal integer
{
  return (long)(rint(x));
}
#else
// A portable function for rounding a float to the nearest integer.
{
  if (fabs(ceil(x) - x) > 0.5)
    return((long)floor(x));
  else
    return((long)ceil(x));
}
#endif

// The following macro is used inside the two next template functions
// to generate a static const double for use in repeated divides
#define RW_LARGEST_POW_10(lgshft) \
   ((lgshft ==  3) ? 1000L /* 16-bit systems  (8-bit halfwords)*/ \
  :((lgshft ==  6) ? 1e+6  /* 32-bit systems (16-bit halfwords)*/ \
  :((lgshft == 10) ? 1e+10 /* 64-bit systems (32-bit halfwords)*/ \
  : (lgshft == 20) ? 1e+20 /*128-bit systems (64-bit halfwords)*/ \
  :rw_rint(::pow(10.0,(double)lgshft))))) /*Any other-bit systems */

#define RW_LOG2 0.301029995664

// This utility function repeatedly divides the integer represented in a 
// halfword array by ten.  The number of divides is indicated by *nshift*.
template< class T >
unsigned int decimalShiftRight(T* hw_array,
                               unsigned int hw_arraysize,
                               unsigned int nshift)
{
  // largest_shift is technically log10(pow(2,HWDigitSize)-1),
  // however, this value is approximately equal to
  // log10(pow(2,HWDigitSize)), which can be simplified to
  //   HWDigitSize * log 2.
  // Using this value is faster than using log10(), and since the
  // result is truncated, there is no extra inaccuracy, even for
  // 16-bit systems (8-bit halfwords). log 2 == .301029995664
  //
  static const unsigned int largest_shift =
       (unsigned int) ((RWIntutilConstants<T>::HWDigitSize) * RW_LOG2);

  // by using predefined values for known "bitnesses", we can can
  // make the computation of largest_pow10 a little more efficient by
  // not calling pow(). For weird platforms, we still use pow().
  //

  static const T largest_pow10 =
#if defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x005
                                RW_LARGEST_POW_10(largest_shift);
#else
  (T)( RW_LARGEST_POW_10(largest_shift) );
#endif

  unsigned int i, nshifted=0, divisor = largest_pow10;

  while (nshift > 0)
  {
    // Determine the divisor.  
    if (nshift >= largest_shift)
    {
      nshift -= largest_shift;
      nshifted += largest_shift;
    }
    else if (nshift > 0)
    {
      divisor = 1U;
      for ( i = 0; i < nshift; ++i )
      {
        divisor *= 10U;
      }
      nshifted += nshift;
      nshift = 0;
    }

    // Fast division algorithm.
    T fast_remainder=0;
    for (i=0; i < hw_arraysize; i++)
    {
      fast_remainder = (fast_remainder << RWIntutilConstants<T>::HWDigitSize) +
                        hw_array[i];
      hw_array[i] = fast_remainder / divisor;
      fast_remainder %= divisor;
    }
  }
  return nshifted;
}


// This utility function repeatedly divides the integer represented in a 
// halfword array by ten.  Upon exit, it is guaranteed that the integer fits
// into a halfword array half the size of the one passed in.
template< class T >
unsigned int decimalShiftRightToHalfSize(T* hw_array, unsigned int hw_arraysize, unsigned int nshift)
{
  // largest_shift is technically log10(pow(2,HWDigitSize)-1),
  // however, this value is approximately equal to
  // log10(pow(2,HWDigitSize)), which can be simplified to
  //   HWDigitSize * log 2.
  // Using this value is faster than using log10(), and since the
  // result is truncated, there is no extra inaccuracy, even for
  // 16-bit systems (8-bit halfwords). log 2 == .301029995664
  //
  static const unsigned int largest_shift =
                (unsigned int) ((RWIntutilConstants<T>::HWDigitSize) * RW_LOG2);

  // by using predefined values for known "bitnesses", we can can
  // make the computation of largest_pow10 a little more efficient by
  // not calling pow(). For weird platforms, we still use pow().
  //

  static const T largest_pow10 =
#if defined(__SUNPRO_CC) && __SUNPRO_CC >= 0x005
                                RW_LARGEST_POW_10(largest_shift);
#else
  (T)( RW_LARGEST_POW_10(largest_shift) );
#endif

  unsigned int i, nshifted=0, divisor = largest_pow10;

  // The second condition below is necessary because always knowing the exact 
  // number of decimal shifts is hard; we know we're within one halfword digit
  // of having a good answer when nshift is zero. The third test checks that
  // will eventually become the sign bit must be zero, along with all more 
  // significant bits.
  T one = 1;
  while (nshift > 0                         ||
        (hw_array[(hw_arraysize/2)-1] != 0) ||
        (hw_array[(hw_arraysize/2)] & (one << (RWIntutilConstants<T>::HWDigitSize-one))))
  {
    // Determine the divisor.  
    if (nshift >= largest_shift)
    {
      nshift -= largest_shift;
      nshifted += largest_shift;
    }
    else if (nshift > 0)
    {
      divisor = 1U;
      for ( i = 0; i < nshift; ++i )
      {
        divisor *= 10U;
      }

      nshifted += nshift;
      nshift = 0;
    }
    else
    {
      divisor = 10;
      nshifted++;
    }

    // Fast division algorithm.
    T fast_remainder=0;
    for (i=0; i < hw_arraysize; i++)
    {
      fast_remainder = (fast_remainder << RWIntutilConstants<T>::HWDigitSize) +
                        hw_array[i];
      hw_array[i] = fast_remainder / divisor;
      fast_remainder %= divisor;
    }
  }
  return nshifted;
}


template< class T >
void printAsBinary( const T* digits, const size_t numDigits )
{
  unsigned bitsPerDigit = sizeof(T)*CHAR_BIT;
  int numBits = numDigits*bitsPerDigit;
  char *binaryDigits = new char[numBits + 1];
  T temp;
  size_t i, j, bdigit;
  for ( i = 0; i < numDigits; i++ )
  {
    bdigit = i*bitsPerDigit + bitsPerDigit - 1;
    for ( j = 0; j < bitsPerDigit; j++ )
    {
      temp = (digits[i] >> j) & ~(~0 << 1);
      if ( temp == 1 )
      {
        binaryDigits[bdigit] = '1';
      }
      else
      {
        binaryDigits[bdigit] = '0';
      }
      --bdigit;
    }
  }

  binaryDigits[numBits] = '\0';
  RW_SL_IO_STD(cout) << binaryDigits << RW_SL_IO_STD(endl);
  delete []binaryDigits;
}

#undef RW_LARGEST_POW_10
#undef RW_LOG2

#endif // __RWINTUTIL_CC__
