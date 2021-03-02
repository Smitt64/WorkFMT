#ifndef __RWMPPOT_CC__
#define __RWMPPOT_CC__
/***********************************************************************
 *
 * Implement RWMPPowersOfTen< int S > static variables if appropriate.
 *
 * $Id: //money/ed6-rel/rw/currency/mppot.cc#1 $
 *
 * Copyright (c) 1993, 1996-2003 Rogue Wave Software, Inc.  All Rights Reserved.
 *
 * This computer software is owned by Rogue Wave Software, Inc. and is
 * protected by U.S. copyright laws and other laws and by international
 * treaties.  This computer software is furnished by Rogue Wave Software,
 * Inc. pursuant to a written license agreement and may be used, copied,
 * transmitted, and stored only in accordance with the terms of such
 * license and with the inclusion of the above copyright notice.  This
 * computer software or any other copies thereof may not be provided or
 * otherwise made available to any other person.
 *
 * U.S. Government Restricted Rights.  This computer software is provided
 * with Restricted Rights.  Use, duplication, or disclosure by the
 * Government is subject to restrictions as set forth in subparagraph (c)
 * (1) (ii) of The Rights in Technical Data and Computer Software clause
 * at DFARS 252.227-7013 or subparagraphs (c) (1) and (2) of the
 * Commercial Computer Software - Restricted Rights at 48 CFR 52.227-19,
 * as applicable.  Manufacturer is Rogue Wave Software, Inc., 5500
 * Flatiron Parkway, Boulder, Colorado 80301 USA.
 *
 ***********************************************************************/

#ifdef RW_NO_STATIC_VAR_IN_STATIC_MEMBER

//#ifndef __BORLANDC__

//*****************************************************************************************
//
// Class RWMPPowersOfTen< S >
//
//*****************************************************************************************
template< int S >
RWBoolean RWMPPowersOfTen<S>::powerIsInitialized = FALSE;

template< int S >
RWMultiPrecisionInt<S>* RWMPPowersOfTen<S>::powers = rwnil;

template< int S >
int RWMPPowersOfTen<S>::numPows = 0;

template< int S >
RWMultiPrecisionInt<S> RWMPPowersOfTen<S>::ten = RWMultiPrecisionInt<S>( 10 );

template< int S >
RWBoolean RWMPPowersOfTen<S>::numPowersInitialized = FALSE;

#ifdef RW_MULTI_THREAD
template< int S >
RWMutex RWMPPowersOfTen<S>::powersOfTenLock = RWMutex();
#endif

//#endif //__BORLANDC__


//*****************************************************************************************
//
// Class RWMPPowersOfTen< 2 >
//
//*****************************************************************************************
RW_MONEY_NEW_TEMPLATE_SYNTAX
RWBoolean RWMPPowersOfTen<2>::powerIsInitialized = FALSE;

RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMultiPrecisionInt<2> RWMPPowersOfTen<2>::powers[19];

RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMultiPrecisionInt<2> RWMPPowersOfTen<2>::ten = RWMultiPrecisionInt<2>( 10 );

#ifdef RW_MULTI_THREAD
RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMutex RWMPPowersOfTen<2>::powersOfTenLock = RWMutex();
#endif

//*****************************************************************************************
//
// Class RWMPPowersOfTen< 3 >
//
//*****************************************************************************************
RW_MONEY_NEW_TEMPLATE_SYNTAX
RWBoolean RWMPPowersOfTen<3>::powerIsInitialized = FALSE;

RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMultiPrecisionInt<3> RWMPPowersOfTen<3>::powers[29];

RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMultiPrecisionInt<3> RWMPPowersOfTen<3>::ten = RWMultiPrecisionInt<3>( 10 );

#ifdef RW_MULTI_THREAD
RW_MONEY_NEW_TEMPLATE_SYNTAX
RWMutex RWMPPowersOfTen<3>::powersOfTenLock = RWMutex();
#endif

#endif // RW_NO_STATIC_VAR_IN_STATIC_MEMBER

#endif // __RWMPPOT_CC__
