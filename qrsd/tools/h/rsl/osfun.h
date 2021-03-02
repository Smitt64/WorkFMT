/*-----------------------------------------------------------------------*-

 File Name   : osfun.h    

 Sybsystem   :   
 Description : OS specific functions

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/osfun.h $
               $Revision: 2 $
               $Date: 29.08.00 18:06 $
               $Author: Kubrin $
               $Modtime: 29.08.00 16:49 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 February 25,2000  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __OSFUN_H
#define __OSFUN_H


// -----------------------------------------------------------------------------
#define  RSL_INSTCOUNT_SECT  0
#define  RSL_UNICLS_SECT     1
#define  RSL_AUTOOBJ_SECT    2
#define  RSL_DBG_SECT        3

#define  RSL_NCRITSECT    RSL_DBG_SECT + 1

#ifdef __cplusplus
extern "C" {
#endif

void    RslInitThreadInst(void);
void    RslDoneThreadInst(void);
void   *RslGetThreadInst (void);
void    RslSetThreadInst (void *inst);

void    RslInitCritSect  ();
void    RslDoneCritSect  ();
void    RslEnterCritSect (int n);
void    RslLeaveCritSect (int n);

bool    RslGetLocaleInfo (int loc, int code, char *buff, int szBuff);
long    RslGetLangId     (int loc);

bool    RsGuiMsgBox      (bool yesNo, const char *caption, const char *fmt, ...);

void    RslGetModuleFileName (DLMHAND id, char *buff, int size);


#ifdef __cplusplus
}
#endif

#endif // __OSFUN_H

/* <-- EOF --> */