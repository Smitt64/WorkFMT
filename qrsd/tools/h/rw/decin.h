#ifndef __RWDECIN_H__
#define __RWDECIN_H__

/***************************************************************************
 *
 * decin.h -- RWDecimalParser: parse decimal fractions from an input stream
 *
 * $Id: decin.h@#/main/1  10/13/97 12:19:42  steves (MNY150_971113_WIN)
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


#include "rw/decport.h"


class RWDCMLExport RWDecimalParser
{
 public:
   RWDecimalParser();
   RWDecimalPortable operator()(RW_SL_IO_STD(istream&));    // parse a number from a stream

 private:
   RWDecimalPortable  number_;

   /*
    * Lexical analysis.
    * Interface is via the reset(), token(), consume(), and digit() functions.
    * The RWCStrings allow adjustments of precicely what matches a token.
    */
   RW_SL_IO_STD(istream)  *input_;
   RWCString  leadingString_, trailingString_, digitSepString_, decString_;

 public:
   // this enum must be public for Borland 3.2 to compile definition of token()
   enum Token
      {
       eRW_DIGIT, eRW_DIGITSEP, eRW_DEC, eRW_LEADING, eRW_TRAILING, eRW_PERCENT,
       eRW_PLUS, eRW_MINUS, eRW_LPAREN, eRW_RPAREN, eRW_NULLSTATE, eRW_MISSING, eRW_NAN, eRW_ERR, eRW_NONE
      };

 private:
   Token      token_;       // current token: access via token() only!
   char       digit_;       // access via digit() only!
   int        unmatchedLparens_;  // RPAREN only found if this is > 0
   void       reset(RW_SL_IO_STD(istream&));    // prepare to start lexical analysis
   Token      token();      // returns the current token
   RWBoolean  consume(Token);//consumes the indicated token, or returns FALSE if wrong token
   char       digit();      // if token() is DIGIT, this is the digit read in
   void       nextToken();  // called by token() when necessary
   RWBoolean  stripFromInput(const char *);

   /*
    * Parsing.  Each parse functions reads in the associated non-terminal.
    * If a syntax error is encountered, FALSE is returned, otherwise TRUE
    * is returned.  The number is filled in as it is parsed.
    */
   RWBoolean  start();
   RWBoolean  anonum();
   RWBoolean  led();
   RWBoolean  sined();      // Can't use signed, it's a keyword
   RWBoolean  frac();
   RWBoolean  digits(RWCString *num);
};

#endif

