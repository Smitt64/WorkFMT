/********************************************************************
   Created: 2001/12/19
   Created: 19:12:2001   15:31
   Filename: D:\Work\RSTools\tools\h\traceinit.h
   Project: RSTools
   Author: Andrew Ushakov
*********************************************************************/

#ifndef __TRACEINIT_H
#define __TRACEINIT_H

#include "tooldef.h"
#include "tracedecl.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// -----------------------------------------------------------------------------
_TOOLEXP HTRACE RSTraceInit    (const char *);
_TOOLEXP void   RSTraceRelease (HTRACE);
_TOOLEXP int    RSTraceAnalise (const char *);
_TOOLEXP int    RSTraceEnable  (bool);
_TOOLEXP bool   RSIsTraceEnable();
_TOOLEXP int    RSTraceSetLogFile(const char *logAlias, const char *logFile);

typedef  void   (*TraceCallback_t)();

_TOOLEXP void   SetTraceCallback(TraceCallback_t cb);
_TOOLEXP TraceCallback_t GetTraceCallback();

#ifdef __cplusplus
}
#endif // __cplusplus


// -----------------------------------------------------------------------------
#define  RSTRACE(hTrace)  if(hTrace) hTrace->ProcTrace

/////////////////////////////////////////////////////////////////
// For example:
// RSTRACE1(g_RSTrace, "Category name", "Event name", LevelOfDetalisation, "Result = %d", iResult)
//    1<=LevelOfDetalisation<=4

#if defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)

    #define RSTRACE0(hTrace, Categ, Evnt, Dtl, Frmt)\
              RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt);

    #define RSTRACE1(hTrace, Categ, Evnt, Dtl, Frmt, Arg1)\
              RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt, Arg1);

    #define RSTRACE2(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2)\
              RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt, Arg1, Arg2);

    #define RSTRACE3(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3)\
              RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3);

    #define RSTRACE4(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3, Arg4)\
              RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3, Arg4);

    #define RSTRACE_IF(Reslt, hTrace, Categ, Evnt, Dtl, Frmt)\
              if(!Reslt) RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, Dtl, Frmt);

#else // defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)

    #define  RSTRACE0(hTrace, Categ, Evnt, Dtl, Frmt)
    #define  RSTRACE1(hTrace, Categ, Evnt, Dtl, Frmt, Arg1)
    #define  RSTRACE2(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2)
    #define  RSTRACE3(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3)
    #define  RSTRACE4(hTrace, Categ, Evnt, Dtl, Frmt, Arg1, Arg2, Arg3, Arg4)
    #define  RSTRACE_IF(Reslt, hTrace, Categ, Evnt, Dtl, Frmt)

#endif // defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)


///////////////////////////////////////////////////////////
// Additional macro definitions


// For all configurations
#define RSTRACE0_DET1(hTrace, Categ, Evnt, Frmt)\
          RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt);

#define RSTRACE1_DET1(hTrace, Categ, Evnt, Frmt, Arg1)\
          RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt, Arg1);

#define RSTRACE2_DET1(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)\
          RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt, Arg1, Arg2);

#define RSTRACE3_DET1(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)\
          RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt, Arg1, Arg2, Arg3);

#define RSTRACE4_DET1(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)\
          RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt, Arg1, Arg2, Arg3, Arg4);

#define RSTRACE_IF_DET1(Reslt, hTrace, Categ, Evnt, Frmt)\
          if (!Reslt) RSTRACE(hTrace) (hTrace, __FILE__, __LINE__, Categ, Evnt, 1, Frmt);


// Only for debug configuration
#if defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)

    # define  RSTRACE0_DET2(hTrace, Categ, Evnt, Frmt)                          RSTRACE0(hTrace, Categ, Evnt, 2, Frmt)
    # define  RSTRACE1_DET2(hTrace, Categ, Evnt, Frmt, Arg1)                    RSTRACE1(hTrace, Categ, Evnt, 2, Frmt, Arg1)
    # define  RSTRACE2_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)              RSTRACE2(hTrace, Categ, Evnt, 2, Frmt, Arg1, Arg2)
    # define  RSTRACE3_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)        RSTRACE3(hTrace, Categ, Evnt, 2, Frmt, Arg1, Arg2, Arg3)
    # define  RSTRACE4_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)  RSTRACE4(hTrace, Categ, Evnt, 2, Frmt, Arg1, Arg2, Arg3, Arg4);
    # define  RSTRACE_IF_DET2(Reslt, hTrace, Categ, Evnt, Frmt)                 RSTRACE_IF(Reslt, hTrace, Categ, Evnt, 2, Frmt)

    # define  RSTRACE0_DET3(hTrace, Categ, Evnt, Frmt)                          RSTRACE0(hTrace, Categ, Evnt, 3, Frmt)
    # define  RSTRACE1_DET3(hTrace, Categ, Evnt, Frmt, Arg1)                    RSTRACE1(hTrace, Categ, Evnt, 3, Frmt, Arg1)
    # define  RSTRACE2_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)              RSTRACE2(hTrace, Categ, Evnt, 3, Frmt, Arg1, Arg2)
    # define  RSTRACE3_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)        RSTRACE3(hTrace, Categ, Evnt, 3, Frmt, Arg1, Arg2, Arg3)
    # define  RSTRACE4_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)  RSTRACE4(hTrace, Categ, Evnt, 3, Frmt, Arg1, Arg2, Arg3, Arg4);
    # define  RSTRACE_IF_DET3(Reslt, hTrace, Categ, Evnt, Frmt)                 RSTRACE_IF(Reslt, hTrace, Categ, Evnt, 3, Frmt)

    # define  RSTRACE0_DET4(hTrace, Categ, Evnt, Frmt)                          RSTRACE0(hTrace, Categ, Evnt, 4, Frmt)
    # define  RSTRACE1_DET4(hTrace, Categ, Evnt, Frmt, Arg1)                    RSTRACE1(hTrace, Categ, Evnt, 4, Frmt, Arg1)
    # define  RSTRACE2_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)              RSTRACE2(hTrace, Categ, Evnt, 4, Frmt, Arg1, Arg2)
    # define  RSTRACE3_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)        RSTRACE3(hTrace, Categ, Evnt, 4, Frmt, Arg1, Arg2, Arg3)
    # define  RSTRACE4_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)  RSTRACE4(hTrace, Categ, Evnt, 4, Frmt, Arg1, Arg2, Arg3, Arg4);
    # define  RSTRACE_IF_DET4(Reslt, hTrace, Categ, Evnt, Frmt)                 RSTRACE_IF(Reslt, hTrace, Categ, Evnt, 4, Frmt)

#else // defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)

    #define  RSTRACE0_DET2(hTrace, Categ, Evnt, Frmt)
    #define  RSTRACE1_DET2(hTrace, Categ, Evnt, Frmt, Arg1)
    #define  RSTRACE2_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)
    #define  RSTRACE3_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)
    #define  RSTRACE4_DET2(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)
    #define  RSTRACE_IF_DET2(Reslt, hTrace, Categ, Evnt, Frmt)

    #define  RSTRACE0_DET3(hTrace, Categ, Evnt, Frmt)
    #define  RSTRACE1_DET3(hTrace, Categ, Evnt, Frmt, Arg1)
    #define  RSTRACE2_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)
    #define  RSTRACE3_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)
    #define  RSTRACE4_DET3(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)
    #define  RSTRACE_IF_DET3(Reslt, hTrace, Categ, Evnt, Frmt)

    #define  RSTRACE0_DET4(hTrace, Categ, Evnt, Frmt)
    #define  RSTRACE1_DET4(hTrace, Categ, Evnt, Frmt, Arg1)
    #define  RSTRACE2_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2)
    #define  RSTRACE3_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3)
    #define  RSTRACE4_DET4(hTrace, Categ, Evnt, Frmt, Arg1, Arg2, Arg3, Arg4)
    #define  RSTRACE_IF_DET4(Reslt, hTrace, Categ, Evnt, Frmt)

#endif // defined(DEBUG) || defined(_DEBUG) || defined(ALL_TRACE)

#endif // !__TRACEINIT_H

/* <-- EOF --> */