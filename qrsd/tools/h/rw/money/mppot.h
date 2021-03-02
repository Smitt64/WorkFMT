#ifndef __RWMPPOT_H__
#define __RWMPPOT_H__

//***************************************************************************************
//
// RWMPPowersOfTen< int S >
//
//  This class stores a static array of RWMultiPrecision int's (mp ints)representing the powers
//  of ten.   If using mp ints in decimal calculations it is frequently necessary to shift
//  an mp int either right or left by a certain number of decimal digits.  One fairly
//  efficient way of doing this is to divide or multiply the number by the apporpriate
//  power of ten.
//
//  NOTE:  Space for the static array of mp ints is allocated off the free store
//         and, since it is a static data member, is never freed.  Thus memory 
//         tracking utilities will report this as a memory leak.
//
//***************************************************************************************

#include "rw/money/intutil.h"

#include <math.h>
#include <string.h>

#include "rw/rstream.h"
#include "rw/cstring.h"

#include "rw/money/mpint.h"


#ifdef RW_MULTI_THREAD
# include "rw/mutex.h"
#endif

//#ifndef __BORLANDC__

template< int S >
class RWMPPowersOfTen
{
public:
  static RWMultiPrecisionInt<S> power( size_t n )
  {
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWBoolean powerIsInitialized = FALSE;
    static RWMultiPrecisionInt<S> *powers;
    static RWMultiPrecisionInt<S> ten = 10;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

#ifdef RW_MULTI_THREAD
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWMutex powersOfTenLock;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    if (!powerIsInitialized) {
      powersOfTenLock.acquire();
      if (powerIsInitialized)
        powersOfTenLock.release();
    }
#endif // RW_MULTI_THREAD

    if (! powerIsInitialized )
    {
      int numP = numPowers();
      powers = new RWMultiPrecisionInt<S>[numP];
      powers[0] = RWMultiPrecisionInt<S>( 1 );
      for ( int i = 1; i < numP; ++i )
      {
        powers[i] = powers[i-1] * ten;
      }
      powerIsInitialized = TRUE;
#ifdef RW_MULTI_THREAD
      powersOfTenLock.release();
#endif
    }
    return powers[n];
  }

  static int numPowers()
  { 
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static int numPows;
    static RWBoolean numPowersInitialized = FALSE;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    if (! numPowersInitialized )
    {
      numPows = (int)(log10( pow(2.0, (double)(S*sizeof(typename RWMultiPrecisionInt<S>::unsigned_type)*CHAR_BIT - 1)) )) + 1;
      numPowersInitialized = TRUE;
    }
    return numPows;
  }  

private:
#ifdef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
  static RWBoolean powerIsInitialized;
  static RWBoolean numPowersInitialized;
  static RWMultiPrecisionInt<S>* powers;
  static RWMultiPrecisionInt<S>  ten;
  static int numPows;
#ifdef RW_MULTI_THREAD
  static RWMutex powersOfTenLock;
#endif
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
};

//#endif //__BORLANDC__


//***************************************************************************************
//
// RWMPPowersOfTen< 2 >
//
//***************************************************************************************

RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWMPPowersOfTen<2>
{
public:
  static RWMultiPrecisionInt<2> power( size_t n )
  {
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWBoolean powerIsInitialized = FALSE;
    static RWMultiPrecisionInt<2> powers[19];
    static RWMultiPrecisionInt<2> ten = 10;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

#ifdef RW_MULTI_THREAD
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWMutex powersOfTenLock;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    if (!powerIsInitialized) {
      powersOfTenLock.acquire();
      if (powerIsInitialized)
        powersOfTenLock.release();
    }
#endif // RW_MULTI_THREAD

    if (! powerIsInitialized )
    {
      int numP = numPowers();
      powers[0] = RWMultiPrecisionInt<2>( 1 );
      for ( int i = 1; i < numP; ++i )
      {
        powers[i] = powers[i-1] * ten;
      }
      powerIsInitialized = TRUE;
#ifdef RW_MULTI_THREAD
      powersOfTenLock.release();
#endif
    }
    return powers[n];
  }

  static int numPowers()
  {
    return 19;
  }

private:
#ifdef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
  static RWBoolean powerIsInitialized;
  static RWMultiPrecisionInt<2>  powers[19];
  static RWMultiPrecisionInt<2>  ten;
#ifdef RW_MULTI_THREAD
  static RWMutex powersOfTenLock;
#endif
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
};


//***************************************************************************************
//
// RWMPPowersOfTen< 3 >
//
//***************************************************************************************

RW_MONEY_NEW_TEMPLATE_SYNTAX
class RWMPPowersOfTen<3>
{
public:
  static RWMultiPrecisionInt<3> power( size_t n )
  {
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWBoolean powerIsInitialized = FALSE;
    static RWMultiPrecisionInt<3> powers[29];
    static RWMultiPrecisionInt<3> ten = 10;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

#ifdef RW_MULTI_THREAD
#ifndef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    static RWMutex powersOfTenLock;
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
    if (!powerIsInitialized) {
      powersOfTenLock.acquire();
      if (powerIsInitialized)
        powersOfTenLock.release();
    }
#endif // RW_MULTI_THREAD

    if (! powerIsInitialized )
    {
      int numP = numPowers();
      powers[0] = RWMultiPrecisionInt<3>( 1 );
      for ( int i = 1; i < numP; ++i )
      {
        powers[i] = powers[i-1] * ten;
      }
      powerIsInitialized = TRUE;
#ifdef RW_MULTI_THREAD
      powersOfTenLock.release();
#endif
    }
    return powers[n];
  }

  static int numPowers()
  {
    return 29;
  }

private:
#ifdef RW_NO_STATIC_VAR_IN_STATIC_MEMBER
  static RWBoolean powerIsInitialized;
  static RWMultiPrecisionInt<3>  powers[29];
  static RWMultiPrecisionInt<3>  ten;
#ifdef RW_MULTI_THREAD
  static RWMutex powersOfTenLock;
#endif
#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER
};


#ifdef RW_MONEY_COMPILE_INSTANTIATE
#  include "rw/money/mppot.cc"
#endif

#endif  // __RWMPPOT_H__
