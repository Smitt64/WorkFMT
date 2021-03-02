/*-----------------------------------------------------------------------*-

 File Name   : rsldlm.h  

 Sybsystem   : RSL  
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
 August 24,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __RSLDLM_H
#define __RSLDLM_H

#ifndef __DLMDEF_H
# include "dlm.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------
EXP32 void DLMAPI EXP AddModuleObjects(void);
EXP32 void DLMAPI EXP InitExec(void);
EXP32 void DLMAPI EXP DoneExec(void);
EXP32 void DLMAPI EXP RslAttach(int activate);
EXP32 int  DLMAPI EXP DlmMain(int isLoad, void *);

// -----------------------------------------------------------------------------
int fs_initDLM_useAPI(void);
int fs_initDLM_useCallBack(void);
int DlmMainEx(int isLoad, void *);

// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
    #define  fs_initDLM  fs_initDLM_useCallBack
#else
    #define  fs_initDLM  fs_initDLM_useAPI
#endif

// -----------------------------------------------------------------------------
#ifndef BUILD_DLM                                         

    extern BACKENTRY  *ExeExports;

    #define  RSL_DLM_IMP  BACKENTRY  *ExeExports = NULL;                        \
                          EXP32 int DLMAPI EXP RslSetCallBacks (BACKENTRY *cmd) \
                          { ExeExports = cmd; return RSL_DLM_VERSION; }

#else
    #define  RSL_DLM_IMP
#endif

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */