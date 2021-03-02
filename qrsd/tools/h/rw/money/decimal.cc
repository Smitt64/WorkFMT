/*--------------------------------------------------------------
 *
 * Implementation of RWDecimal and related classes.
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


#ifndef __RWDECIMAL_CC__
#define __RWDECIMAL_CC__


#include <stdlib.h>
#include "rw/decin.h"
#include "rw/message.h"
#include "rw/strstrea.h"
#ifndef RW_NO_TOOLS
#include "rw/vstream.h"
#include "rw/rwfile.h"
#endif
#include <ctype.h>


static const double MaxDouble = 9007199254740991.0;

#define RW_LARGEST_LDOUBLE 9007199254740991.0

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
// Initialization of static class data ---------------------------------------------------
template< class M >
const RWDecimal<M> RWDecimal<M>::null = RWDecimal<M>(RWDecimalBase::nullstate);

template< class M >
const RWDecimal<M> RWDecimal<M>::missing = RWDecimal<M>( RWDecimalBase::missingstate );

template< class M >
const RWDecimal<M> RWDecimal<M>::NaN = RWDecimal<M>( RWDecimalBase::NaNstate );
#endif

template< class M >
RWDecimal<M> RWDecimal<M>::nullValue()
{
  static const RWDecimal<M> n = RWDecimal<M>(RWDecimalBase::nullstate);
  return n;
}

template< class M >
RWDecimal<M> RWDecimal<M>::missingValue()
{
  static const RWDecimal<M> n = RWDecimal<M>(RWDecimalBase::missingstate);
  return n;
}

template< class M >
RWDecimal<M> RWDecimal<M>::NaNValue()
{
  static const RWDecimal<M> n = RWDecimal<M>(RWDecimalBase::NaNstate);
  return n;
}

template< class M >
#if defined(__hppa)
const char RWDecimal<M>::classname[] = "RWDecimal";
#else     
const char* RWDecimal<M>::classname
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#  else
= "RWDecimal";
#  endif
#endif


// Beginning with version 1.15 HP's aCC compiler cannot parse the following
// initializers of static data.  The somewhat ugly workaround is to initialize
// them for each of the template types M supported by the Currency Module. The
// initializations are done in source/currency/decimal.cpp.
#if !defined(RW_NO_STATIC_TEMPLATE_FUNCTION_PTR_INITIALIZER)
  template< class M >
  void (*RWDecimal<M>::inexactHandler_)(const RWDecimalInexactErr<M>&)
#  ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#  else
= RWDecimal<M>::defaultInexactHandler;
#  endif

  template< class M >
  void (*RWDecimal<M>::overflowHandler_)(const RWDecimalOverflowErr<M>&)
#  ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#  else
= RWDecimal<M>::defaultOverflowHandler;
#  endif
#endif


//------------------------------------------------------------------------
//
// Error handling functions
//------------------------------------------------------------------------

//***************************************************************************
//
// Default error handling routines
//
//***************************************************************************
template< class M >
void RWDecimal<M>::defaultInexactHandler( const RWDecimalInexactErr<M>& err )
{
  if(err.operation() != RWDecimalBase::div) // ignore division inexact errs
    RW_SL_IO_STD(cerr) << err.msg() << RW_SL_IO_STD(endl);
//    RWTHROW(err);  exceptions are for fatal errors
}

template< class M >
void RWDecimal<M>::defaultOverflowHandler( const RWDecimalOverflowErr<M>& err )
{
  RWTHROW(err);
}


//************************************************************************
//
// Short hand for calling error handling routines
//
//************************************************************************
template< class M >
void RWDecimal<M>::overflow( RWDecimalBase::Op op, const RWDecimal<M>& lhs, 
                             const RWDecimal<M>& rhs, const RWMessage& msg )
{
  (*overflowHandler_)( RWDecimalOverflowErr<M>(op, lhs, rhs, msg) );
}

template< class M >
void RWDecimal<M>::inexact( RWDecimalBase::Op op, const RWDecimal<M>& lhs, const RWDecimal<M>& rhs,
                            const RWMessage& msg )
{
  (*inexactHandler_)( RWDecimalInexactErr<M>(op, lhs, rhs, msg) );
}


// Constructors ---------------------------------------------------------

//***********************************************************************
//
// Construct from an RWDecimalPortable.
//
//***********************************************************************
template< class M >
RWDecimal<M>::RWDecimal( const RWDecimalPortable& decport )
{
  switch (decport.state()) 
  {
  case RWDecimalBase::nullstate:
    state_ = nullstate; return;
  case RWDecimalBase::NaNstate:
    state_ = NaNstate; return;
  case RWDecimalBase::missingstate:
    state_ = missingstate; return;
  case RWDecimalBase::normal:
    break;
  case RWDecimalBase::ctorError:
    break;
  }
  
  // Get the parts of the number before and after the decimal points
  RWCString before,after;
  before = decport;
  RWBoolean negative = (before.first('-')==0);
  if (negative) 
  {
    before = before(1,before.length()-1);
  }
  int decimalPointIndex = (int)before.index(".");
  if (decimalPointIndex>=0) 
  {
    after = before(decimalPointIndex+1,before.length()-decimalPointIndex-1);
    before = before(0,decimalPointIndex);
  }
  
  //Instead of not allowing mantissas that are too big, try creating it
  //and stop when overflow is detected. 
  state_ = normal;
  mantissa_ = 0;
  size_t place = 0;
  unsigned int i;
  M tmp, ten(10);
  // Process digits before the decimal point; too many implies overflow.
  for( i=0; i<before.length(); i++) 
  {
    mantissa_ *= ten;
    mantissa_ += (int)(before(i) - '0');
    place++;
  }
  
  if (!mantissa_.isGood())
  {
    // Signal an overflow condition.
    RWDecimal<M>::overflow(RWDecimalBase::conversion, RWDecimal<M>(RWDecimalBase::nullstate), RWDecimal<M>(RWDecimalBase::nullstate),
                           RWMessage(RWDCML_FROMPORTOVER, classname, (const char*)(before),(const char*)(after)));      
    state_ = NaNstate;
    return;
  }

  // Process digits after the decimal point; too many implies inexact mantissa.
  for( i=0; i<after.length(); i++) 
  {
    tmp  = mantissa_ * ten;
    tmp += (int)(after(i) - '0');
    if (!tmp.isGood())
    {
      // Signal an inexact condition.
      RWDecimal<M>::inexact(RWDecimalBase::conversion, RWDecimal<M>(RWDecimalBase::nullstate), RWDecimal<M>(RWDecimalBase::nullstate),
                            RWMessage(RWDCML_FROMPORTINEXACT, classname, (const char*)(before),(const char*)(after)));      
      break;
    }
    mantissa_ = tmp;
    place++;
  }
  // 'place' is the number of processed digits; some acceptable mantissas
  // can exceed the #digits indicated by decimalPrecision() by 1, plus 
  // we may have processed one extra digit to cause an inexact condition.
  RWPOSTCONDITION(place <= M::decimalPrecision()+2UL);
  if ( negative ) 
  {
    mantissa_ = -mantissa_;
  }
  // Check this value under several conditions.
  exponent_ = (int)(place - before.length());
}

//************************************************************************
//
// Construct from a string
//
//************************************************************************
template< class M >
RWDecimal<M>::RWDecimal( const char* numAsString )
{
  if(numAsString == 0 || *numAsString == '\0')
  {
    state_ = nullstate;
  }
  else
  {
    // use the operator<< machinery to extract the number from the string
    // representation.  To ensure that the string contained only the number,
    // and nothing more, ensure that the string state is ok, and that either
    // 1) we are at eof already, or 2) attempting to read another character
    // from the string leaves us at eof.      
    RW_SL_IO_STD(istrstream) stream((char *) numAsString);
    RWDecimal<M> dec;
    (RW_SL_IO_STD(istream)&)stream >> dec;  // For some reason SunPro 410 needs the cast to RW_SL_IO_STD(istream)&
    char c = ' ';
    while((RWMNY_STD::isspace)(c) && !stream.bad() && !stream.eof())
    {
      stream.get(c);  // make real sure not at eof by getting a character
                      // this better leave us at the end of file
    }
    if(stream.bad() || !stream.eof())
    {
      state_ = RWDecimalBase::NaNstate;
    }
    else
    {
      mantissa_ = dec.mantissa_;
      exponent_ = dec.exponent_;
      state_ = dec.state_;
    }
  }
}


//*************************************************************************
//
// Function: decplus
//
// Note: this function was originally named 'plus', but a bug in the MSVC
// 4.2 compiler caused this static member function to be placed in the
// global scope, where it conflicted with the Standard Library struct 'plus'
// defined in the include file <functional>.
//
// Why have a "plus" function? why not just put this code in the overloaded
// "+" operator for RWDecimal<M>'s?  As you can see addition with all the error 
// checking is rather tedious and all the same stuff has to be done for 
// subtraction.  so for subtraction you'd like to be able to use addition:
// a - b -> a + (-b).  But, the error handlers that might be called need to
// know what operation is being performed.  Hence the plus function with the
// third parameter indicating whether addition or subtraction is actually
// being performed.
//
//************************************************************************
template< class M >
RWDecimal<M> RWDecimal<M>::decplus( const RWDecimal<M>& lhs, const RWDecimal<M>& rhs, RWDecimalBase::Op op )
{
  RWDecimalBase::State state = getStatePlus(lhs, rhs);
  if(state != RWDecimalBase::normal)
  {
    return RWDecimal<M>(state);
  }

  // Check for quick return
  M zero(0);
  if(lhs.mantissa_ == zero || lhs.state_ == RWDecimalBase::nullstate)
    return rhs;
  if(rhs.mantissa_ == zero || rhs.state_ == RWDecimalBase::nullstate)
    return lhs;

  int diff = lhs.exponent_ - rhs.exponent_;
  if ( diff == 0 )
  {
    return addEqualExponents( lhs.mantissa_, rhs.mantissa_, lhs.exponent_, op );
  }
  // use pointers to avoid makeing copies of the mantissi
  const M *m1, *m2;
  const int *e1, *e2;
  if ( diff > 0 )
  {
    m1 = &lhs.mantissa_;
    e1 = &lhs.exponent_;
    m2 = &rhs.mantissa_;
    e2 = &rhs.exponent_;
  }
  else // diff < 0
  {
    m1 = &rhs.mantissa_;
    e1 = &rhs.exponent_;
    m2 = &lhs.mantissa_;
    e2 = &lhs.exponent_;
    diff = -diff;
  } 

  M m2AdjustedMantissa = m2->decimalShiftLeft( diff );
  if ( m2AdjustedMantissa.isGood() )
  {
    return addEqualExponents( *m1, m2AdjustedMantissa, *e1, op );
  }

  int overflowFromLeftShift = diff - rwMaxDecimalLeftShift( *m2 );
  M m1AdjustedMantissa = m1->decimalShiftRight( overflowFromLeftShift );
  m2AdjustedMantissa = m2->decimalShiftLeft(*e1 - overflowFromLeftShift - *e2);
  M sumMantissa;
  
  unsigned numTruncated  = M::preciseAdd( m1AdjustedMantissa, m2AdjustedMantissa, sumMantissa );  
  int sumExponent = *e1 - overflowFromLeftShift;

  if ( numTruncated != 0 )
  {
    sumExponent -= numTruncated;
    if ( sumExponent < 0 )
    {
      RWMsgId msgid = (op == RWDecimalBase::add) ? RWDCML_ADDOVER : RWDCML_SUBOVER;
      RWDecimal<M>::overflow( op, lhs, rhs, RWMessage(msgid,classname) );
      return RWDecimal<M>( RWDecimalBase::NaNstate );
    }
  }

  RWMsgId msgid = (op == RWDecimalBase::add) ? RWDCML_ADDINEXACT : RWDCML_SUBINEXACT;
  RWDecimal<M>::inexact( op, lhs, rhs, RWMessage(msgid,classname) );

  return RWDecimal<M>( sumMantissa, sumExponent );
}


//************************************************************************
//
// static function for shifting into and out of streams
//
//************************************************************************
template< class M >
RW_SL_IO_STD(ostream)& RWDecimal<M>::shift( RW_SL_IO_STD(ostream)& strm, const RWDecimal<M>& d )
{
  strm << convertToString(d);
  return strm;
}

template< class M >
RW_SL_IO_STD(istream)& RWDecimal<M>::shift( RW_SL_IO_STD(istream)& strm, RWDecimal<M>& d )
{
  RWDecimalParser parser;
  RWDecimalPortable p = parser( strm );
  if ( strm.good() || strm.eof() ) 
  {
    d = RWDecimal<M>( p );
  }
  return strm;
}


//************************************************************************
//
// Conversion to an RWDecimalPortable
//
//************************************************************************
template< class M >
RWDecimal<M>::operator RWDecimalPortable() const
{
  return RWDecimalPortable(convertToString(*this));
}


//--------------------------------------------------------------------------
//
// Persistence functions
//
//--------------------------------------------------------------------------
#ifndef RW_NO_TOOLS
template< class M >
void RWDecimal<M>::saveOn( RWvostream& s ) const
{
  int stateAsInt = state_;
  s << stateAsInt;
  mantissa_.saveOn( s );
  s << exponent_;
}

template< class M >
void RWDecimal<M>::restoreFrom( RWvistream& s )
{
  int stateAsInt;
  s >> stateAsInt;
  state_ = State( stateAsInt );
  mantissa_.restoreFrom( s );
  s >> exponent_;
}

template< class M >
void RWDecimal<M>::saveOn( RWFile& f ) const
{
  int stateAsInt = state_;
  f.Write( stateAsInt );
  mantissa_.saveOn( f );
  f.Write( exponent_ );
}

template< class M >
void RWDecimal<M>::restoreFrom( RWFile& f )
{
  int stateAsInt;
  f.Read( stateAsInt );
  state_ = State( stateAsInt );
  mantissa_.restoreFrom( f );
  f.Read( exponent_ );
}
#endif //RW_NO_TOOLS


//****************************************************
//
// Conversion from double
//
//****************************************************
template< class M >
RWDecimal<M> RWDecimal<M>::from( double m )
{  
#ifndef RW_NO_LOCAL_STATIC_CONST
  static const double biggestDiv10 = MaxDouble / 10.0;
#else
  static double biggestDiv10 = MaxDouble / 10.0;
#endif

  double mantissa = m;
  int exponent = 0;
  while( mantissa != floor(mantissa) && mantissa <= biggestDiv10 && mantissa >= -biggestDiv10 )
  {
    exponent += 1;
    mantissa = m * pow(10.0, (double)exponent);
  }
  M tmpMantissa = M(mantissa);
  if( !tmpMantissa.isGood() ) // only happens if m > maximum M
  {
    RWDecimal<M>::overflow(RWDecimal<M>::conversion, RWDecimal<M>(RWDecimalBase::nullstate), RWDecimal<M>(RWDecimalBase::nullstate),
                           RWMessage(RWDCML_FROMDBLOVER,classname,m) );
    return RWDecimal<M>( RWDecimalBase::NaNstate );
  }
  return RWDecimal<M>( tmpMantissa, exponent );
}


//****************************************************
//
// Conversion from long double
//
// Wasn't in the spec to look at this funciton for 
// the 1.5 rewrite.  I'm just copying what was already
// there.  I seems that if you have long double support
// that some attempt should be made to preserve the extra
// accuracy and precision.  If time permits I'll try and
// do something.
//****************************************************
/*
template< class M >
RWDecimal<M> RWDecimal<M>::from( long double m )
{
  return from( (double)m );
}
*/
template< class M >
RWDecimal<M> RWDecimal<M>::from(long double m)
{
#ifndef RW_NO_LOCAL_STATIC_CONST
  static const long double biggestDiv10 = RW_LARGEST_LDOUBLE / 10.0L;
#else
  static long double biggestDiv10 = RW_LARGEST_LDOUBLE / 10.0L;
#endif
  long double mantissa = m;
  int exponent = 0;
  while( mantissa != floorl(mantissa) && mantissa <= biggestDiv10 && mantissa >= -biggestDiv10)
  {
    exponent += 1;
    // 10.06.2004 A.Lebedev
    //mantissa *= 10.0L;
    mantissa = m * powl(10.0L, (long double)exponent);
  }
  M tmpMantissa = M(mantissa);
  if(!tmpMantissa.isGood()) // only happens if m > maximum M
  {
    RWDecimal<M>::overflow(RWDecimal<M>::conversion, RWDecimal<M>(RWDecimalBase::nullstate), RWDecimal<M>(RWDecimalBase::nullstate),
                           RWMessage(RWDCML_FROMLDBLOVER,classname,m));
    return RWDecimal<M>(RWDecimalBase::NaNstate);
  }
  return RWDecimal<M>(tmpMantissa, exponent);
}

template< class M >
RWCString RWDecimal<M>::opAsString(RWDecimalBase::Op op)
{
  RWCString s;
  switch(op)
    {
    case RWDecimalBase::assign:
      s = "=";
      break;
    case RWDecimalBase::add:
      s = "+";
      break;
    case RWDecimalBase::sub:
      s = "-";
      break;
    case RWDecimalBase::mult:
      s = "*";
      break;
    case RWDecimalBase::div:
      s = "/";
      break;
    case RWDecimalBase::powop:
      s = "pow";
      break;
    case RWDecimalBase::pow10op:
      s = "pow10";
      break;
    case RWDecimalBase::conversion:
      s = "conversion";
    }
  return s;
}

//######################## Error classes ###########################################

template< class M >
RWDecimalInexactErr<M>::RWDecimalInexactErr(RWDecimalBase::Op o, const RWDecimal<M>& a, const RWDecimal<M>& b,
           const RWMessage& msg)
 : RWExternalErr(msg),
   op_(o),
   operandA_(a),
   operandB_(b)
{
  ;
}

template< class M >
RWDecimalOverflowErr<M>::RWDecimalOverflowErr(RWDecimalBase::Op o, const RWDecimal<M>& a, const RWDecimal<M>& b,
           const RWMessage& msg)
 : RWExternalErr(msg),
   op_(o),
   operandA_(a),
   operandB_(b)
{
  ;
}


template< class M >
RWCString RWDecimalInexactErr<M>::opString() const
{
  return(RWDecimal<M>::opAsString(operation()));
}


template< class M >
RWCString RWDecimalOverflowErr<M>::opString() const
{
  return(RWDecimal<M>::opAsString(operation()));
}


#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
template< class M >
RWDecimalInit<M>::RWDecimalInit()
{
  if(count++ == 0)
  {
  // the casts below cast away const
    *(RWDecimal<M>*)&RWDecimal<M>::null    = RWDecimal<M>(RWDecimal<M>::nullstate);
    *(RWDecimal<M>*)&RWDecimal<M>::missing = RWDecimal<M>(RWDecimal<M>::missingstate);
    *(RWDecimal<M>*)&RWDecimal<M>::NaN     = RWDecimal<M>(RWDecimal<M>::NaNstate);

    // The sunpro compiler wants to give classname undefined symbol status at link unless
    // it's initialized, again, here.  Oh well, an extra initialization can't hurt...
#if !defined(__hppa)    
    RWDecimal<M>::classname = "RWDecimal";
#endif    
  }
}

template< class M >
int RWDecimalInit<M>::count = 0;
#endif

//---------------------------------------------------------------------------------
//
//  Static member functions
//
//---------------------------------------------------------------------------------
template< class M >
RWDecimal<M> RWDecimal<M>::multiply( const RWDecimal<M>& x, const RWDecimal<M>& y )
{
  RWDecimalBase::State state = getStateMultiply(x, y);
  if(state != RWDecimalBase::normal)
  {
    return RWDecimal<M>(state);
  }

  if(x.state_ == RWDecimalBase::nullstate)
  {
    return RWDecimal<M>(y);
  }

  if(y.state_ == RWDecimalBase::nullstate)
  {
    return RWDecimal<M>(x);
  }

  int e = x.exponent_ + y.exponent_;
  M m;

  unsigned numTruncated = M::preciseMultiply( x.mantissa_, y.mantissa_, m );

  if ( numTruncated == 0 )
  {
    return RWDecimal<M>( m, e );
  }

  e -= numTruncated;
  if ( e >= 0 )
  {
    RWDecimal<M>::inexact( RWDecimalBase::mult, x, y, RWMessage( RWDCML_MULINEXACT,RWDecimal<M>::classname) );
    return RWDecimal<M>( m, e );
  }

  RWDecimal<M>::overflow( RWDecimalBase::mult, x, y, RWMessage(RWDCML_MULOVER,RWDecimal<M>::classname) );
  return RWDecimal<M>( RWDecimalBase::NaNstate );
}


template< class M >
RWDecimal<M>  RWDecimal<M>::divide(const RWDecimal<M>& a, const RWDecimal<M>& b)
{
  RWDecimalBase::State state = getStateDivide(a, b);
  if(state != RWDecimalBase::normal)
  {
    return RWDecimal<M>(state);
  }

  if(b.state_ == RWDecimalBase::nullstate)
  {
    return RWDecimal<M>(a);
  }

  if(b == RWDecimal<M>(0))
  {
    return RWDecimal<M>(RWDecimalBase::NaNstate);
  }

  M a_pos(a.mantissa_);
  M b_pos(b.mantissa_);
  RWBoolean aNeg = a.mantissa_ > M(0) ? FALSE : TRUE;
  RWBoolean bNeg = b.mantissa_ > M(0) ? FALSE : TRUE;
  if(aNeg) a_pos = -a.mantissa_;
  if(bNeg) b_pos = -b.mantissa_;
  RWBoolean resultSign;
  if((aNeg && bNeg) || (!aNeg && !bNeg))
  {
    resultSign = TRUE;
  }
  else
  {
    resultSign = FALSE;
  }

  M partialQuotient, quotient, remainder;
  RWBoolean success = M::longDividePos( a_pos, b_pos, quotient, remainder );
  if ( !success )
  {
    return RWDecimal<M>( RWDecimalBase::NaNstate );
  }

  M zero(0), shiftedQuotient, temp, tmp_rem, tmp_mant(b_pos);
  int exp = a.exponent_ - b.exponent_;
  unsigned numTruncated;

  while( remainder != zero )
  {
    // guard against overflow in remainder
    tmp_rem = remainder.decimalShiftLeft();
    if (tmp_rem.isGood())
    {
      remainder = tmp_rem;
    }
    else
    {
      tmp_mant = tmp_mant.decimalShiftRight();
    }
    success = M::longDividePos( remainder, tmp_mant, partialQuotient, remainder );
    if ( !success )
    {
      return RWDecimal<M>( RWDecimalBase::NaNstate );
    }

    if ( quotient == zero ) 
    {
      quotient = partialQuotient;
    }
    else
    {
      shiftedQuotient = quotient.decimalShiftLeft();
      if ( !shiftedQuotient.isGood() )
      {
        break;
      }
      numTruncated = M::preciseAdd( partialQuotient, shiftedQuotient, temp );
      if ( numTruncated > 0 )
      {
        break; 
      }
      quotient = temp;
    }
    ++exp;
  }
  
if(exp < 0)
 {
  if(quotient != zero)
   {
#ifdef _hp11_
    quotient = quotient.decimalShiftLeft(abs((long)exp));
#else
    quotient = quotient.decimalShiftLeft(RWMNY_STD::abs(exp));
#endif

    if(!quotient.isGood())
     {
      RWDecimal<M>::overflow(RWDecimal<M>::div, a, b, RWMessage(RWDCML_DIVOVER,RWDecimal<M>::classname));
      return RWDecimal<M>(RWDecimalBase::NaNstate);
     }
   }

  exp = 0;
 }
 
  if ( remainder != zero )
  {
    RWDecimal<M>::inexact( RWDecimal<M>::div, a, b, RWMessage(RWDCML_DIVINEXACT, RWDecimal<M>::classname) );
  }
  return resultSign == FALSE ? -(RWDecimal<M>(quotient, exp)) : RWDecimal<M>(quotient, exp);
}



//--------------------------------------------------------------------------
//
// Comparison Operators
//
// Note that since RWDecimals are not normalized, numbers do not have a 
// unique representation.  e.g. 2 may be represented as mantissa = 2, 
// exponent = 0, or, mantissa = 20, exponent = 1, or, mantissa = 200,
// exponent = 2, and so forth.  Thus we must attempt to adjust exponents
// when doing comparisons.
//--------------------------------------------------------------------------

//****************************************************
//
//  Equal static member function
//
//****************************************************
template< class M >
int RWDecimal<M>::equal( const RWDecimal<M>& a, const RWDecimal<M>& b )
{
  if ( a.state_ || b.state_ )
  {
    return( a.state_ == b.state_ );
  }

  register int exponentDiff = a.exponent_ - b.exponent_;
  if ( exponentDiff == 0 )
  {
    return( a.mantissa_ == b.mantissa_ );
  }

  static const M zero(0);

  if ( exponentDiff > 0 )
  {
    M tmp = b.mantissa_ == zero ? zero : b.mantissa_.decimalShiftLeft(exponentDiff);
    if(!tmp.isGood())
    {
      return FALSE;
    }
    return(a.mantissa_ == tmp);
  }

  M tmp = a.mantissa_ == zero ? zero : a.mantissa_.decimalShiftLeft(-exponentDiff);
  if(!tmp.isGood())
  {
    return FALSE;
  }
  return(tmp == b.mantissa_);
}

//****************************************************
//
//  static Less than function
//
//****************************************************
template< class M >
int RWDecimal<M>::lessThan( const RWDecimal<M>& a, const RWDecimal<M>& b )    
{
  if ( a.state_ || b.state_ )
  {
    return 0;
  }

  register int exponentDiff = a.exponent_ - b.exponent_;

  if ( exponentDiff == 0 )
  {
    return( a.mantissa_ < b.mantissa_ );
  }

  static const M zero(0);

  if ( exponentDiff > 0 )
  {
    M tmp = b.mantissa_ == zero ? zero : b.mantissa_.decimalShiftLeft(exponentDiff);
    // if b != 0 and b overflows with an equal exponent then abs(b) > a
    if(!tmp.isGood())
    {
      return(b.mantissa_ > zero);
    }
    return(a.mantissa_ < tmp);
  }

  M tmp = a.mantissa_ == zero ? zero : a.mantissa_.decimalShiftLeft(-exponentDiff);
  // if a != 0 and a overflows with an equal exponent them abs(a) > b
  if(!tmp.isGood())
  {
    return(a.mantissa_ < zero);
  }
  return(tmp < b.mantissa_);
}


//****************************************************
//
// static power function
// 
// Uses the "binary" method for exponentiation.  See 
// "The Art of Computer Programming Vol. 2, Seminumerical
// Algoriths" by Donald Knuth.  Section 4.6.3
//
//***************************************************
template< class M >
RWDecimal<M> RWDecimal<M>::power( const RWDecimal<M>& base, int exponent )
{
  if ( exponent == 0 )
  {
    return RWDecimal<M>( 1 );
  }
  if ( exponent < 0 )
  {
    RWDecimal<M> one( 1 );
    
    RWDecimal<M> a = one/pow(base,-exponent);
    return( a );
  }

  RWDecimal<M> product( 1 );
  RWDecimal<M> z = base;
  unsigned exponentBits = exponent;
  RWBoolean odd;

  for ( ; ; )
  {
    // For the purposes of this algorithm zero is odd
    odd = ( exponentBits == 0 ? TRUE : (exponentBits & 1) );
    exponentBits >>= 1;
    if ( odd )
    {
      product = z * product;
      if ( exponentBits == 0 )
      {
        break;
      }
    }
    z = z * z;
  }

  return product;
}


//****************************************************
//
// static absolute value function
// 
//***************************************************
template< class M >
RWDecimal<M> RWDecimal<M>::absoluteValue( const RWDecimal<M>& x )
{
  if(!x.state_ && x.mantissa_ < M(0))
  { 
    return RWDecimal<M>( -x.mantissa_, x.exponent_ );
  }
  return RWDecimal<M>( x );
}


//***************************************************
//
// static round function
//
//***************************************************
template< class M >
RWDecimal<M> RWDecimal<M>::roundDecimal(const RWDecimal<M>& d, int exponent, RWDecimalBase::RoundingMethod method,
                                        unsigned int roundDigit)
{
  if( !d.isNumber() )
    return d;

  int exponentDiff = d.exponent_ - exponent;
  if( exponentDiff <= 0 )
    return d; // We're done.  No need to round.

  M zero( 0 ), one( 1 );
  M mantissa = d.mantissa_;

  RWBoolean negative = FALSE;
  if( mantissa < zero ) // Convert negatives to positive, change back later
  {
    mantissa = -mantissa;
    negative = TRUE;
  }

  M quotient( 0 );
  if( exponentDiff - d.maxDigits() > 1 ) // Result must be zero...
  {
    if(( method == RWDecimalBase::UP ) && ( mantissa != zero )) //...unless...
      quotient = one;    // ...we're rounding UP with a non-zero mantissa.
  }
  else // Rounding away at most maxDigits() + 1 digits
  {
    M remainder( 0 );
    if( exponentDiff > d.maxDigits() ) // exponentDiff must be maxDigits() + 1
    {
      quotient = zero;
      remainder = mantissa;
    }
    else // divide mantissa by 10^exponentDiff to get quotient & remainder
    {
      M divisor( 10 );
      divisor = divisor.decimalShiftLeft( exponentDiff - 1 );
      M::longDivide( mantissa, divisor, quotient, remainder );
    }

    // discriminator is the decision digit, remainder's most-significant digit
    M discriminator( remainder.decimalShiftRight( exponentDiff - 1 ) );
    M split( (int)roundDigit ), tie( 5 );

    switch( method )
    {
      case RWDecimalBase::UP: // Round up unless remainder is zero
        if( remainder != zero )
          quotient += one;
        break;

      case RWDecimalBase::PLAIN: // Round up if greater than the split
        if( discriminator > split )
          quotient += one;
        break;

      case RWDecimalBase::BANKERS: // Round up to an even number on a tie
        if( discriminator >  tie )
          quotient += one;
        else if( discriminator == tie )
        {
          if( remainder == discriminator.decimalShiftLeft( exponentDiff - 1 ))
            remainder = discriminator; // Strip trailing zeroes from remainder

          if( remainder > tie )
            quotient += one;
          else if(( remainder == tie ) && ( !quotient.isEven() ))
            quotient += one;
        }
        break;

      default: ; // Truncate.
    } // switch
  } // if( exponentDiff - d.maxDigits() > 1 )

  if( negative ) // ...restore the sign
    quotient = -quotient;

  // 07.12.2004 A.Lebedev
  // Check for negative exponent
  if( exponent < 0 )
  {
    quotient = quotient.decimalShiftLeft(-exponent);
    exponent = 0;
  }

  return RWDecimal<M>( quotient, exponent );
}


//**************************************************
//
// static convertToString function
//
//**************************************************
template< class M >
RWCString  RWDecimal<M>::convertToString(const RWDecimal<M>& d)
{
  switch(d.state_) {
    case RWDecimalBase::normal:
      break;
    case RWDecimalBase::NaNstate:
      return "(NaN)";
    case RWDecimalBase::nullstate:
      return "(null)";
    case RWDecimalBase::missingstate:
      return "(missing)";
    case RWDecimalBase::ctorError:
      return "(ctorError)";
    default:
      return "(Impossible value)";
  }

  RWCString decSep = ".";
  char negSign = '-';

  RWBoolean negative = FALSE;
  RW_SL_IO_STD(ostrstream) o;  // Format the mantissa into a string
  o << d.mantissa_ << RW_SL_IO_STD(ends);
  RWCString man(o.str());
  o.rdbuf()->freeze(false);

  // If negative, remove the minus sign for now and put it back at the end.
  // Really, it makes life a whole lot easier.
  if ( man(0UL) == '-' )
  {
    negative = TRUE;
    man.remove( 0UL, 1UL );
  }


  while((size_t)d.exponent_ > man.length())  // Add leading zeros if necessary
  {
    man.prepend("0");
  }

  RWCString string;   // create string representation

  // extract piece after the decimal point
  if(d.exponent_ > 0)  { string = decSep + man(man.length()-d.exponent_,d.exponent_); }

  // extract piece before the decimal point
  if((size_t)d.exponent_ < man.length()) { string.prepend(man(0,man.length()-d.exponent_)); }
  else { string.prepend("0"); }

  if(negative) { string.prepend(negSign); }

  return string;
}


//**************************************************
//
// convertToDouble function
//
//**************************************************
template< class M >
long double RWDecimal<M>::convertToDouble(const RWDecimal<M>& x)
{
  static M one( 1 );
  double mant = x.mantissa_.asDouble();
  return (long double)(mant/pow(10.0, (double)x.exponent_));
}


//**************************************************
//
// convertToInt function
//
//**************************************************
template< class M >
long int RWDecimal<M>::convertToInt(const RWDecimal<M>& x, RWDecimalBase::RoundingMethod method, unsigned int roundDigit )
{
  const RWDecimal<M> rounded = round( x, 0, method, roundDigit );
  return (long int)toDouble( rounded );
}

//**************************************************
//
// convertToBigInt function
//
//**************************************************
template <class M>
__int64 RWDecimal<M>::convertToBigInt(const RWDecimal<M> &x, RWDecimalBase::RoundingMethod method, unsigned int roundDigit)
{
 const RWDecimal<M>  rounded = round(x, 0, method, roundDigit);

 return (__int64)toDouble(rounded);
}

#endif // __RWDECIMAL_CC__
