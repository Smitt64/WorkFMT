/*-----------------------------------------------------------------------*-

 File Name   : timer.h   

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 November 12,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

#include "platform.h"
#include "tooldef.h"

#ifdef __cplusplus
extern "C" {
#endif

enum
   {
    RSL_EV_TIMEOUT,
    RSL_EV_CTRLB,
    RSL_EV_BEGINIDLE,
    RSL_EV_KBDFILTER,
    RSL_EV_BREAK,
    RSL_EV_ADVERTISE
   };

_TOOLEXP unsigned long GetTimerTick(void);

typedef void (*TRslTimerProc)(unsigned long curTick, unsigned id, void *data);

_TOOLEXP int  RslAddTimerProc(unsigned long tmOut, unsigned id, TRslTimerProc proc, void *data);
_TOOLEXP int  RslRemTimerProc(unsigned id, TRslTimerProc proc);
_TOOLEXP void RslCheckTimer  (void);

_TOOLEXP int  RslAddEventProc  (int evType, unsigned id, TRslTimerProc proc, void *data);
_TOOLEXP int  RslRemEventProcEx(unsigned id, TRslTimerProc proc, void *data);
_TOOLEXP void RslRaiseEventProc(int evType);

_TOOLEXP void RslRaiseEventProcParm(int evType, unsigned long parm);

#define  RslRemEventProc(id, proc)  RslRemTimerProc(id, proc)

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */