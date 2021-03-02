/*-----------------------------------------------------------------------*-

 File Name   : expdef.h  

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 11,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __EXPDEF_H
#define __EXPDEF_H

// -----------------------------------------------------------------------------                         
#if defined(RSL_MD_FLAT) && defined(RSL_PL_MS)
    #if defined(RSL_BUILD_DLL)
        #define  RSLLINK     __declspec(dllexport)
        #define  _RSLLINK    __declspec(dllexport)

        #if defined(RSL_USE_STDCALL)
            #define  RSLAPI       __stdcall
            #define  RSLCALLBACK  __stdcall
        #else
            #define  RSLAPI
            #define  RSLCALLBACK
        #endif
    #else
        #if defined(__RSRTL_DLL) && !defined(RSL_USE_DLL)
            #define RSL_USE_DLL
        #endif

        #if defined(RSL_USE_DLL)
            #define  RSLLINK    __declspec(dllimport)
            #define  _RSLLINK   __declspec(dllimport)

            #if defined(RSL_USE_STDCALL)
                #define  RSLAPI       __stdcall
                #define  RSLCALLBACK  __stdcall
            #else
                #define  RSLAPI
                #define  RSLCALLBACK
            #endif
        #else
            #define  RSLAPI
            #define  RSLCALLBACK
            #define  RSLLINK
            #define  _RSLLINK
        #endif
    #endif
#else
    #define  RSLAPI
    #define  RSLCALLBACK
    #define  RSLLINK
    #define  _RSLLINK
#endif

// -----------------------------------------------
#ifdef BUILD_DLM
    #define  DLMLINK 
    #define  RSLDLMAPI   DLMAPI
    #define  RSLDLMAPIC  DLMAPIC
#else 
    #define  DLMLINK     RSLLINK  
    #define  RSLDLMAPI   RSLAPI
    #define  RSLDLMAPIC  RSLAPI

    #ifndef DLMAPI
        #define  DLMAPI
    #endif

    #ifndef DLMAPIC
        #define  DLMAPIC
    #endif
#endif

#endif //__EXPDEF_H

/* <-- EOF --> */