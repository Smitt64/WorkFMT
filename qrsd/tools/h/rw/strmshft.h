#ifndef __RW_VSTREAM_SHIFT_H__
#define __RW_VSTREAM_SHIFT_H__

/*
 * Macros for use with virtual streams
 *
 * $Id: strmshft.h@#/main/8  08/20/97 17:53:32  kevinj (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
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
 * The following macros are provided for use when compilers complain of
 * ambiguity between RWvostream,Collectable and RWderivedStream,builtin.
 * Rogue Wave "vstreamable" class headers now use these macros to provide
 * complete dis-ambiguation for the streams and classes that ship with
 * the Tools.h++ library.
 *
 * Since the functions are global inlines, the macros should be expanded
 * in a header file that can be #included only once per compilation unit.
 * 
 *  // Here is an example
 *  // You need not provide a full declaration of the stream class here
 *  class RWExport myOStream; // derived from RWvostream
 *
 *  // You need not provide a full declaration of the insertable class here
 *  class RWExport myClass;   // can be inserted into an RWvostream
 *
 *  // use the macro to provide an inline definition of the << operator
 *  RW_PROVIDE_DVSTREAM_INSERTER(myOStream,myClass)
 *
 * Similar code can be used to provide extractor methods.
 */

#include "rw/compiler.h"

class RWExport RWvostream;
class RWExport RWvistream;
#define RW_PROVIDE_DVSTREAM_INSERTER(DerivedOstream,vstreamable)         \
inline RWvostream& operator<<(DerivedOstream& dos, const vstreamable& ct)\
        { return RW_AWFUL_OCAST(dos) << ct; }

#define RW_PROVIDE_DVSTREAM_EXTRACTOR(DerivedIstream,vstreamable)        \
inline RWvistream& operator>>(DerivedIstream& dis, vstreamable& ct)      \
        { return RW_AWFUL_ICAST(dis) >> ct; }

#endif /* __RW_VSTREAM_SHIFT_HELP__ */
