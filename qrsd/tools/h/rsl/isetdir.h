/*-----------------------------------------------------------------------*-

 File Name   : isetdir.h                             11.02.94
 Programmer  : Serg Kubrin

 Sybsystem   : RSL
 Description : Содержит прототипы функций инициализации RSL
               Used by TOOLS only!!!

 Source      :

 Library(s)  : icmp.lib


 Copyright (c) 1991 - 1994 by R-Style JSC
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
-*-----------------------------------------------------------------------*/

#ifndef __ISETDIR_H
#define __ISETDIR_H


#ifdef __cplusplus
extern "C" {
#endif

#include "rsldirs.h"

// -----------------------------------------------------------------------------
#define  BfileSetDictionary  ToolsBfileSetDictionary
#define  BfileSetDataDir     ToolsBfileSetDataDir
#define  SetIncDir           ToolsSetIncDir
#define  SetTextDir          ToolsSetTextDir
#define  SetDbfDir           ToolsSetDbfDir
#define  SetRslResFile       ToolsSetRslResFile

// -----------------------------------------------------------------------------
RSLLINK void RSLAPI InitRubToStr(const char *cnfFile);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */