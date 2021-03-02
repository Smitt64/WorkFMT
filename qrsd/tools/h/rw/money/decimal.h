/*--------------------------------------------------------------
 *
 * Definition of RWDecimal and related classes.
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

#ifndef __RWDECIMAL_H__
#define __RWDECIMAL_H__

/*
 * The RWDecimalPortable class is declared via #define decport.h rather than
 * using a forward declaration so that you can convert from one decimal type
 * to another simply by including the two types of interest.  Otherwise, you'd
 * have to include decport.h explicitly in user programs in order to do the
 * conversion, and this didn't seem like a good idea.  (conversion from type X
 * to Y happens by first converting X to RWDecimalPortable using operator
 * RWDecimalPortable and then constructing a Y from that).
 *
 * Copy constructors and assignment operators aren't explicitly provided; we
 * count on the compiler to synthesize them for us.  (except for op= for the
 * fixed classes, since this has special symantics).
 * 
 */

#include "rw/dcmldefs.h"
#include "rw/money/decdefs.h"
#include "rw/decbase.h"
#include "rw/decport.h"     /* See note in top of file comments */
#include "rw/money/mpint.h"
#include "rw/money/mp1int.h"
#include "rw/rwerr.h"
#include "rw/dcmlerr.h"

#if defined(RW_MNY_NO_NEW_HEADER)
    #include <math.h>
#else
    #include <cmath>
#endif

#ifndef RW_NO_TOOLS
    #include "rw/rwfile.h"
    #include "rw/vstream.h"
#endif

// If using standard library, include <utility> for possible
// relational operator definitions.
#ifndef RW_NO_STL
    #include <utility>
#endif

#if !((defined(RW_NAMESPACE_REQUIRED) || ( defined(__TURBOC__) && ( __TURBOC__ >= 0x530 ))) && !defined(RW_NO_IOSTD)) && !(defined(__GNUG__) && (__GNUG__ >= 3))
class RW_SL_IO_STD(istream);
class RW_SL_IO_STD(ostream);
#endif

template< class M >
class RWDCMLTExport RWDecimalInexactErr;

template< class M >
class RWDCMLTExport RWDecimalOverflowErr;

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
template< class M >
class RWDecimalInit;
#endif

template< class M >
class RWDCMLTExport RWDecimal : public RWDecimalBase
{
 public:
   // g++ can't handle having static data members that are the same type as
   // the class that are contained in.  Complains about "incomplete types"
   // when instantiating the RWDecimal template class.  So for g++ we use
   // static functions that return these prototypical values instead.

#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
   static const RWDecimal<M>  null;     // prototype values, use with op= and op==
   static const RWDecimal<M>  missing;
   static const RWDecimal<M>  NaN;
#endif

   static RWDecimal<M>  nullValue();     // prototype values, use with op= and op==
   static RWDecimal<M>  missingValue();
   static RWDecimal<M>  NaNValue();


 //  Public constructors--------------------------------------------------

   // default ctor makes a null
   RWDecimal()
   {
    state_ = nullstate;
   }

   // copy ctor
   RWDecimal(const RWDecimal<M> &d)
   {
    mantissa_ = d.mantissa_;
    exponent_ = d.exponent_;
    state_    = d.state_;
   }

   // Conversion from/to portable representation
   // This lets you convert to and from decimal classes of different precisions
   RWDecimal(const RWDecimalPortable &);
   operator RWDecimalPortable() const;

   // Conversion from int
   RWDecimal(int i)
   {
    mantissa_ = M(i);
    exponent_ = 0;
    state_    = normal;
   }

   // Conversion from long int
   RWDecimal(long int i)
   {
    mantissa_ = M(i);
    exponent_ = 0;
    state_    = normal;
   }

   // Conversion from __int64
   RWDecimal(__int64 i)
   {
    mantissa_ = M(i);
    exponent_ = 0;
    state_    = normal;
   }

   // specify an integer mantissa and exponent, this makes
   // declarations like RWDecimal<M>(0,2) unambiguous
   RWDecimal(int m, int e)
   {
    mantissa_ = M(m);
    exponent_ = e;

    RWPRECONDITION(exponent_ >= 0);

    state_ = normal;
   }

   // Likewise for a long integer and exponent.
   RWDecimal(long int m, int e)
   {
    mantissa_ = M(m);
    exponent_ = e;

    RWPRECONDITION(exponent_ >= 0);

    state_ = normal;
   }

   // Likewise for a __int64 and exponent.
   RWDecimal(__int64 m, int e)
   {
    mantissa_ = M(m);
    exponent_ = e;

    RWPRECONDITION(exponent_ >= 0);

    state_ = normal;
   }

   // Construct from an ascii decimal string
   RWDecimal(const char *str);


 // Error handlers--------------------------------------------------------------------------
 // These allow you to specify resumptive exceptions.

   static void setInexactHandler(void (*eh)(const RWDecimalInexactErr<M> &))
   {
    inexactHandler_ = eh;
   }

   static void setOverflowHandler(void (*eh)(const RWDecimalOverflowErr<M> &))
   {
    overflowHandler_ = eh;
   }

   static void defaultInexactHandler (const RWDecimalInexactErr<M>  &err);
   static void defaultOverflowHandler(const RWDecimalOverflowErr<M> &err);

   static RWCString opAsString(RWDecimalBase::Op op);

   // Unary arithmetic operators
   RWDecimal<M> operator-() const
   {
    return (RWDecimal<M>(-mantissa_, exponent_, state_));
   }

   RWDecimal<M> operator+() const
   {
    return (RWDecimal<M>(mantissa_, exponent_, state_));
   }

   // Arithmetic assignment operators
#if defined(__SUNPRO_CC)
   RWDecimal<M> & operator=(const RWDecimal<M> &a)
   {
    state_    = a.state_;
    mantissa_ = a.mantissa_;
    exponent_ = a.exponent_;

    return *this;
   }
#endif

   RWDecimal<M> & operator+=(const RWDecimal<M> &a)
   {
    RWDecimal<M>  sum = *this + a;
    
    return (*this = sum);
   }

   RWDecimal<M> & operator-=(const RWDecimal<M> &a)
   {
    RWDecimal<M>  diff = *this - a;
    
    return (*this = diff);
   }

   RWDecimal<M> & operator*=(const RWDecimal<M> &a)
   {
    RWDecimal<M>  prod = *this * a;
    
    return (*this = prod);
   }

   RWDecimal<M> & operator/=(const RWDecimal<M> &a)
   {
    RWDecimal<M>  result = *this / a;
    
    return (*this = result);
   }

 // Persistence --------------------------------------------------------------------------
#ifndef RW_NO_TOOLS
   void saveOn(RWvostream &) const;
   void restoreFrom(RWvistream &);
   void saveOn(RWFile &) const;
   void restoreFrom(RWFile &);
#endif //RW_NO_TOOLS

 // Utility functions ------------------------------------------------------------------------

   static int maxDigits()
   {
    return M::decimalPrecision();
   }

   static RWDecimal<M> maxValue()
   {
    return RWDecimal<M>(M::maxValue(), 0);
   }

   static RWDecimal<M> minValue()
   {
    return RWDecimal<M>(M::minValue(), 0);
   }

   int isNumber() const
   {
    return (state_ == normal);
   }

   int decimalPlaces() const
   {
    return (state_ == normal ? exponent_ : 0);
   }


 // Narrowing conversion functions ------------------------------------------------------------

   static RWDecimal<M>  from(double);
   static RWDecimal<M>  from(long double);
   
   static RWDecimal<M>           multiply       (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static RWDecimal<M>           divide         (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static int                    equal          (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static int                    lessThan       (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static RW_SL_IO_STD(istream) &shift          (RW_SL_IO_STD(istream)&, RWDecimal<M> &);
   static RW_SL_IO_STD(ostream) &shift          (RW_SL_IO_STD(ostream)&, const RWDecimal<M> &);
   static RWDecimal<M>           power          (const RWDecimal<M> &, int);
   static RWDecimal<M>           absoluteValue  (const RWDecimal<M> &);
   static RWDecimal<M>           roundDecimal   (const RWDecimal<M> &, int, RWDecimalBase::RoundingMethod, unsigned int roundDigit = 4);
   static RWCString              convertToString(const RWDecimal<M> &);
   static long double            convertToDouble(const RWDecimal<M> &);
   static long int               convertToInt   (const RWDecimal<M> &d, RWDecimalBase::RoundingMethod, unsigned int roundDigit = 4);
   static __int64                convertToBigInt(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod, unsigned int roundDigit = 4);
   
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
 // Class that when instatiated initializes the static data members  
   friend class RWDecimalInit<M>;
#endif

 protected:
 //State state_;    from RWDecimalBase
   int   exponent_;
   M     mantissa_;

   // HP aCC gives ugly warnings if classname is declared to be a char*; when
   // doing the initialization it complains that assigning a literal string constant to
   // a char* is depricated and may not be supported in the future.
   // Sunpro, OTOH, complains that classname is an undefined symbol at link
   // if you DON'T delcare it to be a char*.  
#if defined(__hppa)
   static const char   classname[];
#else  
   static const char  *classname;
#endif  

#if (defined(_MSC_VER) && (_MSC_VER == 1310))
   static void RWDCMLExport (*overflowHandler_)(const RWDecimalOverflowErr<M> &);
   static void RWDCMLExport (*inexactHandler_)(const RWDecimalInexactErr<M> &);
#else
   static void (*overflowHandler_)(const RWDecimalOverflowErr<M> &);
   static void (*inexactHandler_)(const RWDecimalInexactErr<M> &);
#endif

 // Protected constructors -----------------------------------------------------------------

   RWDecimal(State s)
   {
    state_    = s;
    exponent_ = 0;
   }

   // Specify the pieces explicitly
   RWDecimal(const M &m, int e, State s = RWDecimalBase::normal) : mantissa_(m), exponent_(e)
   {
    state_ = s;
    
    RWPRECONDITION(exponent_ >= 0);
   }


 // Protected utility functions -------------------------------------------------------  
   static void   overflow(Op oper, const RWDecimal<M> &a, const RWDecimal<M> &b, const RWMessage &msg);
   static void   inexact (Op oper, const RWDecimal<M> &a, const RWDecimal<M> &b, const RWMessage &msg);

   static inline RWDecimalBase::State getStatePlus     (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static inline RWDecimalBase::State getStateDivide   (const RWDecimal<M> &a, const RWDecimal<M> &b);
   static inline RWDecimalBase::State getStateMultiply (const RWDecimal<M> &a, const RWDecimal<M> &b);  
   static inline RWDecimal<M>         addEqualExponents(const M &lhs, const M &rhs, int exp, RWDecimalBase::Op op);

 public:
   static RWDecimal<M> decplus(const RWDecimal<M> &lhs, const RWDecimal<M> &rhs, RWDecimalBase::Op op);
};

/*
 * Exception classes.  These are used as arguments to the error
 * handlers.  The default error handlers throw them as exceptions,
 * but you are free to change this behaviour.
 */

template< class M >
class RWDCMLTExport RWDecimalInexactErr : public RWExternalErr
{
 public:
   RWDecimalInexactErr(RWDecimalBase::Op o, const RWDecimal<M> &a, const RWDecimal<M> &b, const RWMessage &msg);
   
   RWDecimal<M>       leftOperand()  const { return operandA_; }
   RWDecimal<M>       rightOperand() const { return operandB_; }
   RWDecimalBase::Op  operation()    const { return op_; }
   RWCString          opString()     const;
   const char        *msg()          const { return why(); }

 private:
   RWDecimalBase::Op  op_;
   RWDecimal<M>       operandA_, operandB_;
};


template< class M >
class RWDCMLTExport RWDecimalOverflowErr : public RWExternalErr
{
 public:
   RWDecimalOverflowErr(RWDecimalBase::Op o, const RWDecimal<M> &a, const RWDecimal<M> &b, const RWMessage &);
   RWDecimal<M>       leftOperand()  const { return operandA_; }
   RWDecimal<M>       rightOperand() const { return operandB_; }
   RWDecimalBase::Op  operation()    const { return op_; }
   RWCString          opString()     const;
   const char        *msg()          const { return why(); }

 private:
   RWDecimalBase::Op  op_;
   RWDecimal<M>       operandA_, operandB_;
};



#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
/*
 * The RWDecimalInit class initializes static variables before user code
 * using the iostream trick of putting a static init object in every
 * translation unit and initializing the first time one is constructed.
 * The implementation data structures and static vars NaN, null, missing
 * are initialized.
 */

template< class M >
class RWDecimalInit
{
   static int  count;


 public:
   RWDecimalInit();
};
#endif

template< class M >
inline unsigned rwMaxDecimalLeftShift(const M &x)
{
 double  max = x >= M(0) ? M::maxValue().asDouble() : -(M::minValue().asDouble());
 int     n;


 if(x == M(0))
   n = (int)log10(max);
 else
   n = (int)(log10(max/fabs(x.asDouble())));

 return (n <= 0 ? 0U : (unsigned)n);
}

//***************************************************************************
//
// getStatePlus
//
// This function determines the state of the result of adding two RWDecimals 
// together.
//
//****************************************************************************
template< class M >
inline RWDecimalBase::State RWDecimal<M>::getStatePlus(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 if(a.state_ == RWDecimalBase::missingstate || b.state_ == RWDecimalBase::missingstate)
   return RWDecimalBase::missingstate;

 if(a.state_ == RWDecimalBase::NaNstate || b.state_ == RWDecimalBase::NaNstate)
   return RWDecimalBase::NaNstate;

 if(a.state_ == RWDecimalBase::nullstate)
   return b.state_;

 if(b.state_ == RWDecimalBase::nullstate)
   return a.state_;

 return RWDecimalBase::normal;
}


//***************************************************************************
//
// getStateMultiply
//
// This function determines the state of the result of multiplying two
// RWDecimals together.
//
// Note: This code is identical to getStatePlus.  It is duplicated here
// because of a SunPro 4.2p7 compiler problem.
//
//****************************************************************************
template< class M >
inline RWDecimalBase::State RWDecimal<M>::getStateMultiply(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 if(a.state_ == RWDecimalBase::missingstate || b.state_ == RWDecimalBase::missingstate)
   return RWDecimalBase::missingstate;

 if(a.state_ == RWDecimalBase::NaNstate || b.state_ == RWDecimalBase::NaNstate)
   return RWDecimalBase::NaNstate;

 if(a.state_ == RWDecimalBase::nullstate)
   return b.state_;

 if(b.state_ == RWDecimalBase::nullstate)
   return a.state_;

 return RWDecimalBase::normal;
}


//***************************************************************************
//
// getStateDivide
//
// This function determines the state of the result of dividing two RWDecimals 
// together.
//
// Note: This code is identical to getStatePlus.  It is duplicated here
// because of a SunPro 4.2p7 compiler problem.
//
//****************************************************************************
template< class M >
inline RWDecimalBase::State RWDecimal<M>::getStateDivide(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 if((a.state_ == RWDecimalBase::nullstate) && (b.state_ == RWDecimalBase::normal))
   return RWDecimalBase::NaNstate;

 if(a.state_ == RWDecimalBase::missingstate || b.state_ == RWDecimalBase::missingstate)
   return RWDecimalBase::missingstate;

 if(a.state_ == RWDecimalBase::NaNstate || b.state_ == RWDecimalBase::NaNstate)
   return RWDecimalBase::NaNstate;

 if(a.state_ == RWDecimalBase::nullstate)
   return b.state_;

 if(b.state_ == RWDecimalBase::nullstate)
   return a.state_;

 return RWDecimalBase::normal;
}


template< class M >
inline RWDecimal<M> RWDecimal<M>::addEqualExponents(const M &lhs, const M &rhs, int exp, RWDecimalBase::Op op)
{
 M         sumMantissa;
 int       sumExponent  = exp;
 unsigned  numTruncated = M::preciseAdd(lhs, rhs, sumMantissa);


 if(numTruncated == 0)
   return RWDecimal<M>(sumMantissa, sumExponent);

 sumExponent -= numTruncated;

 if(sumExponent < 0)
  {
   RWMsgId  msgid = (op == RWDecimalBase::add) ? RWDCML_ADDOVER : RWDCML_SUBOVER;


   RWDecimal<M>::overflow(op, RWDecimal<M>(lhs, exp), RWDecimal<M>(rhs, exp), RWMessage(msgid, classname));

   return RWDecimal<M>(RWDecimalBase::NaNstate);
  }

 RWMsgId  msgid = (op == RWDecimalBase::add) ? RWDCML_ADDINEXACT : RWDCML_SUBINEXACT;


 RWDecimal<M>::inexact(op, RWDecimal<M>(lhs, exp), RWDecimal<M>(rhs, exp), RWMessage(msgid, classname));

 return RWDecimal<M>(sumMantissa, sumExponent);
}

//---------------------------------------------------------
//
// RWDecimalInit vars that initialize static data in the
// RWDecimal class.
//
//------------------------------------------------------------

// MSVC needs to have have an instance of the RWDecimal class
// instantiated before it can instantiate the RWDecimalInit
// class.
#if defined(_MSC_VER)
static RWDecimal< RWMultiPrecisionInt<1> >     rwDecimal52InstVar_;
static RWDecimal< RWMultiPrecisionInt<2> >     rwDecimal64InstVar_;
static RWDecimal< RWMultiPrecisionInt<3> >     rwDecimal96InstVar_;

static RWDecimalInexactErr< RWMultiPrecisionInt<3> > rwDecimal96InexactErrInstVar_(RWDecimalBase::assign,
                                                                             RWDecimal<RWMultiPrecisionInt<3> >(),
                                                                             RWDecimal<RWMultiPrecisionInt<3> >(),
                                                                             RWMessage(""));

static RWDecimalInexactErr< RWMultiPrecisionInt<2> > rwDecimal64InexactErrInstVar_(RWDecimalBase::assign,
                                                                             RWDecimal<RWMultiPrecisionInt<2> >(),
                                                                             RWDecimal<RWMultiPrecisionInt<2> >(),
                                                                             RWMessage(""));

static RWDecimalInexactErr< RWMultiPrecisionInt<1> > rwDecimal52InexactErrInstVar_(RWDecimalBase::assign,
                                                                             RWDecimal<RWMultiPrecisionInt<1> >(),
                                                                             RWDecimal<RWMultiPrecisionInt<1> >(),
                                                                             RWMessage(""));


static RWDecimalOverflowErr< RWMultiPrecisionInt<3> > rwDecimal96OverflowErrInstVar_(RWDecimalBase::assign,
                                                           RWDecimal< RWMultiPrecisionInt<3> >(),
                                                           RWDecimal< RWMultiPrecisionInt<3> >(),
                                                           RWMessage(""));

static RWDecimalOverflowErr< RWMultiPrecisionInt<2> > rwDecimal64OverflowErrInstVar_(RWDecimalBase::assign,
                                                           RWDecimal< RWMultiPrecisionInt<2> >(),
                                                           RWDecimal< RWMultiPrecisionInt<2> >(),
                                                           RWMessage(""));

static RWDecimalOverflowErr< RWMultiPrecisionInt<1> > rwDecimal52OverflowErrInstVar_(RWDecimalBase::assign,
                                                           RWDecimal< RWMultiPrecisionInt<1> >(),
                                                           RWDecimal< RWMultiPrecisionInt<1> >(),
                                                           RWMessage(""));

#endif

// The purpose of the init class is to intialize the static constants
// null, missing and NaN.  g++ doesn't use these constants, so don't
// try and initialize them.
#if !defined(RW_NO_STATIC_TEMPLATE_ATTRIBUTES)
static RWDecimalInit< RWMultiPrecisionInt<1> >  rwDecimal52InitVar_;
static RWDecimalInit< RWMultiPrecisionInt<2> >  rwDecimal64InitVar_;
static RWDecimalInit< RWMultiPrecisionInt<3> >  rwDecimal96InitVar_;
#endif


// Arithmetic operators -----------------------------------------------------------------
template<class M>
inline RWDecimal<M> rwdcmlexport operator+(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(a, b, RWDecimalBase::add);
}
  
template<class M>
inline RWDecimal<M> rwdcmlexport operator+(const RWDecimal<M> &a, int b)
{
 return RWDecimal<M>::decplus(a, RWDecimal<M>(b), RWDecimalBase::add);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator+(const RWDecimal<M> &a, long b)
{
 return RWDecimal<M>::decplus(a, RWDecimal<M>(b), RWDecimalBase::add);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator+(const RWDecimal<M> &a, __int64 b)
{
 return RWDecimal<M>::decplus(a, RWDecimal<M>(b), RWDecimalBase::add);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator+(int a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(RWDecimal<M>(a), b, RWDecimalBase::add);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator+(long a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(RWDecimal<M>(a), b, RWDecimalBase::add);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator-(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(a, -b, RWDecimalBase::sub);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator-(const RWDecimal<M> &a, int b)
{
 return RWDecimal<M>::decplus(a, RWDecimal<M>(-b), RWDecimalBase::sub);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator-(const RWDecimal<M> &a, long b)
{
 return RWDecimal<M>::decplus(a, RWDecimal<M>(-b), RWDecimalBase::sub);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator-(int a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(RWDecimal<M>(a), -b, RWDecimalBase::sub);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator-(long a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::decplus(RWDecimal<M>(a), -b, RWDecimalBase::sub);
}

template< class M >
inline RWDecimal<M> rwdcmlexport operator*(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::multiply(a, b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(double a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::multiply(RWDecimal<M>::from(a), b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(const RWDecimal<M> &a, double b)
{
 return RWDecimal<M>::multiply(a, RWDecimal<M>::from(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(const RWDecimal<M> &a, int b)
{
 return RWDecimal<M>::multiply(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(const RWDecimal<M> &a, long b)
{
 return RWDecimal<M>::multiply(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(const RWDecimal<M> &a, __int64 b)
{
 return RWDecimal<M>::multiply(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(int a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::multiply(RWDecimal<M>(a), b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator*(long a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::multiply(RWDecimal<M>(a), b);
}


// // I wish someone would explain it to me.  For all the other binary operators
// // conversions happen as they should.  But not the / operator.  e.g.
// //
// //  RWDecimal<M> x("123"), y;
// //  y = x + 2;  // OK. the int 2 is converted to decimal
// //  y = x - 2;  // OK.
// //  y = x * 2;  // OK
// //  y = x / 2;  // Compile error!! not operator / for lhs RWDecimal<M> and rhs int
// //
// // And this is not for just some screwy compiler.  I get this error on all of them!
// // This is the reason for the all the overloads of the / operator.  Two for each
// // conversion that should happen via a ctor.


template< class M >
inline RWDecimal<M> rwdcmlexport operator/(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::divide(a, b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(const RWDecimal<M> &a, int b)
{
 return RWDecimal<M>::divide(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(const RWDecimal<M> &a, long b)
{
 return RWDecimal<M>::divide(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(const RWDecimal<M> &a, __int64 b)
{
 return RWDecimal<M>::divide(a, RWDecimal<M>(b));
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(int a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::divide(RWDecimal<M>(a), b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(long a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::divide(RWDecimal<M>(a), b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(double a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::divide(RWDecimal<M>::from(a), b);
}

template<class M>
inline RWDecimal<M> rwdcmlexport operator/(const RWDecimal<M> &a, double b)
{
 return RWDecimal<M>::divide(a, RWDecimal<M>::from(b));
}

template< class M >
inline int rwdcmlexport operator==(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::equal(a, b);
}

template< class M >
inline int rwdcmlexport operator<(const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::lessThan(a, b);
}

// Don't provide relational operators if using Standard Library and
// namespaces are broken, since the definitions below would be redundant.
#if defined(RW_NO_STL) || (!defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE))
template< class M >
inline int rwdcmlexport operator != (const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return !RWDecimal<M>::equal(a, b);
}

template< class M >
inline int rwdcmlexport operator > (const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return RWDecimal<M>::lessThan(b, a);
}

template< class M >
inline int rwdcmlexport operator >= (const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return !RWDecimal<M>::lessThan(a, b);
}

template< class M >
inline int rwdcmlexport operator <= (const RWDecimal<M> &a, const RWDecimal<M> &b)
{
 return !RWDecimal<M>::lessThan(b, a);
}
#endif

template< class M > 
inline RWDecimal<M> rwdcmlexport pow(const RWDecimal<M> &d, int e)
{
 return RWDecimal<M>::power(d, e);
}

template< class M >
inline RWDecimal<M> rwdcmlexport abs(const RWDecimal<M> &d)
{
 return RWDecimal<M>::absoluteValue(d);
}


// Begin duplicates: AIX compiler does not allow default initializers here.
template<class M>
inline RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, int digits, RWDecimalBase::RoundingMethod m, unsigned int roundDigit)
{
 return RWDecimal<M>::roundDecimal(d, digits, m, roundDigit);
}

template<class M>
inline  RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, int digits, RWDecimalBase::RoundingMethod m)
{
 return RWDecimal<M>::roundDecimal(d, digits, m, 4U);
}

template<class M>
inline  RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m, unsigned int roundDigit)
{
 return RWDecimal<M>::roundDecimal(d, 0, m, roundDigit);
}

template< class M >
inline RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m)
{
 return RWDecimal<M>::roundDecimal(d, 0, m, 4U);
}

template<class M> 
inline  RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, int digits, unsigned int roundDigit)
{
 return RWDecimal<M>::roundDecimal(d, digits, RWDecimalBase::PLAIN, roundDigit);
}

template< class M >
inline RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, int digits)
{
 return RWDecimal<M>::roundDecimal(d, digits, RWDecimalBase::PLAIN, 4U);
}

template<class M> 
inline  RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d, unsigned int roundDigit)
{
 return RWDecimal<M>::roundDecimal(d, 0, RWDecimalBase::PLAIN, roundDigit);
}

template< class M >
inline RWDecimal<M> rwdcmlexport round(const RWDecimal<M> &d)
{
 return RWDecimal<M>::roundDecimal(d, 0, RWDecimalBase::PLAIN, 4U);
}

template< class M >
inline long int rwdcmlexport toInt(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m, unsigned int roundDigit)
{
 return RWDecimal<M>::convertToInt(d, m, roundDigit);
}

template< class M >
inline long int rwdcmlexport toInt(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m)
{
 return RWDecimal<M>::convertToInt(d, m, 4U);
}

template< class M >
inline  long int rwdcmlexport toInt(const RWDecimal<M> &d)
{
 return RWDecimal<M>::convertToInt(d, RWDecimalBase::PLAIN, 4U);
}

template< class M >
inline __int64 rwdcmlexport toBigInt(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m, unsigned int roundDigit)
{
 return RWDecimal<M>::convertToBigInt(d, m, roundDigit);
}

template< class M >
inline __int64 rwdcmlexport toBigInt(const RWDecimal<M> &d, RWDecimalBase::RoundingMethod m)
{
 return RWDecimal<M>::convertToBigInt(d, m, 4U);
}

template< class M >
inline  __int64 rwdcmlexport toBigInt(const RWDecimal<M> &d)
{
 return RWDecimal<M>::convertToBigInt(d, RWDecimalBase::PLAIN, 4U);
}

// End duplicates

template< class M >
inline RWCString rwdcmlexport toString(const RWDecimal<M> &d)
{
 return RWDecimal<M>::convertToString(d);
}

template< class M >
inline long double rwdcmlexport toDouble(const RWDecimal<M> &d)
{
 return RWDecimal<M>::convertToDouble(d);
}

template< class M >
inline RW_SL_IO_STD(ostream) &rwdcmlexport operator<<(RW_SL_IO_STD(ostream) &strm, const RWDecimal<M> &d)
{
 return RWDecimal<M>::shift(strm, d);
}

template< class M >
inline RW_SL_IO_STD(istream) &rwdcmlexport operator>>(RW_SL_IO_STD(istream) &strm, RWDecimal<M> &d)
{
 return RWDecimal<M>::shift(strm, d);
}


#ifdef RW_MONEY_COMPILE_INSTANTIATE
    #include "rw/money/decimal.cc"
#endif

#endif /* __RWDECIMAL_H__ */
