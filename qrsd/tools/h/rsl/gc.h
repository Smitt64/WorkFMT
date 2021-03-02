//-*--------------------------------------------------------------------------*-
//
// File Name   : gc.h      
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// July 11,2005  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSLGC_H
#define __RSLGC_H


#include "packpshn.h"

#ifdef __cplusplus
extern "C" {
#endif

int  RslSetGenerationToObject(TGenObject *obj, unsigned short gen);
void MarkGcGlobals(HRD inst);

int  RslGCollect(HRD inst, long *notCollected, bool doTrace);

// -----------------------------------------------
typedef struct 
      {
       unsigned int  generation;
       int           count;
      } TCalcGC;

// -----------------------------------------------
typedef struct 
      {
       PROCPROLOG  *st;
       HRD          inst;

       TGenObject  *obj;
       int          level;

       int          reportCounter;
      } TFinfRefData;

// -----------------------------------------------
void Indent(HRD inst, int level);

void FindRefGlobals(TFinfRefData*);

int setRslGeneration(TRslObject *obj, unsigned short gen);
int setArrGeneration(TArrayObj *obj, unsigned short gen);

#ifdef __cplusplus
}
#endif

#include "packpop.h"

#endif // __RSLGC_H

/* <-- EOF --> */