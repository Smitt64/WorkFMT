/*--------------------------------------------------------------
 *
 * Utility functions used by RWMultiPrecisionInt
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

#ifndef __RWINTUTIL_H__
#define __RWINTUTIL_H__

#include "rw/dcmldefs.h"
#if defined(RW_MNY_NO_NEW_HEADER)
#  include <math.h>
#else
#  include <cmath>
#endif        // for pow()
#include "rw/money/decdefs.h"
#include "rw/money/utlconst.h"    // for RWIntutilConstants<T> constants


// The following declaration guarantees that
// RWIntutilConstants<unsigned long> is initialized.
#if !defined(__hppa) || defined(HP_ANSI)
// HP-CC build doesn't need this initialization
static RWIntutilConstants<unsigned long> RWIntutilConstantsVarUL;
#endif

//
// Global function prototypes found in intutil.cc
//
template <class T>
void printAsBinary( const T* digits, const size_t numDigits );

template< class T >
unsigned int decimalShiftRight(T * hw_array, unsigned int hw_arraysize, unsigned int nshift);

template< class T >
unsigned int decimalShiftRightToHalfSize(T * hw_array, unsigned int hw_arraysize, unsigned int nshift);


//
// RWHalfwordSplit: splits a zero-referenced array of words into
// an array of halfwords.  Works only on nonnegative numbers.
//
// Note: the parameter *S* refers to the size of the *word* array.
// Calling form: RWHalfwordSplit<DigitType,S-1>::split(arr_n,arr_2n);
//
template <class T, int N>
class RWHalfwordSplit {
public:
  static inline void split(const T* arr_n, T* arr_2n)
  {
    arr_2n[2*N]   = arr_n[N] >> RWIntutilConstants<T>::HWDigitSize;
    arr_2n[2*N+1] = arr_n[N] &  RWIntutilConstants<T>::LowerMask;
    RWHalfwordSplit<T, N-1>::split(arr_n, arr_2n);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordSplit<unsigned long, -1> {
public:
    static inline void split(const unsigned long*, unsigned long*)
    { }
};
/*
class RWHalfwordSplit<unsigned int, -1> {
public:
    static inline void split(const unsigned int*, unsigned int*)
    { }
};

class RWHalfwordSplit<unsigned short, -1> {
public:
    static inline void split(const unsigned short*, unsigned short*)
    { }
};

class RWHalfwordSplit<unsigned char, -1> {
public:
    static inline void split(const unsigned char*, unsigned char*)
    { }
};
*/


//
// RWHalfwordSplitReverse: splits a zero-referenced array of words into 
// an array of halfwords, but puts the halfwords in reverse order (for 
// simplifying a multiplication algorithm).  Works only on nonnegative
// numbers.
//
// Note: this should always be called with the first two parameters only.
//       In the call, *S* refers to the size of the *word* array.
// Calling form: RWHalfwordSplitReverse<DigitType,S-1>::split(arr_n,arr_2n);
//
template <class T, int N>
class RWHalfwordSplitReverse {
public:
  static inline void split(const T* arr_n, T* arr_2n, size_t size = N+1)
  {
    arr_2n[2*N]   = arr_n[size-1-N] &  RWIntutilConstants<T>::LowerMask;
    arr_2n[2*N+1] = arr_n[size-1-N] >> RWIntutilConstants<T>::HWDigitSize;
    RWHalfwordSplitReverse<T, N-1>::split(arr_n, arr_2n, size);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordSplitReverse<unsigned long, -1> {
public:
    static inline void split(const unsigned long*, unsigned long*, size_t = 0)
    { }
};
/*
class RWHalfwordSplitReverse<unsigned int, -1> {
public:
    static inline void split(const unsigned int*, unsigned int*, size_t)
    { }
};

class RWHalfwordSplitReverse<unsigned short, -1> {
public:
    static inline void split(const unsigned short*, unsigned short*, size_t)
    { }
};

class RWHalfwordSplitReverse<unsigned char, -1> {
public:
    static inline void split(const unsigned char*, unsigned char*, size_t)
    { }
};
*/


//
// RWHalfwordPropagate: propagates carries in a halfword array, so that
// a subsequent merge from halfwords to words can be performed without
// loss of carry information.  The number represented must be nonnegative.
//
// Note: The parameter *S* refers to the size of a *word* array;
//       if array has length 2*S, call is made with parameter (2*S-2). 
// Calling form: RWHalfwordPropagate<DigitType,2*S-2>::propagate(array);
//
template <class T, int N>
class RWHalfwordPropagate {
public:
  static inline void propagate(T* array)
  {
    array[N]   += array[N+1] >> RWIntutilConstants<T>::HWDigitSize;
    array[N+1] &= RWIntutilConstants<T>::LowerMask;
    RWHalfwordPropagate<T,N-1>::propagate(array);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordPropagate<unsigned long, -1> {
public:
    static inline void propagate(const unsigned long*)
    { }
};
/*
class RWHalfwordPropagate<unsigned int, -1> {
public:
    static inline void propagate(const unsigned int*)
    { }
};

class RWHalfwordPropagate<unsigned short, -1> {
public:
    static inline void propagate(const unsigned short*)
    { }
};

class RWHalfwordPropagate<unsigned char, -1> {
public:
    static inline void propagate(const unsigned char*)
    { }
};
*/


//
// RWHalfwordMerge: merges a zero-referenced array of halfwords into 
// an array of words.  Works only on nonnegative numbers.
//
// Note: the template parameter *S* refers to the size of the *word* array.
// Calling form: RWHalfwordMerge<DigitType,S-1>::merge(arr_2n,arr_n);
//
template <class T, int N>
class RWHalfwordMerge {
public:
  static inline void merge(const T* arr_2n, T* arr_n)
  {
    arr_n[N] = (arr_2n[2*N] << RWIntutilConstants<T>::HWDigitSize) |
               (arr_2n[2*N+1] & RWIntutilConstants<T>::LowerMask);
    RWHalfwordMerge<T, N-1>::merge(arr_2n, arr_n);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordMerge<unsigned long, -1> {
public:
    static inline void merge(const unsigned long*, unsigned long*)
    { }
};
/*
class RWHalfwordMerge<unsigned int, -1> {
public:
    static inline void merge(const unsigned int*, unsigned int*)
    { }
};

class RWHalfwordMerge<unsigned short, -1> {
public:
    static inline void merge(const unsigned short*, unsigned short*)
    { }
};

class RWHalfwordMerge<unsigned char, -1> {
public:
    static inline void merge(const unsigned char*, unsigned char*)
    { }
};
*/


//
// RWArrayCopy: copies one array to another.
//
// Note: here, *S* refers to the size of the array.
// Calling form: RWArrayCopy<DigitType,S-1>::copy(source,result);
//
template <class T, int N>
class RWArrayCopy {
public:
  static inline void copy(const T* source, T* result)
  {
    result[N] = source[N];
    RWArrayCopy<T, N-1>::copy(source, result);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWArrayCopy<unsigned long, -1> {
public:
    static inline void copy(const unsigned long*, unsigned long*)
    { }
};
/*
class RWArrayCopy<unsigned int, -1> {
public:
    static inline void copy(const unsigned int*, unsigned int*)
    { }
};

class RWArrayCopy<unsigned short, -1> {
public:
    static inline void copy(const unsigned short*, unsigned short*)
    { }
};

class RWArrayCopy<unsigned char, -1> {
public:
    static inline void copy(const unsigned char*, unsigned char*)
    { }
};
*/

//
// RWHalfwordInit: initializes each element in a zero-referenced array of
// halfwords with a given value.
//
// Note: here, *S* refers to the size of a *word* array.
// Calling form: RWHalfwordInit<DigitType,2*S-1>::init(array,value);
//
template <class T, int N>
class RWHalfwordInit
{
public:
  static inline void init(T* array, const T value)
  {
    array[N] = value;

    RWHalfwordInit<T, N-1>::init(array, value);
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordInit<unsigned long, -1>
{
public:
    static inline void init(unsigned long*, const unsigned long)
   {
   }
};

/*
class RWHalfwordInit<unsigned int, -1> {
public:
    static inline void init(unsigned int*, const unsigned int)
    { }
};

class RWHalfwordInit<unsigned short, -1> {
public:
    static inline void init(unsigned short*, const unsigned short)
    { }
};

class RWHalfwordInit<unsigned char, -1> {
public:
    static inline void init(unsigned char*, const unsigned char)
    { }
};

*/

//
// RWDigitAdd: adds two wholeword arrays and puts result into a third array.
// Returns overflow status.
//
// Note: this function should be called with the first three parameters only.
// Calling form: RWDigitAdd<DigitType,S-1>::add(op1,op2,result);
//
template <class T, int N>
class RWDigitAdd {
public:
  static inline int add(const T* op1, const T* op2, T* result, int carry=0)
  {
    T res = op1[N] + op2[N] + carry;  // use *res* in case result=(op1 or op2)
    if ((res <= op1[N] && op2[N] != 0) || 
        (res <= op2[N] && op1[N] != 0))
    {
      carry = 1;
    }
    else 
    {
      carry = 0;
    }
    result[N] = res;
    return( RWDigitAdd<T, N-1>::add(op1, op2, result, carry) );
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWDigitAdd<unsigned long, -1> {
public:
    static inline int add(const unsigned long*, const unsigned long*, unsigned long*, int carry=0)
    { return(carry); }
};
/*
class RWDigitAdd<unsigned int, -1> {
public:
    static inline RWBoolean add(const unsigned int*, const unsigned int*, unsigned int*, int carry)
    { return(carry); }
};

class RWDigitAdd<unsigned short, -1> {
public:
    static inline RWBoolean add(const unsigned short*, const unsigned short*, unsigned short*, int carry)
    { return(carry); }
};

class RWDigitAdd<unsigned char, -1> {
public:
    static inline RWBoolean add(const unsigned char*, const unsigned char*, unsigned char*, int carry)
    { return(carry); }
};

*/
//
// RWHalfwordAdd: adds two zero-referenced halfword arrays and puts
// result into a third array.
//
// Note: this should be called with the first three parameters only; here,
//       *S* is the size of a *word* array.
// Calling form: RWHalfwordAdd<DigitType,2*S-1>::add(op1,op2,result);
//
template <class T, int N>
class RWHalfwordAdd {
public:
  static inline RWBoolean add(const T* op1, const T* op2, T* result, T remainder=0)
  {
    remainder += op1[N] + op2[N];
    result[N] = remainder & RWIntutilConstants<T>::LowerMask;
    remainder >>= RWIntutilConstants<T>::HWDigitSize;
    return( RWHalfwordAdd<T, N-1>::add(op1, op2, result, remainder) );
  };
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordAdd<unsigned long, -1> {
public:
    static inline RWBoolean add(const unsigned long*, const unsigned long*, unsigned long*, unsigned long remainder = 0)
    { 
      return( remainder != 0);
    }
};
/*
class RWHalfwordAdd<unsigned int, -1> {
public:
    static inline RWBoolean add(const unsigned int*, const unsigned int*, unsigned int*, unsigned int remainder)
    {     
      return( remainder != 0);
    }
};

class RWHalfwordAdd<unsigned short, -1> {
public:
    static inline RWBoolean add(const unsigned short*, const unsigned short*, unsigned short*, unsigned short remainder)
    {
      return( remainder != 0);
    }
};

class RWHalfwordAdd<unsigned char, -1> {
public:
    static inline RWBoolean add(const unsigned char*, const unsigned char*, unsigned char*, unsigned char remainder)
    {
      return( remainder != 0);
    }
};

*/

//
// RWHalfwordSubtract: subtracts two zero-referenced halfword 
// arrays and puts result into a third array. Works only for two nonnegative
// operands.
//
// Note: this function should always be called with three parameters only.
//       In the calling notation, *S* refers to the size of a *word* array.
// Calling form: RWHalfwordSubtract<DigitType,2*S-1>::subtract(op1,op2,result);
//
template <class T, int N>
class RWHalfwordSubtract {
public:
  // some compilers don't allow template expressions as default parameters; 
  // hence an extra definition.
  static inline RWBoolean subtract(const T* op1, const T* op2, T* result)
  {
    return( subtract(op1, op2, result, RWIntutilConstants<T>::HWBorrowBit) );
  }
  static inline RWBoolean subtract(const T* op1, const T* op2, T* result, T borrow)
  {
    borrow = RWIntutilConstants<T>::LowerMask + op1[N] - op2[N] + 
             (borrow >> RWIntutilConstants<T>::HWDigitSize);
    result[N] = borrow & RWIntutilConstants<T>::LowerMask;
    return( RWHalfwordSubtract<T, N-1>::subtract(op1, op2, result, borrow) );
  }
};

// Base cases for the recursion; return TRUE if subtraction went negative.
// The Microsoft 4.2 compiler can't handle using the constant HWBorrowBit,
// so we waste a little efficiency in creating our own version here.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordSubtract<unsigned long, -1> {
public:
    static inline RWBoolean subtract(const unsigned long*, const unsigned long*, unsigned long*, unsigned long borrow)
    { 
      unsigned long bbit = 1UL << RWIntutilConstants<unsigned long>::HWDigitSize;
      return(borrow < bbit);
    }
};
/*
class RWHalfwordSubtract<unsigned int, -1> {
public:
    static inline RWBoolean subtract(const unsigned int*, const unsigned int*, unsigned int*, unsigned int borrow)
    { 
      unsigned int bbit = 1 << RWIntutilConstants<unsigned int>::HWDigitSize;
      return(borrow < bbit);
    }
};

class RWHalfwordSubtract<unsigned short, -1> {
public:
    static inline RWBoolean subtract(const unsigned short*, const unsigned short*, unsigned short*, unsigned short borrow)
    { 
      unsigned short bbit = 1 << RWIntutilConstants<unsigned short>::HWDigitSize;
      return(borrow < bbit);
    }
};

class RWHalfwordSubtract<unsigned char, -1> {
public:
    static inline RWBoolean subtract(const unsigned char*, const unsigned char*, unsigned char*, unsigned char borrow)
    { 
      unsigned char bbit = 1 << RWIntutilConstants<unsigned char>::HWDigitSize;
      return(borrow < bbit);
    }
};

*/

// RWHalfwordShiftRight: shift right a zero-referenced halfword array, so
// that the padded-zero areas in each digit are ignored.  Used by the
// MPInt division algorithm.  This algorithm works only if
// the amount to shift does not exceed the number of bits in a halfword.
//
// Note: the function should be called with the first two parameters only.
//       In the calling notation, *S* refers to the size of a *word* array.
// Calling form: RWHalfwordShiftLeft<DigitType,2*S-1>::left(shiftBy,array);
//
template <class T, int N>
class RWHalfwordShiftRight {
public:
  static inline void right(const size_t shiftBy, T* array)
  {
    array[N] >>= shiftBy;
    array[N]  |= (array[N-1] << (RWIntutilConstants<T>::HWDigitSize - shiftBy)) &
                 RWIntutilConstants<T>::LowerMask;
    RWHalfwordShiftRight<T, N-1>::right(shiftBy, array);
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordShiftRight<unsigned long, 0> {
public:
  static inline void right(const size_t shiftBy, unsigned long* array)
  { array[0] >>= shiftBy; }
};
/*
class RWHalfwordShiftRight<unsigned int, 0> {
public:
  static inline void right(const size_t shiftBy, unsigned int* array)
  { array[0] >>= shiftBy; }
};

class RWHalfwordShiftRight<unsigned short, 0> {
public:
  static inline void right(const size_t shiftBy, unsigned short* array)
  { array[0] >>= shiftBy; }
};

class RWHalfwordShiftRight<unsigned char, 0> {
public:
  static inline void right(const size_t shiftBy, unsigned char* array)
  { array[0] >>= shiftBy; }
};

*/



// RWHalfwordShiftLeft: shift left a zero-referenced halfword array, so
// that the padded-zero areas in each digit are ignored.  Used by the
// MPInt division algorithm.  This algorithm works only if
// the amount to shift does not exceed the number of bits in a halfword.
//
// Note: the function should be called with the first two parameters only.
//       In the calling notation, *S* refers to the size of a *word* array.
// Calling form: RWHalfwordShiftLeft<DigitType,2*S-1>::left(shiftBy,array);
//
template <class T, int N>
class RWHalfwordShiftLeft {
public:
  static inline void left(const size_t shiftBy, T* array, size_t maxindex = N)
  {
    array[maxindex-N]  = (array[maxindex-N] << shiftBy) & RWIntutilConstants<T>::LowerMask;
    array[maxindex-N] |= array[maxindex-N+1] >> (RWIntutilConstants<T>::HWDigitSize - shiftBy);
    RWHalfwordShiftLeft<T, N-1>::left(shiftBy, array, maxindex);
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordShiftLeft<unsigned long, 0> {
public:
  static inline void left(const size_t shiftBy, unsigned long* array, size_t maxindex = 0)
  { array[maxindex] = (array[maxindex] << shiftBy) & RWIntutilConstants<unsigned long>::LowerMask; }
};
/*
class RWHalfwordShiftLeft<unsigned int, 0> {
public:
  static inline void left(const size_t shiftBy, unsigned int* array, size_t maxindex)
  { array[maxindex] = (array[maxindex] << shiftBy) & RWIntutilConstants<unsigned int>::LowerMask; }
};

class RWHalfwordShiftLeft<unsigned short, 0> {
public:
  static inline void left(const size_t shiftBy, unsigned short* array, size_t maxindex)
  { array[maxindex] = (array[maxindex] << shiftBy) & RWIntutilConstants<unsigned short>::LowerMask; }
};

class RWHalfwordShiftLeft<unsigned char, 0> {
public:
  static inline void left(const size_t shiftBy, unsigned char* array, size_t maxindex)
  { array[maxindex] = (array[maxindex] << shiftBy) & RWIntutilConstants<unsigned char>::LowerMask; }
};
*/


// RWHalfwordMultiply: multiplies a zero-indexed halfword array by a halfword
// value.  Used by the MPInt division algorithm; works only on nonnegative
// values.  Returns carry-out from most significant digit.
//
// Note: This function should be called with the first three parameters only;
//       in the calling form, *S* refers to the typical size of a *word* array.
// Calling form: RWHalfwordMultiply<DigitType, 2*S-1>::multiply(array,value,result);
//
template <class T, int N>
class RWHalfwordMultiply {
public:
  static inline T multiply(const  T* array, T value, T* result, T carry = 0)
  {
    carry += array[N] * value;
    result[N] = carry & RWIntutilConstants<T>::LowerMask;
    carry >>= RWIntutilConstants<T>::HWDigitSize;
    return( RWHalfwordMultiply<T, N-1>::multiply(array, value, result, carry) );
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWHalfwordMultiply<unsigned long, -1> {
public:
  static inline unsigned long multiply(const unsigned long*, unsigned long, unsigned long*, unsigned long carry = 0)
  { return carry; }
};
/*
class RWHalfwordMultiply<unsigned int, -1> {
public:
  static inline unsigned int multiply(const unsigned int*, unsigned int, unsigned int*, unsigned int carry)
  { return carry; }
};

class RWHalfwordMultiply<unsigned short, -1> {
public:
  static inline unsigned short multiply(const unsigned short*, unsigned short, unsigned short*, unsigned short carry)
  { return carry; }
};

class RWHalfwordMultiply<unsigned char, -1> {
public:
  static inline unsigned char multiply(const unsigned char*, unsigned char, unsigned char*, unsigned char carry)
  { return carry; }
};
*/


//
// RWIntToDouble: converts a multiple precision integer to a double
// precision value.  Works only for nonnegative numbers.  Algorithm
// works from least significant to most significant digit; it
// multiplies each digit by its appropriate power of two before adding in
// its contribution.
//
// Note: this function should always be called with the first parameter only.
// Calling form: RWIntToDouble<DigitType,S-1>::todouble(array);
//
template <class T, int N>
class RWIntToDouble {
public:
  static inline double todouble(const T* array, double value=0.0, size_t ndigits=N)
  {
    // Convert this value to double by recursively calculating the decimal value
    // of each halfword, starting with the most significant. We multiply by
    // MaxDigitInt + 1 (which represents pow(2,X), where X is the "bitness" of
    // the current platform) each time, which conceptually shifts the calculated
    // halfwords to the left each time. In this way, we never have to multiply by
    // anything other than pow(2,X), (and thus don't have to directly call pow()
    // which is slower [and less accurate on HP-UX 10.20] than multiplication).
    //
    return( RWIntToDouble<T, N-1>::todouble(array, value, ndigits) *
     ((double)RWIntutilConstants<T>::MaxDigitInt + 1)+array[N] );
  };
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWIntToDouble<unsigned long, -1> {
public:
  static inline double todouble(const unsigned long*, double value = 0.0, size_t = (size_t) -1)
  { 
    return(value);
  }
};
/*
class RWIntToDouble<unsigned int, -1> {
public:
  static inline double todouble(const unsigned int*, double value, size_t)
  { 
    return(value);
  }
};

class RWIntToDouble<unsigned short, -1> {
public:
  static inline double todouble(const unsigned short*, double value, size_t)
  { 
    return(value);
  }
};

class RWIntToDouble<unsigned char, -1> {
public:
  static inline double todouble(const unsigned char*, double value, size_t)
  { 
    return(value);
  }
};

*/

// RWDigitNegate: negates a wholeword array and puts result into a second array.
// Returns overflow status.
//
// Note: this function should be called with the first two parameters only.
// Calling form: RWDigitNegate<DigitType,S-1>::negate(op1,result);
//
template <class T, int N>
class RWDigitNegate {
public:
  static inline int negate(const T* op1, T* result, int carry=1)
  {
    T res = (RWIntutilConstants<T>::MaxDigitInt - op1[N]);
    res += T(carry);
    if (res == T(0) &&  carry == 1 )
    {
      carry = 1;
    }
    else 
    {
      carry = 0;
    }
    result[N] = res;
    return( RWDigitNegate<T, N-1>::negate(op1, result, carry) );
  };
};

// Base cases for the recursion
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWDigitNegate<unsigned long, -1> {
public:
    static inline int negate(const unsigned long*, unsigned long*, int carry = 1)
    { return(carry); }
};
/*
class RWDigitNegate<unsigned int, -1> {
public:
    static inline int negate(const unsigned int*, unsigned int*, int carry)
    { return(carry); }
};

class RWDigitNegate<unsigned short, -1> {
public:
    static inline int negate(const unsigned short*, unsigned short*, int carry)
    { return(carry); }
};

class RWDigitNegate<unsigned char, -1> {
public:
    static inline int negate(const unsigned char*, unsigned char*, int carry)
    { return(carry); }
};

*/

// RWMPIntEqual: returns true if two arrays of type T represent the same
// integer, i.e., if corresponding elements in the arrays are equal. 
//
// Calling form: RWMPIntEqual<DigitType,S-1>::equal(lhs,rhs);
//
template< class T, int N >
class RWMPIntEqual
{
public:
  static inline RWBoolean equal( const T* lhs, const T* rhs )
  {
    RWBoolean result;
    if ( lhs[N] != rhs[N] )
    {
      result = FALSE;
    }
    else
    {
      result = RWMPIntEqual<T, N-1>::equal( lhs, rhs );
    }
    return result;
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWMPIntEqual< unsigned long, -1 >
{
public:
  static inline RWBoolean equal( const unsigned long*, const unsigned long* )
  { return TRUE; }
};
/*
class RWMPIntEqual< unsigned int, -1 >
{
public:
  static inline RWBoolean equal( const unsigned int*, const unsigned int* )
  { return TRUE; }
};

class RWMPIntEqual< unsigned short, -1 >
{
public:
  static inline RWBoolean equal( const unsigned short*, const unsigned short* )
  { return TRUE; }
};

class RWMPIntEqual< unsigned char, -1 >
{
public:
  static inline RWBoolean equal( const unsigned char*, const unsigned char* )
  { return TRUE; }
};
*/


// RWIndexOfLastNonZero: Used by the MPInt multiplication algorithm to
// avoid unnecessary multiplies by zero.  When passed an ordinal array
// of type T, it returns the index of the least significant nonzero value.
//
// Calling form: RWIndexOfLastNonZero<DigitType,2*S-1>::index(array);
//
template< class T, int N >
class RWIndexOfLastNonZero
{
public:
  static inline size_t index( const T* array )
  {
    size_t result;
    if ( array[N] != T(0) )
    {
      result = N;
    }
    else
    {
      result = RWIndexOfLastNonZero<T,N-1>::index( array );
    }
    return result;
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWIndexOfLastNonZero<unsigned long, -1 >
{
public:
  static inline size_t index( const unsigned long* )
  { return RW_NPOS; }
};
/*
class RWIndexOfLastNonZero<unsigned int, -1 >
{
public:
  static inline size_t index( const unsigned int* )
  { return RW_NPOS; }
};

class RWIndexOfLastNonZero<unsigned short, -1 >
{
public:
  static inline size_t index( const unsigned short* )
  { return RW_NPOS; }
};

class RWIndexOfLastNonZero<unsigned char, -1 >
{
public:
  static inline size_t index( const unsigned char* )
  { return RW_NPOS; }
};
*/


// RWIndexOfFirstDiff: Returns the index of the first place the two
// arrays differ.
//
// Calling form: RWIndexOfFirstDiff<DigitType,2*S-1>::index(array1, array2);
//
template< class T, int N >
class RWIndexOfFirstDiff
{
public:
  static inline size_t index( const T* array1, const T* array2, size_t index = RW_NPOS )
  {
    if ( array1[N] != array2[N] )
    {
      index = N;
    }
    return RWIndexOfFirstDiff<T,N-1>::index( array1, array2, index );
  }
};

// Base cases for the recursion.
RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWIndexOfFirstDiff<unsigned long, -1 >
{
public:
  static inline size_t index( const unsigned long*, const unsigned long*, size_t index = RW_NPOS )
  { return index; }
};

/*
class RWIndexOfFirstDiff<unsigned int, -1 >
{
public:
  static inline size_t index( const unsigned int*, const unsigned int*, size_t index )
  { return index; }
};


class RWIndexOfFirstDiff<unsigned short, -1 >
{
public:
  static inline size_t index( const unsigned short*, const unsigned short*, size_t index )
  { return index; }
};


class RWIndexOfFirstDiff<unsigned char, -1 >
{
public:
  static inline size_t index( const unsigned char*, const unsigned char*, size_t index )
  { return index; }
};
*/

#ifdef RW_MONEY_COMPILE_INSTANTIATE
#  include <rw/money/intutil.cc>
#endif

#endif // __RWINTUTIL_H__


