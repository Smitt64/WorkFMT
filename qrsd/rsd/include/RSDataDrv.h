//-*--------------------------------------------------------------------------*-
//
// File Name   : RSDataDrv.h 
//
// Copyright (c) 1991 - 2004 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// February 2, 2000  Sergey Kubrin, Alex Fedosov, Sergey Khorev - Create file
//-*--------------------------------------------------------------------------*-

// Интерфейс драйверов баз данных и серверов приложений
//!!! #define __RSDATADRVDLL_BUILD is very important for making .DLL library !!!



#ifndef __RSDATADRV_H
#define __RSDATADRV_H

#ifdef USE_FDECIMAL
#include "fdecimal.h"
#endif

#ifdef USE_NUMERIC
#include "rsnumeric.h"
#endif

#include <bdate.h>
#include <rstypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

   // ************************* main function *************************
   typedef struct tagIRSDB IRSDB;
#define MAIN_RSDRV_FUNCTION(premodif, postmodif) premodif IRSDB* postmodif RSDGetModuleInterface (const char *IName, const char *ClsName);

   // ************************* platform defines *************************
#if defined (_Windows) || defined (_WIN32)
#define RSDRV_PLATFORM_WIN32
#endif

#if defined (RSDRV_PLATFORM_WIN32)
#if defined (__BORLANDC__)
#define __EXPMODIF __export
#define __IMPMODIF __import
#define POST_MODIF
#else
#if defined (_MSC_VER)
#define __EXPMODIF __declspec (dllexport)
#define __IMPMODIF __declspec (dllimport)
#undef POST_MODIF
#endif
#endif

#if defined (__RSDATADRVDLL_BUILD)
#define RSDATADRV_SPEC __EXPMODIF
#else
#define RSDATADRV_SPEC __IMPMODIF
#endif

#define WARN_NEUTRALIZE

#ifdef POST_MODIF
   MAIN_RSDRV_FUNCTION (WARN_NEUTRALIZE, RSDATADRV_SPEC)
      //    IRSDB* __export RSDGetModuleInterface (const char *IName, const char *ClsName);
      // or IRSDB* __import RSDGetModuleInterface (const char *IName, const char *ClsName);
#else
   MAIN_RSDRV_FUNCTION (RSDATADRV_SPEC, WARN_NEUTRALIZE)
      //    __declspec (dllexport) IRSDB* RSDGetModuleInterface (const char *IName, const char *ClsName);
      // or __declspec (dllimport) IRSDB* RSDGetModuleInterface (const char *IName, const char *ClsName);
#endif
#endif

      // type difference purpose only
   struct  RSDHandle_ {};
   typedef RSDHandle_ *RSDHandle; // указатель на пустую структуру

#ifdef __cplusplus
#define RSD_DEF_HANDLE(name) \
   struct  name##__ : public RSDHandle_ {}; \
   typedef name##__ *name;
   typedef RSDHandle_ DRIVERHANDLE;  // common handle
#else
#define RSD_DEF_HANDLE(name) \
   struct name##__ { int stub; }; \
   typedef struct name##__ *name;
   typedef void* DRIVERHANDLE;  // common handle
#endif

   // ************************* types *************************
   // дескрипторы типа указатель на пустую структуру
   RSD_DEF_HANDLE (RSDHEN);  // environment дескриптор,->на RSDHEN##__
   RSD_DEF_HANDLE (RSDHCN);  // connection handle,     ->на RSDHCN##__
   RSD_DEF_HANDLE (RSDHCM);  // command handle,        ->на RSDHCM##__
   RSD_DEF_HANDLE (RSDHRS);  // recordset handle,      ->на RSDHRS##__
   typedef RSDHRS RSDHMR;   // multirecordset handle
   RSD_DEF_HANDLE (RSDHRW);  // row handle,            ->на RSDHRW##__

   // fundamental 
   typedef char             RSDCHAR;
   typedef unsigned char    RSDBYTE;
   typedef unsigned long    RSDDWORD;
   typedef short int        RSDSHORT;
   typedef long             RSDLONG;
   typedef unsigned long    RSDULONG;
   typedef int64_t          RSDBIGINT;  
   typedef uint64_t         RSDUBIGINT;  
   typedef float            RSDFLOAT;
   typedef double           RSDDOUBLE;
   typedef RSDCHAR *        RSDLPSTR;
   typedef void *           RSDBINARY;

   typedef wchar_t          RSDWCHAR;
   typedef RSDWCHAR *       RSDLPWSTR;

   typedef unsigned short   RSDUSHORT;
   typedef bool             RSDBIT;

   typedef btime            RSDTIME;
   typedef bdate            RSDDATE;
   typedef btimestamp       RSDTIMESTAMP;

#ifdef NUMERIC_AS_MONEY
   typedef Numeric_t        RSDDECIMAL;
#else
   typedef FDecimal_t       RSDDECIMAL;
#endif

   typedef void *           RSDBOOKMARK;

#if defined(_WIN64)
   typedef int64_t          RSDHANDLE;      // В записимости от платформы имеет размер 4 или 8 байт
   typedef int64_t *        RSDPHANDLE;
#else
   typedef long             RSDHANDLE;
   typedef long *           RSDPHANDLE;
#endif


#ifdef USE_NUMERIC
   typedef Numeric_t        RSDNUMERIC;
#endif

   typedef int              RSDKEY;         // номер ключа для MultiRecordset
#define RSDKEYNULLVALUE -2                  // пустое значение ключа 
#define RSDKEYPHYORDER  -1                  // физический порядок записей

   // driver specific
   typedef RSDLONG          RSDVALTYPE;
   typedef RSDBINARY        RSDVALUEPTR;
   typedef RSDLONG          RSDRESULT;
   typedef RSDLONG          RSDDIRECTION;
   typedef RSDLONG          INFOTYPE;
   typedef size_t           RSDACCDESC; // indirect access descriptor
   typedef size_t           RSDBIND;    // access binding descriptor

   typedef RSDCHAR *        RSDBFILE;   // имя файла, в котором "лежит" BLOB

   typedef intptr_t         RSDINTPTR;
   typedef uintptr_t        RSDUINTPTR;

   // stub! FIXME!
   typedef void (*RSDCallBackProc) (); 

   // ************************* enumerators *************************
   // RSDRESULT values
   enum RSDResult_t {
      RSDRES_NOERRORINFO     =  0x0001, 
      RSDRES_OK_WITH_INFO    =  0x0002,
      RSDRES_FAIL_WITH_INFO  =  0x0004,
      RSDRES_NOT_SUPPORTED   =  0x0008,
      RSDRES_NOT_IMPLEMENTED =  0x0010,
      RSDRES_UNKNOWN         =  0x0020,
      RSDRES_ILLEGALPARAMS   =  0x0040,
      RSDRES_HASCHILDREN     =  0x0080,
      RSDRES_NODATA          =  0x0100, 

      RSDRES_OK_PLAIN        =  RSDRES_OK_WITH_INFO   | RSDRES_NOERRORINFO,
      RSDRES_FAIL_PLAIN      =  RSDRES_FAIL_WITH_INFO | RSDRES_NOERRORINFO,
      RSDRES_OK              =  RSDRES_OK_PLAIN | RSDRES_OK_WITH_INFO,
      RSDRES_FAIL            = ~RSDRES_OK,
      RSDRES_WAITTIMEOUT     =  0x200
   };

#define  RSD_SUCCEEDED(code)       ((code) & RSDRES_OK)
#define  RSD_INFO(code)            (!((code) & RSDRES_NOERRORINFO))

#define  RSD_SUCCEEDED_TRACE(res)  (int)(RSD_SUCCEEDED((res)) ? 0 : (res))

   enum RSDEDIT_MODE {  RSDNONE, RSDEDIT, RSDINSERT };

   enum RSDEncoding_t
   {
      RSDENC_OEM=1,
      RSDENC_ANSI,
   };

   enum RSDConversion_t
   {
      RSDCONV_NONE=0,
      RSDCONV_OEMANSI,
      RSDCONV_ANSIOEM
   };

   // RSDErrorInfo values
   enum RSDErrorSeverity_t {
      RSDERS_INFO  = 0,
      RSDERS_WARN  = 1,
      RSDERS_ERROR = 2
   };

   enum RSDErrorLevel_t {
      RSDERL_DESIGN  = 0,
      RSDERL_RUNTIME = 1
   };

   enum RSDErrorSubsystem_t {
      RSDERB_DRVCOMN  = 0,
      RSDERB_DRVSPEC  = 1,
      RSDERB_DRVLOWER = 2,
      RSDERB_OTHER    = 3
   };

   // RSDPROP values
   enum RSDPropOptions_t {
      RSDPO_REQUIRED   = 0,
      RSDPO_OPTIONAL   = 1,
      RSDPO_AUTOSELECT = 2
   };

   enum RSDPropStatus_t {
      RSDPS_SUCCESS      = 0,
      RSDPS_FAILURE      = 1,
      RSDPS_NOTSUPPORTED = 2
   };
   /*   
   enum AuxilaryType
   {
   RSDT_NULL = -1
   };
   */   
   // RSDPROP, RSDBINDING & RSDCOLUMNINFO values
   // 1) Valid RSDPROP values set:
   //  scalars only

   // 2) Valid RSDBINDING values set:
   //
   //  scalar, scalar  | RSDPT_INDIRECT 
   //  RSDPT_RESULTSET | RSDPT_INDIRECT | RSDPT_ARRAY 
   //  RSDPT_RESULTSET | RSDPT_INDIRECT
   //  RSDPT_ARRAY 

   enum RSDValType_t {
      RSDPT_UNDEFINED = 0x00000001, // unknown type in RSDCOLUMNINFO 
      RSDPT_INDIRECT  = 0x00000002, // indirect access type flag
      RSDPT_ARRAY     = 0x00000004, // array type flag
      RSDPT_RESULTSET = 0x00000008, // value type = RSDBIND  (indirect only)
      RSDPT_BYTE      = 0x00000010, // value type = RSDBYTE  (scalar type)
      RSDPT_SHORT     = 0x00000020, // value type = RSDSHORT  -"-"-
      RSDPT_LONG      = 0x00000040, // value type = RSDLONG   -"-"-
      RSDPT_FLOAT     = 0x00000080, // value type = RSDFLOAT  -"-"-
      RSDPT_DOUBLE    = 0x00000100, // value type = RSDDOUBLE -"-"-
      RSDPT_LPSTR     = 0x00000200, // value type = RSDLPSTR  -"-"-
      RSDPT_BINARY    = 0x00000400, // value type = RSDBINARY -"-"-
#ifdef NUMERIC_AS_MONEY
      RSDPT_DECIMAL   = 0x02000000,  // RSDNUMERIC
# else
      RSDPT_DECIMAL   = 0x00000800,  // RSDDECIMAL
# endif
      RSDPT_BIT       = 0x00001000,  // RSDBIT
      RSDPT_VALNAME   = 0x00002000,  // значение берется из текущей записи recordset 
      RSDPT_DATE      = 0x00004000,  // RSDDATE   
      RSDPT_TIME      = 0x00008000,  // RSDTIME
      RSDPT_TIMESTAMP = 0x00010000,  // RSDTIMESTAMP

      RSDPT_COLUMN   =  0x00020000,   // столбец: в этом случае внутри команды
      // эффективно создается курсор, чтобы фетчить записи оптом, если возможно.
      RSDPT_REBIND   =  0x00040000,   // изменение target существующего binding'а
      RSDPT_HANDLE   =  0x00080000,
      RSDPT_LPWSTR   =  0x00100000,

      RSDPT_BFILE    =  0x00200000,   // Бинарный BLOB
      RSDPT_CHAR     =  0x00400000,
      RSDPT_USHORT   =  0x00800000,
      RSDPT_ULONG    =  0x01000000,
      RSDPT_NUMERIC  =  0x02000000,
      RSDPT_CFILE    =  0x04000000,   // Текстовый BLOB
      RSDPT_UNICODE  =  0x08000000,   // Индикатор ширины поля SQL параметра для RSDPT_LPSTR, RSDPT_LPWSTR, RSDPT_CHAR, RSDPT_CFILE
      RSDPT_OBJECT   =  0x20000000,
      RSDPT_BIGINT   =  0x40000000,    // RSDBIGINT
      RSDPT_UBIGINT  =  0x80000000     // RSDUBIGINT
   };

// Для обратной совместимости
#define RSDPT_BLOB RSDPT_BFILE
#define RSDPT_CLOB RSDPT_CFILE

   // RSDBINDING values
   enum RSDBindParamIO_t {
      RSDBP_IN     = 0x01, // input  parameter
      RSDBP_OUT    = 0x02, // output parameter
      RSDBP_IN_OUT = RSDBP_IN | RSDBP_OUT,
      RSDBP_RETVAL = 0x04  // return value
   };

   // RSDDIRECTION
   enum RSDFetchDirection_t {
      RSDFD_NEXT  = 0,
      RSDFD_PREV  = 1,
      RSDFD_FIRST = 2,
      RSDFD_LAST  = 3,
      RSDFD_ABS   = 4,
      RSDFD_REL   = 5
   };

   enum RSDOrientation_t {
      RSDO_RELATIVE = 0,
      RSDO_ABSOLUTE,
      RSDO_BOOKMARK
   };

   // RSDBINDING values
   enum RSDBindParamVer_t 
   {
      RSDRVER_NONE   = 0,
      RSDRVER_OLDVAL = 1,
      RSDRVER_NEWVAL = 2,
      RSDRVER_CHANGE = 3
   };

   // INFOTYPE
   enum RSDInfoType_t {
      RSDIT_PARAM             = 0,
      RSDIT_RSET              = 1,
      RSDIT_ARGS              = 2,
      RSDIT_ROWID             = 3,
      RSDIT_PK                = 4,
      RSDIT_INDEXINFO_CREATE  = 5,
      RSDIT_UNIQUE_INDEXINFO_CREATE  = 6,      
      RSDIT_INDEX_COUNT       = 7,
      RSDIT_INDEX_COLS_COUNT  = 8,
      RSDIT_INDEX_COLS        = 9,      
      RSDIT_INDEXINFO_DELETE  = 10,
      RSDIT_TABLE_COLS_COUNT  = 11,
      RSDIT_IS_CACHE_BLOB_AUTO =12
   };

   // RSDRSETBINDING struct, obStatus value
   // pGetFld, parameter 'status'

   enum RSDRSetBindStatusType {
      RSDBS_OK      = 0,   // record is loaded
      RSDBS_NODATA  = -1,  // record isn't loaded
      RSDBS_NULL    = -2,  // null value
      RSDBS_TRUNC   = -3,  // string value truncated (conversion warning)
      RSDBS_ERRCONV = -4,  // conversion error
      RSDBS_DEFVAL  = -5   // default value
   };


   typedef long RSDRSetBindStatus_t;


   enum RSDRSetFindCond{
      RSDFC_NONE  = 0x00,
      RSDFC_EQ    = 0x01,
      RSDFC_LT    = 0x02,
      RSDFC_GT    = 0x04,
      RSDFC_GE    = 0x08,
      RSDFC_LE    = 0x10,

      RSDFC_INDEX = 0x20,   // признак индексного поиска
      RSDFC_LIKE  = 0x40,
      RSDFC_NE    = 0x80
   };

   enum RSDNullKeyKind_t{
      RSDNK_NONE   = 0, 
      RSDNK_ALLSEG = 1,
      RSDNK_ANYSEG = 2
   };

   // ************************* structures *************************
#define MAX_ERRORINFO_LENGTH 1024
#define MAX_FIELDNAME_LENGTH 128

   typedef struct tagRSDErrorInfo
   {
      RSDRESULT   code;
      RSDULONG    severity; 
      RSDULONG    level;
      RSDULONG    subsystem;
      RSDCHAR     description[MAX_ERRORINFO_LENGTH];
      RSDCHAR     SQLState[6];
      RSDHandle   hSource;
      RSDLONG     nativeError;
   } RSDErrorInfo;

   typedef struct tagRSDPROP
   {
      RSDULONG     dwPropertyID; // The ID of the property
      RSDCHAR      dwOptions;    // Required or optional
      RSDCHAR      dwStatus;     // Success or failure or even not supported
      RSDVALTYPE   dwType;       // Value type
      RSDVALUEPTR  vValue;       // Value
   } RSDPROP;

   // binding struct for IRSCmd
   typedef struct tagRSDBINDING {
      RSDULONG     obOrdinal;   // the number of parameters/columns
      RSDCHAR      obParamIO;   // input/output parameter type
      RSDVALTYPE   obType;      // Value type  
      RSDVALUEPTR  obValue;     // Value
      RSDLONG     *obValLen;    // Null indicator/Length of return value
      RSDACCDESC  *obAccDesc;   // Не используется более! if obType & RSDPT_INDIRECT it's returned descriptor
      RSDLONG      obLength;    // if obType == RSDPT_LPSTR | RSDPT_BINARY | (plain length)
                                // RSDPT_ARRAY | RSDPT_RESULTSET (array length)
      RSDLPSTR     obValueName; // имя столбца в наборе данных, остальные значения берутся 
                                // из метаинформации во время связывания
      RSDLONG      obVer;       // версия значения при Update
   } RSDBINDING;

   typedef struct tagRSDCOLUMNINFO {
      RSDULONG     ciOrdinal;   // the number of parameters/columns
      RSDCHAR      ciParamIO;   // input/output parameter type (тут всегда RSDBP_IN :-)
      RSDVALTYPE   ciType;      // Value type  
      RSDVALTYPE   ciTypeForOut;// Value type for output to user, example: BINARY; but type = BLOB. pgsql
      RSDLONG      ciLength;    // if obType == RSDPT_LPSTR | RSDPT_BINARY 
      RSDRESULT    ciNullable;  // RSDRES_OK || RSDRES_FAIL || RSDRES_UNKNOWN
      RSDLONG      ciPrecision;
      RSDCHAR      ciName[MAX_ERRORINFO_LENGTH];
   } RSDCOLUMNINFO;

   // binding struct for IRSRecordSet
   typedef struct tagRSDRSETBINDING {
      RSDULONG     obOrdinal;   // the number of parameters/columns
      RSDVALTYPE   obType;      // Value type  
      RSDLONG      obLength;    // if obType == RSDPT_LPSTR 
      RSDINTPTR    obValueOff;  // Value offset
      RSDINTPTR    obStatusOff; // Status field offset
   } RSDRSETBINDING;

   typedef struct tagRSDRSNAMEBINDING {
      RSDULONG     obOrdinal;   // the number of parameters/columns
      RSDVALTYPE   obType;      // Value type  
      RSDLONG      obLength;    // if obType == RSDPT_LPSTR 
      RSDINTPTR    obValueOff;  // Value offset
      RSDLONG      obKeyKind;   // Key Kind
      //      RSDULONG*    obSearchCond;   // Search condition
      RSDLONG      obSortOrder; // Sort order
      RSDCHAR      obName[MAX_FIELDNAME_LENGTH];
   } RSDRSNAMEBINDING;

   typedef struct tagRSDADVHINTS
   {
      RSDLONG      iFirstPlain;    // количество первых совпадающих сегментов в ключе
      RSDLONG      iRangeType;     // тип range оптимизации
      void*        pAddBuffer;     // буффер для Top значения (или буффер для RANGE_BTRVSTYLE)
      char*        cHints;         // подсказки
      void*        pAddBuffer2;    // буффер для Bot значения
   }RSDADVHINTS;

   enum RSDAdvHintsIndices_t{
      RSDAHIDX_FIND = 0,
      RSDAHIDX_NEXTPREV = 1,
      RSDAHIDX_LASTFIRST = 2,
      RSDAHIDX_COUNT /* для подсчета */ 
   };

   enum RSDRangeTypes_t{
      RANGE_NONE        = 0,
      RANGE_BTRVSTYLE   = 1,    // Range оптимизайия в стиле Pervasive
      RANGE_SEGMENT_EQ  = 2, // Посегментная range оптимизация, для случая, 
      // когда одна из границ совпадает со значением поиска            
      RANGE_SEGMENT_NEQ = 3  // Посегментная range оптимизация, общий случай
   };

   // ************************* interfaces *************************
   typedef struct tagRSDErrorSink
   {
      struct tagIRSDErrorSink* vtbl;
   }  RSDErrorSink, * HRSDERRSINK;

   typedef struct tagIRSDErrorSink
   {
      void (*pAddError) (HRSDERRSINK, RSDErrorInfo* EInfo);
   }  IRSDErrorSink;

   typedef struct tagIRSEnv
   {
      RSDRESULT (*pCreateEnvironment) (RSDHEN*, RSDULONG pcProperties, RSDPROP *prgProperties, HRSDERRSINK);
      RSDRESULT (*pCloseEnvironment) (RSDHEN);

      RSDRESULT (*pSetEventCallBack) (RSDHEN, HRSDERRSINK);
   }  IRSEnv;

   typedef struct tagIRSCon
   {
      RSDRESULT (*pCreateConnection) (RSDHEN, RSDHCN*, RSDULONG pcProperties, RSDPROP *prgProperties);
      RSDRESULT (*pCloseConnection) (RSDHCN);

      RSDRESULT (*pBeginTransaction) (RSDHCN);
      RSDRESULT (*pCommitTransaction) (RSDHCN);
      RSDRESULT (*pAbortTransaction) (RSDHCN);

      RSDRESULT (*pSetProperty) (RSDHCN, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pGetProperty) (RSDHCN, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pTestTable) (RSDHCN, RSDLPSTR TableName, RSDLONG* Ex );
      RSDRESULT (*isTableColumnRawGetLen)(RSDHCN, RSDLPSTR table_name, RSDLPSTR column_name, RSDULONG *out_len);

      RSDRESULT (*pCancelCurrentQuery) (RSDHCN);
   }  IRSCon;


   typedef struct tagIRSCmd
   { 
      RSDRESULT (*pCreateCommand) (RSDHCN, RSDHCM*, RSDULONG pcProperties, RSDPROP *prgProperties);
      RSDRESULT (*pCloseCommand) (RSDHCM);

      RSDRESULT (*pCreateBinding) (RSDHCM, RSDULONG cBindings, 
         RSDBINDING rgBindings[], RSDBIND*);

      RSDRESULT (*pCancel) (RSDHCM);
      RSDRESULT (*pExecute) (RSDHCM, RSDULONG pcProperties, 
         RSDPROP *prgProperties, RSDBIND dataBindDesc);  

      RSDRESULT (*pFetch) (RSDHCM, RSDACCDESC,  RSDDIRECTION, 
         RSDLONG lRowsOffset, RSDULONG *pcRowsObtained);

      RSDRESULT (*pSetData)   (RSDHCM, RSDACCDESC); 
      RSDRESULT (*pInsertRow) (RSDHCM, RSDACCDESC); 
      RSDRESULT (*pDeleteRow) (RSDHCM, RSDACCDESC);

      RSDRESULT (*pGetInfo) (RSDHCM, INFOTYPE iType, RSDULONG *pcColumns,
         RSDCOLUMNINFO prgInfo[]);
      RSDRESULT (*pGetProperty) (RSDHCM, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue);
      RSDRESULT (*pClearBindings) (RSDHCM, RSDLONG bCancel);

      RSDRESULT (*pGetBlobData) (RSDHCM, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);
      RSDRESULT (*pSetBlobData) (RSDHCM, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);
      RSDRESULT (*pSetBlobFilename) (RSDHCM, RSDLONG nOrdinal, RSDLPSTR Filename);
      RSDRESULT (*pGetBlobFilename) (RSDHCM, RSDLONG nOrdinal, RSDLPSTR Filename);

      RSDRESULT (*pMoreResults) (RSDHCM);
      RSDRESULT (*pSetProperty) (RSDHCM, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue);
   }  IRSCmd;

   typedef struct tagIRSRecordSet
   {
      RSDRESULT (*pCreateRecordset) (RSDHRS*, RSDULONG pcProperties, RSDPROP *prgProperties, HRSDERRSINK);
      RSDRESULT (*pResetRecordset) (RSDHRS, RSDRSNAMEBINDING*, RSDLONG KeyNum);
      RSDRESULT (*pCloseRecordset) (RSDHRS);

      RSDRESULT (*pDefBinding) (RSDHRS, RSDULONG cBindings, 
         RSDRSETBINDING rgBindings[], RSDVALUEPTR, RSDACCDESC*);

      RSDRESULT (*pGetData) (RSDHRS, RSDACCDESC);
      RSDRESULT (*pGetFldCount) (RSDHRS, RSDULONG *count);
      RSDRESULT (*pGetFld) (RSDHRS, RSDULONG fldNum, RSDVALUEPTR fldPtr, 
         RSDVALTYPE fldType, RSDULONG fldLen, RSDLONG* status);
      RSDRESULT (*pGetFldInfo) (RSDHRS, RSDULONG fldNum, RSDCOLUMNINFO* fldInfo);

      RSDRESULT (*pFirst) (RSDHRS);
      RSDRESULT (*pLast) (RSDHRS);
      RSDRESULT (*pNext) (RSDHRS);
      RSDRESULT (*pPrev) (RSDHRS);
      RSDRESULT (*pEOF) (RSDHRS);
      RSDRESULT (*pBOF) (RSDHRS);

      RSDRESULT (*pGetBookmark) (RSDHRS, RSDBOOKMARK* bmark);
      RSDRESULT (*pAddRefBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pReleaseBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pMoveToBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pMove) (RSDHRS, RSDLONG recNum, RSDULONG orientation);
      RSDRESULT (*pGetRecCount) (RSDHRS, RSDULONG *count);

      RSDRESULT (*pEdit) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pCancelEdit) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pUpdate) ( RSDHRS, RSDACCDESC accd, RSDLONG pFields[] );

      RSDRESULT (*pInsert) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pDelete) ( RSDHRS );

      RSDRESULT (*pGetBlobData) (RSDHRS, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);
      RSDRESULT (*pSetBlobData) (RSDHRS, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);

      RSDRESULT (*pSetBlobFilename) (RSDHRS, RSDLONG nOrdinal, RSDLPSTR Filename);
      RSDRESULT (*pGetBlobFilename) (RSDHRS, RSDLONG nOrdinal, RSDLPSTR Filename);

      RSDRESULT (*pFind) (RSDHRS, RSDULONG cBindings, RSDRSETBINDING rgBindings[], RSDVALUEPTR, RSDLONG cond);
      RSDRESULT (*pPKFind) (RSDHRS, RSDVALUEPTR, RSDLONG bUseCached);
      RSDRESULT (*pKeyFind) (RSDHRS, RSDULONG cond, RSDVALUEPTR);
      RSDRESULT (*pLock) (RSDHRS, RSDLONG);

      RSDRESULT (*pGetProperty) (RSDHRS, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pSetProperty) (RSDHRS, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pMoreResults) (RSDHRS);
      RSDRESULT (*pDeleteBinding) (RSDHRS, RSDACCDESC);
      RSDRESULT (*pSetData) ( RSDHRS, RSDACCDESC accd, RSDLONG pFields[] );

      RSDRESULT (*pAddField) (RSDHRS, RSDRSETBINDING *rgBinding, RSDLPSTR name, RSDLONG pos);

      RSDRESULT (*pSetUserCommand) (RSDHRS, RSDLONG cmdType, RSDBINDING *rgBinding, RSDHCM);
      RSDRESULT (*pRefreshRecord) (RSDHRS);
      RSDRESULT (*pStrFind) (RSDHRS, RSDLPSTR findCond);
   }  IRSRecordSet;

   typedef struct tagIRSMultiRecordSet
   {
      RSDRESULT (*pCreateRecordset) (RSDHRS*, RSDULONG pcProperties, RSDPROP *prgProperties, HRSDERRSINK);
      RSDRESULT (*pResetRecordset) (RSDHRS, RSDRSNAMEBINDING*, RSDLONG KeyNum);
      RSDRESULT (*pCloseRecordset) (RSDHRS);

      RSDRESULT (*pDefBinding) (RSDHRS, RSDULONG cBindings, 
         RSDRSETBINDING rgBindings[], RSDVALUEPTR, RSDACCDESC*);

      RSDRESULT (*pGetData) (RSDHRS, RSDACCDESC);
      RSDRESULT (*pGetFldCount) (RSDHRS, RSDULONG *count);
      RSDRESULT (*pGetFld) (RSDHRS, RSDULONG fldNum, RSDVALUEPTR fldPtr, 
         RSDVALTYPE fldType, RSDULONG fldLen, RSDLONG* status);
      RSDRESULT (*pGetFldInfo) (RSDHRS, RSDULONG fldNum, RSDCOLUMNINFO* fldInfo);

      RSDRESULT (*pFirst) (RSDHRS);
      RSDRESULT (*pLast) (RSDHRS);
      RSDRESULT (*pNext) (RSDHRS);
      RSDRESULT (*pPrev) (RSDHRS);
      RSDRESULT (*pEOF) (RSDHRS);
      RSDRESULT (*pBOF) (RSDHRS);

      RSDRESULT (*pGetBookmark) (RSDHRS, RSDBOOKMARK* bmark);
      RSDRESULT (*pAddRefBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pReleaseBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pMoveToBookmark) (RSDHRS, RSDBOOKMARK bmark);
      RSDRESULT (*pMove) (RSDHRS, RSDLONG recNum, RSDULONG orientation);
      RSDRESULT (*pGetRecCount) (RSDHRS, RSDULONG *count);

      RSDRESULT (*pEdit) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pCancelEdit) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pUpdate) ( RSDHRS, RSDACCDESC accd, RSDLONG pFields[] );

      RSDRESULT (*pInsert) ( RSDHRS, RSDACCDESC accd );
      RSDRESULT (*pDelete) ( RSDHRS );

      RSDRESULT (*pGetBlobData) (RSDHRS, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);
      RSDRESULT (*pSetBlobData) (RSDHRS, RSDLONG nOrdinal, RSDVALUEPTR pData, RSDLONG* nCount);

      RSDRESULT (*pSetBlobFilename) (RSDHRS, RSDLONG nOrdinal, RSDLPSTR Filename);
      RSDRESULT (*pGetBlobFilename) (RSDHRS, RSDLONG nOrdinal, RSDLPSTR Filename);

      RSDRESULT (*pFind) (RSDHRS, RSDULONG cBindings, RSDRSETBINDING rgBindings[], RSDVALUEPTR, RSDLONG cond);
      RSDRESULT (*pPKFind) (RSDHRS, RSDVALUEPTR, RSDLONG bUseCached);
      RSDRESULT (*pKeyFind) (RSDHRS, RSDULONG cond, RSDVALUEPTR);
      RSDRESULT (*pLock) (RSDHRS, RSDLONG);

      RSDRESULT (*pGetProperty) (RSDHRS, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pSetProperty) (RSDHRS, RSDULONG dwPropertyID, RSDVALTYPE dwType, RSDVALUEPTR vValue );
      RSDRESULT (*pMoreResults) (RSDHRS);
      RSDRESULT (*pDeleteBinding) (RSDHRS, RSDACCDESC);
      RSDRESULT (*pSetData) ( RSDHRS, RSDACCDESC accd, RSDLONG pFields[] );

      RSDRESULT (*pCreateMultiRecordset) (RSDHMR* hmr, RSDULONG pcProperties, RSDPROP *prgProperties, HRSDERRSINK sink);
      RSDRESULT (*pSwitchRecordset) (RSDHMR hmr, RSDKEY nKey/*, RSDLONG KeyKind*/);
      RSDRESULT (*pCloseMultiRecordset) (RSDHMR hmr) ;

      RSDRESULT (*pAddField) (RSDHRS, RSDRSETBINDING *rgBinding, RSDLPSTR name, RSDLONG pos);
   }  IRSMultiRecordSet;

   typedef struct tagIRSDB
   {
      const char* IName; 
      const char* ClsName;
      int         verhi;
      int         verlo;
      void        *iptr;
   }  IRSDB;


   typedef struct tagRSDBLOB
   {
      char*          file;
      RSDVALUEPTR    data;
      size_t         size;
      short int      reserve;
   }  RSDBLOB;

   typedef struct tagRSDBLOBEX
   {
      char*          file;
      RSDVALUEPTR    data;
      size_t         size;

      short int      bufferValType; // SQL_C_WCHAR or SQL_C_CHAR
   }  RSDBLOBEX;


   enum RSDCursorType_t
   {
      RSDVAL_FORWARD_ONLY = 0,
      RSDVAL_STATIC,
      RSDVAL_KEYSET_DRIVEN,
      RSDVAL_DYNAMIC
   };


   enum RSDCursorLocation_t
   {
      RSDVAL_SERVER = 0,
      RSDVAL_CLIENT,
      RSDVAL_CLIENT_IF_NEEDED
   };

   enum RSDLock_t
   {
      RSDVAL_NOLOCK = -1,
      RSDVAL_WAIT = 0,
      RSDVAL_NOWAIT = 1,
      RSDVAL_TABLE = 0x100000
   };

   enum RSDSort_t
   {
      RSDVAL_ASC  = 1,
      RSDVAL_DESC = 2
   };

   enum RsdFilterGroupEnum 
   { 
      RSD_FILTER_NONE = 0,
      RSD_FILTER_SELECTED = 1, 
      RSD_FILTER_INSERTED = 2,
      RSD_FILTER_CHANGED = 4,
      RSD_FILTER_DELETED = 8
   };

   enum RSDSpecFieldType
   {
      SELECTED_FIELD = 1,
      LEVEL_FIELD = 2
   };


#define RSD_SELECT_NAME "%SELECTED"
#define RSD_SELECT_TITLE "SELECTED"
#define RSD_LEVEL_NAME "%LEVEL"
#define RSD_LEVEL_TITLE "LEVEL"

   // Временно перенесено из RSDataDrvO.h

   // ************************* enumerators *************************

   enum RSDPropId_ODBC_t {
      // pCreateEnvironment
      RSDPID_VERSION = 0,  // required, RSDLONG,  номер версии ODBC 
      // pCreateConnection
      RSDPID_DSN     = 1,  // optional, RSDLPSTR, имя источника данных
      RSDPID_USER    = 2,  // optional, RSDLPSTR, имя пользователя
      RSDPID_PWD     = 3,  // optional, RSDLPSTR, пароль
      RSDPID_TIMEOUT = 4,  // optional, RSDLONG,  время ожидания соединения
      ///         RSDPID_CMTMODE = 5,  // optional, RSDLONG,  режим завершения транзакций
      RSDPID_CONNSTRING = 6,   // custom conn.string

      // pCreateCommand
      RSDPID_NOSCAN  = 7,  // optional, RSDLONG, режим передачи командной строки серверу
      RSDPID_SCROLL  = 8,  // optional, RSDLONG, режим перемещения по наборам данных
      RSDPID_SENS    = 9,  // optional, RSDLONG, режим обновления набора данных
      RSDPID_MAXRSET = 10,  // optional, RSDLONG, номер последнего набора данных

      // pExecute
      RSDPID_SPNAME  = 11, // required, RSDLPSTR, командная строка
      RSDPID_PREPARE = 12,  // optional, RSDLONG, выполнять ли "подготовку" команды

      RSDPID_CURTYPE  = 13,
      RSDPID_BLOCKSIZE = 14,
      RSDPID_UPDATABLE = 15,   // возможен ли позиционный update?
      RSDPID_OWNERHANDLE = 16,
      RSDPID_CURLOC = 17,   // Cursor location

      RSDPID_CONVERSION = 18,   // преобразование набора символов, пока OEM<->ANSI
      RSDPID_MAXROWS = 19, // максимальное количество строк, вычитываемых из курсора
      RSDPID_AFFRECS_PTR = 20,
      RSDPID_DONTEXEC = 21,
      RSDPID_LOCK = 22,
      RSDPID_DBMS_NAME = 23,
      RSDPID_OWNERITF = 24,
      RSDPID_REREADPARAMS = 25,
      RSDPID_ENLIST_IN_DTC = 26,
      RSDPID_ODBC_HANDLE = 27,
      RSDPID_ODBC_SERVICE_HANDLE = 28,
      RSDPID_NULL_CONVERSION = 29,
      RSDPID_PARAM_ARRAY_SIZE = 30,
      RSDPID_PARAM_ARRAY_COLUMN_WISE = 31,
      RSDPID_SPNAMELEN  = 32,
      RSDPID_CODEPAGE = 33,
      RSDPID_AINCFIELD = 34,
      RSDPID_SEQNAME = 35,
      RSDPID_ODBC_HANDLE_ENV = 36,
      RSDPID_REPAIR_UPDATED_CACHE_FIELDS_NULL_DATA = 37,
      RSDPID_EXECUTE_DIRECT = 38,
 //     RSDPID_NATIVE_SQL = 39,
      RSDPID_DISABLE_ORA_TO_PG_CONVERTER = 40
   };

   enum RSDPropValue_ODBC_t {
      // pCreateEnvironment, RSDPID_VERSION
      RSDVAL_VERSION_2    = 0,
      RSDVAL_VERSION_3    = 1,
      // pCreateConnection,  RSDPID_CMTMODE
      //         RSDVAL_CMT_ON       = 2, 
      //         RSDVAL_CMT_OFF      = 3,
      // pCreateCommand, RSDPID_NOSCAN
      RSDVAL_NOSCAN_ON    = 4,
      RSDVAL_NOSCAN_OFF   = 5,
      //                 RSDPID_SCROLL
      RSDVAL_SCROLL_ON    = 6,
      RSDVAL_SCROLL_OFF   = 7,
      //                 RSDPID_SENS
      RSDVAL_SENS_ON      = 8,
      RSDVAL_SENS_OFF     = 9,
      RSDVAL_SENS_UNDEF   = 10,
      // pExecute,       RSDPID_PREPARE
      RSDVAL_PREPARE_ON   = 11,
      RSDVAL_PREPARE_OFF  = 12,
      RSDVAL_PREPARE_ONLY = 13
   };

#ifdef __cplusplus
} //extern "C"

#endif

#include <limits>

#ifdef max
  #pragma push_macro("max")
  #undef max
  const size_t const_RSDRecordSet_binder = std::numeric_limits<size_t>::max();
  const size_t const_CRsdCommand_binder = std::numeric_limits<size_t>::max() - 1;
#pragma pop_macro("max")
#else
  const size_t const_RSDRecordSet_binder = std::numeric_limits<size_t>::max();
  const size_t const_CRsdCommand_binder = std::numeric_limits<size_t>::max() - 1;
#endif
  enum EBinderUID {
    e_RSDRecordSet_binder = 0xFFFFFFF,
    e_CRsdCommand_binder = 0xFFFFFFF-1
  };

#endif // __RSDATADRV_H

