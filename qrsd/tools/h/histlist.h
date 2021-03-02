/*-----------------------------------------------------------------------*-
 File Name   : histlist.h

 Sybsystem   : editor  
 Description : 

 Source      : histlist.c

 Library(s)  : 

 Copyright (c) 1991 - 1994 by R-Style JSC
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 January 5,1995   Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __HISTLIST_H
#define __HISTLIST_H

#ifdef __cplusplus
extern "C" {
#endif

void InitHist(char *file);
void DoneHist(char *file);

void PositionHist(View *v);
void AddHist     (View *v);

char *PickList(void);
TFilePosition *TagsList(CNTLIST *lst);

#define  FLPOS_FILE   1
#define  FLPOS_NAME   2
#define  FLPOS_NUM    4

TFilePosition *ViewFilePositions(CNTLIST *lst, const char *head, const char *statLn, int fldMask);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */