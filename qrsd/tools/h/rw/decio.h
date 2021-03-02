#ifndef __RWDECIO_H__
#define __RWDECIO_H__

/***************************************************************************
 *
 * Classes for fancy output of RWDecimal numbers
 *
 * $Id: decio.h@#/main/21  10/13/97 12:20:11  steves (MNY150_971113_WIN)
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
 ***************************************************************************
 *
 * See notes in decio.cpp for implementation details on how formatted
 * numbers are built and how pictures are parsed.
 *
 * There are two classes:
 *
 *   RWPosDecimalFormat formats a decimal which must be positive
 *   RWDecimalFormat contains two RWDPosDecimalFormat objects: it uses one
 *     for positive numbers and the other for negative numbers.
 *
 * RWPosDecimalFormat is an implementation specific class: it may change.
 * Ideally, it would be declared somewhere outside the public eye, but this
 * is not possible since it must be visible to the RWDecimalFormat declaration
 * (since RWDecimalFormat uses instances of RWPosDecimalFormat). 
 *
 ***************************************************************************/

#include "rw/cstring.h"
#include "rw/money/decdefs.h"
#include "rw/decbase.h"
#include "rw/decport.h"

//#if !((defined(RW_NAMESPACE_REQUIRED) || ( defined(__TURBOC__) && ( __TURBOC__ >= 0x530 ))) && !defined(RW_NO_IOSTD)) && ! (defined(__GNUG__) && (__GNUG__ >= 3))
//class RW_SL_IO_STD(ostream);
//#endif

class RWDCMLExport RWDecimalFormatScope {      // class for scoping enums
public:
  enum Justification    { LEFT, CENTER, RIGHT };
  enum Sign             { POSITIVE, NEGATIVE, BOTH };
};

class RWDCMLExport RWPosDecimalFormat : public RWDecimalFormatScope {
  friend class RWDCMLExport RWDecimalFormat;   // so we can skip defining access functions
              // and instead just access members directly
public:
  RWPosDecimalFormat();
  RWPosDecimalFormat(const char* fmt);

  /* Functions to query and change attributes */
  RWCString     operator()(const RWDecimalPortable&) const;
  void printState(RW_SL_IO_STD(ostream)&);

private:
  RWBoolean      fixedWidth_;           
  unsigned       width_;                
  Justification  justification_;        
  RWBoolean      doRounding_;           
  RWDecimalBase::RoundingMethod roundMethod_;           
  int            roundPlaces_;          
  RWBoolean      showDecimalPoint_;     
  RWBoolean      allDecimals_;          
  int            decimalPlaces_;        
  RWCString      decimalSeparator_;     
  RWCString      leftDigitSeparator_;   
  unsigned       leftGroupSize_;        
  RWCString      rightDigitSeparator_;  
  unsigned       rightGroupSize_;       
  RWCString      nearLeftText_;         
  RWCString      nearRightText_;        
  char           leftFillChar_;         
  char           rightFillChar_;        
  RWCString      farLeftText_;          
  RWCString      farRightText_;         

  // following functions helpers in printing
  void      createDigits(const RWDecimalPortable&, RWCString *before, RWCString *after) const;
  RWCString buildLeftDigitString(const RWCString&, unsigned, const RWCString&) const;
  RWCString buildRightDigitString(const RWCString&, unsigned, const RWCString&) const;
};

class RWDCMLExport RWDecimalFormat : public RWDecimalFormatScope
{
public: 
  RWDecimalFormat();
  RWDecimalFormat(const char* fmt);

  /* Functions to query attributes */
  RWBoolean      fixedWidth(Sign) const;           
  unsigned       width(Sign) const;             
  Justification  justification(Sign) const;     
  RWBoolean      doRounding(Sign) const;                
  RWDecimalBase::RoundingMethod roundMethod(Sign) const;                
  int            roundPlaces(Sign) const;               
  RWBoolean      showDecimalPoint(Sign) const;  
  RWBoolean      allDecimals(Sign) const;               
  int            decimalPlaces(Sign) const;     
  RWCString      decimalSeparator(Sign) const;  
  RWCString      leftDigitSeparator(Sign) const;        
  RWCString      decimalSeperator(Sign s) const { return decimalSeparator(s); }
  RWCString      leftDigitSeperator(Sign s) const { return leftDigitSeparator(s); }
  unsigned       leftGroupSize(Sign) const;     
  RWCString      rightDigitSeparator(Sign) const;       
  RWCString      rightDigitSeperator(Sign s) const { return rightDigitSeparator(s); }
  unsigned       rightGroupSize(Sign) const;    
  RWCString      nearLeftText(Sign) const;              
  RWCString      nearRightText(Sign) const;     
  char           leftFillChar(Sign) const;              
  char           rightFillChar(Sign) const;     
  RWCString      farLeftText(Sign) const;               
  RWCString      farRightText(Sign) const;

  /* Functions to set attributes.  Be careful when changing the
     nearLeftText attribute that you don't accidently eliminate the
     negative sign */
  void           setFixedWidth(RWBoolean x, Sign=BOTH);           
  void           setWidth(unsigned x, Sign=BOTH);               
  void           setJustification(Justification x, Sign=BOTH);  
  void           setDoRounding(RWBoolean x, Sign=BOTH);         
  void           setRoundMethod(RWDecimalBase::RoundingMethod x, Sign=BOTH);            
  void           setRoundPlaces(int x, Sign=BOTH);              
  void           setShowDecimalPoint(RWBoolean x, Sign=BOTH);   
  void           setAllDecimals(RWBoolean x, Sign=BOTH);                
  void           setDecimalPlaces(int x, Sign=BOTH);    
  void           setDecimalSeparator(const RWCString& x, Sign=BOTH);   
  void           setLeftDigitSeparator(const RWCString& x, Sign=BOTH); 
  void           setDecimalSeperator(const RWCString& x, Sign s=BOTH) { setDecimalSeparator(x, s); }
  void           setLeftDigitSeperator(const RWCString& x, Sign s=BOTH) { setLeftDigitSeparator(x, s); }
  void           setLeftGroupSize(unsigned x, Sign=BOTH);       
  void           setRightDigitSeparator(const RWCString& x, Sign=BOTH);        
  void           setRightDigitSeperator(const RWCString& x, Sign s=BOTH) { setRightDigitSeparator(x, s); }
  void           setRightGroupSize(unsigned x, Sign=BOTH);      
  void           setLeftFillChar(char x, Sign=BOTH);            
  void           setRightFillChar(char x, Sign=BOTH);   
  void           setNearLeftText(const RWCString& x, Sign=BOTH);               
  void           setNearRightText(const RWCString& x, Sign=BOTH);      
  void           setFarLeftText(const RWCString& x, Sign=BOTH);                
  void           setFarRightText(const RWCString& x, Sign=BOTH);
  void           appendFarRightText(const RWCString& x, Sign=BOTH);
  void           appendNearRightText(const RWCString& x, Sign=BOTH);
  void           appendFarLeftText(const RWCString& x, Sign=BOTH);
  void           appendNearLeftText(const RWCString& x, Sign=BOTH);  

  RWCString     operator()(const char*) const;
  RWCString     operator()(const RWDecimalPortable&) const;
  void printState(RW_SL_IO_STD(ostream)&);
  friend RWCString rwdcmlexport operator <<( const RWDecimalFormat&, const RWDecimalPortable& );

private:
  RWPosDecimalFormat positiveFormat_;
  RWPosDecimalFormat negativeFormat_;
};
  
#endif  /* wrapper */
