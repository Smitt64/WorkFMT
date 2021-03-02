/*-----------------------------------------------------------------------*-

 File Name   : erobj.h   

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 May 20,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif


TGenObject  *CreateErrObject      (void);
void         DeleteErrObject      (TGenObject *obj);
void         SetErrObject         (TGenObject *obj, ERRINFO *er);
ERRINFO     *GetErrObjInfo        (TGenObject *o);
void         ClearErrObject       (TGenObject *o);

void         SetErrObjectAx       (TGenObject *o, TRslAxError *axErr);
TRslAxError *GetErrObjectAx       (TGenObject *o);

void         SetErrObjectExtInfo  (TGenObject *o, VALUE *val);
VALUE       *GetErrObjectExtInfo  (TGenObject *o);
void         CheckForRSCOMerror   (TGenObject *obj);
bool         FillRSCOMerror       (TGenObject *obj, TRsComErrMes *data);

void         SetErrObjectThrowCode(TGenObject *o, int code);
int          GetErrObjectThrowCode(TGenObject *o);

void         SetUserObj2ErrObject (TGenObject *userObj);

#ifdef __cplusplus
}
#endif

/* <-- EOF --> */