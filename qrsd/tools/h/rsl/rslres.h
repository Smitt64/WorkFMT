/*-----------------------------------------------------------------------*-

 File Name   : rslres.h                              10.27.94
 Programmer  : Serg Kubrin 

 Sybsystem   : RSL  
 Description : Загружает структуру полей панели в BTRSTRUCTURE

 Source      : rslres.c

 Library(s)  : icmp.lib


 Copyright (c) 1991 - 1994 by R-Style JSC
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
-*-----------------------------------------------------------------------*/

#ifndef __RSLRES_H
#define __RSLRES_H

#include "packpshn.h"

typedef struct
      {
       LOBJ         element;
       ResBtrField  field;
       char         name[1];
      } FieldListElement;


// for IM_GET_RESPANEL2
typedef struct
      {
       LOBJ         element;
       ResBtrField  field;
       short int    decPoint;
       char         name[1];
      } FieldListElement2;

// Для обслуживания сообщения IM_OPEN_RESFILE, IM_CLOSE_RESFILE

typedef struct TMesResData
      {
       const char  *name;
       HRESFILE     hd;
       int          error;
      } TMesResData;

// Для обслуживания сообщения IM_GET_RESPANEL, IM_GET_RESPANEL2, IM_FREE_RESPANEL

typedef struct TMesPanelData
      {
       const char  *name;
       HRESFILE     hd;
       int          error;
       CNTLIST      fieldList;
      } TMesPanelData;

typedef struct
      {
       TBtStructure  *str;
       int            size;
       char          *buff;
       HRESFILE       hd;
       char          *name;
      } TRslDlgData;


#ifdef __cplusplus
extern "C" {
#endif

void InitRslRes(void);
void DoneRslRes(void);


RSLLINK HRESFILE       RSLAPI MakeLoginRes(char *altDic);
RSLLINK BTRSTRUCTURE * RSLAPI LoadPanelStructure(HRESFILE hd, char *resName);

RSLLINK void           RSLAPI RslGetDlgData  (SREF *sref, TRslDlgData *dlg);
RSLLINK void           RSLAPI RslSetStrucData(SREF *sref, void *data);
RSLLINK void         * RSLAPI RslGetStrucData(SREF *sref);

BTRSTRUCTURE *MakeStrucFromFieldDefs(TGenObject *ar, char *name, bool shared);

#ifdef __cplusplus
}
#endif

#include "packpop.h"

#endif

/* <-- EOF --> */