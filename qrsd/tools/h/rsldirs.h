/*-----------------------------------------------------------------------*-

 File Name   : rsldirs.h

 Sybsystem   :
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
 August 6,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __RSLDIRS_H
#define __RSLDIRS_H

#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
_APPTLFUNC void  ToolsBfileSetDictionary   (const char *name);
_APPTLFUNC const char *ToolsBfileSetDataDir(const char *dir);
_APPTLFUNC void  ToolsSetIncDir            (const char *dirlist);
_APPTLFUNC void  ToolsSetTextDir           (const char *dir);
_APPTLFUNC void  ToolsSetDbfDir            (const char *dir);
_APPTLFUNC void  ToolsSetRslResFile        (const char *fileName);
_APPTLFUNC void  ToolsSetRslLbrDir         (const char *name);
_APPTLFUNC void  ToolsSetRsmDir            (const char *dirlist);
_APPTLFUNC const char *ToolsSetErrDir      (const char *name);
_APPTLFUNC void  ToolsSetRsfmLbrDir        (const char *name);

// -----------------------------------------------------------------------------
_APPTLFUNC const char *ToolsBfileGetDictionary(void);
_APPTLFUNC const char *ToolsBfileGetDataDir(void);

_APPTLFUNC const char *ToolsGetIncDir(void);
_APPTLFUNC const char *ToolsGetTextDir(void);
_APPTLFUNC const char *ToolsGetErrDir(void);
_APPTLFUNC const char *ToolsGetRsmDir(void);
_APPTLFUNC const char *ToolsGetRsfmLbrDir(void);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */