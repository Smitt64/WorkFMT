#ifndef __RWDCMLERR_H__
#define __RWDCMLERR_H__

/***************************************************************************
 *
 * Money.h++ error messages and exception classes
 *
 * $Id: dcmlerr.h@#/main/6  11/10/97 10:28:12  brianb (MNY150_971113_WIN)
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
 **************************************************************************/

#include "rw/money/decdefs.h"
#include "rw/message.h"
#include "rw/rwerr.h"
#include "rw/decbase.h"

#ifndef RW_NO_CPP_RECURSION
#define RWDCML_FROMDBLOVER     RWDCML_FROMDBLOVER()
#define RWDCML_FROMDBLINEXACT  RWDCML_FROMDBLINEXACT()
#define RWDCML_FROMLDBLOVER    RWDCML_FROMLDBLOVER()
#define RWDCML_FROMLDBLINEXACT RWDCML_FROMLDBLINEXACT()
#define RWDCML_FROMPORTOVER    RWDCML_FROMPORTOVER()
#define RWDCML_FROMPORTINEXACT RWDCML_FROMPORTINEXACT()
#define RWDCML_ASSIGNOVER      RWDCML_ASSIGNOVER()
#define RWDCML_ADDOVER         RWDCML_ADDOVER()
#define RWDCML_SUBOVER         RWDCML_SUBOVER()
#define RWDCML_MULOVER         RWDCML_MULOVER()
#define RWDCML_DIVOVER         RWDCML_DIVOVER()
#define RWDCML_ADDINEXACT      RWDCML_ADDINEXACT()
#define RWDCML_SUBINEXACT      RWDCML_SUBINEXACT()
#define RWDCML_MULINEXACT      RWDCML_MULINEXACT()
#define RWDCML_DIVINEXACT      RWDCML_DIVINEXACT()
#define RWDCML_POWRANGE        RWDCML_POWRANGE ()
#define RWDCML_FORMATSDIFF     RWDCML_FORMATSDIFF()
#else
#define RWDCML_FROMDBLOVER     rwdcml_FROMDBLOVER()
#define RWDCML_FROMDBLINEXACT  rwdcml_FROMDBLINEXACT()
#define RWDCML_FROMLDBLOVER    rwdcml_FROMLDBLOVER()
#define RWDCML_FROMLDBLINEXACT rwdcml_FROMLDBLINEXACT()
#define RWDCML_FROMPORTOVER    rwdcml_FROMPORTOVER()
#define RWDCML_FROMPORTINEXACT rwdcml_FROMPORTINEXACT()
#define RWDCML_ASSIGNOVER      rwdcml_ASSIGNOVER()
#define RWDCML_ADDOVER         rwdcml_ADDOVER()
#define RWDCML_SUBOVER         rwdcml_SUBOVER()
#define RWDCML_MULOVER         rwdcml_MULOVER()
#define RWDCML_DIVOVER         rwdcml_DIVOVER()
#define RWDCML_ADDINEXACT      rwdcml_ADDINEXACT()
#define RWDCML_SUBINEXACT      rwdcml_SUBINEXACT()
#define RWDCML_MULINEXACT      rwdcml_MULINEXACT()
#define RWDCML_DIVINEXACT      rwdcml_DIVINEXACT()
#define RWDCML_POWRANGE        rwdcml_POWRANGE ()
#define RWDCML_FORMATSDIFF     rwdcml_FORMATSDIFF()
#endif // RW_NO_CPP_RECUSRION


#ifdef RW_TRAILING_RWEXPORT
RWMsgId rwdcmlexport RWDCML_FROMDBLOVER;
RWMsgId rwdcmlexport RWDCML_FROMDBLINEXACT;
RWMsgId rwdcmlexport RWDCML_FROMLDBLOVER;
RWMsgId rwdcmlexport RWDCML_FROMLDBLINEXACT;
RWMsgId rwdcmlexport RWDCML_FROMPORTOVER;
RWMsgId rwdcmlexport RWDCML_FROMPORTINEXACT;
RWMsgId rwdcmlexport RWDCML_ASSIGNOVER;
RWMsgId rwdcmlexport RWDCML_ADDOVER;
RWMsgId rwdcmlexport RWDCML_SUBOVER;
RWMsgId rwdcmlexport RWDCML_MULOVER;
RWMsgId rwdcmlexport RWDCML_DIVOVER;
RWMsgId rwdcmlexport RWDCML_ADDINEXACT;
RWMsgId rwdcmlexport RWDCML_SUBINEXACT;
RWMsgId rwdcmlexport RWDCML_MULINEXACT;
RWMsgId rwdcmlexport RWDCML_DIVINEXACT;
RWMsgId rwdcmlexport RWDCML_POWRANGE;
RWMsgId rwdcmlexport RWDCML_FORMATSDIFF;
#else
rwdcmlexport RWMsgId RWDCML_FROMDBLOVER;
rwdcmlexport RWMsgId RWDCML_FROMDBLINEXACT;
rwdcmlexport RWMsgId RWDCML_FROMLDBLOVER;
rwdcmlexport RWMsgId RWDCML_FROMLDBLINEXACT;
rwdcmlexport RWMsgId RWDCML_FROMPORTOVER;
rwdcmlexport RWMsgId RWDCML_FROMPORTINEXACT;
rwdcmlexport RWMsgId RWDCML_ASSIGNOVER;
rwdcmlexport RWMsgId RWDCML_ADDOVER;
rwdcmlexport RWMsgId RWDCML_SUBOVER;
rwdcmlexport RWMsgId RWDCML_MULOVER;
rwdcmlexport RWMsgId RWDCML_DIVOVER;
rwdcmlexport RWMsgId RWDCML_ADDINEXACT;
rwdcmlexport RWMsgId RWDCML_SUBINEXACT;
rwdcmlexport RWMsgId RWDCML_MULINEXACT;
rwdcmlexport RWMsgId RWDCML_DIVINEXACT;
rwdcmlexport RWMsgId RWDCML_POWRANGE;
rwdcmlexport RWMsgId RWDCML_FORMATSDIFF;
#endif //RW_TRAILING_RWEXPORT


#endif // __RWDCMLERR_H__

