/*--------------------------------------------------------------
 *
 * Intialization of constants used by RWMultiPrecisionInt
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

#ifndef __RWUTLCONST_CC__
#define __RWUTLCONST_CC__



// sizeof() is not evaluated at compile time; set constants
// at run time.
template <class T>
RW_MNY_STATIC_CONST_DEF size_t RWIntutilConstants<T>::HWDigitSize 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= sizeof(T)*CHAR_BIT/2;
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::LowerMask 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( ~(T(~0) << RWIntutilConstants<T>::HWDigitSize) );
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::UpperMask 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( T(~0) << RWIntutilConstants<T>::HWDigitSize );
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::HWBorrowBit 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( (T(1) << RWIntutilConstants<T>::HWDigitSize) );
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::MaxDigitInt
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( ~0 );
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::HighDigitBit 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( T(~0) << (sizeof(T)*CHAR_BIT - 1) );
#endif

template <class T>
RW_MNY_STATIC_CONST_DEF T RWIntutilConstants<T>::LowDigitBit 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= T( ~(~0 << 1) );
#endif

template <class T>
int RWIntutilConstants<T>::count 
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
;
#else
= 0;
#endif


#if !defined(__DECCXX)
#if !defined(__hppa) || defined(HP_ANSI)
// HP-CC and DEC don't need this initialization
template <class T>
RWIntutilConstants<T>::RWIntutilConstants()
{
#ifdef RW_MNY_NO_EXPLICIT_INSTANTIATION
  count = 0;
#endif
  if (count++ == 0)
  {
     *(size_t *)&RWIntutilConstants<T>::HWDigitSize = sizeof(T)*CHAR_BIT/2;
     *(T *)&RWIntutilConstants<T>::LowerMask = T( ~(T(~0) << RWIntutilConstants<T>::HWDigitSize) );
     *(T *)&RWIntutilConstants<T>::UpperMask = T( T(~0) << RWIntutilConstants<T>::HWDigitSize );
     *(T *)&RWIntutilConstants<T>::HWBorrowBit = T( (T(1) << RWIntutilConstants<T>::HWDigitSize) );
     *(T *)&RWIntutilConstants<T>::MaxDigitInt = T( ~0 );
     *(T *)&RWIntutilConstants<T>::HighDigitBit = T( T(~0) << (sizeof(T)*CHAR_BIT - 1) );
     *(T *)&RWIntutilConstants<T>::LowDigitBit = T( ~(~0 << 1) );
  }
}
#  endif
#endif

#endif // __RWUTLCONST_CC__
