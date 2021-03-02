/*--------------------------------------------------------------
 *
 * Definition of RWMultiPrecisionInt and related classes.
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

#ifndef __RWMPINT_H__
#define __RWMPINT_H__

#include "rw/dcmldefs.h"
#include "rw/money/intutil.h"
#include "rw/money/utlconst.h"

#if defined(RW_MNY_NO_NEW_HEADER)
    #include <math.h>
    #include <string.h>
#else
    #include <cmath>
    #include <cstring>
#endif

#include "rw/rstream.h"
#include "rw/cstring.h"

#ifndef RW_NO_TOOLS
    #include "rw/vstream.h"
    #include "rw/rwfile.h"
#endif

// If using standard library, include <utility> for possible
// relational operator definitions.
#ifndef RW_NO_STL
    #include <utility>
#endif


template <int S>
class RWMPPowersOfTen;

template <int S>
class RWMultiPrecisionInt;

template <int S>
class RWMultiPrecisionInt
{
public:
  typedef long int                                   signed_type;
  typedef unsigned long int                          unsigned_type;
  typedef __int64                                    signed64_type;
  typedef unsigned __int64                           unsigned64_type;
  typedef RWMultiPrecisionInt<S>                     MPInt_S;
  typedef RWIndexOfFirstDiff<unsigned_type, S - 2>   indexType;
  typedef RWMPIntEqual<unsigned long, S - 2>         MPIntEqual;

  //  Constants used to set the minumum and maximum values for an M.P. int
  static const   unsigned long int UNSIGNED_TYPE_MAX;
  static const   unsigned long int UNSIGNED_TYPE_MIN;
  static const   long int          SIGNED_TYPE_MAX;
  static const   long int          SIGNED_TYPE_MIN;

  typedef RWIntutilConstants<unsigned_type> Constants;

// Constructors ---------------------------------------
  RWMultiPrecisionInt() : leadDigit_(0)
  {
   RWHalfwordInit<unsigned_type, S - 2>::init(trailingDigits_, (unsigned_type)0);
  }
    
  RWMultiPrecisionInt(int x)
  {
   unsigned_type  fillValue = x < 0 ? ~0 : 0;


   leadDigit_ = fillValue;

   RWHalfwordInit<unsigned_type, S - 3>::init(trailingDigits_, fillValue);

   trailingDigits_[S - 2] = (unsigned_type)x;
  }

  RWMultiPrecisionInt(signed_type x)
  {
   unsigned_type  fillValue = x < 0 ? ~0 : 0;


   leadDigit_ = fillValue;

   RWHalfwordInit<unsigned_type, S - 3>::init(trailingDigits_, fillValue);

   trailingDigits_[S - 2] = (unsigned_type)x;
  }

  RWMultiPrecisionInt(unsigned_type x) : leadDigit_(0)
  {
   RWHalfwordInit<unsigned_type, S - 3>::init(trailingDigits_, (unsigned_type)0);

   trailingDigits_[S - 2] = (unsigned_type)x;
  }

  RWMultiPrecisionInt(signed64_type x)
  {
   unsigned_type  fillValue = x < 0 ? ~0 : 0;


   leadDigit_ = fillValue;

   RWHalfwordInit<unsigned_type, S - 3>::init(trailingDigits_, fillValue);

   trailingDigits_[S - 2] = (unsigned_type)(x & 0x00000000ffffffff);
   trailingDigits_[S - 3] = (unsigned_type)((x >> 32) & 0x00000000ffffffff);
  }

  RWMultiPrecisionInt(unsigned64_type x) : leadDigit_(0)
  {
   RWHalfwordInit<unsigned_type, S - 3>::init(trailingDigits_, (unsigned_type)0);

   trailingDigits_[S - 2] = (unsigned_type)(x & 0x00000000ffffffff);
   trailingDigits_[S - 3] = (unsigned_type)((x >> 32) & 0x00000000ffffffff);
  }

  RWMultiPrecisionInt(double num);
  RWMultiPrecisionInt(long double num);
  RWMultiPrecisionInt(const char *string);

  RWMultiPrecisionInt(const RWMultiPrecisionInt<S> &x)
  {
   leadDigit_ = x.leadDigit_;

   RWArrayCopy<unsigned_type, S - 2>::copy(x.trailingDigits_, trailingDigits_);
  }


#ifndef RW_NO_NONTYPE_TEMPLATE_FUNCTIONS
// // Comparison operators ---------------------------------------------------------------------------
  friend RWBoolean operator == FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &rhs, const RWMultiPrecisionInt<S> &lhs);

  friend RWBoolean operator <  FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);

#if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )
  friend RWBoolean operator >  FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);

  friend RWBoolean operator != FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);

  friend RWBoolean operator >= FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);

  friend RWBoolean operator <= FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);
#  endif // RW_NO_STL
#else
  friend RWBoolean operator == (const RWMultiPrecisionInt<S> &rhs, const RWMultiPrecisionInt<S> &lhs)
  {
   RWBoolean  result;


   if(lhs.leadDigit_ != rhs.leadDigit_)
     result = FALSE;
   else
     result = MPIntEqual::equal(lhs.trailingDigits_, rhs.trailingDigits_);

   return result;
  }

  friend RWBoolean operator < (const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   RWBoolean  result;


   if(lhs.leadDigit_ != rhs.leadDigit_)
    {
     result = (lhs.leadDigit_ < rhs.leadDigit_);
    }
   else
    {
     size_t  index = indexType::index(lhs.trailingDigits_, rhs.trailingDigits_);

     result = (index == RW_NPOS ? FALSE : (lhs.trailingDigits_[index] < rhs.trailingDigits_[index]));
    }

   return result;
  }  

#if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )
  friend RWBoolean operator >  (const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   return (rhs < lhs);
  }

  friend RWBoolean operator != (const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   return !(lhs == rhs);
  }

  friend RWBoolean operator >= (const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   return !(lhs < rhs);
  }

  friend RWBoolean operator <= (const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   return !(lhs > rhs);
  }
#  endif // RW_NO_STL
#endif  // RW_NO_NONTYPE_TEMPLATE_FUNCTIONS

// Unary minus (negation)
  RWMultiPrecisionInt<S> operator-() const
#ifdef RW_MONEY_NO_INLINE_ARRAYS
  ;
#else
  {
   unsigned_type  trailing[S - 1];  
   int            carry = RWDigitNegate<unsigned_type, S - 2>::negate(trailingDigits_, trailing);
   signed_type    lead  = (Constants::MaxDigitInt - leadDigit_) + carry;

   return RWMultiPrecisionInt<S>(lead, trailing);
  }
#endif

// static member functions
  static const RWMultiPrecisionInt<S> &maxValue() 
  { 
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWMultiPrecisionInt<S>  maxValue_;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

    if(maxValue_.leadDigit_ == 0)
     {
      RWHalfwordInit<unsigned_type, S - 2>::init(maxValue_.trailingDigits_, UNSIGNED_TYPE_MAX);

      maxValue_.leadDigit_ = SIGNED_TYPE_MAX;
     }

    return maxValue_;
  }

  static const RWMultiPrecisionInt<S> &minValue()
  { 
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
   static RWMultiPrecisionInt<S>  minValue_;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

   if(minValue_.leadDigit_ == 0)
     preciseAdd(badValue(), RWMultiPrecisionInt<S>(1), minValue_);

   return minValue_;
  }
  

  static unsigned int decimalPrecision()
  {
   if(decimalPrecision_ == 0)
     decimalPrecision_ = (int)(log10(pow(2.0, (double)(S*sizeof(unsigned_type)*CHAR_BIT - 1))));

   return decimalPrecision_;
  }


//----------------------------------------------------------
//
// preciseAdd
//
// This function carries out addition in full precision.  If
// the answer is too big to fit in an RWMultiPrecisionInt, The
// last digit(s) is truncated to make it fit.  The function returns
// the number of digits truncated from the result.
//
//---------------------------------------------------------------  
static unsigned preciseAdd(const RWMultiPrecisionInt<S> &x, const RWMultiPrecisionInt<S> &y, RWMultiPrecisionInt<S> &sum)
#if defined(HP_ANSI)
{
 int  carry = RWDigitAdd<unsigned_type, S - 2>::add(x.trailingDigits_, y.trailingDigits_, sum.trailingDigits_);


 sum.leadDigit_ = x.leadDigit_ + y.leadDigit_ + carry;

 // We have an overflow condition if both summands have the same sign and the sum 
 // has a different sign, OR if the sum is equal to "badValue()" (checked by the isGood()
 // member function).
 if((((x.leadDigit_ & Constants::HighDigitBit) == (y.leadDigit_   & Constants::HighDigitBit)) &&
     ((x.leadDigit_ & Constants::HighDigitBit) != (sum.leadDigit_ & Constants::HighDigitBit))) ||
      !sum.isGood())
  {
   // Overflow condition. Remember that we currently have S words of precision (one leading
   // digit of type signed_type and S-1 digits of type unsigned_type). 
   // Redo the addition with S+1 words of precision, truncate one decimal
   // digit from the result, and then put the result back into S words of precision.
   // Unfortunately, the routine that truncates a decimal digit (decimalShiftRight) only
   // works on nonnegative values.  So we must switch to positive values (note that
   // we are in this block of code only if both summands have the same sign)
   RWMultiPrecisionInt<S>  xPos, yPos;
   RWBoolean               neg;


   if((x.leadDigit_ & Constants::HighDigitBit) == 1)
    {
     xPos = -x;
     yPos = -y;
     neg  = TRUE;
    }
   else
    {
     xPos = x;
     yPos = y;
     neg  = FALSE;
    }

   // Copy the original summands into the higher precision arrays and do the addition.
   unsigned_type  trailingDigitsX[S], trailingDigitsY[S], trailingDigitsSum[S];
   signed_type    leadDigitSum;

   
   RWArrayCopy<unsigned_type, S - 2>::copy(xPos.trailingDigits_, &trailingDigitsX[1]);
   trailingDigitsX[0] = (unsigned_type)xPos.leadDigit_;

   RWArrayCopy<unsigned_type, S - 2>::copy(yPos.trailingDigits_, &trailingDigitsY[1]);
   trailingDigitsY[0] = (unsigned_type)yPos.leadDigit_;

   carry = RWDigitAdd<unsigned_type, S - 1>::add(trailingDigitsX, trailingDigitsY, trailingDigitsSum);
   leadDigitSum = carry;

   // The decimal shift function needs things in halfword arrays. So do the split.  The
   // leading digit goes into the the first 2 elements of the halfword array and the trailing
   // digits go int the rest.
   unsigned_type  halfwords[2*S + 2];


   RWHalfwordSplit<unsigned_type, S - 1>::split(trailingDigitsSum, &halfwords[2]);
   RWHalfwordSplit<unsigned_type, 0>::split((unsigned_type *)&leadDigitSum, halfwords);

    // Now do the shift
   ::decimalShiftRight(halfwords, (unsigned int)2*S + 2, (unsigned int)1);

   // Merge the result back into the sum
   RWHalfwordMerge<unsigned_type, S - 2>::merge(&halfwords[4], sum.trailingDigits_);
   RWHalfwordMerge<unsigned_type, 0>::merge(&halfwords[2], (unsigned_type *)&sum.leadDigit_);

   if(neg)
     sum = -sum;

   return 1;
  }
 else
  {
   return 0;
  }
}
#else
  ;
#endif
    

static RWBoolean longDivide(const RWMultiPrecisionInt<S> &dividend, const RWMultiPrecisionInt<S> &divisor,
                            RWMultiPrecisionInt<S> &quot, RWMultiPrecisionInt<S> &rem)
{
 RWBoolean               result_sign;
 RWMultiPrecisionInt<S>  divisor_pos(divisor), dividend_pos(dividend);


 result_sign = !((divisor.leadDigit_ < 0) ^ (dividend.leadDigit_ < 0));

// If divisor or *this negative, convert to positive.
 if(divisor.leadDigit_ < 0)
   divisor_pos = -divisor;

 if(dividend.leadDigit_ < 0)
   dividend_pos = -dividend;

 RWBoolean  success = longDividePos(dividend_pos, divisor_pos, quot, rem);


 if(result_sign == 0)
   quot = -quot;

// Remainder has same sign as the dividend.
 if(dividend.leadDigit_ < 0)
   rem = -rem;

 return success;
}

//-----------------------------------------------------------------------------
// Algorithm is from Knuth Vol.2: the "Classical Algorithm" for dividing
// two nonnegative integers to get a quotient and remainder.  The algorithm
// must take place over halfwords to guarantee no overflow in intermediate
// calculations.
// Algorithm returns TRUE on success; if unsuccessful, values for *quot* and
// *rem* are set to badValue().
//-------------------------------------------------------------------------------
static RWBoolean longDividePos(const RWMultiPrecisionInt<S> &dividend, const RWMultiPrecisionInt<S> &divisor,
                               RWMultiPrecisionInt<S> &quot, RWMultiPrecisionInt<S> &rem)
#if defined(HP_ANSI)    
{
 // Check for division by zero or bad operands.
 if((divisor == RWMultiPrecisionInt<S>(0)) || !dividend.isGood() || !divisor.isGood())
  {
   quot = badValue();
   rem  = badValue();

   return FALSE;
  }

 // Check if divisor exceeds dividend.
 if(divisor > dividend)
  {
   quot = 0;
   rem  = dividend;

   return TRUE;
  }

 // Split divisor and dividend into halfwords; initialize quotient.
 // Set extra, most significant digits for the dividend and divisor
 // equal to zero.
 unsigned_type  hw_dividend[2*S + 1], hw_divisor[2*S + 1], hw_quot[2*S];


 hw_dividend[0] = 0;
 RWHalfwordSplit<unsigned_type, 0>::split((unsigned_type *)&(dividend.leadDigit_), &(hw_dividend[1]));
 RWHalfwordSplit<unsigned_type, S - 2>::split(dividend.trailingDigits_, &(hw_dividend[3]));

 hw_divisor[0] = 0;
 RWHalfwordSplit<unsigned_type, 0>::split((unsigned_type *)&(divisor.leadDigit_), &(hw_divisor[1]));
 RWHalfwordSplit<unsigned_type, S - 2>::split(divisor.trailingDigits_, &(hw_divisor[3]));

 RWHalfwordInit<unsigned_type, 2*S - 1>::init(hw_quot, 0);


// Find the first nonzero halfword digits of divisor and dividend; 
// the difference equals the number of halfword digits in the quotient.
 unsigned  int i = 0, j = 0;


 while(hw_divisor[j] == 0)
      j++;

 while(hw_dividend[i] == 0)
      i++;

 // Perform fast division if the divisor fits into a single halfword.
 // Start performing division where the dividend is nonzero (value *i*).
 if(j == 2*S)
  {
   unsigned_type  fast_remainder = 0;


   for(; i <= 2*S; i++)
     {
      fast_remainder = (fast_remainder << RWIntutilConstants<unsigned_type>::HWDigitSize) + hw_dividend[i];
      hw_quot[i - 1] = fast_remainder/hw_divisor[j];
      fast_remainder %= hw_divisor[j];
     }

   RWHalfwordMerge<unsigned_type, S - 2>::merge(&(hw_quot[2]), quot.trailingDigits_);

   quot.leadDigit_ = (hw_quot[0] << RWIntutilConstants<unsigned_type>::HWDigitSize) + hw_quot[1];

   rem = fast_remainder;

   return TRUE;
  }  

 // Multiply the divisor and dividend; choose a multiplier such that
 // the divisor's most significant digit has a nonzero leftmost bit.
 unsigned_type  multiplier = 0, divisor_digit = hw_divisor[j], quot_digit;
 unsigned_type  one = 1;


 while(!(divisor_digit & (one << (RWIntutilConstants<unsigned_type>::HWDigitSize - one))))
     {
      divisor_digit <<= 1;
      multiplier++;
     }

 RWHalfwordShiftLeft<unsigned_type, 2*S>::left(multiplier, hw_divisor);
 RWHalfwordShiftLeft<unsigned_type, 2*S>::left(multiplier, hw_dividend);

 // Make the divisor's number of digits match the dividend's number of 
 // digits by left shifting the divisor digits.  In the division loop
 // below, the value of the divisor will "shift right" to equal the size 
 // of the dividend as the dividend gets smaller through subtraction.
 size_t  nquot_digits = j - i;


 {
  memmove(hw_divisor, &(hw_divisor[nquot_digits]), ((2*S + 1) - nquot_digits)*sizeof(unsigned_type));
  memset(&(hw_divisor[(2*S + 1) - nquot_digits]), 0, nquot_digits*sizeof(unsigned_type));
 }

 // Loop over quotient digits.
 size_t  q = 0;


 do  // while (q <= nquot_digits)
  {
   // Start with the leading dividend digit, which may have shifted left
   // by one.
   size_t  curr = i + q - 1;


   if(hw_dividend[curr] == hw_divisor[curr + 1])
     quot_digit = RWIntutilConstants<unsigned_type>::LowerMask;
   else
     quot_digit = ((hw_dividend[curr] << RWIntutilConstants<unsigned_type>::HWDigitSize) + hw_dividend[curr + 1])/hw_divisor[curr + 1];
    
   // Test if the quotient digit is too large; reduction should never
   // happen more than twice (proof in Knuth vol. 2).  The test for a
   // second decrement occurs in subsequent steps.  A preliminary test 
   // prevents exceeding array bounds.
   unsigned_type  dividend_curr_2;


   if((2*S - curr) >= 2)
     dividend_curr_2 = hw_dividend[curr + 2];
   else
     dividend_curr_2 = 0;

   // Straightforward evaluation of Knuth's test can result in overflow,
   // so it is done in stages. 
   unsigned_type  rhs_part;


   rhs_part = ((hw_dividend[curr] << RWIntutilConstants<unsigned_type>::HWDigitSize) +
              hw_dividend[curr + 1] - quot_digit*hw_divisor[curr + 1]);

   if ((quot_digit*hw_divisor[curr + 2] >
       ( rhs_part << RWIntutilConstants<unsigned_type>::HWDigitSize) + dividend_curr_2) &&
       ((rhs_part >> RWIntutilConstants<unsigned_type>::HWDigitSize) == 0))
    {
     quot_digit--;
    }

   // Compute how much to subtract from the dividend; shift amount left 
   // to match the current dividend digits being subtracted.  Any carry-out
   // from multiplication should be assigned to the most significant digit
   // of the subtractor.
   unsigned_type  subtractor[2*S + 1];

   RWHalfwordMultiply<unsigned_type, 2*S>::multiply(hw_divisor, quot_digit, subtractor);

   // Subtract from the dividend.
   RWBoolean  went_negative = RWHalfwordSubtract<unsigned_type, 2*S>::subtract(hw_dividend, subtractor, hw_dividend);


   // Add back an amount equal to the divisor if the result went negative,
   // and subtract one from the quotient digit.  Overflow should occur in
   // the addition, but this just cancels out the "borrow" from subtraction.
   if(went_negative)
    {
     RWHalfwordAdd<unsigned_type, 2*S>::add(hw_divisor, hw_dividend, hw_dividend);

     quot_digit--;
    }

   // Set quotient digit, shift divisor, and increment quotient digit counter.
   // I should eventually change divisor shift so that only nonzero values 
   // are shifted right.
   hw_quot[(2*S - 1) - nquot_digits + q] = quot_digit;

   memmove(&(hw_divisor[1]), hw_divisor, 2*S*sizeof(unsigned_type));

   hw_divisor[0] = 0;

   q++;
  } while(q <= nquot_digits);

 // Get the remainder by right shifting what's left in the dividend.
 RWHalfwordShiftRight<unsigned_type, 2*S>::right(multiplier, hw_dividend);

 // Merge halfwords back into words.
 RWHalfwordMerge<unsigned_type, S - 2>::merge(&(hw_dividend[3]), rem.trailingDigits_);
 rem.leadDigit_ = (hw_dividend[1] << RWIntutilConstants<unsigned_type>::HWDigitSize) + hw_dividend[2];

 RWHalfwordMerge<unsigned_type, S - 2>::merge(&(hw_quot[2]), quot.trailingDigits_);
 quot.leadDigit_ = (hw_quot[0] << RWIntutilConstants<unsigned_type>::HWDigitSize) + hw_quot[1];

 return(TRUE);
}
#else
  ;
#endif  

//--------------------------------------------------------------------------
// preciseMultiply(): perform multiplication to full precision, truncate
// least significant decimal digits in the result, and return the number 
// of truncated digits.
//
//Note: this multiply algorithm breaks down if there are too many
//intermediate adds, which eventually cause overflow: breakdown can
//occur when the number of words used to represent a MultiPrecisionInt
//exceeds the number of bits in a halfword.  In the case where words are
//of type "unsigned long", the limit on number of words is typically 16
//(since an unsigned long often comprises 32 bits).
//----------------------------------------------------------------------------
static unsigned int preciseMultiply(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs, RWMultiPrecisionInt<S> &result)
#if defined(HP_ANSI)    
{
 register int            i, j;
 register unsigned_type  temp;
 int                     lhs_hw_size, rhs_hw_size;   // number of nonzero halfwords in operands

 // Change the sign of both operands to positive.
 RWBoolean               result_sign;
 RWMultiPrecisionInt<S>  lhs_pos, rhs_pos;
 

 result_sign = !((lhs.leadDigit_ < 0) ^ (rhs.leadDigit_ < 0));

 if(lhs.leadDigit_ < 0)
   lhs_pos = - lhs;
 else
   lhs_pos = lhs;

 if(rhs.leadDigit_ < 0)
   rhs_pos = - rhs;
 else
   rhs_pos = rhs;

 // Split the current digits into halfword digits to make enough
 // room for intermediate multiplications and additions.
 const unsigned int  opsize = 2*S;
 unsigned_type       hw_lhs[2*S], hw_rhs[2*S], hw_result[4*S];


 RWHalfwordSplit<unsigned_type, S - 2>::split(lhs_pos.trailingDigits_, &(hw_lhs[2]));
 RWHalfwordSplit<unsigned_type, 0>::split((unsigned_type *)&(lhs_pos.leadDigit_), hw_lhs);
 RWHalfwordSplitReverse<unsigned_type, S - 2>::split(rhs_pos.trailingDigits_, hw_rhs);
 RWHalfwordSplitReverse<unsigned_type, 0>::split((unsigned_type *)&(rhs_pos.leadDigit_), &(hw_rhs[opsize - 2]));

 // Reset the digits used for intermediate calculation and determine the
 // sizes of the operands (point to their first zero-values).
 RWHalfwordInit<unsigned_type, 4*S - 1>::init(hw_result, 0);

 lhs_hw_size = 0;

 while(lhs_hw_size < opsize && hw_lhs[lhs_hw_size] == 0)
      ++lhs_hw_size;

 rhs_hw_size = RWIndexOfLastNonZero<unsigned_type, 2*S - 1>::index(hw_rhs);
 
 // Exit if all halfword digits were zero in the above loop.
 if(unsigned_type(lhs_hw_size) == opsize || unsigned_type(rhs_hw_size) == RW_NPOS)
  {
   result = 0;

   return 0;
  }

 // Perform multiplication.
 int  indexLower, indexUpper;


 for(i = 0; i <= rhs_hw_size; i++)
   {
    // Central steps of multiply.
    for(j = opsize - 1; j >= lhs_hw_size; j--)
      {
       temp       = hw_lhs[j]*hw_rhs[i];
       indexLower = opsize + j - i;
       indexUpper = opsize + j - i - 1;

       hw_result[indexLower] += temp &  RWIntutilConstants<unsigned_type>::LowerMask;
       hw_result[indexUpper] += temp >> RWIntutilConstants<unsigned_type>::HWDigitSize;
      }
   }

 // Propagate carries in the result.
 RWHalfwordPropagate<unsigned_type, 4*S - 2>::propagate(hw_result);
 
 // Perform decimal shift right until the result fits into S digits.
 // The number of nonzero halfword digits in *hw_result* is
 // (opsize-lhs_hw_size+this_hw_size); reduce this number to 2*S. 

 // The number of full halfwords we need to shift.
 int  ndigshift = (int)rwmax((int)-1, (rhs_hw_size - lhs_hw_size - 1));

 // The number of additional bits we need to shift.
 int  nbitshift = 0;


 if((ndigshift != -1) && hw_result[(opsize - 1) - (rhs_hw_size - lhs_hw_size)] != 0)
  {
   // If the result does not fit into the answer (ndigshift != -1 ), and there are some
   // bits we need to shift (hw_result[(opsize-1)-(rhs_hw_size-lhs_hw_size)] != 0), then
   // count the number of bits we need to shift.
   //

   // Start out assuming we need to shift all the bits.
   //
   nbitshift = RWIntutilConstants<unsigned_type>::HWDigitSize;

   // Decrement this value until it is correct by shifting the halfword bitwise left
   // (in order to count the number of significant bits in it). As long as the
   // most significant bit in the halfword is 0, continue counting. The first 1
   // represents the start of where we need to shift.
   //
   for(long  hw_value(hw_result[(opsize - 1) - (rhs_hw_size - lhs_hw_size)]);
     ( hw_value = (hw_value << 1 )) == 0; --ndigshift);
  }

 if((hw_result[0] >> RWIntutilConstants<unsigned_type>::HWDigitSize) != 0)
   ndigshift++;

 // Convert the ndigshift and nbitshift values into a decimal equivalent. This is done
 // by converting ndigshift to bits (ndigshift*HWDigitSize), adding nbitshift, and
 // converting this to decimal digits by multiplying by log 2 (0.301029995664). We add
 // one to account for fractional results.
 //
 unsigned int  ndecshift    = (int)(ndigshift == -1 ? 0 : 0.301029995664*(ndigshift*RWIntutilConstants<unsigned_type>::HWDigitSize + nbitshift) + 1);
 unsigned int  numTruncated = ::decimalShiftRightToHalfSize(hw_result, (unsigned int)4*S, ndecshift);
   

 // copy shifted result into an MPInt of size S.
 result.leadDigit_ = (hw_result[2*S] << RWIntutilConstants<unsigned_type>::HWDigitSize) + 
                     (hw_result[2*S + 1] & RWIntutilConstants<unsigned_type>::LowerMask);

 RWHalfwordMerge<unsigned_type, S - 2>::merge(&(hw_result[2*S + 2]), result.trailingDigits_);

 // Set result sign.
 if(result_sign == 0)
   result = - result;
 
 return numTruncated;
}
#else
  ;
#endif  

// Assignment operators ----------------------------------------------
RWMultiPrecisionInt<S> & operator+=(const RWMultiPrecisionInt<S> &lhs)
{
 if(!isGood() || !lhs.isGood())
  {
   *this = badValue();
  }
 else
  {
   int          carry = RWDigitAdd<unsigned_type, S - 2>::add(trailingDigits_, lhs.trailingDigits_, trailingDigits_);
   signed_type  lead  = leadDigit_ + lhs.leadDigit_ + carry;


   if((leadDigit_ & Constants::HighDigitBit) == (lhs.leadDigit_ & Constants::HighDigitBit) &&
      (leadDigit_ & Constants::HighDigitBit) != (lead & Constants::HighDigitBit))
    {
     *this = badValue();
    }
   else
    {
     leadDigit_ = lead;
    }
  }

 return *this;
}


RWMultiPrecisionInt<S> & operator-=(const RWMultiPrecisionInt<S> &lhs)
{
 *this += -(lhs);
 
 return *this;
}


//----------------------------------------------------------------------
//Note: this multiply algorithm breaks down if there are too many
//intermediate adds, which eventually cause overflow: breakdown can
//occur when the number of words used to represent a MultiPrecisionInt
//exceeds the number of bits in a halfword.  In the case where words are
//of type "unsigned long", the limit on number of words is typically 16
//(since an unsigned long often comprises 32 bits).
//-----------------------------------------------------------------------  
RWMultiPrecisionInt<S> & operator*=(const RWMultiPrecisionInt<S> &rhs)
#if defined(HP_ANSI)    
{
 register unsigned int   i, j;
 register unsigned_type  temp;
 int                     rhs_hw_size, this_hw_size;   // number of nonzero halfwords in operands


 // Perform operation only if both operands are good.
 if(!isGood() || !rhs.isGood())
  {
   *this = badValue();

   return *this;
  }

 // Change the sign of both operands to positive.
 RWBoolean               result_sign;
 RWMultiPrecisionInt<S>  this_pos, rhs_pos;


 result_sign = !((leadDigit_ < 0) ^ (rhs.leadDigit_ < 0));

 if(leadDigit_ < 0)
   this_pos = - *this;
 else
   this_pos = *this;

 if(rhs.leadDigit_ < 0)
   rhs_pos = - rhs;
 else
   rhs_pos = rhs;

 // Split the current digits into halfword digits to make enough
 // room for intermediate multiplications and additions.
 const unsigned int  opsize = 2*S;
 unsigned_type       hw_op1[2*S], hw_op2[2*S], hw_result[2*S];


 RWHalfwordSplit<unsigned_type, S - 2>::split(this_pos.trailingDigits_, &(hw_op1[2]));
 RWHalfwordSplit<unsigned_type, 0>::split((unsigned_type *)&(this_pos.leadDigit_), hw_op1);

 RWHalfwordSplitReverse<unsigned_type, S - 2>::split(rhs_pos.trailingDigits_, hw_op2);
 RWHalfwordSplitReverse<unsigned_type, 0>::split((unsigned_type *)&(rhs_pos.leadDigit_), &(hw_op2[opsize - 2]));

 // Reset the digits used for intermediate calculation and determine the
 // sizes of the operands (find their first nonzero halfwords).
 RWHalfwordInit<unsigned_type, 2*S - 1>::init(hw_result, 0);

 this_hw_size = 0;

 while(unsigned(this_hw_size) < opsize && hw_op1[this_hw_size] == 0)
      this_hw_size++;

 rhs_hw_size = RWIndexOfLastNonZero<unsigned_type, 2*S - 1>::index(hw_op2);
 
 // Exit if all halfword digits were zero in the above loop.
 if(unsigned_type(this_hw_size) == RW_NPOS || unsigned_type(rhs_hw_size) == RW_NPOS)
  {
   *this = 0;

   return *this;
  }

 // Check for possible overflow caused by too many nonzero digits.
 // "Too many" is defined as more than 2*S+1 digits combined from both
 // operands.
 if(rhs_hw_size > this_hw_size)
  {
   *this = badValue();

   return(*this);
  }

 // Perform multiplication.
 unsigned int  termination;
 int           upperIndex, lowerIndex;


 for(i = 0; i <= (unsigned int)rhs_hw_size; i++)
   {
    // Central steps of multiply; looping to *termination* prevents 
    // negative indexing into result, plus it avoids unnecessary 
    // multiplies by zero-valued digits in *halfword_digits_*.
    termination = (this_hw_size - 1 > (int)i ? this_hw_size - 1 : i);

    for(j = opsize - 1; j>termination; j--)
      {
       temp       = hw_op2[i]*hw_op1[j];
       upperIndex = j - i;
       lowerIndex = j - i - 1;

       hw_result[upperIndex] += temp &  RWIntutilConstants<unsigned_type>::LowerMask;
       hw_result[lowerIndex] += temp >> RWIntutilConstants<unsigned_type>::HWDigitSize;
      }

    if(termination == i)
      hw_result[0] += hw_op2[i]*hw_op1[i];
   }

 // Propagate carries in the result.
 RWHalfwordPropagate<unsigned_type, 2*S - 2>::propagate(hw_result);
 
 // Test for overflow; test one extra bit because this bit must stay 
 // zero or the result becomes negative.
 if(hw_result[0] >> (RWIntutilConstants<unsigned_type>::HWDigitSize - 1))
  {
   *this = badValue();

   return(*this);
  }

 // Put digits back together again. 
 RWHalfwordMerge<unsigned_type, S - 2>::merge(&(hw_result[2]), trailingDigits_);

 leadDigit_ = (hw_result[0] << RWIntutilConstants<unsigned_type>::HWDigitSize) + 
              (hw_result[1] & RWIntutilConstants<unsigned_type>::LowerMask);

 // Set result sign.
 if(result_sign == 0)
   *this = - *this;

 return(*this);
}
#else
  ;
#endif  

RWMultiPrecisionInt<S> & operator/=(const RWMultiPrecisionInt<S> &lhs)
{
 MPInt_S  quot, rem;


 longDivide(*this, lhs, quot, rem);

 *this = quot;

 return *this;
}


// Binary operators -----------------------------------------------------------------------------
// if the compiler allows no type template functions these global operators are defined outside
// the class.
#ifndef RW_NO_NONTYPE_TEMPLATE_FUNCTIONS
  friend RWMultiPrecisionInt<S> operator- FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &rhs, const RWMultiPrecisionInt<S> &lhs);
  friend RWMultiPrecisionInt<S> operator+ FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);
  friend RWMultiPrecisionInt<S> operator* FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);
  friend RWMultiPrecisionInt<S> operator/ FRIEND_TEMPLATE_SPECIFIER(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs);

  friend RW_SL_IO_STD(ostream) & operator << FRIEND_TEMPLATE_SPECIFIER(RW_SL_IO_STD(ostream) &strm, const RWMultiPrecisionInt<S> &n);
#else
  friend RWMultiPrecisionInt<S> operator-(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   MPInt_S  diff(lhs);    

   return diff -= rhs;
  }

  friend RWMultiPrecisionInt<S> operator+(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   MPInt_S  sum(lhs);

   return sum += rhs;
  }

  friend RWMultiPrecisionInt<S> operator*(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   MPInt_S  prod(lhs);

   return prod *= rhs;
  }

  friend RWMultiPrecisionInt<S> operator/(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
  {
   MPInt_S  quotient(lhs);

   return quotient /= rhs;
  }

  // Shifting in and out of streams
  friend RW_SL_IO_STD(ostream) & operator <<( RW_SL_IO_STD(ostream) &strm, const RWMultiPrecisionInt<S> &n)
  {
   RWCString  s = n.asString();
   
   
   strm << s;

   return strm;
  }
#endif

// Public member functions ------------------------------------------------------------------
  double    asDouble() const
#ifdef RW_MONEY_NO_INLINE_ARRAYS
  ;
#else
  {
   RWMultiPrecisionInt<S>  tmp(*this);
   RWBoolean               neg;


   if(leadDigit_ < 0)
    {
     tmp = -(*this);
     neg = TRUE;
    }
   else
    {
     neg = FALSE;
    }
   
   unsigned_type  digits[S];


   digits[0] = tmp.leadDigit_;

   RWArrayCopy<unsigned_type, S - 2>::copy(tmp.trailingDigits_, &(digits[1]));
   
   double  value = RWIntToDouble<unsigned_type, S - 1>::todouble(digits);


   if(neg)
     value = -value;

   return value;
  }
#endif

  
  RWCString asString() const
#if defined(HP_ANSI)          
  {
   RWMultiPrecisionInt<S>  tmp(*this);
   RWCString               s;
   int                     i;
   RWBoolean               positive;


   if(!isGood())
    {
     s = "(bad value)";

     return s;
    }

   // Make a positive version of the number.
   if(leadDigit_ < 0)
    {
     tmp      = - *this;
     positive = FALSE;
    }
   else
    {
     positive = TRUE;
    }

   RWMultiPrecisionInt<S>  zero(0);


   if(tmp == zero)
    {
     s = "0";

     return s;
    }

   char  char_digit;


   while(tmp != zero)
       {
        // Compute the least significant decimal digit.  Algorithm takes
        // advantage of the relationship between moduli of powers of 2:
        // [ 2^(4t) * x ] % 10 = 6 * (x % 10) for any positive integers x, t.
        unsigned long  modctr = tmp.trailingDigits_[S - 2] % 10;


        for(i = S - 3; i >= 0; i--)
           modctr += (tmp.trailingDigits_[i] % 10) * 6;

        modctr += (tmp.leadDigit_ % 10)*6;
        char_digit = '0' + (char)(modctr % 10);
        s.prepend(char_digit);
        tmp = tmp.decimalShiftRight();
        
        if(!tmp.isGood())
         {
          RW_SL_IO_STD(cerr) << "asString(): Fatal: Bad decimal shift right!" << RW_SL_IO_STD(endl);

          exit(-1);
         }
       }
 
   if(!positive)
     s.prepend('-');

   return s;
  } 
#else
  ;
#endif

  RWMultiPrecisionInt<S> decimalShiftLeft (unsigned int shiftBy = 1) const;
  RWMultiPrecisionInt<S> decimalShiftRight(unsigned int shiftBy = 1) const;

  RWBoolean isGood() const
  {
   RWBoolean  result;


   if((unsigned_type)leadDigit_ != Constants::HighDigitBit)
    {
     result = TRUE;
    }
   else 
    {
     if(RWIndexOfLastNonZero<unsigned_type, S - 2>::index(trailingDigits_) == RW_NPOS)
       result = FALSE;
     else
       result = TRUE;
    }

   return result;
  }

  RWBoolean isEven() const
  {
   return !(trailingDigits_[S - 2] & Constants::LowDigitBit);
  }

#ifndef RW_NO_TOOLS  
  void saveOn(RWvostream &) const;
  void saveOn(RWFile &)     const;
  void restoreFrom(RWvistream &);
  void restoreFrom(RWFile &f);
#endif //RW_NO_TOOLS

private:
  signed_type    leadDigit_;
  unsigned_type  trailingDigits_[S - 1];

  RWMultiPrecisionInt(signed_type ld, unsigned_type *td) : leadDigit_(ld)
  {
   RWArrayCopy<unsigned_type, S - 2>::copy(td, trailingDigits_);
  }


  inline static const RWMultiPrecisionInt<S> &badValue()
  {
   // Sunpro multiply defines badValue_ in its template database files.  So for
   // this compiler only badValue_ is a static class variable.
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
   static RWMultiPrecisionInt<S>  badValue_;
#endif

   if(badValue_.leadDigit_ == 0)
    {
     badValue_.leadDigit_ = (signed_type)Constants::HighDigitBit;

     RWHalfwordInit<unsigned_type, S - 2>::init(badValue_.trailingDigits_, (unsigned_type)0);
    }

   return badValue_;
  }

#if RW_NO_STATIC_VAR_IN_STATIC_MEMBER
  static       RWMultiPrecisionInt<S> badValue_;
  static       RWMultiPrecisionInt<S> maxValue_;
  static       RWMultiPrecisionInt<S> minValue_;
#endif
  static       int                    decimalPrecision_;
};

// End class RWMultiPrecisionInt ---------------------------------------------------------------------------


#ifndef RW_NO_NONTYPE_TEMPLATE_FUNCTIONS
// Arithemetic operators ----------------------------------------------------------------------
template <int S>
inline RWMultiPrecisionInt<S> operator-(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 RWMultiPrecisionInt<S>  diff(lhs);    

 return diff -= rhs;
}

template <int S>
inline RWMultiPrecisionInt<S> operator+(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 RWMultiPrecisionInt<S>  sum(lhs);    

 return sum += rhs;
}

template <int S>
inline RWMultiPrecisionInt<S> operator*(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 RWMultiPrecisionInt<S>  prod(lhs);

 return prod *= rhs;
}

template <int S>
inline RWMultiPrecisionInt<S> operator/(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 RWMultiPrecisionInt<S>  quotient(lhs);

 return quotient /= rhs;
}

// Shifting in and out of streams
template <int S>
inline RW_SL_IO_STD(ostream) & operator <<(RW_SL_IO_STD(ostream) &strm, const RWMultiPrecisionInt<S> &n)
{
 RWCString  s = n.asString();


 strm << s;

 return strm;
}

// // Comparison operators ---------------------------------------------------------------------------
template <int S>
inline RWBoolean operator ==(const RWMultiPrecisionInt<S> &rhs, const RWMultiPrecisionInt<S> &lhs)
{
 RWBoolean  result;


 if(lhs.leadDigit_ != rhs.leadDigit_)
   result = FALSE;
 else
   result = RWMPIntEqual<unsigned long, S - 2>::equal(lhs.trailingDigits_, rhs.trailingDigits_);

 return result;
}

template <int S>
inline RWBoolean operator <(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 RWBoolean  result;


 if(lhs.leadDigit_ != rhs.leadDigit_)
  {
   result = (lhs.leadDigit_ < rhs.leadDigit_);
  }
 else
  {
#if defined(RW_NO_TYPENAME_OUTSIDE_DECLARATION) || (defined(__SUNPRO_CC) && (__SUNPRO_CC < 0x530))
   size_t  index = RWIndexOfFirstDiff<RWMultiPrecisionInt<S>::unsigned_type, S - 2>::index(lhs.trailingDigits_, rhs.trailingDigits_);
#else
   size_t  index = RWIndexOfFirstDiff<typename RWMultiPrecisionInt<S>::unsigned_type, S - 2>::index(lhs.trailingDigits_, rhs.trailingDigits_);
#endif

   result = (index == RW_NPOS ? FALSE : (lhs.trailingDigits_[index] < rhs.trailingDigits_[index]));
  }

 return result;
}  

#if defined(RW_NO_STL) || (!defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE))
template <int S>
inline RWBoolean operator >(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 return (rhs < lhs);
}

template <int S>
inline RWBoolean operator !=(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 return !(lhs == rhs);
}

template <int S>
inline RWBoolean operator >=(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 return !(lhs < rhs);
}

template <int S>
inline RWBoolean operator <=(const RWMultiPrecisionInt<S> &lhs, const RWMultiPrecisionInt<S> &rhs)
{
 return !(lhs > rhs);
}
#endif  

#endif //RW_NO_NONTYPE_TEMPLATE_FUNCTIONS

#ifdef RW_MONEY_NO_INLINE_ARRAYS
template <int S>
RWMultiPrecisionInt<S> RWMultiPrecisionInt<S>::operator-() const
{
 unsigned_type  trailing[(S <= 1) ? 1 : S - 1];
 int            carry = RWDigitNegate<unsigned_type, S - 2>::negate(trailingDigits_, trailing);
 signed_type    lead  = (Constants::MaxDigitInt - leadDigit_) + carry;

 return RWMultiPrecisionInt<S>(lead, trailing);
}
#endif

#ifdef RW_MONEY_NO_INLINE_ARRAYS
template <int S>
double RWMultiPrecisionInt<S>::asDouble() const
{
 RWMultiPrecisionInt<S>  tmp(*this);
 RWBoolean               neg;


 if(leadDigit_ < 0)
  {
   tmp = -(*this);
   neg = TRUE;
  }
 else
  {
   neg = FALSE;
  }
 
 unsigned_type  digits[S];


 digits[0] = tmp.leadDigit_;

 RWArrayCopy<unsigned_type, S - 2>::copy(tmp.trailingDigits_, &(digits[1]));
   
 double  value = RWIntToDouble<unsigned_type, S - 1>::todouble(digits);

 
 if(neg)
   value = -value;

 return value;
}
#endif

#include "rw/money/mppot.h"

#ifdef RW_MONEY_COMPILE_INSTANTIATE
    #include "rw/money/mpint.cc"
#endif

#endif //__RWMPINT_H__
