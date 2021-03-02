/******************************************************************************
    File Name           : parsestr.hpp                          05.02.2001
    Programmer          : A.Tokmashev

    Subsystem           : Interpreter RSL
    Description         : Парсировка и выполнение строк сложной праводки
                    

    Copyright(C) 1991 - 2000 by R-Style Software Lab.
    All Rights Reserved.

******************************************************************************/
#ifndef __PARSESTR_H
#define __PARSESTR_H
///////////////////////////////////////////////////////////////////////////////

#ifndef RSL_NO_HOST

///////////////////////////////////////////////////////////////////////////////
                             
    #ifndef __PARSESTRINGS_H
        #include "rsl/ParseStrings.h"
    #endif

    #ifndef __PARSESTREX_H
        #include "parsestrex.h"
    #endif


    // Направлены на функции в Tools
    #define  ParseStrings   ParseStringsEx
    #define  ParseStrings2  ParseStringsEx2


#endif // RSL_NO_HOST

///////////////////////////////////////////////////////////////////////////////
#endif // PARSESTR_H

/* <-- EOF --> */