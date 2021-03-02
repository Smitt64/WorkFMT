/******************************************************************************
    File Name           : parsestr.hpp                          05.02.2001
    Programmer          : A.Tokmashev

    Subsystem           : Interpreter RSL
    Description         : Парсировка и выполнение строк сложной праводки
                    

    Copyright(C) 1991 - 2000 by R-Style Software Lab.
    All Rights Reserved.

******************************************************************************/

#ifndef __PARSESTREX_H
#define __PARSESTREX_H
///////////////////////////////////////////////////////////////////////////////

#ifndef RSL_NO_HOST
    ///////////////////////////////////////////////////////////////////////////////

    #include "rsl/krnlintf.h"

    #define  SEPARATOR_CHAR  '\x02'

    ///////////////////////////////////////////////////////////////////////////////

    #ifdef __cplusplus
    extern "C" {
    #endif

    _APPTLFUNC bool ParseStringsEx(char *memSource, TRslMesProc userMsgProc,
                                     void *userData, TRslActionProc ProcFun,
                                     char sepCh,             // sepCh = SEPARATOR_CHAR
                                     TRSLMsgHandler *host ); // host  = RslGetMainHost();

    _APPTLFUNC bool ParseStringsEx2(char *memSource, TRslMesProc userMsgProc,
                                      void *userData, TRslActionProc ProcFun,
                                      char sepCh,            // sepCh == SEPARATOR_CHAR
                                      HRSLINST hInst );


    #ifdef __cplusplus
    }
    #endif

    ///////////////////////////////////////////////////////////////////////////////
#endif // RSL_NO_HOST

///////////////////////////////////////////////////////////////////////////////
#endif // PARSESTREX_H

/* <-- EOF --> */