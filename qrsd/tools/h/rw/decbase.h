#ifndef __RW_DECBASE_H__
#define __RW_DECBASE_H__

/***************************************************************************
 *
 * decbase.h -- RWDecimalBase class
 *   provide a scope for the enums used in the different RWDecimal classes.
 *
 * $Id: decbase.h@#/main/4  11/11/97 10:47:46  steves (MNY150_971113_WIN)
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

#include "rw/defs.h"

/*
 * RWDecimalBase class
 *
 * The basic use of this class is to provide a scope for the enums used
 * in the different RWDecimal classes.
 *
 * The member state_ would, in a pure design sense, be better included with
 * each individual RWDecimal class so that RWDecimalBase existed purely for
 * scoping.  The trouble with a class purely for scoping is that the compiler
 * will put a dummy variable in it to give it non-zero size.  Obviously, this
 * just won't do, so state_ is included since it is used in all the decimal
 * classes anyway.
 */

class RWExport RWDecimalBase {
public:
  enum RoundingMethod { PLAIN=0, UP=1, DOWN=2, BANKERS=3, TRUNCATE=2 };
  enum State          { normal = 0, nullstate = 1, NaNstate = 2, ctorError = 4,
                        missingstate = 8 };
  enum Op             { assign, add, sub, mult, div, powop, pow10op, conversion };

  /*
   * Now the one data member, used mostly just to give the class non-zero size
   */
protected:
  State state_;
};

#endif
