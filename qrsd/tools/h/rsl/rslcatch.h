/*-----------------------------------------------------------------------*-

 File Name   : rslcatch.h

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 24,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __RSLCATCH_H
#define __RSLCATCH_H

#include <stdlib.h>
#include <setjmp.h>

#include "listobj.h"

#ifndef __EXPDEF_H
#include "rsl/expdef.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------
enum JmpErrors
   {
    JMP_NOMEM = 1,
    JMP_ERROR,
    JMP_FPUER,
    JMP_RUNTIME,
    JMP_BTRTRN,
    JMP_EXIT,
    JMP_FDECIMAL,
    JMP_CALLDBG,
    JMP_USERBREAK,
    JMP_RESUME,
    JMP_USER = 1000
   };

#include <packpshn.h>

typedef struct
      {
       LOBJ     elem;
       void    *erInfo;
       jmp_buf  buff;
      } TCatchData;

#include <packpop.h>

// -----------------------------------------------------------------------------
#define  RSTRY             { TCatchData  RSLCatchData; RslCatchInit(&RSLCatchData);   \
                           if(setjmp(RSLCatchData.buff) == 0) {

#define  RSTRY2(inst)      { TCatchData  RSLCatchData; RslCatchInit2(&RSLCatchData, inst); \
                           if(setjmp(RSLCatchData.buff) == 0) {

#define  RSCATCH           RslCatchProcess(&RSLCatchData); } else { RslCatchProcess(&RSLCatchData);

#define  RSCATCH2(inst)    RslCatchProcess2(&RSLCatchData, inst); } else { RslCatchProcess2(&RSLCatchData, inst);

#define  THROWERR          RSLCatchGetErr()
#define  RSCLEARERR        RSLCatchClear()
#define  MAKETHROWERR(er)  RSLCatchSetErr(er)

#define  RSTHROW(TheEr)    { TCatchData *ptr = RslThrowData(TheEr); if(ptr) longjmp(ptr->buff, TheEr); exit(5); }

#define  RSRETHROW         { TCatchData *ptr = RslThrowData(0); if(ptr) longjmp(ptr->buff, 1); exit(5); }

#define  RSENDCATCH        }}

// -----------------------------------------------------------------------------
RSLLINK void        RSLAPI RslCatchInit(TCatchData *dt);
RSLLINK void        RSLAPI RslCatchProcess(TCatchData *dt);
RSLLINK TCatchData *RSLAPI RslThrowData(int err);
RSLLINK int         RSLAPI RSLCatchGetErr(void);
RSLLINK void        RSLAPI RSLCatchClear(void);
RSLLINK void        RSLAPI RslCatchThrow(int err);
RSLLINK void        RSLAPI RSLCatchSetErr(int er);

#ifdef __cplusplus
}
#endif

#endif // __RSLCATCH_H

/* <-- EOF --> */