#ifndef __RWMESSAGE_H__
#define __RWMESSAGE_H__

/*
 * Message string facility
 *
 * $Id: message.h@#/main/9  05/07/96 22:49:52  kyle (TOOLS707_WIN32_971021)
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
 * $Log: message.h,v $
 * Revision 7.4  1996/05/07 22:49:52  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.3  1996/02/18 01:45:19  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.2  1995/09/05 19:10:54  jims
 * Use new copyright macro
 *
 * Revision 7.1  1994/10/16  02:52:11  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/05/16  18:12:19  jims
 * Port to Win32 DLL
 *
 * Revision 6.1  1994/04/15  19:10:30  vriezen
 * Move all files to 6.1
 *
 * Revision 1.3  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 1.2  1993/05/31  21:45:26  keffer
 * New messaging architecture for localization
 *
 * Revision 1.1  1993/05/18  00:41:12  keffer
 * Initial revision
 */

#include "rw/defs.h"

struct RWCatMsg
{
  const char* domainName_;      // Called "Catalog Name" by catgets()
  int         msgNumber_;       // Message number
  const char* msg_;             // Default string (catgets())
                                //   or lookup key (gettext())
};

typedef const RWCatMsg* RWMsgId;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWMessage
{
public:
  RWMessage(const char*);
  RWMessage(RWMsgId ...);
  RWMessage(const RWMessage&);
  ~RWMessage();
  RWMessage& operator=(const RWMessage&);
  const char* str() const {return msg_;}
private:
  char* msg_;
};

#ifndef RW_TRAILING_RWEXPORT
rwexport const char* rwMsgLookup(RWMsgId msgId);
#else
const char* rwexport rwMsgLookup(RWMsgId msgId);
#endif

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif /* __RWMESSAGE_H__ */
