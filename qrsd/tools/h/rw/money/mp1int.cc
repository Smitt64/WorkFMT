/*--------------------------------------------------------------
 *
 * Implementation of RWMultiPrecisionInt where the underlying
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

#ifndef __RWMP1INT_CC__
#define __RWMP1INT_CC__

#if defined RW_MONEY_SPECIALIZATION_IN_LIBRARY
RWMultiPrecisionInt<1> RWMultiPrecisionInt<1>::decimalShiftLeft(unsigned int shiftBy ) const
#else
inline RWMultiPrecisionInt<1> RWMultiPrecisionInt<1>::decimalShiftLeft(unsigned int shiftBy ) const
#endif
{
  double data = data_;

  // Call pow() if shiftBy is large. 
  if (shiftBy > 200)
  {
    // Make sure that the pow() function won't cause an overflow...
    if ( shiftBy > (unsigned int)(DBL_MAX_10_EXP - RWMNY_STD::floor(RWMNY_STD::log10(data))) )
    {
      return RWMultiPrecisionInt<1>( badValue() );
    }
    return RWMultiPrecisionInt<1>( data * RWMNY_STD::pow(10.0, (double) shiftBy) );
  }

  register int i = shiftBy;
  while ( --i >= 0 )
  {
    data *= 10.0;
  }
    
  if ( data > maxDouble() || data < -maxDouble() )
  {
    data = badValue();
  }
  return RWMultiPrecisionInt<1>( data );
}


#if defined RW_MONEY_SPECIALIZATION_IN_LIBRARY
unsigned RWMultiPrecisionInt<1>::preciseMultiply( const RWMultiPrecisionInt<1>& x, const RWMultiPrecisionInt<1>& y,
                                               RWMultiPrecisionInt<1>& product )
#else
inline unsigned RWMultiPrecisionInt<1>::preciseMultiply( const RWMultiPrecisionInt<1>& x, const RWMultiPrecisionInt<1>& y,
                                               RWMultiPrecisionInt<1>& product )
#endif
{
  double numTruncated = 0.0;
  product.data_ = x.data_ * y.data_;
  double absProd = RWMNY_STD::fabs( product.data_ );
  if ( absProd > maxDouble() )
  {
    numTruncated = RWMNY_STD::ceil( RWMNY_STD::log10(absProd/maxDouble()) );
    product.data_ /= RWMNY_STD::pow( 10.0, numTruncated );
  }
  return (unsigned)numTruncated;
}


#if defined RW_MONEY_SPECIALIZATION_IN_LIBRARY
RWCString RWMultiPrecisionInt<1>::asString() const
#else
inline RWCString RWMultiPrecisionInt<1>::asString() const
#endif
{
  RW_SL_IO_STD(ostrstream) strm;
  strm << *this << RW_SL_IO_STD(ends);
  RWCString string(strm.str());
  strm.rdbuf()->freeze(false);
  return string;
}


#endif // __RWMP1INT_CC__
