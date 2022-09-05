//-*-----------------------------------------------------------------------*-
//
// File Name   : winclip.h 
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
// October 29,2002  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-
#ifndef __WINCLIP_H
#define __WINCLIP_H

#ifndef   __EDIT_FBUFF_H
    #include "edit/fbuff.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

void FbRegisterWinClipFormat (void);
int  FbTransferToWinClip  (FBUFF *from);
int  FbCopyColomnToWinClip(FBUFF *from);
int  FbTransferFromWinClip(FBUFF *to, int *isLineInsert);

int  FbTransferToWinClip_remote  (FBUFF *from);
int  FbCopyColomnToWinClip_remote(FBUFF *from);
int  FbTransferFromWinClip_remote(FBUFF *to,int *isLineInsert);

#ifdef __cplusplus
}
#endif

#endif // __WINCLIP_H

/* <-- EOF --> */