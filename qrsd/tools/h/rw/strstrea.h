#ifndef __RWSTRSTREA_H__
#define __RWSTRSTREA_H__

/***************************************************************************
 *
 * strstrea.h
 * Includes either strstream.h or strstrea.h, depending
 * on the compiler.
 *
 * $Id: strstrea.h@#/main/5  04/16/97 17:47:43  griswolf (MNY150_971113_WIN)
 * $Id: strstrea.h,v 1.4 1996/03/25 19:41:47 tibbi Exp $
 *
 ***************************************************************************
 *
 * (c) Copyright 1991-1997 Rogue Wave Software, Inc.
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
 * This file exists because on the Microsoft NT compiler, trying
 * to include <strstream.h> fails.  For some stupid reason, the
 * NT compiler doesn't find strstrea.h, even though the headers are
 * stored on a fat file system.
 *
 ***************************************************************************
 *
 * $Log: strstrea.h,v $
 * Revision 1.2.1.2  1996/05/20 19:53:34  bass
 * check for __WIN16__ or __MSDOS__ needed if compiling under Win95
 *
 * Revision 1.2.1.1  1996/05/15 20:32:26  bass
 * start new branch for Math.h++ 5.2
 *
 * Revision 1.4  1996/03/25 19:41:47  tibbi
 * added the copyright macros with the mathyrs.str file
 *
 * Revision 1.3  1995/08/10 18:23:43  steves
 * add OS/2 to the list of os's that need to include strstrea.h
 *
 * Revision 1.2  1993/10/04  21:44:59  alv
 * ported to Windows NT
 *
 * Revision 1.1  1993/10/01  22:20:09  alv
 * Initial revision
 *
 **************************************************************************/

#include "rw/defs.h"   /* Need to know about __WIN32__ */

#ifndef RW_NO_IOSTD
#  include <strstream>
#else
#  if defined(__WIN32__) || defined(__WIN16__) || defined(__MSDOS__) || defined(__OS2__)
#    include <strstrea.h>
#  else
#    include <strstream.h>
#  endif
#endif

#endif /* __RWSTRSTREA_H__ */
