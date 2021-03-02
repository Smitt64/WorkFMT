/*--------------------------------------------------------------
 *
 * Definition of RWFixedDecimal and related classes.
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

#ifndef __RWFIXEDDEC_H__
#define __RWFIXEDDEC_H__

//******************************************************************************
//
// fixeddec.h
//
// Declarations for the RWFixedDecimal<M> class.  The RWFixedDecimal class
// is derived from the RWDecimal class using private inheritance.  Although
// The public interface for the fixed decimal classes is a subset of the
// public interface for the decimal classes, private inheritance is used
// for backward compatibility reasons.  In  previous releases of Money.h++
// the decimal and fixed decimal classes were unrelated to each other.
// Thus we do not want pointers and references to fixed decimal instances
// being implicitly converted to pointers or references to decimal
// classes.  
//
// Although friend functions may implicitly convert pointers to a private 
// base class, a few compilers had trouble with this.  Most notably MSVC 5
// had a major bug that prevented this.  So, instead of a bunch of ifdefs
// global operators and functions for the fixed decimal class are reimplemented
// here.  Their implementation involves explicitly casting their fixed decimal
// arguments to the decimals (the base class) and invoking the cooresponding
// operator for decimals.
//
//******************************************************************************

#include "rw/money/decimal.h"

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
template< class M >
class RWFixedDecimalInit;
#endif

template< class M >
class RWFixedDecimal : private RWDecimal<M>
{
public:

  // Rounding method enum values from RWDecimalBase.
  // Must change access to public since we use
  // private inheritance.
  RWDecimal<M>::UP;
  RWDecimal<M>::PLAIN;
  RWDecimal<M>::DOWN;
  RWDecimal<M>::BANKERS;
  RWDecimal<M>::TRUNCATE;

  // prototype values, use with op= and op==
  // g++ can't handle having static data members that are the same type as
  // the class that are contained in.  Complains about "incomplete types"
  // when instantiating the RWDecimal template class.  So for g++ we use
  // static functions that return these prototypical values instead.
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
  static const RWFixedDecimal<M> null;
  static const RWFixedDecimal<M> missing;
  static const RWFixedDecimal<M> NaN;
#endif
  static RWFixedDecimal<M> nullValue();
  static RWFixedDecimal<M> missingValue();
  static RWFixedDecimal<M> NaNValue();

//  Public constructors--------------------------------------------------

  // default ctor makes a null
  RWFixedDecimal():RWDecimal<M>() { ; }

  // copy ctor
  RWFixedDecimal(const RWFixedDecimal<M>& d) : RWDecimal<M>( d ) { ; }

  // Conversion from/to portable representation
  // This lets you convert to and from decimal classes of different precisions
  RWFixedDecimal(const RWDecimalPortable& p) : RWDecimal<M>(p) { ; }
  operator RWDecimalPortable() const { const RWDecimal<M>& tmp = *this; return (RWDecimalPortable)tmp; }

  // Conversion from an RWDecimal
  RWFixedDecimal( const RWDecimal<M>& d ) : RWDecimal<M>(d) {;}

  // Conversion from int
  RWFixedDecimal(int i) : RWDecimal<M>(i) { ; }

  // Conversion from long int
  RWFixedDecimal(long int i) : RWDecimal<M>(i) {;}

  // Conversion from __int64
  RWFixedDecimal(__int64 i) : RWDecimal<M>(i) {;}

  // specify an integer mantissa and exponent, this makes
  // declarations like RWFixedDecimal<M>(0,2) unambiguous
  RWFixedDecimal(int m, int e) : RWDecimal<M>(m, e) {;}

  // Likewise for a long integer and exponent.
  RWFixedDecimal(long int m, int e) : RWDecimal<M>(m, e) {;}

  // Likewise for a __int64 and exponent.
  RWFixedDecimal(__int64 m, int e) : RWDecimal<M>(m, e) {;}

  // Construct from an ascii decimal string
  RWFixedDecimal(const char* str) : RWDecimal<M>(str) {;}


// Error handlers--------------------------------------------------------------------------
// These allow you to specify resumptive exceptions.

  static void setInexactHandler( void (*eh)(const RWDecimalInexactErr<M>&) ) { RWDecimal<M>::setInexactHandler(eh); }
  static void setOverflowHandler( void (*eh)(const RWDecimalOverflowErr<M>&) ) { RWDecimal<M>::setOverflowHandler(eh); }
  static void defaultInexactHandler(const RWDecimalInexactErr<M>& err) { RWDecimal<M>::defaultInexactHandler(err); }
  static void defaultOverflowHandler(const RWDecimalOverflowErr<M>& err) { RWDecimal<M>::defaultOverflowHandler(err); }

  static RWCString opAsString( RWDecimalBase::Op op );
  
  // Unary arithmetic operators
  RWFixedDecimal<M> operator-() const
    { return( RWFixedDecimal<M>(-this->mantissa_, this->exponent_, this->state_) ); }
  RWFixedDecimal<M> operator+() const
    { return( RWFixedDecimal<M>(this->mantissa_, this->exponent_, this->state_) ); }


  // Assignment operators -------------------------------------------
  RWFixedDecimal<M>& operator=( const RWFixedDecimal<M>& x );

  RWFixedDecimal<M>& operator+=(const RWFixedDecimal<M>& a)
  { RWFixedDecimal<M> sum = *this + a; return(*this = sum); }

  RWFixedDecimal<M>& operator-=(const RWFixedDecimal<M>& a)
  { RWFixedDecimal<M> sum = *this - a; return(*this = sum); }

  RWFixedDecimal<M>& operator*=(const RWFixedDecimal<M>& a)
  { RWFixedDecimal<M> sum = *this * a; return(*this = sum); }

  RWFixedDecimal<M>& operator/=(const RWFixedDecimal<M>& a)
  { RWFixedDecimal<M> sum = *this / a; return(*this = sum); }

#ifndef RW_NO_TOOLS
  void saveOn(RWvostream& s) const { ((RWDecimal<M>*)this)->saveOn(s); }
  void restoreFrom(RWvistream& s) { ((RWDecimal<M>*)this)->restoreFrom(s); }
  void saveOn(RWFile& f) const { ((RWDecimal<M>*)this)->saveOn(f); }
  void restoreFrom(RWFile& f) { ((RWDecimal<M>*)this)->restoreFrom(f); }
#endif //RW_NO_TOOLS

// Utility functions ------------------------------------------------------------------------

  static int               maxDigits() { return M::decimalPrecision(); }
  static RWFixedDecimal<M> maxValue() { return RWFixedDecimal<M>( M::maxValue(), 0 ); }
  static RWFixedDecimal<M> minValue() { return RWFixedDecimal<M>( M::minValue(), 0 ); }

  int                   isNumber() const { return( this->state_ == RWDecimalBase::normal ); }
  int                   decimalPlaces() const { return( this->state_ == RWDecimalBase::normal ? this->exponent_ : 0 ); }


  static RWFixedDecimal<M>  from(double x) { return RWDecimal<M>::from( x ); }
  static RWFixedDecimal<M>  from(long double x) { return RWDecimal<M>::from( x ); }

//#########################################################################
//
// GLOBAL FRIEND FUNCTIONS
//
//#########################################################################

// Arithemtic operators ---------------------------------------------
#if !defined(RW_DCML_NO_FRIEND_TEMPLATE_FUNC)
  friend RWFixedDecimal<M> rwdcmlexport operator+ FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend RWFixedDecimal<M> rwdcmlexport operator- FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend RWFixedDecimal<M> rwdcmlexport operator* FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend RWFixedDecimal<M> rwdcmlexport operator/ FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
#endif

// Comparison operators -------------------------------------------------------------------

#if !defined(RW_DCML_NO_FRIEND_TEMPLATE_FUNC)
#  if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )
  friend int rwdcmlexport operator!= FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend int rwdcmlexport operator> FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend int rwdcmlexport operator<= FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend int rwdcmlexport operator>= FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
#  endif
  friend int rwdcmlexport operator== FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
  friend int rwdcmlexport operator< FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b);
#endif

//  Math functions -------------------------------------------------------------------------

#if !defined(RW_DCML_NO_FRIEND_TEMPLATE_FUNC)
  friend RWFixedDecimal<M> rwdcmlexport abs FRIEND_TEMPLATE_SPECIFIER ( const RWFixedDecimal<M>& x);
  friend RWFixedDecimal<M> rwdcmlexport pow FRIEND_TEMPLATE_SPECIFIER ( const RWFixedDecimal<M>& x, int e );
  friend RWFixedDecimal<M> rwdcmlexport round FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& x, int digits, RWDecimalBase::RoundingMethod rm );
  friend RWFixedDecimal<M> rwdcmlexport round FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& x, int digits);
  friend RWCString rwdcmlexport toString FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& d);
  friend long double rwdcmlexport toDouble FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& d);
  friend long int rwdcmlexport toInt FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& d);
  friend long int rwdcmlexport toInt FRIEND_TEMPLATE_SPECIFIER (const RWFixedDecimal<M>& d, RWDecimalBase::RoundingMethod m);
#endif

// I/O operators ----------------------------------------------------------------------------

#if !defined(RW_DCML_NO_FRIEND_TEMPLATE_FUNC)
  friend RW_SL_IO_STD(istream)& operator>> FRIEND_TEMPLATE_SPECIFIER ( RW_SL_IO_STD(istream)& strm, RWFixedDecimal<M>& x);
  friend RWvostream& operator<< FRIEND_TEMPLATE_SPECIFIER ( RWvostream& strm, const RWFixedDecimal<M>& x);
  friend RWFile& operator>> FRIEND_TEMPLATE_SPECIFIER ( RWFile& strm, RWFixedDecimal<M>& x);
  friend RWvistream& operator>> FRIEND_TEMPLATE_SPECIFIER ( RWvistream& strm, RWFixedDecimal<M>& x);
  friend RWFile& operator<< FRIEND_TEMPLATE_SPECIFIER ( RWFile& strm, const RWFixedDecimal<M>& x);
#endif

// Protected constructors -----------------------------------------------------------------

protected:
  // The RWFixedDecimalInit class is used to initialize the static data 
  // consisting of the prototypical values null, missing, and NaN.  These
  // values are returned by static member functions for the g++ compiler, so
  // we don't want the init class for g++
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
  friend class RWFixedDecimalInit<M>;
#endif

  RWFixedDecimal(RWDecimalBase::State s) : RWDecimal<M>( s ) { ; }

  // Specify the pieces explicitly
  RWFixedDecimal(const M& m, int e, RWDecimalBase::State s = RWDecimalBase::normal) : RWDecimal<M>(m, e, s) { ; }
//    :mantissa_(m), exponent_(e)
//  { state_ = s; RWPRECONDITION(exponent_ >= 0);}

};

/*
// The AIX CSet 3.1.4 compiler has trouble with global template-based
// functions whose implementations are provided in the above class
// declaration.  So we put the implementation here.

template <class M>
inline RW_SL_IO_STD(istream)& operator>>( RW_SL_IO_STD(istream)& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template <class M>
inline RW_SL_IO_STD(ostream)& operator<<( RW_SL_IO_STD(ostream)& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

template <class M>
inline RWvistream& operator>>( RWvistream& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template <class M>
inline RWvostream& operator<<( RWvostream& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

template <class M>
inline RWFile& operator>>( RWFile& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template <class M>
inline RWFile& operator<<( RWFile& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

*/

/*
 * The RWFixedDecimalInit class initializes static variables before user code
 * using the iostream trick of putting a static init object in every
 * translation unit and initializing the first time one is constructed.
 * The implementation data structures and static vars NaN, null, missing
 * are initialized.
 */

  // These values are returned by static member functions for the g++ compiler,
  // so we don't want the init class for g++
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
template< class M >
class RWDCMLTExport RWFixedDecimalInit
{
  static int count;
public:
  RWFixedDecimalInit();
};
#endif

//#########################################################################
//
// GLOBAL FUNCTIONS
//
//#########################################################################

// Arithemtic operators ---------------------------------------------
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{return (const RWDecimal<M>&)a + (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWFixedDecimal<M>& a, const RWDecimal<M>& b)
{return (const RWDecimal<M>&)a + b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWDecimal<M>& a, const RWFixedDecimal<M>& b)
{return a + (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWFixedDecimal<M>& a, int b)
{ return (const RWDecimal<M>&)a + b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWFixedDecimal<M>& a, long b)
{ return (const RWDecimal<M>&)a + b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(const RWFixedDecimal<M>& a, __int64 b)
{ return (const RWDecimal<M>&)a + b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(int a, const RWFixedDecimal<M>& b)
{ return a + (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator+(long a, const RWFixedDecimal<M>& b)
{ return a + (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a - (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWFixedDecimal<M>& a, const RWDecimal<M>& b)
{ return (const RWDecimal<M>&)a - b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWDecimal<M>& a, const RWFixedDecimal<M>& b)
{return a - (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWFixedDecimal<M>& a, int b)
{ return (const RWDecimal<M>&)a - b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWFixedDecimal<M>& a, long b)
{ return (const RWDecimal<M>&)a - b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(const RWFixedDecimal<M>& a, __int64 b)
{ return (const RWDecimal<M>&)a - b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(int a, const RWFixedDecimal<M>& b)
{ return a - (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator-(long a, const RWFixedDecimal<M>& b)
{ return a - (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a * (const RWDecimal<M>&) b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, const RWDecimal<M>& b)
{ return (const RWDecimal<M>&)a * b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWDecimal<M>& a, const RWFixedDecimal<M>& b)
{return a * (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, double b)
{ return (const RWDecimal<M>&)a * b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(double a, const RWFixedDecimal<M>& b)
{return a * (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, int b)
{ return (const RWDecimal<M>&)a * b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, long b)
{ return (const RWDecimal<M>&)a * b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(const RWFixedDecimal<M>& a, __int64 b)
{ return (const RWDecimal<M>&)a * b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(int a, const RWFixedDecimal<M>& b)
{ return a * (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator*(long a, const RWFixedDecimal<M>& b)
{ return a * (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a / (const RWDecimal<M>&)b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, const RWDecimal<M>& b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWDecimal<M>& a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, double b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(double a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, int b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, long b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, __int64 b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(int a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(long a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, const char *b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const char *a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/(const RWFixedDecimal<M>& a, const RWDecimalPortable& b)
{ return (const RWDecimal<M>&)a / b; }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport operator/( const RWDecimalPortable& a, const RWFixedDecimal<M>& b)
{return a / (const RWDecimal<M>&)b; }
  
// Comparison operators --------------------------------------------------------------------

template< class M >
inline int rwdcmlexport operator==(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a == (const RWDecimal<M>&)b; }

template< class M >
inline int rwdcmlexport operator<(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a < (const RWDecimal<M>&)b; }

// Don't provide relational operators if using Standard Library and
// namespaces are broken, since the definitions below would be redundant.
#if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )
template< class M >
inline int rwdcmlexport operator!=(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a != (const RWDecimal<M>&)b; }

template< class M >
inline int rwdcmlexport operator>(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a > (const RWDecimal<M>&)b; }

template< class M >
inline int rwdcmlexport operator<=(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a <= (const RWDecimal<M>&)b; }

template< class M >
inline int rwdcmlexport operator>=(const RWFixedDecimal<M>& a, const RWFixedDecimal<M>& b)
{ return (const RWDecimal<M>&)a >= (const RWDecimal<M>&)b; }
#endif // stdlib and namespaces

// I/O operators ----------------------------------------------------------------------------

template< class M >
inline RW_SL_IO_STD(istream)& operator>>( RW_SL_IO_STD(istream)& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template< class M >
inline RWvostream& operator<<( RWvostream& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

template< class M >
inline RWFile& operator>>( RWFile& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template< class M >
inline RWvistream& operator>>( RWvistream& strm, RWFixedDecimal<M>& x)
{ return( strm >> (RWDecimal<M>&)x ); }

template< class M >
inline RWFile& operator<<( RWFile& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

template< class M >
inline RW_SL_IO_STD(ostream)& operator<<( RW_SL_IO_STD(ostream)& strm, const RWFixedDecimal<M>& x)
{ return( strm << (const RWDecimal<M>&)x ); }

//  Math functions -------------------------------------------------------------------------

// I really want to give default arguments to the last two arguments of the round function.
// But xlC says no default arguments to function templates! So Ima doin it the hard way...
template< class M >
inline RWFixedDecimal<M> rwdcmlexport round(const RWFixedDecimal<M>& x)
{ return round( (const RWDecimal<M>&)x, 0, RWDecimalBase::PLAIN ); }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport pow( const RWFixedDecimal<M>& x, int e )
{ return pow( (const RWDecimal<M>&)x, e );}

template< class M >
inline RWFixedDecimal<M> rwdcmlexport round(const RWFixedDecimal<M>& x, int digits, RWDecimalBase::RoundingMethod rm )
{ return round( (const RWDecimal<M>&)x, digits, rm ); }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport round(const RWFixedDecimal<M>& x, int digits)
{ return round( (const RWDecimal<M>&)x, digits, RWDecimalBase::PLAIN ); }

template< class M >
inline RWFixedDecimal<M> rwdcmlexport abs( const RWFixedDecimal<M>& x)
{ return abs( (const RWDecimal<M>&)x ); }


// Narrowing conversion functions ------------------------------------------------------------

template< class M >
inline RWCString rwdcmlexport toString(const RWFixedDecimal<M>& d)
{ return RWDecimal<M>::convertToString( (const RWDecimal<M>&)d ); }

template< class M >  
inline long double rwdcmlexport toDouble(const RWFixedDecimal<M>& d)
{ return RWDecimal<M>::convertToDouble( (const RWDecimal<M>&)d ); }

template< class M >  
inline long int rwdcmlexport toInt(const RWFixedDecimal<M>& d)
{ return RWDecimal<M>::convertToInt( (const RWDecimal<M>&)d, RWDecimalBase::PLAIN ); }

template< class M >
inline long int rwdcmlexport toInt(const RWFixedDecimal<M>& d, RWDecimalBase::RoundingMethod m)
{ return RWDecimal<M>::convertToInt( (const RWDecimal<M>&)d, m ); }

template< class M >  
inline __int64 rwdcmlexport toBigInt(const RWFixedDecimal<M>& d)
{ return RWDecimal<M>::convertToBigInt((const RWDecimal<M>&)d, RWDecimalBase::PLAIN); }

template< class M >
inline __int64 rwdcmlexport toBigInt(const RWFixedDecimal<M>& d, RWDecimalBase::RoundingMethod m)
{ return RWDecimal<M>::convertToBigInt((const RWDecimal<M>&)d, m); }

//---------------------------------------------------------
//
// RWFixedDecimalInit vars that initialize static data in the
// RWFixedDecimal class.
//
//------------------------------------------------------------

// MSVC needs to have have an instance of the RWFixedDecimal class
// instantiated before it can instantiate the RWFixedDecimalInit
// class.
#if defined(_MSC_VER)
static RWFixedDecimal< RWMultiPrecisionInt<1> >     rwFixedDecimal52InstVar_;
static RWFixedDecimal< RWMultiPrecisionInt<2> >     rwFixedDecimal64InstVar_;
static RWFixedDecimal< RWMultiPrecisionInt<3> >     rwFixedDecimal96InstVar_;
#endif

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
static RWFixedDecimalInit< RWMultiPrecisionInt<1> > rwFixedDecimal52InitVar_;
static RWFixedDecimalInit< RWMultiPrecisionInt<2> > rwFixedDecimal64InitVar_;
static RWFixedDecimalInit< RWMultiPrecisionInt<3> > rwFixedDecimal96InitVar_;
#endif


#ifdef RW_MONEY_COMPILE_INSTANTIATE
#  include <rw/money/fixeddec.cc>
#endif

#endif  // __RWFIXEDDEC_H__
