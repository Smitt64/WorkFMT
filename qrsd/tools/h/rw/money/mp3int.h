/*--------------------------------------------------------------
 *
 * Type definition for convenience
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

#ifndef __RWMP3INT_H__
#define __RWMP3INT_H__

#include "rw/money/mpint.h"

typedef RWMultiPrecisionInt<3> RWMP3Int;

// Some compilers will not generate symbols for these static variable unless you
// make a specific reference to them in the compilation unit.
#if defined(__TURBOC__)
static RWMultiPrecisionInt<3> __RWStaticInitMaxValVar3 = RWMultiPrecisionInt<3>::maxValue();
static RWMultiPrecisionInt<3> __RWStaticInitMinValVar3 = RWMultiPrecisionInt<3>::minValue();
static long int __RWStaticInitSTypeMaxVar3 = RWMultiPrecisionInt<3>::SIGNED_TYPE_MAX;
static long int __RWStaticInitSTypeMinVar3 = RWMultiPrecisionInt<3>::SIGNED_TYPE_MIN;
static unsigned long int __RWStaticInitUSTypeMaxVar3 = RWMultiPrecisionInt<3>::UNSIGNED_TYPE_MAX;
static unsigned long int __RWStaticInitUSTypeMinVar3 = RWMultiPrecisionInt<3>::UNSIGNED_TYPE_MIN;
#endif

#endif  // __RWMP3INT_H__
