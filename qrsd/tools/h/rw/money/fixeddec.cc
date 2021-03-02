/*--------------------------------------------------------------
 *
 * Implementation of RWFixedDecimal and related classes.
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

#ifndef __RWFIXEDDEC_CC__
#define __RWFIXEDDEC_CC__


//-----------------------------------------------------------
//
// Note:
//
// For some reason the HPUX aCC compiler (ver 1.0.6) gives an error (future) that
// the member data exponent_ and mantissa_ are undeclared, because, perhaps we forgot
// to make it depend on the template type parameters.  I'm not sure what this means
// but the message goes away when we quailify these variables with a "this->".  Hence
// all the really unnecessary this->'s in the following code.
//
//--------------------------------------------------------------------

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
// Initialized static data
template< class M >
const RWFixedDecimal<M> RWFixedDecimal<M>::null = RWFixedDecimal<M>(RWDecimalBase::nullstate);

template< class M >
const RWFixedDecimal<M> RWFixedDecimal<M>::missing = RWFixedDecimal<M>( RWDecimalBase::missingstate );

template< class M >
const RWFixedDecimal<M> RWFixedDecimal<M>::NaN = RWFixedDecimal<M>( RWDecimalBase::NaNstate );
#endif

// g++ must use static functions to return the values.
template< class M >
RWFixedDecimal<M> RWFixedDecimal<M>::nullValue()
{
  static const RWFixedDecimal<M> n = RWFixedDecimal<M>( RWDecimalBase::nullstate );
  return n;
}

template< class M >
RWFixedDecimal<M> RWFixedDecimal<M>::missingValue()
{
  static const RWFixedDecimal<M> n = RWFixedDecimal<M>( RWDecimalBase::missingstate );
  return n;
}

template< class M >
RWFixedDecimal<M> RWFixedDecimal<M>::NaNValue()
{
  static const RWFixedDecimal<M> n = RWFixedDecimal<M>( RWDecimalBase::NaNstate );
  return n;
}


template< class M >
RWFixedDecimal<M>& RWFixedDecimal<M>::operator=( const RWFixedDecimal<M>& x )
{
  if ( !x.state_ )
  {
    if ( this->state_ )
    {
      // Do not allow the exponent to exceed maxDigits, or overflow may occur
      // on reassignment.
      if(x.exponent_ > x.maxDigits())
      {
        this->exponent_ = RWFixedDecimal<M>::maxDigits();
        this->mantissa_ = x.mantissa_.decimalShiftRight(x.exponent_ - x.maxDigits());
      }
      else
      {
        this->mantissa_ = x.mantissa_;
        this->exponent_ = x.exponent_;
      }
    }
    else
    {
      if(this->exponent_ == x.exponent_)
      {
        this->mantissa_ = x.mantissa_;
      }
      else if(this->exponent_ > x.exponent_)
      {
        this->mantissa_ = x.mantissa_.decimalShiftLeft( this->exponent_ - x.exponent_ );
        if(!this->mantissa_.isGood())
        {
          RWDecimal<M>::overflow( RWDecimalBase::assign, RWFixedDecimal<M>(RWDecimalBase::nullstate), x,
          RWMessage(RWDCML_ASSIGNOVER,RWDecimal<M>::classname));
        }
      }
      else
      {
        RWFixedDecimal<M> tmp = round(x, this->exponent_);
        this->mantissa_ = tmp.mantissa_;
      }
    }
  }

  this->state_ = x.state_;
  return *this;
}

  // The RWFixedDecimalInit class is used to initialize the static data
  // consisting of the prototypical values null, missing, and NaN.  These
  // values are returned by static member functions for the g++ compiler, so
  // we don't want the init class for g++
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES )
template< class M >
int RWFixedDecimalInit<M>::count = 0;

template< class M >
RWFixedDecimalInit<M>::RWFixedDecimalInit()
{
  if(count++ == 0)
  {
  // the casts below cast away const
    *(RWFixedDecimal<M>*)&RWFixedDecimal<M>::null    = RWFixedDecimal<M>(RWDecimalBase::nullstate);
    *(RWFixedDecimal<M>*)&RWFixedDecimal<M>::missing = RWFixedDecimal<M>(RWDecimalBase::missingstate);
    *(RWFixedDecimal<M>*)&RWFixedDecimal<M>::NaN     = RWFixedDecimal<M>(RWDecimalBase::NaNstate);
  }
}

#endif // RW_NO_STATIC_TEMPLATE_ATTRIBUTES 

#endif // __RWFIXEDDEC_CC__
