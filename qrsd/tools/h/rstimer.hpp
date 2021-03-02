//-*-----------------------------------------------------------------------*-
//
// File Name   : rstimer.hpp
//
// Sybsystem   :   
// Description : 
//
// Source      : 
//
// Library(s)  : 
//
// Copyright (c) 1991 - 2002 by R-Style Softlab.
// All Rights Reserved.
//
//-*- History -------------------------------------------------------------*-
// December 2,2002  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-

#ifndef __RSTIMER_HPP
#define __RSTIMER_HPP

#include "smartlst.hpp"
#include "timer.h"

#include "critsect.h"

class TRslTimerElem;

class TRslTimer
    {
     public:
       TRslTimer();
      ~TRslTimer();

       bool addTimer(unsigned long tmOut, unsigned id, TRslTimerProc proc, void *data);
       bool remTimer(unsigned id, TRslTimerProc proc);
       bool remTimerEx(unsigned id, TRslTimerProc proc, void* data);
       bool addEventProc(int evType, unsigned id, TRslTimerProc proc, void *data);

       void checkTimer();
       void callEventProc(int evType);
       void callEventProc(int evType, unsigned long parm);

     private:
       static int findProc (TRslTimerElem *, void *);
       static int checkProc(TRslTimerElem *, void *);
       static int eventProc(TRslTimerElem *, void *);

       TSmartListBase<TRslTimerElem>  list;

       TRslTimerElem *elemToDel;
       bool           inCheck;

       RSL_DECL_CRITICAL_SECTION(critSec);
    };

#endif // __RSTIMER_HPP

/* <-- EOF --> */