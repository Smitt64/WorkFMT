//-*-----------------------------------------------------------------------*-
//
// File Name   : hd32imp.h 
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
// September 3,2004  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-

#ifndef __HD32IMP_H
#define __HD32IMP_H


typedef struct
      {
       LOBJ    listElem;
       uint32  handle;
      } THandleElem;

#ifdef __cplusplus
extern "C" {
#endif

void     InitHandle32Support(void);
void     DoneHandle32Support(void);
uint32   AddNewHandle32Elem(THandleElem *elem);
void     RemHandle32Elem(THandleElem *elem);
THandleElem *GetElemFromHandle32(uint32 key);
uint32   GetHandle32FromElem(THandleElem *elem);
void     RemHandle32Elem2(uint32 key);

#ifdef __cplusplus
}
#endif

#endif // __HD32IMP_H

/* <-- EOF --> */