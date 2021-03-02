/*-----------------------------------------------------------------------*-

 File Name   : btintf.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 December 27,1996  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __BTINTF_H
#define __BTINTF_H

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// ���� ᮮ�饭�� ��� BtMesProc
typedef enum tagBtMesType
      {
       BT_ERROR,       // Btrieve operation error
       BT_IN_USE,      // File in use. Need asc for continue
       BT_PAUSE,       // Need make a pause
       BT_NOMEM,

       BT_DICT,        // Dictionary check error
       BT_FILE_PLACE,  // Can not locate file in data directories
       BT_BINDTYPE,    // Bad type used for bind field
       BT_NOFIELD,     // Requested field not found
       BT_SQLERROR     // SQL error
      } BtMesType_v;

// -----------------------------------------------------------------------------
typedef int  BtMesType;

// -----------------------------------------------------------------------------
typedef struct
      {
       char      name[20];
       long      Code_File;    // ��� 䠩��
       long      fileFlags;
       short     maxvarlen;
       char      comment[40];
      } TBtLoop;

// -----------------------------------------------------------------------------
enum
   {
    BT_LIB_ERROR,
    BT_ENGINE_ERROR
   };

// -----------------------------------------------------------------------------
enum
   {
    BT_LIB_OK,
    BT_LIB_NOMEM,
    BT_LIB_NOFILE,
    BT_LIB_NOFIELD,
    BT_LIB_BINDER,
    BT_LIB_DICTER
   };

// -----------------------------------------------------------------------------
enum
   {
    RSL_BCNV_NOCNV,
    RSL_BCNV_DBO_USRA,
    RSL_BCNV_DBA_USRO
   };

// -----------------------------------------------------------------------------
#define  RSL_BT_INFO_LEN  80

// -----------------------------------------------------------------------------
typedef struct
      {
       int    level;   // BT_LIB_ERROR or BT_ENGINE_ERROR
       int    erCode;  // Btrieve or LIB errcode.
       int    btOp;
       char   info[RSL_BT_INFO_LEN]; // Btrieve file name for RSL_BTLIB_ERROR
       bool   isSQL;   // ���������� �� �ᥣ�� � �� �����!
      } TBtError;

// -----------------------------------------------------------------------------
// ���ᠭ�� ����� Btrieve 䠩�� nfields ���
typedef struct
      {
       int  fieldtype;       // Type of this field
       int  size;            // Size of this field
       int  pos;             // Position of this field (base == 0)
      } ResBtrField;

// -----------------------------------------------------------------------------
// ���ᠭ�� ����� Btrieve 䠩�� nfields ���
typedef struct
      {
       unsigned short  outLen;
       short           decPoint;
       unsigned char   hidden;
      } ResBtrFieldAttr;

// -----------------------------------------------------------------------------
//  ���ᠭ�� ᥣ���⮢ ���祩. nkeyseg ���
typedef struct
      {
       short int   key;           // Key number ( 0 is the first key )
       short int   seg;           // Key segment ( 0 is the first segment )
       short int   field;         // Correspondent field index
       short int   size;          // Key len
      } ResBtrKey;

// -----------------------------------------------------------------------------
//  ���ᠭ�� ᥣ���⮢ ���祩. nkeyseg ���
typedef struct
      {
       short int      flags;
       unsigned char  keyType;
       unsigned char  nullVal;
      } ResBtrKeyAttr;

// -----------------------------------------------------------------------------
// ��� �����⢫���� ���樨 �� ᥣ���⠬ ����
typedef struct
      {
       int         key;
       ResBtrKey  *ptr;   // ����騩 ᥣ����
       ResBtrKey  *limit; // �����⥫� �� ��᫥���� ᥣ����
      } KeyIter;

// -----------------------------------------------------------------------------
enum
   {
    BT_BLOB_NO,
    BT_BLOB_VAR,
    BT_BLOB_STREAM,
    BT_CLOB
   };

// -----------------------------------------------------------------------------
typedef struct
      {
       char             name[25];   // ��� � ᫮���
       char             own[9];     // ��� ��������
       int              nfields;
       ResBtrField     *fields;
       int              nkeyseg;
       ResBtrKey       *keys;
       short int        pKeyIndex;  // The number of Primary Key or -1
       unsigned char    blobType;
       int              maxvarlen;  // ���ᨬ���� ࠧ��� ��६����� ���
       ResBtrKeyAttr   *kAttr;
       ResBtrFieldAttr *fAttr;
       unsigned char    isTmp;
      } TBtStructure;

// -----------------------------------------------------------------------------
#if defined(_BUILDRSLDLL) || defined(_RSLDLL)
    #ifdef RSL_PL_WIN32
        #define TUSERCALL  __stdcall
    #elif defined(RSL_PL_OS2)
        #define TUSERCALL  __syscall
    #else
        #define TUSERCALL  __pascal __far __loadds
    #endif
#elif defined(__DLMDEF_H)
    #define TUSERCALL  DLMAPIC
#else
    #define TUSERCALL
#endif

// -----------------------------------------------------------------------------
#ifdef __BTREXP_H
    typedef int (TUSERCALL *TBtMesProc)(BDHANDLE db,BtMesType code,void *);
#else
    struct tagBtDataBase;

    typedef int (TUSERCALL *TBtMesProc)(struct tagBtDataBase *db, BtMesType code, void *);
#endif

// -----------------------------------------------------------------------------
typedef void *(TUSERCALL *TBtMemAllocProc)(unsigned size);
typedef void  (TUSERCALL *TBtMemFreeProc)(void RSL_FAR *ptr);
typedef void  (TUSERCALL *TAltColProc)(AltCol RSL_FAR *alt, void RSL_FAR *data);
typedef int   (TUSERCALL *TLoopProc)(TBtLoop RSL_FAR *rec, void RSL_FAR *dt);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */