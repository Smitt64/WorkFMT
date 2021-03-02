/********************************************************************
   Created: 2001/12/19
   Created: 19:12:2001   13:11
   Filename: D:\Work\RSTrace\TraceDecl.h
   Project: RSTrace
   Author: Andrew Ushakov
*********************************************************************/

#ifndef __TRACEDECL_H
#define __TRACEDECL_H

#include "packpshn.h"

// AV 10.02.2011 максимальная длина строки, выводимой в лог
#define  RSTRACE_MAX_OUTPUT_STR  4096

// Trace handle
typedef struct tagTrace
      {
       void (RSL_CDECL *ProcTrace)(struct tagTrace *, const char *, int, const char *, const char *, int, const char *, ...);
      } Trace, *HTRACE;

#include "packpop.h"

//// TraceEnable results
// Trace failed
#define  TRACE_FAIL            0
// Trace enabled
#define  TRACE_SUCCESS         1
// Trace enabled, not for all files
#define  TRACE_NOT_ALL_FILES   2
// Trace failed, no opened files
#define  TRACE_NO_FILES        3

#endif // !__TRACEDECL_H

/* <-- EOF --> */