/*@H*********************************************************************
*        File Name:   itypes.h           January 5,1994
*       Programmer:   S. Kubrin
*      Description:
*  Public routines:
* Private routines:
*
* VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsl/itypes.h $
*                  $Revision: 57 $
*                  $Date: 18.07.00 13:44 $
*                  $Author: Lebedev $
*                  $Modtime: 18.07.00 12:13 $
*
**********************************************************************@H*/
#ifndef  __ITYPES_H
#define  __ITYPES_H

#ifdef NO_BTR
    #define  NO_DBASE
    #define  RSL_NO_DIALOGS
    #define  NO_BTRMAC
    #define  NO_SPECST
    #define  RSL_MIN_EXP
#endif

#ifndef NO_DBASE
    #ifndef DBASE
        #define DBASE
    #endif
#endif

#include <bdate.h>
#include <rstypes.h>

// -----------------------------------------------
#ifndef NO_BTR
    struct tagBFILE;
#endif

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif

//------------ Тип значения переменной --------------------------------

#ifndef RSL_USE_DLL
    typedef struct tagTRD  *HRD;
    typedef struct tagTRD   TRD;
#else
    RSL_MAKE_HANDLE(HRD);
#endif

#ifdef ENUMINTSIZE
    #pragma option -b-
#endif

// -----------------------------------------------
// Массив sym_type_array индексируется значениями VALTYPE !
typedef enum VALTYPE_V
      {
       V_UNDEF,              //  0
       V_INTEGER,            //  1
       V_MONEY_FDEC,         //  2
       V_DOUBLE,             //  3

       V_MONEY_DBL10,        //  4
       V_DOUBLE10,           //  5

       V_STRING,             //  6
       V_BOOL,               //  7
       V_UNUSED,             //  8
       V_DATE,               //  9
       V_TIME,               // 10

       #ifndef NO_BTR
          V_FREF,            // 11
          V_BINST,           // 12
          V_SREF,            // 13
          V_SINST,           // 14
       #else
          V_UNUSED1, V_UNUSED2, V_UNUSED3, V_UNUSED4,
       #endif

       V_AREF,               // 15
       V_AINST,              // 16
       V_TREF,               // 17
       V_TINST,              // 18
       V_GENOBJ,             // 19
       V_PROC,               // 20

       #ifdef DBASE
           V_DREF,           // 21
           V_DINST,          // 22
       #else
           V_UNUSED5, V_UNUSED6,
       #endif

       V_DTTM,               // 23
       V_MEMADDR,            // 24
       V_R2M,                // 25
       V_SPECVAL,            // 26
       V_DECIMAL,            // 27

       V_NUMERIC,            // 28
       V_BIGINT,             // 29

       /* Для завершения списка параметров RunProc */
       V_ENDLIST = 100
      } VALTYPE_V;

// -----------------------------------------------
typedef unsigned char  VALTYPE;

// [!!!:x64] Закомментировано в рамках портации на х64
#if/* defined(RSL_PL_WIN64) ||*/ defined(USE_NEWVALUE)
    //  No long double. Value size is 16 bytes.
    #define VALUE_NUMERIC
#endif

// This is a special constant. Its map to V_MONEY or V_NUMERIC, based on MoneyMode
#define  V_RSLMONEY   99

// -----------------------------------------------
// Define V_MONEYL
#ifdef USE_FDECIMAL
    #ifdef NUMERIC_AS_MONEY
        #define  V_MONEY   V_NUMERIC
    #else
        #define  V_MONEY   V_MONEY_FDEC
    #endif

    #define      V_MONEYL  V_MONEY
#else
    #define      V_MONEY   V_MONEY_FDEC

    #define      V_MONEYL  V_MONEY 
#endif

// -----------------------------------------------
// Define V_DOUBLEL
#ifdef _MSC_VER
    #define  V_DOUBLEL  V_DOUBLE
#else
    #define  V_DOUBLEL  V_DOUBLE10
#endif 

#ifdef ENUMINTSIZE
    #pragma option -b
#endif

//------------ Ячейка со значением переменной -------------------------
struct tagISYMBOL;

typedef struct tagVALUE  VALUE;

// -----------------------------------------------
typedef struct
      {
       void  *sym;  // pointer to SYMPROC
      } PROCREF;

// -----------------------------------------------
typedef struct
      {
       unsigned long  refCount;
      } RSLStringRef;

// -----------------------------------------------------------------------------
#ifndef NO_BTR
    typedef struct
          {
           struct tagBFILE   *bfile;
          } BINST;

    // -----------------------------------------------
    typedef struct
          {
           void    *file;  // SYMFILE
           BINST   *inst;
          } FREF;

    // -----------------------------------------------
    typedef struct
          {
           char   *Buff;    // Буфер данных
           char    alloced; // == 1 если Buff распределен динамически
          } SINST;

    // -----------------------------------------------
    typedef struct
          {
           void    *sym;  // SYMSTRUC
           SINST   *sinst;
          } SREF;
#endif

// -----------------------------------------------
typedef struct
      {
       VALUE  *array;  // Элементы массива
       int     size;   // Количество элементов массива
      } AINST;

// -----------------------------------------------
typedef struct
      {
       void    *sym;  // SYMARRAY
       AINST   *inst;
      } AREF;

// -----------------------------------------------
typedef struct
      {
       FILE   *fp;    // Элементы массива
       char   *Buff;  // Буфер под строку
      } TINST;

// -----------------------------------------------
typedef struct
      {
       void   *sym;  // SYMTXTFILE
       TINST  *inst;
      } TREF;

// -----------------------------------------------
typedef struct
      {
       void  *file;  // dBase file object
      } DINST;

// -----------------------------------------------
typedef struct
      {
       void   *sym;  // SYMTXTFILE
       DINST  *inst;
      } DREF;

// -----------------------------------------------
typedef struct   // 2 in 1 :-))
      {
       bdate  date;
       btime  time;
      } RslDtTm;

// -----------------------------------------------
typedef struct
      {
       int   tp;
       long  id;
      } TExtMap;

// -----------------------------------------------
typedef struct
      {
       void  *obj;
       long   id;
      } RslR2M;

// -----------------------------------------------------------------------------
typedef union
      {
       long          intval;
       double        doubval;

#ifdef  VALUE_NUMERIC
       double        doubvalL;
#else
       long double   doubvalL;
#endif
          
#ifndef NUMERIC_AS_MONEY
    #if defined (USE_FDECIMAL) && !defined(__cplusplus)
       FDecimal_t    monval;
       FDecimal_t    monvalL; 
    #else
       basemoney     monval;
       lbasemoney    monvalL;
    #endif
#endif // NUMERIC_AS_MONEY

#if defined (USE_FDECIMAL) 
       FDecimal_t    decimal;
#endif

       char         *string;
       bool          boolval;

       bdate         date;
       btime         time;

#ifndef NO_BTR
       BINST         iref;      // V_BINST
       FREF          fref;      // V_FREF

       SINST         sinst;     // V_SINST
       SREF          sref;      // V_SREF
#endif

       AINST         ainst;     // V_SINST
       AREF          aref;      // V_SREF

       TINST         tinst;     // V_TINST
       TREF          tref;      // V_TREF

       DINST         dinst;     // V_DINST
       DREF          dref;      // V_DREF


       void         *obj;       // V_GENOBJ

       PROCREF       proc;

       void         *addr;      // V_MEMADDR
       RslDtTm       dttm;      // V_DTTM

       RslR2M        r2m;       // V_R2M


       TExtMap       extMap;

#if defined(USE_NUMERIC) && !defined (USE_NEWVALUE)
       Numeric_t    *numVal;
#endif

#ifndef  VALUE_NUMERIC
       char          doub10[10];
#endif
       int64_t       bigint;    // V_BIGINT
      } VALDATA;

// -----------------------------------------------
struct tagVALUE
     {
      VALTYPE        v_type;  // Выбирает TYPESTRUCT из массива sym_type_array

#ifdef  VALUE_NUMERIC
      unsigned char  r1;
      short int      r2;
      long           r3;
#endif

      VALDATA        value;
     };

//------------- Функции операции --------------------------------------

// mixMode: 0 - OK, 1 - for decimal only, 2 - for Numeric only,
// -----------------------------------------------
typedef struct
      {
       unsigned char  mixMode;
       VALTYPE        left;
       VALTYPE        right;
       void         (*operfunc)(VALUE *left, VALUE *right, HRD inst);
      } BINARYOPER;

// -----------------------------------------------
typedef struct
      {
       unsigned char  mixMode;
       VALTYPE        v_type;
       void         (*operfunc)(VALUE *val, HRD inst);
      } UNARYOPER;

// -----------------------------------------------
typedef void (*ExitProcType)(void);


// -----------------------------------------------------------------------------
extern int  RslVersion;

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif

/* <-- EOF --> */