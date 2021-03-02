/*-----------------------------------------------------------------------*-

 File Name   : btrexp.h

 Sybsystem   : Btrieve
 Description : If you use this library as DLL, you must define _RSLDLL

 Source      :

 Library(s)  : rslbtr.dll


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 June 15,1995  Sergei Kubrin (K78) - Create file
 January 24,1996   Serg Kubrin (K78)
            MS and Borland compilers support
-*-----------------------------------------------------------------------*/

/*   August 25,1995 (K78)
     Добавить процедуру BtGetInfo (BTHANDLE) возвращающую информацию
     о количестве записей в файле, количестве ключей и т.д.

     Для работы с полями добавить:
         BtGetFieldNum  - количество полей в файле
         BtGetFieldName - название поля по его номеру

         Процедуры вида BtGetInt, BtGetDouble, BtGetString и т.д.
         с преобразованием типа при необходимости


     Добавить процедуры позиционирования в BLOB и режим открытия
     Read/Write. В этом режиме при первой операции записи в заголовок
     BLOB-а заносить признак, что BLOB модифицирован.

     Добавить процедуру BtIsBlob (BTHANDLE), которая возвращает признак
     наличия BLOB в файле.

     Для обработки ошибок добавить пользовательский CALLBACK

*/

#ifndef __BTREXP_H
#define __BTREXP_H

#ifndef  __PLATFORM_H
#include "platform.h"
#endif

#ifndef NO_OLD_FIELD_TYPES
    #define NO_OLD_FIELD_TYPES
#endif

#ifndef TFVT_DEFINED
#include "fieldtp.h"
#endif

#include <bdate.h>

// -----------------------------------------------------------------------------
enum
   {
    FT_DB_CHR,
    FT_DB_INT,
    FT_DB_LONG,
    FT_DB_FLOAT4,
    FT_DB_FLOAT8,
    FT_DB_FLOAT10,
    FT_DB_MONEY,
    FT_DB_MN8,
    FT_DB_MN10,
    FT_DB_STRING,
    FT_DB_DATE,
    FT_DB_TIME,
    FT_DB_NUMERIC
   };

// -----------------------------------------------------------------------------
enum
   {
    FT_C_CHR            = 13,
    FT_C_INT            = 0,
    FT_C_LONG           = 1,
    FT_C_FLOAT4         = 2,
    FT_C_FLOAT8         = 4,
    FT_C_FLOAT10        = 17,

    FT_C_MONEY          = 15,

    FT_C_MN8            = 6,
    FT_C_MN10           = 14,

    FT_C_STRING         = 7,

    FT_C_DATE           = 9,
    FT_C_TIME           = 10,
    FT_C_NUMERIC        = 25
   };

// -----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
#ifndef __NWBTRV_H

#include <packpsh1.h>

// -----------------------------------------------------------------------------
typedef struct
      {
       char  sign;      /* Signature byte 0xAC                  */
       char  name[8];   /* Uniquely sequence name               */
       char  tab[256];  /* Translation table                    */
      } AltCol;

// -----------------------------------------------------------------------------
typedef struct TBtID
      {
       char            reserv[12];
       char            id1[2];
       unsigned short  id2;
      } BTClientID;

#include <packpop.h>

// -----------------------------------------------------------------------------
typedef short  bclient_h;

// -----------------------------------------------------------------------------
#define  BCLIENT_BAD_HANDLE  ((bclient_h)-1)
#define  BCLIENT_DEF_HANDLE  ((bclient_h)0)
#define  BCLIENT_EXT_HANDLE  ((bclient_h)-2)

// -----------------------------------------------------------------------------
enum Berors
   {
    BEOK        =   0,        // By (JNS)
    BEinoper    =   1,
    BEioerr     =   2,
    BEnoopen    =   3,
    BEnorec     =   4,
    BEdupkey    =   5,
    BEinvkey    =   6,
    BEdifkey    =   7,
    BEinpos     =   8,
    BEeof       =   9,
    BEmodkey    =  10,
    BEfilenm    =  11,
    BEnofile    =  12,
    BEexfile    =  13,
    BEclose     =  17,
    BEfulld     =  18,
    BEldata     =  22,
    BEnobfile   =  30,
    BEnumrec    =  43,
    BEmaxop     =  47,
    BEowner     =  50,
    BEefile     =  59,
    BEdes       =  62,
    BEdeadLock  =  78,
    BEconflict  =  80,
    BElostpos   =  82,
    BErecloc    =  84,
    BEfileloc   =  85,
    BEtableful  =  86,
    BEchunkOffs = 103
   };

#endif

// -----------------------------------------------------------------------------
RSL_MAKE_HANDLE(BTHANDLE);
RSL_MAKE_HANDLE(BDHANDLE);
RSL_MAKE_HANDLE(TConId);
RSL_MAKE_HANDLE(BNDHANDLE);

// -----------------------------------------------------------------------------
#ifndef BKMHDEFINED
    #define BKMHDEFINED
    RSL_MAKE_HANDLE(BKMHANDLE);
#endif

#ifndef __BTINTF_H
#include "rsl/btintf.h"
#endif

// -----------------------------------------------------------------------------
#if defined(RSL_MD_SUN)
    #define BTAPI
#elif defined(RSL_PL_WIN32)
    #define BTAPI  __stdcall
#elif  defined(RSL_PL_OS2)
    #define BTAPI  __syscall
#else
    #define BTAPI _far _pascal _loadds
#endif

// -----------------------------------------------------------------------------
#if defined(_BUILDRSLDLL) || defined(RSL_BUILD_DLL)
        #define BREXPIMP
        #define MSEXPIMP  __declspec(dllexport)
#elif (defined(_RSLDLL) || defined(RSL_USE_DLL) || defined(__RSRTL_DLL)) && defined(RSL_PL_WIN32)
        #define BREXPIMP
        #define MSEXPIMP __declspec(dllimport)
#else   // Use Static lib or use WIN16 DLL
    #define BREXPIMP
    #define MSEXPIMP
#endif

// -----------------------------------------------------------------------------
// Modes for BtOpenTable
#define  BT_OPEN_ACCELERATED  -1
#define  BT_OPEN_READONLY     -2
#define  BT_OPEN_VERIFY       -3
#define  BT_OPEN_EXCLUSIVE    -4
#define  BT_OPEN_NORMAL        0

// -----------------------------------------------------------------------------
// BLOB types
#define  BT_NOBLOB     0
#define  BT_SBLOB      1

// -----------------------------------------------------------------------------
// Modes for BtOpenBlob
#define  BT_BLOB_READ   0
#define  BT_BLOB_WRITE  1
#define  BT_BLOB_RDWR   2

// -----------------------------------------------------------------------------
// Flags for BtBeginTransaction
#define  BT_TRN_CONCURENT

// -----------------------------------------------------------------------------
// Flags for BtSetRecordLock and BtBeginTransaction
#define  BT_LOCK_SWL  100
#define  BT_LOCK_SNL  200
#define  BT_LOCK_MWL  300
#define  BT_LOCK_MNL  400

// -----------------------------------------------------------------------------
// Modes for BtUnlock
#define  BT_UNLOCK_SINGLE  0
#define  BT_UNLOCK_ONE    -1
#define  BT_UNLOCK_ALL    -2


// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBaseSimple
                                 (
                                  char RSL_FAR       *path,  // Dictionary pathname  (c:\data\mydb.def)
                                  char RSL_FAR       *data,  // Paths to data files  (c:\data;d:\samples;d:\base)
                                  int                 rdOnly,
                                  TBtError RSL_FAR   *erInfo
                                 );

// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBaseSimpleClntID
                                 (
                                  char RSL_FAR       *path,  // Dictionary pathname  (c:\data\mydb.def)
                                  char RSL_FAR       *data,  // Paths to data files  (c:\data;d:\samples;d:\base)
                                  int                 rdOnly,
                                  TBtError RSL_FAR   *erInfo,
                                  bclient_h           clientID,
                                  BTClientID         *BtrID
                                 );

// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBaseEx3
                                 (
                                  char RSL_FAR      *path,  // Dictionary pathname  (c:\data\mydb.def)
                                  char RSL_FAR      *data,  // Paths to data files  (c:\data;d:\samples;d:\base)
                                  TBtMesProc         aBtMesProc,
                                  TBtMemAllocProc    allocPtr,
                                  TBtMemFreeProc     freePtr,
                                  char RSL_FAR      *globalPassword,
                                  int                reportError, // if true call aBtMesProc for Btrieve errors
                                  int                rdOnly,
                                  TBtError RSL_FAR  *erInfo,
                                  bclient_h          clientID,
                                  BTClientID        *BtrID
                                 );

// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBaseEx2
                                 (
                                  char RSL_FAR      *path,  // Dictionary pathname  (c:\data\mydb.def)
                                  char RSL_FAR      *data,  // Paths to data files  (c:\data;d:\samples;d:\base)
                                  TBtMesProc         aBtMesProc,
                                  TBtMemAllocProc    allocPtr,
                                  TBtMemFreeProc     freePtr,
                                  char RSL_FAR      *globalPassword,
                                  int                reportError, // if true call aBtMesProc for Btrieve errors
                                  int                rdOnly,
                                  TBtError RSL_FAR  *erInfo,
                                  TConId             id   // Connection ID (reserved)
                                 );

// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBase(char RSL_FAR *path, char RSL_FAR *data,
                                                char RSL_FAR *globalPassword, int reportError);

// -----------------------------------------------------------------------------
MSEXPIMP BDHANDLE BTAPI BREXPIMP BtOpenDataBaseEx
                                 (
                                  char RSL_FAR    *path,  // Dictionary pathname  (c:\data\mydb.def)
                                  char RSL_FAR    *data,  // Paths to data files  (c:\data;d:\samples;d:\base)
                                  TBtMesProc       aBtMesProc,  // May be NULL
                                  TBtMemAllocProc  allocPtr,  // May be NULL
                                  TBtMemFreeProc   freePtr,  // May be NULL
                                  char RSL_FAR    *globalPassword,  // Owner name for all files
                                  int              reportError,  // If 0 do not show Btrieve error dialog box
                                                                 // Else show only errors that not in Scip List
                                                                 // Default Beof,Benorec,BEdupkey
                                  TConId           id            // Connection ID (reserved)
                                 );

// -----------------------------------------------------------------------------
MSEXPIMP void           BTAPI BREXPIMP BtCloseDataBase     (BDHANDLE db);

MSEXPIMP void           BTAPI BREXPIMP BtErrorDialog       (BDHANDLE db, int er, int show);

MSEXPIMP TAltColProc    BTAPI BREXPIMP BtSetAltColProc     (BDHANDLE db, TAltColProc newProc, void RSL_FAR *data);
MSEXPIMP void           BTAPI BREXPIMP BtSetBlobType       (BDHANDLE db, int tp);

MSEXPIMP int            BTAPI BREXPIMP BtCreateTable       (BDHANDLE db, char RSL_FAR *strName, char RSL_FAR *file);
MSEXPIMP int            BTAPI BREXPIMP BtCreateTableEx     (BDHANDLE db, char RSL_FAR *name, char RSL_FAR *buff, int len);

MSEXPIMP BTHANDLE       BTAPI BREXPIMP BtOpenTable         (BDHANDLE db, char RSL_FAR *name, int mode, int keypath, char RSL_FAR *fileName);
MSEXPIMP void           BTAPI BREXPIMP BtCloseTable        (BTHANDLE hd);

MSEXPIMP void           BTAPI BREXPIMP BtSetLockFlag       (BDHANDLE db, int reclock);
MSEXPIMP void           BTAPI BREXPIMP BtSetNoCurChangeFlag(BDHANDLE db, int isNoChange);

MSEXPIMP void          *BTAPI BREXPIMP BtGetVarlenInfo     (BTHANDLE hd, unsigned RSL_FAR *size);
MSEXPIMP unsigned       BTAPI BREXPIMP BtGetVarlenSize     (BTHANDLE hd);
MSEXPIMP void           BTAPI BREXPIMP BtSetVarlenSize     (BTHANDLE hd, unsigned size);

MSEXPIMP int            BTAPI BREXPIMP BtFetchNext         (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtFetchPrev         (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtUpdate            (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtInsert            (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtDelete            (BTHANDLE hd);
MSEXPIMP void           BTAPI BREXPIMP BtReset             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtGetEQ             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtGetGE             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtGetLE             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtGetGT             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtGetLT             (BTHANDLE hd);
MSEXPIMP int            BTAPI BREXPIMP BtChangeKeyPath     (BTHANDLE hd, int path);
MSEXPIMP int            BTAPI BREXPIMP BtGetPos            (BTHANDLE hd, BKMHANDLE RSL_FAR *pos);
MSEXPIMP int            BTAPI BREXPIMP BtGetDirect         (BTHANDLE hd, BKMHANDLE pos);
MSEXPIMP int            BTAPI BREXPIMP BtReleasePos        (BTHANDLE hd, BKMHANDLE pos);
MSEXPIMP int            BTAPI BREXPIMP BtUnlock            (BTHANDLE tb, int mode, unsigned long addr);

MSEXPIMP int            BTAPI BREXPIMP BtGetLastError      (BDHANDLE db);


MSEXPIMP BNDHANDLE      BTAPI BREXPIMP BtBindField         (BTHANDLE hd, char RSL_FAR *name, void RSL_FAR *data, unsigned char dType, int len);
MSEXPIMP int            BTAPI BREXPIMP BtUnbindField       (BTHANDLE hd, BNDHANDLE id);
MSEXPIMP void           BTAPI BREXPIMP BtUnbindAll         (BTHANDLE hd);

//  Return Field number or -1 on error
MSEXPIMP int            BTAPI BREXPIMP BtGetFieldID        (BTHANDLE hd, char RSL_FAR *name);

// Return Field type or -1 on error
MSEXPIMP int            BTAPI BREXPIMP BtGetField          (BTHANDLE hd, int ind, void RSL_FAR *aPtr, unsigned char dType, int bufLen);
MSEXPIMP int            BTAPI BREXPIMP BtPutField          (BTHANDLE hd, int ind, void RSL_FAR *aPtr, unsigned char dType);


MSEXPIMP int            BTAPI BREXPIMP BtBeginTransaction  (BDHANDLE db,int flags);
MSEXPIMP int            BTAPI BREXPIMP BtEndTransaction    (BDHANDLE db);
MSEXPIMP int            BTAPI BREXPIMP BtAbortTransaction  (BDHANDLE db);

MSEXPIMP void           BTAPI BREXPIMP BtSetBlobBuffSize   (BDHANDLE db, unsigned size); // Default 1024
MSEXPIMP int            BTAPI BREXPIMP BtOpenBlob          (BTHANDLE bf, int openMode);
MSEXPIMP int            BTAPI BREXPIMP BtCloseBlob         (BTHANDLE bf);

// Return Number of bytes read
MSEXPIMP int            BTAPI BREXPIMP BtReadBlob          (BTHANDLE bf, char RSL_FAR *buff, int size);

// Return number of bytes written
MSEXPIMP int            BTAPI BREXPIMP BtWriteBlob         (BTHANDLE bf, char RSL_FAR *buff, int size);
MSEXPIMP int            BTAPI BREXPIMP BtPosBlob           (BTHANDLE bf, unsigned long pos);
MSEXPIMP int            BTAPI BREXPIMP BtGetBlobSize       (BTHANDLE bf, unsigned long *size);
MSEXPIMP int            BTAPI BREXPIMP BtTruncateBlob      (BTHANDLE bf);

MSEXPIMP char RSL_FAR  *BTAPI BREXPIMP BtErrorText         (int erCode);

MSEXPIMP void           BTAPI BREXPIMP BtStopClient        (TConId id);

MSEXPIMP int            BTAPI BREXPIMP BtLoopTables        (BDHANDLE db, TLoopProc proc, void RSL_FAR *data);

// 6.08.1997
MSEXPIMP void           BTAPI BREXPIMP BtClearRecordBuff   (BTHANDLE hd);

MSEXPIMP int            BTAPI BREXPIMP BtSetCnvMode        (BDHANDLE hdb, int mode);
MSEXPIMP TBtError      *BTAPI BREXPIMP BtGetErrorInfo      (BDHANDLE db, int *numInfo);

MSEXPIMP TBtStructure  *BTAPI BREXPIMP BtGetStructure      (BDHANDLE hd, const char *name);

MSEXPIMP void           BTAPI BREXPIMP BtFreeStructure     (TBtStructure *str);
MSEXPIMP char          *BTAPI BREXPIMP BtStrGetFieldName   (TBtStructure *str, int nf);
MSEXPIMP char          *BTAPI BREXPIMP BtStrGetComment     (TBtStructure *str, int nf);
MSEXPIMP char          *BTAPI BREXPIMP BtStrGetFileComment (TBtStructure *str);
MSEXPIMP char          *BTAPI BREXPIMP BtStrGetKeyComment  (TBtStructure *str, int nf);

MSEXPIMP void          *BTAPI BREXPIMP BtGetRecBuffer      (BTHANDLE bf, unsigned *size);

#ifdef __cplusplus
}
#endif

#endif // __BTREXP_H

/* <-- EOF --> */