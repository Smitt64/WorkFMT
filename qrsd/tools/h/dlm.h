/***********************************************************************
       Extantion  for DOS programs.
       DOS DLL  system

       S. Kubrin        31.03.1992

    VERSION INFO :  $Archive: /RS-Tools/RsRt/DLL/RSLDLM/H/dlm.h $
                    $Revision: 18 $
                    $Date: 13.09.99 19:03 $
                    $Author: Kubrin $
                    $Modtime: 7.09.99 14:58 $
************************************************************************/

#ifndef __EXT_H
#define __EXT_H

#include "dlmdef.h"

// -----------------------------------------------------------------------------
#define  RSL_DLM_VERSION  309

// -----------------------------------------------------------------------------
//    Error codes
#define  EXT_INITLIB    1  // InitLib  fail
#define  EXT_OVLLOAD    2  // DOS OVL load fail
#define  EXT_DLLMEMR    3  // No memory for DLL module
#define  EXT_ERRREAD    4  // Error reding or open DLL file
#define  EXT_NTFOUND    5  // Not found object
#define  EXT_BADCOMM    6  // Incorrect command, may be NULL DLMHAND

#define  EXT_ERLOAD    10  // Ошибка загрузки DLM, возможно неверно указано имя
#define  EXT_VERSION   11  // Bad DLM version
#define  EXT_ERFORMAT  12  // This is not a DLM or RSCOM module
#define  EXT_ERWIN32   13  // Win32 error occured

#define  EXT_UNSUPP    99  // DLM currently is not supported for platform

#include <packpsh1.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
typedef struct tagDLMHAND
      {
       int dummy;
      } *DLMHAND;

// -----------------------------------------------------------------------------
int RslLoadLibrary(DLMHAND *hd, char *name);
int RslFreeLibrary(DLMHAND id);
int RslGetDllProc (DLMHAND hd, char *name, void **proc);

int RslLoadLibraryEx(DLMHAND *hd, char *name);
int RslFreeLibraryEx(DLMHAND id);

// -----------------------------------------------------------------------------
typedef struct
      {
       void *fun;
      } BACKENTRY;

EXP32 int DLMAPI EXP RslSetCallBacks(BACKENTRY *cmd);
EXP32 int DLMAPI EXP DlmMain(int isLoad, void *);

int DLMAPI RslSetCallBacksImp (BACKENTRY *cmd);
void  DLMAPI RslSetInstData (void *ptr);
void *DLMAPI RslGetInstData (void);
int   DLMAPI RslGetInstCount(void);


// -----------------------------------------------------------------------------
extern BACKENTRY  cmdBack[];

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */