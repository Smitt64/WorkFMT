#ifndef __RWRSTREAM_H__
#define __RWRSTREAM_H__

/*
 * Includes either stream.h or iostream.h, depending
 * on the compiler.
 *
 * $Id: rstream.h@#/main/13  09/04/97 15:23:01  griswolf (TOOLS707_WIN32_971021)
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
 * $Log: rstream.h,v $
 * Revision 7.4  1996/08/14 23:10:25  griswolf
 * Add suport for standard iostream header.
 *
 * Revision 7.3  1995/10/26 17:27:40  pearson
 * Remove .hpp includes for Zortech compiler
 *
 * Revision 7.2  1995/09/05  19:27:21  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  02:54:21  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/02  18:22:13  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:10:41  vriezen
 * Move all files to 6.1
 *
 * Revision 2.5  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.4  1993/03/13  01:53:16  keffer
 * Moved OPEN[IO]STREAM to backward.h
 *
 * Revision 2.2  1992/11/15  22:12:09  keffer
 * Removed use of macro NL
 *
 * Revision 2.1  1992/11/04  23:03:15  myersn
 * *** empty log message ***
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.8   04 Mar 1992 10:20:22   KEFFER
 * Includes "rw/defs.h" instead of "rw/compiler.h"
 * 
 *    Rev 1.7   18 Feb 1992 09:54:36   KEFFER
 * 
 *    Rev 1.6   27 Oct 1991 17:39:06   keffer
 * Corrected Glock inclusion of iostream.hxx
 * 
 *    Rev 1.5   17 Oct 1991 09:12:56   keffer
 * Changed include path to <rw/xxx.h>
 * 
 *    Rev 1.3   24 Sep 1991 11:08:10   keffer
 * Zortech now includes iostream.hpp; Glock iostream.hxx.
 * 
 *    Rev 1.2   08 Sep 1991 19:10:08   keffer
 * Changed name of eatwhite() to rwEatwhite()
 * 
 *    Rev 1.1   24 Jul 1991 13:06:46   keffer
 * Added pvcs keywords
 *
 */

#include "rw/defs.h"

#  ifdef __GLOCK__
#    include <iostream.hxx>
#  else
#    ifndef RW_NO_IOSTD
#      include <iostream>
#    else
#      include <iostream.h>
#    endif
#  endif

inline RW_SL_IO_STD(istream)& rwEatwhite(RW_SL_IO_STD(istream)& s) {return s >> RW_SL_IO_STD(ws);}

#ifdef RW_BOOL_NEEDS_IO

inline RW_SL_IO_STD(ostream)& operator<<(RW_SL_IO_STD(ostream)& os, bool b) 
    { os << ( b ? '1' : '0'); return os;}
inline RW_SL_IO_STD(istream)& operator>>(RW_SL_IO_STD(istream)& is, bool &b) 
    { char c; is >> c; b = (c == '1' ? true : false); return is;}
#endif

#endif /* __RWRSTREAM_H__ */
