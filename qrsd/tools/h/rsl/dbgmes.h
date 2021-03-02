/*-----------------------------------------------------------------------*-

 File Name   : dbgmes.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/dbgmes.h $
               $Revision: 11 $
               $Date: 23.09.00 11:32 $
               $Author: Kubrin $
               $Modtime: 23.09.00 11:27 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 August 30,2000  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __DBGMES_H
#define __DBGMES_H


#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
TDbgStub * FindStubByKey (RslHandle32 key);

#ifdef __cplusplus
}
#endif

#include <packpsh1.h>

// -----------------------------------------------
#ifndef RSL_DLM_MES_TABLE
    #define  RSL_DLM_MES_TABLE(name)  int name (int cmd, const void *inMes, void *outMes)   \
                                      { switch (cmd) {  default:  break;
#endif

// -----------------------------------------------
#ifndef RSL_DLM_HANDLE
    #define  RSL_DLM_HANDLE(msg)  case msg:  return mes_##msg (inMes, outMes);
#endif

// -----------------------------------------------
#ifndef RSL_DLM_MES_END
    #define  RSL_DLM_MES_END   } return 0; }
#endif

// -----------------------------------------------------------------------------
enum
   {
    CMD_INITDBG = 10,
    CMD_DONEDBG,
    CMD_BREAK,
    CMD_TRACE,
    CMD_NOTIFYMOD
   };

// -----------------------------------------------
enum
   {
    CMD_ENUMSTACK,
    CMD_ENUMMOD,
    CMD_GETSTMTOFPOS,
    CMD_BRKAT,
    CMD_CLRBRK,
    CMD_SETBRK,
    CMD_REMBRK,
    CMD_PARSEEXP,
    CMD_REMEXP,
    CMD_EXECEXP,
    CMD_FIRSTLOCALS,
    CMD_FIRSTPROPS,
    CMD_GETNEXTVAL,
    CMD_REFRESHVAL,
    CMD_SETNEWVAL,
    CMD_FREEVAL,
    CMD_OPENSRC,
    CMD_READSRC,
    CMD_GETSZSRC,
    CMD_CLOSESRC,
    CMD_INT,
    CMD_EXEC,
    CMD_GETWND,
    CMD_SETFLAG,
    CMD_VERSION,
    CMD_FLINFO,
    CMD_SETEXPVAL,
    CMD_GETVALSZ,
    CMD_GETVALDATA,
    CMD_SETVALDATA,
    CMD_GETEXPSZ,
    CMD_GETEXPDATA,
    CMD_SETEXPDATA,
    CMD_FIRSTARRAY,
    CMD_LASTERRTEXT,
    CMD_EXECEXPINTER,
    CMD_EXPINTERSTAT
   };

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  data;
      } TCmdCallRemote;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  sz;
       char       buff[1];
      } TCmdCallTrace;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hmod;
       db_uint16  add;
      } TCmdCallNotifyModule;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hstack;
       db_uint16  szProcName;
       db_uint16  szModName;
      } TCmdCallEnumStack;

// -----------------------------------------------
typedef struct
      {
       db_uint32  hstack;
       db_uint32  hproc;
       db_uint32  hmod;
       db_uint32  hstmt;
       db_int32   offs;
       db_int32   len;
       char       buff[2]; // procName and modName
      } TCmdResultEnumStack;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hmod;
       db_uint16  szModName;
      } TCmdCallEnumModule;

// -----------------------------------------------
typedef struct
      {
       db_uint32  hmod;
       db_int32   modType;
       char       buff[1]; // modName
      } TCmdResultEnumModule;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hmod;

       db_int32   offs;
       db_int32   len;
      } TCmdCallStOfPos;

// -----------------------------------------------
typedef struct
      {
       db_uint32  hstmt;

       db_int32   offs;
       db_int32   len;
       db_int32   line;
      } TCmdResultStOfPos;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hexp;
       db_uint32  hstack;

       db_int32   szTpName;
       db_int32   szValue;
      } TCmdCallExecExp;

// -----------------------------------------------
typedef struct
      {
       db_uint32  hval;
       db_int16   isObject;
       char       buff[2]; // tpName, ValBuff
      } TCmdResultExecExp;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;
      } TCmdGetValSz;

// -----------------------------------------------
typedef struct
      {
       db_int32  size;
      } TCmdPutValSz;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;
       db_int32   size;
       db_int32   offs;
      } TCmdGetValData;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;
       db_int32   phase;
       db_int32   size;
      } TCmdSetValData;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;

       db_int32   szName;
       db_int32   szTpName;
       db_int32   szValue;
      } TCmdCallValInfo;

// -----------------------------------------------
typedef struct
      {
       db_uint32  hinfo;
       db_uint32  hval;
       db_int16   isObject;
       char       buff[3]; // name, tpName, ValBuff
      } TCmdResultValInfo;

// -----------------------------------------------
typedef struct
      {
       db_int32   result;
       db_uint32  handle;
      } TCmdResultMes;

// -----------------------------------------------
typedef struct
      {
       db_int32   result;
       db_uint32  handle;
       db_int32   lVal;
      } TCmdResultParse;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hd;
       db_uint32  size;
      } TEnumHandleIn;

// -----------------------------------------------
typedef struct
      {
       db_uint32  size;
       db_uint32  buff[1];
      } TEnumHandleOut;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  data;
       db_int32   sz;
      } TGetStringIn;

// -----------------------------------------------
typedef struct
      {
       db_int32  sz;
       char      buff[1];
      } TGetStringOut;

// -----------------------------------------------
typedef struct
      {
       db_uint16  isBtr;
       char       buff[1];
      } TGetModOut;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;

       db_uint32  uint1;
      } TCmdCall1UInt;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;

       db_int32   int1;
       db_int32   int2;
      } TCmdCall2Int;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  handle;

       char       buff[1];
      } TCmdCallStr;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hinfo;
       db_uint32  hstack;

       char       buff[1];
      } TCmdCallSetNewInfo;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hexp;
       db_uint32  hstack;

       char       buff[1];
      } TCmdCallSetNewExpVal;

// -----------------------------------------------
typedef struct
      {
       db_int32   int1;
       db_int32   int2;
      } TCmdRet2Int;

// -----------------------------------------------
typedef struct
      {
       db_uint32  data;
       db_uint32  offs;
      } TRslBreakElemCall;

// -----------------------------------------------
typedef struct
      {
       db_uint32  stmt;
       db_uint32  offs;
       db_uint16  len;
       db_uint16  codeLine;
      } TRslBreakElemResult;

// -----------------------------------------------
typedef struct
      {
       db_uint32  inst;
       db_uint32  hmod;
       db_uint16  num;

       TRslBreakElemCall  elem[1];
      } TCmdCallSetBrk;

// -----------------------------------------------
typedef struct
      {
       db_uint32  tm;
       char       name[1];
      } TCmdResultFileInfo;
#include <packpop.h>

#endif // __DBGMES_H

/* <-- EOF --> */