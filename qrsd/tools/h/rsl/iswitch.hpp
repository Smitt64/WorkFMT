/*-----------------------------------------------------------------------*-

 File Name   : iswitch.hpp

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 August 13,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __ISWITCH_HPP
#define __ISWITCH_HPP

// -----------------------------------------------------------------------------
class InstSwitch
{
 public:
   // --------------------------------------------
   InstSwitch(HRD inst)
   {
    oldInst = RslSetInstance(inst);
   }

   // --------------------------------------------
  ~InstSwitch()
   {
    RslSetInstance(oldInst);
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   HRD  oldInst;
};

#endif // __ISWITCH_HPP

/* <-- EOF --> */