/******************************************************************************
    File Name           : parsestr.hpp                          05.02.2001
    Programmer          : A.Tokmashev

    Subsystem           : Interpreter RSL
    Description         : Парсировка и выполнение строк сложной праводки
                    

    Copyright(C) 1991 - 2000 by R-Style Software Lab.
    All Rights Reserved.

******************************************************************************/

#ifndef __PARSESTRINGS_H
#define __PARSESTRINGS_H
///////////////////////////////////////////////////////////////////////////////

#ifndef __KRNLINTF_H
#include "rsl/krnlintf.h"
#endif

#include "rsl/objintf.hpp"

typedef struct tagTMemSourceSpec
      {
       const char     *mem;
       const char     *curCh;
       bool            curLine;
       unsigned char   sepCh;
      } TMemSourceSpec;

///////////////////////////////////////////////////////////////////////////////
// class TSpecInstIntf
///////////////////////////////////////////////////////////////////////////////
class TSpecInstIntf : public TRSLObjInstIntf
    {
     public:
      TSpecInstIntf(HRSLINST inst) : TRSLObjInstIntf(inst) {};

     public:
       bool (*pfParseSpec)(TSpecInstIntf *, char *);
       bool (*pfExecuteSpec)(TSpecInstIntf *);
    };


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

RSLLINK bool RSLAPI ParseStrings (char *, TRslMesProc, void *, TRslActionProc, char, TRSLMsgHandler *);
RSLLINK bool RSLAPI ParseStrings2(char *, TRslMesProc, void *, TRslActionProc, char, HRSLINST);

RSLLINK TSpecInstIntf *RSLAPI CreateSpecInst(HRSLINST, char *, void *, TRslActionProc, char);
RSLLINK void RSLAPI           DeleteSpecInst(TSpecInstIntf *);

#ifdef __cplusplus
}
#endif // __cplusplus


///////////////////////////////////////////////////////////////////////////////
#endif // __PARSESTRINGS_H

/* <-- EOF --> */