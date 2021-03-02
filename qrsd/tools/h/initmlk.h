//-*-----------------------------------------------------------------------*-
//
// File Name   : initmlk.h 
//
// Sybsystem   :   
// Description : 
//
// Source      : 
//
// Library(s)  : 
//
// Copyright (c) 1991 - 2002 by R-Style Softlab.
// All Rights Reserved.
//
//-*- History -------------------------------------------------------------*-
// October 21,2004  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-

#ifndef __INITMLK_H
#define __INITMLK_H

#if defined(USE_LEAKDETECT) && defined(__cplusplus)  && defined(_MSC_VER) && defined(_DEBUG)

    #ifdef BUILD_RSMLK
        #define  MLKEXPORT  __declspec(dllexport)
    #else
        #define  MLKEXPORT  __declspec(dllimport)
    #endif


    MLKEXPORT void RslMemLeakInit(void);
    MLKEXPORT void RslMemLeakDone(void);
    MLKEXPORT void RslMemLeakReport(void);
    MLKEXPORT void RslMemLeakClear(void);

    class RslMemLeakInitClass
        {
         public:
           RslMemLeakInitClass()
           {
            RslMemLeakInit();
           }

          ~RslMemLeakInitClass()
           {
            RslMemLeakDone();
           }
        };

    #ifndef NO_LEAK_INIT
        static RslMemLeakInitClass  initObject_;
        #pragma comment( lib, "rsmlkd.lib" )
    #endif
  
#else
    #define  RslMemLeakInit()
    #define  RslMemLeakDone()
    #define  RslMemLeakReport()
    #define  RslMemLeakClear()
#endif // __cplusplus

#endif // __INITMLK_H

/* <-- EOF --> */