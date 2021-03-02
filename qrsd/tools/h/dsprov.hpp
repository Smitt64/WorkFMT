//-*--------------------------------------------------------------------------*-
//
// File Name   : dsprov.hpp
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// June 15,2005  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __DSPROV_HPP
#define __DSPROV_HPP

#include "apptldef.h"
#include "listobj.h"

#include "packpshn.h"


#ifdef __cplusplus
extern "C" {
#endif

const int  DISPID_INITDS = -2;

struct TDsProvData;

typedef TRsStat(RSCOMCALL *CreateDSrcType)(TAppObj *app, const char *parms, void *rslHost, IChanel *cn,
                TDsProvData *ptr, bool isDesignTime, IRsObj **result);

struct TDsProvData
     {
      LOBJ           elem;
      int            version;
      void          *data;
      CreateDSrcType creator;
     };


_APPTLFUNC void RslRegisterDataProvider  (TDsProvData *data);
_APPTLFUNC void RslUnRegisterDataProvider(TDsProvData *data);


//--------------------------------------------------------------------------------------
// Tools internal functions 
//

TRsStat RSCOMCALL RslCreateDataSources(TAppObj *app, const char *parms, void *rslHost, IChanel *cn,
                                       bool isDesignTime, IRsObj **result);

void RslInitDataProvider(void);
void RslDoneDataProvider(void);

#ifdef __cplusplus
}
#endif

// -----------------------------------------------------------------------------
class RslCmdInfo
    {
     public:
       RslCmdInfo(const char *str);

       TRsString   tagName;
       TRsString   instName;
       TRsString   macName;
       TRsString   clsName;
       TRsString   objName;
       TRsString   params;
       TRsString   callProc;
       int         cmdType;  
       bool        noOwner;
       bool        contextWnd;

     private:
       const char *getCmdWord(char *buff, int sz, const char *ptr);
    };


const int  AddModuleID = 0;
const int  ExecuteID   = 1;
const int  StopID      = 2;
const int  CallID      = 3;

//
//--------------------------------------------------------------------------------------
//

#include "packpop.h"

#endif // __DSPROV_HPP

/* <-- EOF --> */