#ifndef __RWDECPORT_H__
#define __RWDECPORT_H__

/***************************************************************************
 *
 * RWDecimalPortable: portable decimal representation, used for I/O and
 *                    as an intermediate in conversions
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
 ***************************************************************************
 *
 * This class represents an arbitrary precision decimal fraction.  The
 * representation is not exposed via member functions for accessing the
 * strings representing the pieces before and after the decimal.  This
 * is done because it might make sense to one day replace this implementation
 * with an arbitrary precision integer based implementation.
 *
 * The concept of trailing zeros after the decimal place is supported. If
 * you construct an RWDecimalPortable using the string constructor and have
 * trailing zeros (eg 1.200), then you are guaranteed that the string conversion
 * operator will return a string with these trailing zeros.  This is necessary
 * for correct conversions between fixed decimal types in Money.h++.  Also,
 * rounding supports the concept of trailing zeros, so round("1.2",2) yields
 * 1.20.  The math functions (+-*) truncate trailing zeros.  Use trimZeros()
 * to strip off the excess zeros.  Note that because trailing zeroes are 
 * allowed the relation x==y does not guarantee that RWCString(x)==RWCString(y)
 * since "1.200"=="1.2".
 *
 * Input note: for details on allowable input syntax, see RWDecimalParser.
 *
 **************************************************************************/

#include "rw/cstring.h"
#include "rw/money/decdefs.h"
#include "rw/decbase.h"


// If using standard library, include <utility> for possible
// relational operator definitions.
#ifndef RW_NO_STL
# include <utility>
#endif


class RWDCMLExport RWDecimalPortable : public RWDecimalBase {
private:
  // Private constructor
    RWDecimalPortable( State s ) { state_ = s; }
    
public:

  static const RWDecimalPortable null;        // prototype values, use with op= and op==
  static const RWDecimalPortable missing;
  static const RWDecimalPortable NaN;
  
  RWDecimalPortable();                     // initializes to zero
  // RWDecimalPortable(const RWDecimalPortable&);  default is ok.
  // RWDecomalPortable& operator=(const RWDecimalPortable&); default is ok.
  RWDecimalPortable(const char *);         // parses input or sets to NaN if unparseable
  
  RWDecimalPortable(long);
  RWDecimalPortable(int);

  operator RWCString() const;          // format is, eg, -23.21 or 0.32 or 432

  size_t   numSignificantDigits() const; // Number of digits. Does not count leading
                                         // 0's (for numbers < 1), minus signs or
                                         // decimal points.
    
  RWBoolean         isNumber() const   {return state_ == normal;}
  State             state() const      {return state_;}
  void              setState(State);
  void              trimZeros();       // Remove leading zeros before decimal point and trailers after

  friend RWDecimalPortable rwdcmlexport
                    round(const RWDecimalPortable&, int digits, 
        RoundingMethod=RWDecimalBase::PLAIN);
  friend double rwdcmlexport toDouble(const RWDecimalPortable&);

// RWCollectable compliant member functions

  RWspace   binaryStoreSize() const;
  unsigned  hash() const;

#ifndef RW_NO_TOOLS
  void      restoreFrom(RWvistream&);
  void      restoreFrom(RWFile&);
  void      saveOn(RWvostream&) const;
  void      saveOn(RWFile&) const;
#endif //RW_NO_TOOLS

// Rudimentary math functions.  These are not very efficient.
// For efficiency, use one of the decimal classes of Money.h++
// These are very useful because they provide a
// second implementation of arithmetic which can be used for testing
// purposes with the RWDecimal classes.  They could also be used as a
// poor man's arbitrary precision decimal class.

friend RWDecimalPortable rwdcmlexport operator-(const RWDecimalPortable&);
friend RWDecimalPortable rwdcmlexport operator+(const RWDecimalPortable&, const RWDecimalPortable&);
friend RWDecimalPortable rwdcmlexport operator-(const RWDecimalPortable&, const RWDecimalPortable&);
friend RWDecimalPortable rwdcmlexport operator*(const RWDecimalPortable&, const RWDecimalPortable&);

// Relational operators.
//
// == and < are implemented the hard way.  All the rest are inline functions
// which refer back to these two.
friend RWBoolean rwdcmlexport operator==(const RWDecimalPortable& x, const RWDecimalPortable& y) ;
friend RWBoolean rwdcmlexport operator<(const RWDecimalPortable& x, const RWDecimalPortable& y) ;


private:
  RWBoolean negative_;      // TRUE indicates number less than zero
  RWCString before_;        // digits before the decimal
  RWCString after_;         // digits after the decimal

  // Implementation helper functions
  static RWDecimalPortable plus(const RWDecimalPortable& x, const RWDecimalPortable& y);
  static RWDecimalPortable minus(const RWDecimalPortable& x, const RWDecimalPortable& y);
friend class RWExport RWDecimalParser;
friend class RWDCMLExport RWDecimalPortableInit;

//#if defined(__GNUG__)
    // These are a workaround for a g++ bug DON'T USE THEM in user code
//    static RWDecimal64    nullValue();
//    static RWDecimal64    missingValue();
//    static RWDecimal64    NaNValue();
//#endif

};

/*
 * I/O.  All RWDecimal I/O is done via RWDecimalPortable objects.
 */

#ifndef RW_TRAILING_RWEXPORT
rwdcmlexport RW_SL_IO_STD(ostream&) operator<<(RW_SL_IO_STD(ostream&),const RWDecimalPortable&);
rwdcmlexport RW_SL_IO_STD(istream&) operator>>(RW_SL_IO_STD(istream&),RWDecimalPortable&);
#else
RW_SL_IO_STD(ostream&) rwdcmlexport operator<<(RW_SL_IO_STD(ostream&),const RWDecimalPortable&);
RW_SL_IO_STD(istream&) rwdcmlexport operator>>(RW_SL_IO_STD(istream&),RWDecimalPortable&);
#endif


// If using the standard library with broken namespace, then these functions
// are in <utility>.  Otherwise they are here.
#if defined(RW_NO_STL) || ( !defined(RW_NO_STL) && !defined(RWSTD_NO_NAMESPACE) )
RWBoolean rwdcmlexport operator!=(const RWDecimalPortable& x, const RWDecimalPortable& y);

RWBoolean rwdcmlexport operator>=(const RWDecimalPortable& x, const RWDecimalPortable& y);

RWBoolean rwdcmlexport operator>(const RWDecimalPortable& x, const RWDecimalPortable& y);

RWBoolean rwdcmlexport operator<=(const RWDecimalPortable& x, const RWDecimalPortable& y);
#endif


/*
 * The RWDecimalPortableInit class initializes static variables before user code
 * using the iostream trick of putting a static init object in every
 * translation unit and initializing the first time one is constructed.
 * The implementation data structures and static vars NaN, null, missing
 * are initialized.
 */

class RWDCMLExport RWDecimalPortableInit
{
  static int count;
public:
  RWDecimalPortableInit();
};

static RWDecimalPortableInit RWDecimalPortableInitVar;


#endif







