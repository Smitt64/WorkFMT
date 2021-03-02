/*-----------------------------------------------------------------------*-

 File Name   : rsforms\msgloop.hpp
 Programmer  : Leonid S. Pavlov

 Sybsystem   : RSForms library
 Description : 

 Copyright (c) 1991 - 2003 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Nov 25, 2003 Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __msgloop_hpp__
#define __msgloop_hpp__

#ifndef __KEYS_H
#include "keys.h"
#endif


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

typedef struct tag_LoopInfo
{
   int retCode;
   int exitFlag;
} 
TLoopInfo;

/*-----------------------------------------------------------------------*/

inline int RunStackLoop(TAppObj* app, TLoopInfo* pInf = NULL)
{
    if(pInf)
    {
        pInf->retCode = 0;
        pInf->exitFlag = 0;
    }

    TFSEvent event = {0,0,0,0,0,0};
    while(event.key != K_QUIT)
    { 
        if(pInf && pInf->exitFlag) break;
        fs_getEventAppA(app, &event, 10000); // 10 sec 
    }

    if(event.key == K_QUIT)
        fs_addEventAppA(app, &event, true);
    
    return pInf ? pInf->retCode : 0;
}

inline void EndStackLoop(TAppObj* app, int iRetCode = 0, TLoopInfo* pInf = NULL)
{
    TFSEvent event; 

    if(pInf)
    {
        pInf->retCode = iRetCode;
        pInf->exitFlag = 1;
        event.key = K_KICK;
    }
    else
    {
        event.key = K_QUIT;
    }

    fs_addEventAppA(app, &event, true);
}

/*-----------------------------------------------------------------------*/
#endif // __msgloop_hpp__
