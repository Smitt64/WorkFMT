/*--------------------------------------------------------------
 *
 * Definition of RWMultiPrecisionInt where the underlying
 * integer representation is based on a double precision variable
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

#ifndef __RWMP1INT_H__
#define __RWMP1INT_H__

#include "rw/dcmldefs.h"
#include "rw/decbase.h"
#include "rw/strstrea.h"
#include "rw/cstring.h"
#include "rw/decio.h"

#ifndef RW_NO_TOOLS
#include "rw/rwfile.h"
#include "rw/vstream.h"
#endif

#if defined(RW_MNY_NO_NEW_HEADER)
#  include <math.h>
#  include <float.h>
#else
#  include <cmath>
#  include <cfloat>
#endif

#if (defined(__hppa) && !defined(HP_ANSI)) || (defined(__HP_aCC) && defined(HP_ANSI)) || ( defined(__TURBOC__) && (__TURBOC__ >= 0x530) )
#include "rw/money/mpint.h"
#else
template< int N >
class RWMultiPrecisionInt;
#endif

RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWDCMLTExport RWMultiPrecisionInt<1>
{
public:
  // Constructors ---------------------------------------
  RWMultiPrecisionInt():data_(0) {;}
  RWMultiPrecisionInt( int num ):data_(num) {;}
  RWMultiPrecisionInt( long num ):data_(num) {;}
  RWMultiPrecisionInt( double data ):data_(data) {;}
  RWMultiPrecisionInt( long double data ):data_(double(data)) {;}
  RWMultiPrecisionInt(const RWMultiPrecisionInt<1>& x):data_(x.data_) {;}

  static unsigned int decimalPrecision()
  { return DBL_DIG; }                 // DBL_DIG is defined in <float.h>
  
  // Unary arithmetic operators
  RWMultiPrecisionInt<1> operator-() const
  {
    return RWMultiPrecisionInt<1>( -data_ );
  }

    // Public member functions ------------------------------------------------------------------
  RWBoolean isGood()   const { return( data_ != badValue() ); }
  double    asDouble() const { return data_; }

  // Shifting out  ------------------------------------------------------------
  // Gotta be defined inline since most compilers do not support non-type
  // template parameters for function templates.
  friend RW_SL_IO_STD(ostream)& operator <<( RW_SL_IO_STD(ostream)& strm, const RWMultiPrecisionInt<1>& n )
  {
    if ( !n.isGood() )
    {
      strm << "(bad value)";
    }
    else
    {
#ifndef __hppa      
      // Set flags so that it looks like an int (no scientific notation
      // and no digits to the right of the decimal point).
# if (((defined(__GNUG__) && (__GNUG__ >= 3)) || (defined(__INTEL_COMPILER) && !defined(_WIN32))) && (RWSTDLIBTYPE != 1))
      RW_SL_IO_STD(_Ios_Fmtflags) oldFlags = strm.setf( RW_SL_IO_STD(ios)::fixed, RW_SL_IO_STD(ios)::floatfield );
#else
      long oldFlags = strm.setf( RW_SL_IO_STD(ios)::fixed, RW_SL_IO_STD(ios)::floatfield );
#endif
      int oldPrecision = (int)strm.precision( 0 );
      strm << n.data_;
      strm.setf( oldFlags, RW_SL_IO_STD(ios)::floatfield );
      strm.precision( oldPrecision );
#else
      // We have seen rounding errors on the last digit when outputing
      // a double on a stream using precision 0.  We get around this by
      // using precision 1 and truncating the trailing ".0".
      RW_SL_IO_STD(ostrstream) stringStrm;
      stringStrm.setf( RW_SL_IO_STD(ios)::fixed, RW_SL_IO_STD(ios)::floatfield );
      stringStrm.precision( 1 );
      stringStrm << n.data_ << RW_SL_IO_STD(ends);
      char* sptr = stringStrm.str();           // pointer to the string
      char* decPtr = sptr + strlen(sptr)  - 2; // pointer to the decimal pt.
      if (*decPtr == '.') // should always be true
      {
         *decPtr = '\0';  // terminate string here
      }
      strm << sptr;
      //delete sptr;  // string returned by .str() is the user's responsibility
      stringStrm.rdbuf()->freeze(false);
#endif      
    }
    
    return strm;
  }

  // Assignment operators ----------------------------------------------
  RWMultiPrecisionInt<1>& operator+=( const RWMultiPrecisionInt<1>& lhs );
  RWMultiPrecisionInt<1>& operator-=( const RWMultiPrecisionInt<1>& lhs );
  RWMultiPrecisionInt<1>& operator*=( const RWMultiPrecisionInt<1>& lhs );
  RWMultiPrecisionInt<1>& operator/=( const RWMultiPrecisionInt<1>& lhs );
    

  // Binary operators -----------------------------------------------------------------------------
  //
  // Must define inline here since most (all?) compilers don't support non-type function templates
  //
  inline friend RWMultiPrecisionInt<1> operator-( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    RWMultiPrecisionInt<1> result;
    if ( !lhs.isGood() || !rhs.isGood() )
    {
      result = RWMultiPrecisionInt<1>( badValue() );
    }
    else
    {
      double data = lhs.data_ - rhs.data_;
      if ( data > maxDouble() || data < -maxDouble() )
      {
        result = RWMultiPrecisionInt<1>( badValue() );
      }
      else
      {
        result = RWMultiPrecisionInt<1>( data );
      }
    }
    return result;
  }

  inline friend RWMultiPrecisionInt<1> operator+( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    RWMultiPrecisionInt<1> result;
    if ( !lhs.isGood() || !rhs.isGood() )
    {
      result = RWMultiPrecisionInt<1>( badValue() );
    }
    else
    {
      double data = lhs.data_ + rhs.data_;
      if ( data > maxDouble() || data < -maxDouble() )
      {
        result = RWMultiPrecisionInt<1>( badValue() );
      }
      else
      {
        result = RWMultiPrecisionInt<1>( data );
      }
    }
    return result;
  }

  inline friend RWMultiPrecisionInt<1> operator*( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    RWMultiPrecisionInt<1> result;
    if ( !lhs.isGood() || !rhs.isGood() )
    {
      result = RWMultiPrecisionInt<1>( badValue() );
    }
    else
    {
      double data = lhs.data_ * rhs.data_;
      if ( data > maxDouble() || data < -maxDouble() )
      {
        result = RWMultiPrecisionInt<1>( badValue() );
      }
      else
      {
        result = RWMultiPrecisionInt<1>( data );
      }
    }
    return result;
  }

  inline friend RWMultiPrecisionInt<1> operator/( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    RWMultiPrecisionInt<1> result;
    if ( !lhs.isGood() || !rhs.isGood() )
    {
      result = RWMultiPrecisionInt<1>( badValue() );
    }
    else
    {
      result = RWMultiPrecisionInt<1>( RWMNY_STD::floor( lhs.data_ / rhs.data_ ) );
    }
    return result;
  }


  // Comparison operators ---------------------------------------------------------------------------
  friend RWBoolean operator ==( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ == rhs.data_ );
  }

  friend RWBoolean operator <(  const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ < rhs.data_ );
  }

  // If using the standard library without namespaces, we will pick up the rest of the relational
  // operator from <utility>
#if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )  
  friend RWBoolean operator >(  const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ > rhs.data_ );
  }

  friend RWBoolean operator !=( const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ != rhs.data_ );
  }

  friend RWBoolean operator >=(  const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ >= rhs.data_ );
  }

  friend RWBoolean operator <=(  const RWMultiPrecisionInt<1>& lhs, const RWMultiPrecisionInt<1>& rhs )
  {
    return( lhs.data_ <= rhs.data_ );
  }
#endif
  
  // Global long division ----------------------------------------------
  static RWBoolean longDividePos(const RWMultiPrecisionInt<1>& dividend, const RWMultiPrecisionInt<1>& divisor,
                                                          RWMultiPrecisionInt<1>& quot, RWMultiPrecisionInt<1>& rem )
  {
    return longDivide( dividend, divisor, quot, rem );
  }

  static RWBoolean longDivide(const RWMultiPrecisionInt<1>& dividend, const RWMultiPrecisionInt<1>& divisor,
               RWMultiPrecisionInt<1>& quot, RWMultiPrecisionInt<1>& rem )
  {
    RWBoolean result;
    if ( divisor.data_ == 0.0 )
    {
      quot.data_ = badValue();
      rem.data_ = badValue();
      result = FALSE;
    }
    else
    {
    quot.data_ = RWMNY_STD::floor( dividend.data_ / divisor.data_ );
    rem.data_ = dividend.data_ - divisor.data_*quot.data_;
    result = TRUE;
    }
  return result;
  }

  // Public member functions ------------------------------------------------------------------  
  RWMultiPrecisionInt<1> decimalShiftLeft(unsigned int shiftBy = 1 ) const;

  RWMultiPrecisionInt<1> decimalShiftRight(unsigned int shiftBy = 1 ) const
  {
    double divisor = RWMNY_STD::pow( 10.0, (double)shiftBy );
    double data = data_ / divisor;
    if ( RWMNY_STD::fabs(data) < 1.0 )
    {
      data = 0.0;
    }
    return RWMultiPrecisionInt<1>( RWMNY_STD::floor(data) );
  }
  
  RWBoolean isEven() { return (data_/2.0 == RWMNY_STD::floor(data_/2.0)) ? TRUE : FALSE; }

#ifndef RW_NO_TOOLS
  void saveOn( RWFile& f ) const { f.Write( data_ ); }
  void restoreFrom( RWFile& f ) { f.Read( data_ ); }
  void saveOn( RWvostream& strm ) const { strm << data_; }
  void restoreFrom( RWvistream& strm ) { strm >> data_; }
#endif //RW_NO_TOOLS

  static RWMultiPrecisionInt<1> maxValue() { return maxDouble(); }

  static RWMultiPrecisionInt<1> minValue() { return -maxDouble(); }

  static unsigned preciseAdd( const RWMultiPrecisionInt<1>& x, const RWMultiPrecisionInt<1>& y,
               RWMultiPrecisionInt<1>& sum )
  {
    unsigned result;
    sum.data_ = x.data_ + y.data_;
    if ( sum.data_ > maxDouble() || sum.data_ < -maxDouble() )
    {
      sum.data_ /= 10.0;
      result = 1;
    }
    else
    {
      result = 0;
    }
    return result;
  }

  static unsigned preciseMultiply( const RWMultiPrecisionInt<1>& x, const RWMultiPrecisionInt<1>& y,
                    RWMultiPrecisionInt<1>& product );

  RWCString asString() const;

private:
  double data_;
  
  static double badValue() { return maxDouble() + 1.0; }
  static double maxDouble() { return 9007199254740991.0; }
    
};


//*****************************************************************************
//
// Inline Assignment Operators
//
//*****************************************************************************
inline RWMultiPrecisionInt<1>& RWMultiPrecisionInt<1>::operator+=( const RWMultiPrecisionInt<1>& lhs )
{
  if ( !isGood() || !lhs.isGood() )
  {
    data_ = badValue();
  }
  else
  {
    data_ += lhs.data_;
    if ( data_ > maxDouble() || data_ < -maxDouble() )
    {
      data_ = badValue();
    }
  }
  return *this;
}


inline RWMultiPrecisionInt<1>& RWMultiPrecisionInt<1>::operator-=( const RWMultiPrecisionInt<1>& lhs )
{
  if ( !isGood() || !lhs.isGood() )
  {
    data_ = badValue();
  }
  else
  {
    data_ -= lhs.data_;
    if ( data_ > maxDouble() || data_ < -maxDouble() )
    {
      data_ = badValue();
    }
  }
  return *this;
}


inline RWMultiPrecisionInt<1>& RWMultiPrecisionInt<1>::operator*=( const RWMultiPrecisionInt<1>& lhs )
{
  if ( !isGood() || !lhs.isGood() )
  {
    data_ = badValue();
  }
  else
  {
    data_ *= lhs.data_;
    if ( data_ > maxDouble() || data_ < -maxDouble() )
    {
      data_ = badValue();
    }
  }
  return *this;
}


inline RWMultiPrecisionInt<1>& RWMultiPrecisionInt<1>::operator/=( const RWMultiPrecisionInt<1>& lhs )
{
  if ( !isGood() || !lhs.isGood() )
  {
    data_ = badValue();
  }
  else
  {
    data_ = RWMNY_STD::floor( data_/lhs.data_ );
  }
  return *this;
}


typedef RWMultiPrecisionInt<1> RWMP1Int;


#if !defined RW_MONEY_SPECIALIZATION_IN_LIBRARY
#  include <rw/money/mp1int.cc>
#endif

#endif //__RWMP1INT_H__
