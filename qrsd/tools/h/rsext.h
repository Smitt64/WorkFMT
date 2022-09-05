/*-----------------------------------------------------------------------*-

 File Name   : rsext.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 August 14,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __RSEXT_H
#define __RSEXT_H

#include <stddef.h>
#include "rstypes.h"

#ifndef USEDEBUGGER
    #define RSL_STEP_INTO    2
    #define RSL_DBG_RUN      3
#endif

#define  RSTRM_GEN_ERROR    1    // Compile or execute
#define  RSTRM_MACRO_ERROR  2    // Macro to call not found or error in exec it
#define  RSTRM_NO_RSSCIPT   3    // Error init rsscript.dll

// -----------------------------------------------------------------------------
// Номера 50 и 51 заняты за CMD_RSL_HLP и CMD_RSL_HLP2 !
enum
   {
    CMD_CALLRSL,                  //
    CMD_CALLRSL_DBG,              //
    CMD_GET_VERSION  = 60,        // Получить версию RSEXT
    CMD_SEND_FILE    = 100,
    CMD_RECV_FILE,                // 101
    CMD_GET_DIRS,                 // 102
    CMD_COPY_FILE,                // 103
    CMD_RENAME_FILE,              // 104
    CMD_MKRMDIR,                  // 105
    CMD_CURDIR,                   // 106
    CMD_FILE_INFO,                // 107
    CMD_SYNC_FILE,                // 108
    CMD_GET_DIRS_EX,              // 109
    CMD_FILE_INFO_EX,             // 110
    CMD_EXISTDIR,                 // 111
    CMD_PROPERTY,                 // 112
    CMD_GET_DATETIME,             // 113

    CMD_EXEC_PROGRAM = 200,
    CMD_EDIT_MANAGER,             // 201
    CMD_EXT_EDITOR,               // 202
    CMD_CALL_VCS,                 // 203
    CMD_TOWINCLIP,                // 204
    CMD_FROMWINCLIP,              // 205
    CMD_GETLOCALEINFO,            // 206
    CMD_GETLANGID,                // 207
    CMD_ACTIVATE_PROGRAM,         // 208
    CMD_GET_LOGIC_DRIVES,         // 209
    CMD_GET_FULLPATH,             // 210
    CMD_IS_PATH_DIR,              // 211

    CMD_HTMLHELP     = 300,
    CMD_WINCLIP_SET  = 310,       // 310
    CMD_WINCLIP_GET,              // 311
    CMD_WINCLIP_EMPTY             // 312
   };

// CMD 300 используется в hhelp.c !

// -----------------------------------------------
enum
   {
    RSL_LIST_FILE = 1,
    RSL_LIST_DIR  = 2
   };

// -----------------------------------------------
enum
   {
    RSL_ACTION_BUILD,
    RSL_ACTIN_DELETE,
    RSL_ACTIN_COPY,
    RSL_ACTIN_MOVE
   };

// -----------------------------------------------
enum
   {
    RSL_GDT_DATE = 1,
    RSL_GDT_TIME = 2
   };

// -----------------------------------------------------------------------------
#define  HH_TERMEXT      "$RSEXTT"

#define  TRMEXTNAME_APP  "$RSEXTT"
#define  TRMEXTNAME      "RSEXTT"

#include <packpsh1.h>


// -----------------------------------------------------------------------------
typedef struct
      {
       db_int32  action;     // Резерв
      } TPutExtVersion;

// -----------------------------------------------------------------------------
typedef struct
      {
       db_uint16  build;
       db_uint16  verHi;
       db_uint16  verLow;
       db_uint16  subBuild;
      } TGetExtVersion;

// -----------------------------------------------------------------------------
typedef struct
      {
       db_int16  nameLen;
       db_int16  macLen;
       db_int16  numParm;
      } TPutCallRSL;

// -----------------------------------------------
typedef struct
      {
       db_int16  nameLen;
       db_int16  macLen;
       db_int16  numParm;
       db_int16  dbgMode;
       db_int16  traceFlag;
      } TPutCallRSL_DBG;

// -----------------------------------------------
typedef struct
      {
       db_int16  status;
      } TGetCallRSL;

// -----------------------------------------------
typedef struct
      {
       db_int16  status;
       db_int16  traceFlag;
      } TGetCallRSL_DBG;

// -----------------------------------------------
typedef struct
      {
       db_uint32     fileId; // remote file ID
       db_uint32     size;   // data size
       unsigned char code;   // operation code
      } TPutSendFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32      fileId; // remote file ID
       db_uint32      size;   // data size
       unsigned char  code;   // operation code
       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       db_uint32      atime;
       db_uint32      mtime;
       db_uint32      ctime;

       bool           forceCopy;  // Копирование открытых для записи файлов
      } TPutSendFileEx;

// -----------------------------------------------
typedef struct
      {
       db_uint32     fileId;
       db_int16      status;
      } TGetSendFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32     fileId; // remote file ID
       db_uint32     size;   // data size to read
       unsigned char code;   // operation code
      } TPutRecvFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32     fileId;
       db_int16      status;
       db_uint32     size; // read data size
      } TGetRecvFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32      fileId;
       db_int16       status;
       db_uint32      size; // read data size
       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       db_uint32      atime;
       db_uint32      mtime;
       db_uint32      ctime;

       bool           forceCopy;  // Копирование открытых для записи файлов
      } TGetRecvFileEx;

// -----------------------------------------------
typedef struct
      {
       db_uint32      Id;   // remote search ID
       unsigned char  code; // operation code
       db_uint32      attr;
       db_uint32      size;
       db_uint16      action;
      } TPutDirs;

// -----------------------------------------------
typedef struct
      {
       db_uint32  Id;  // remote search ID
       db_uint16  num; // Number of TDirElem
      } TGetDirs;

// -----------------------------------------------
typedef struct
      {
       db_uint32      fsize;
       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       unsigned char  isDir; // if action RSL_ACTIN_COPY or RSL_ACTIN_MOVE, this is a copy flag
       unsigned char  isDel;
       db_uint16      len;
       //  ... len bytes - the filename
      } TGetDirElem;

// -----------------------------------------------
typedef struct
      {
       db_uint32      fsize;
       db_int64       fsizeFull;
       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       unsigned char  isDir; // if action RSL_ACTIN_COPY or RSL_ACTIN_MOVE, this is a copy flag
       unsigned char  isDel;
       db_uint16      len;
       //  ... len bytes - the filename
      } TGetDirElemEx;

// -----------------------------------------------
typedef struct
      {
       db_uint32     dstSize;
       db_uint32     srcSize;
       // .....
      } TPutGetCopyFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32     dstSize;
       db_uint32     srcSize;
       // .....
      } TPutRenameFile;

// -----------------------------------------------
typedef struct
      {
       db_int32    status;
      } TGetRenameFile;

// -----------------------------------------------
typedef struct
      {
       db_uint32     dword;
      } TPutGetLogicDrives;

// -----------------------------------------------
typedef struct
      {
       db_uint32 stat;
      } TPutGetFullPath;

// -----------------------------------------------
typedef struct
      {
       db_uint32     srcSize;
       db_int16      oper;
       // .....
      } TPutMkRmDir;

// -----------------------------------------------
typedef struct
      {
       db_int32    status;
      } TGetMkRmDir;

// -----------------------------------------------
typedef struct
      {
       db_int16       oper;
      } TPutCurDir;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;
       db_uint32      size;
       // ....
      } TGetCurDir;

// -----------------------------------------------------------------------------
typedef int (*TFileCopyProgress)(int code, size_t fileSize, size_t curSz, void *data);

// -----------------------------------------------
#define  PGM_START         0
#define  PGM_STOP          1
#define  PGM_WAIT          2
#define  PGM_ACTIVATE      3
#define  PGM_OPENRESOURCE  4
#define  PGM_SHELLEXECUTE  5

// -----------------------------------------------
typedef struct
      {
       unsigned char  cmdID;
       db_uint32      processID;
       db_int16       exitCode;
       db_int16       timeOut;
      } TPutExecPgm;

// -----------------------------------------------
typedef struct
      {
       db_int32    status;
      } TGetExecPgm;

// -----------------------------------------------
typedef struct
      {
       unsigned char  dummy;
      } TPutFileInfo;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;

       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       db_uint32      size;
       db_uint32      atime;
       db_uint32      mtime;
       db_uint32      ctime;
      } TGetFileInfo;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;

       unsigned char  day;
       unsigned char  mon;
       db_uint16      year;
       unsigned char  hour;
       unsigned char  min;
       unsigned char  sec;
       db_uint32      size;
       db_uint32      atime;
       db_uint32      mtime;
       db_uint32      ctime;
       db_int64       sizeFull;
      } TGetFileInfoEx;

// -----------------------------------------------
typedef struct
      {
       db_uint16   syncType;
       db_uint32   syncDataLow;
       db_uint32   syncDataHigh;
      } TPutSyncFile;

// -----------------------------------------------
typedef struct
      {
       db_int32    status;
      } TGetSyncFile;


// -----------------------------------------------------------------------------
#define  EDITMGR_CONNECT         1
#define  EDITMGR_DISCONNECT      2
#define  EDITMGR_GETCOOKIE       3
#define  EDITMGR_SENDCOMMAND   100
#define  EDITMGR_CRTWINEDIT    101
#define  EDITMGR_SETDESID      102

#define  EDITMGR_ASYNC_TABLE   "EDITMGR_ASYNC_TABLE"

// -----------------------------------------------
typedef struct
      {
       unsigned char  cmdID;
       db_int32       lParam;
      } TPutEditManager;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;
      } TGetEditManager;


// -----------------------------------------------------------------------------
#define  EXTEDIT_INIT            0
#define  EXTEDIT_DONE            1
#define  EXTEDIT_PREMODAL        2
#define  EXTEDIT_POSTMODAL       3
#define  EXTEDIT_LOADFILE        4
#define  EXTEDIT_SAVEALL         5
#define  EXTEDIT_RELOADALL       6
#define  EXTEDIT_GETFNAME        7
#define  EXTEDIT_CLEARERROR      8
#define  EXTEDIT_ADDERROR        9
#define  EXTEDIT_GOTOERROR      10
#define  EXTEDIT_SHOWEDITOR     11
#define  EXTEDIT_ONCOMMAND     100

#define  EXTEDIT_ASYNC_TABLE   "EXTEDIT_ASYNC_TABLE"

#define  EXTEDIT_CMD_RUN               1
#define  EXTEDIT_CMD_DEBUG             2
#define  EXTEDIT_CMD_COMPILE           3
#define  EXTEDIT_CMD_RUNNODBG          4
#define  EXTEDIT_CMD_RUN_DESIGNER     10
#define  EXTEDIT_CMD_OPEN_RESOURCE    11
#define  EXTEDIT_CMD_QUIT           9999

// -----------------------------------------------
typedef struct
      {
       unsigned char  cmdID;
       db_int32       lParam;
       db_uint32      hEditor;
      } TPutExtEditor;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;
      } TGetExtEditor;

// -----------------------------------------------
#define  VCS_CHECKOUT         0
#define  VCS_CHECKIN          1
#define  VCS_UNDOCHECKOUT     2
#define  VCS_GETFILENAME      3
#define  VCS_COPYFILE       100

#define  VCS_ASYNC_TABLE    "VCS_ASYNC_TABLE"

// -----------------------------------------------
typedef struct
      {
       unsigned char  cmdID;
       db_int32       lParam;
      } TPutVCS;

// -----------------------------------------------
typedef struct
      {
       db_int32       status;
      } TGetVCS;

// -----------------------------------------------
typedef struct
      {
       unsigned char  selType;
       unsigned char  codePage;
       db_int32       size;
      } TPutWinClip;

// -----------------------------------------------
typedef struct
      {
       db_int32  stat;
      } TGetWinClip;

// -----------------------------------------------
typedef struct
      {
       unsigned char  codePage;
      } TPutWinClip2;

// -----------------------------------------------
typedef struct
      {
       db_int32        stat;
       db_int32        size;
       unsigned  char  selType;
      } TGetWinClip2;

// -----------------------------------------------
typedef struct
      {
       db_int32  id;
       db_int32  code;
      } TPutGetLocaleInfo;

// -----------------------------------------------
typedef struct
      {
       db_int32  result;
      } TGetGetLocaleInfo;

// -----------------------------------------------
typedef struct
      {
       db_int32  fmt;       // Формат данных
       db_int32  type;      // Пока не используется
       db_int32  cp;        // Пока не используется
       db_int32  size;      // Размер данных
      } TWinClipData;

// -----------------------------------------------
typedef struct
      {
       db_int32  size;      // Размер данных
       db_int32  stat;      // Результат
      } TGetWinClipData;

// -----------------------------------------------
typedef struct
      {
       db_int32  size;      // Размер строки, размещенной за структурой
       db_int16  code;      // Код операции (см. code в TToolsPropertyMsg)
                            // Дополнение: если -1, выполнить очистку списка (новые параметры)
                            // при этом могут отсутствовать.
      } TPutProperty;

// -----------------------------------------------
typedef struct
      {
       db_int32  size;      // Размер строки, размещенной за структурой
       db_int32  ret;       // Возвращаемое значение
      } TGetProperty;

// -----------------------------------------------
typedef struct
      {
       db_int32  code;      // 1 - получить дату, 2 - получить время
      } TPutDateTime;

// -----------------------------------------------
typedef struct
      {
       db_int32  stat;      // Результат операции

       db_int32  p1;        // Год/часы
       db_int32  p2;        // Месяц/минуты
       db_int32  p3;        // День/секунды
       db_int32  p4;        // День недели/Сотые доли секунды
      } TGetDateTime;

#include <packpop.h>

// -----------------------------------------------------------------------------
RSL_CFUNC void RsExtsTable(void);
RSL_CFUNC void RegDirListClass(void);
RSL_CFUNC bool UseRemote(void);

// -----------------------------------------------------------------------------
// Записать пару "параметр=значение" в список параметров на расширение.
// Если value = NULL, параметр key содержит готовую пару "параметр=значение"
// (либо несколько пар, разделенных символом ';').
// В случае успеха возвращает 0.
int RsExtsAddProperty(const char *key, const char *value);

// -----------------------------------------------------------------------------
// Получить значение свойства key. Параметр len содержит полный размер буфера для value.
// Возвращает размер значения параметра, который будет записан в value (без учета 0-символа)
int RsExttGetProperty(const char *key, char *value, int len);

// -----------------------------------------------------------------------------
extern char  *GlobalIncDir;

#endif // __RSEXT_H

/* <-- EOF --> */