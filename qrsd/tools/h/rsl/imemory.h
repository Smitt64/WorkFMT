/*-----------------------------------------------------------------------*-

 File Name   : imemory.h 

 Sybsystem   : Blocked memory allocation functions   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 October 24,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __IMEMORY_H
#define __IMEMORY_H


#define  DEF_PAGE_SIZE   4096  // Page size in bytes


#ifdef __cplusplus
extern "C" {
#endif

void  InitBlockMem(unsigned pageSize);
void  DoneBlockMem(void);
void *AllocBlockMem(unsigned size);
void  FreeBlockMem(void *ptr);

#ifdef __cplusplus
}
#endif

#endif // __IMEMORY_H

/* <-- EOF --> */