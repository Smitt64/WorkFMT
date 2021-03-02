/*@H*********************************************************************
*        File Name:   isymbol.h          December 10,1993
*       Written by:  S. Kubrin   VECsoft TM.
*      Description:
*  Public routines:
* Private routines:
**********************************************************************@H*/
#ifndef  __ISYMBOL_H
#define  __ISYMBOL_H

#ifndef __IMESSAGE_H
#include "rsl/msg.h"
#endif

#include <time.h>

#include <packpshn.h>

////////////////////////////////////////////////////////////////////////////
// for PCode
////////////////////////////////////////////////////////////////////////////

#ifndef RSL_NO_PCODE

RSL_MAKE_HANDLE(HPCODE);

////////////////////////////////////////////////////////////////////////////
// Макросы для управления работой PCode
////////////////////////////////////////////////////////////////////////////
#define  RSLMODE_DISABLE_PCODE      1000  // запрещается генерировать и загружать PCode.
#define  RSLMODE_ONLYLOAD_PCODE     1001  // запрещается генерировать PCode, только загружать
#define  RSLMODE_GENLOAD_PCODE      1002  // разрешается генерировать и загружать PCode.
#define  RSLMODE_AUTOGENLOAD_PCODE  1003  // разрешается загружать PCode и автоматически генерировать PCode.
#define  RSLMODE_NOMAC_PCODE        1004  // разрешается загружать PCode. использование mac файлов блокируется.
////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
#ifndef RSL_PCODE2

// -----------------------------------------------------------------------------
typedef uint32  PCODEREF;

// -----------------------------------------------------------------------------
typedef struct tagPCODEENTRYPOINT
      {
       PCODEREF  mod;
       PCODEREF  point;
      } PCODEENTRYPOINT;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_ISYMBOL
      {
       PCODEREF  name; // table of names
       int8      s_type; // SYMTYPE
       int32     v_attr;
       int32     level;
       PCODEREF  module; // table of names
      } PCODE_ISYMBOL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMVAR
      {
       PCODE_ISYMBOL   derived;
       int8            v_type; // VALTYPE
       PCODEREF        sym; // SYMCLASS*
      } PCODE_SYMVAR;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_VALUE
      {
       int8    v_type;
       int8    value[sizeof(VALDATA)];
      } PCODE_VALUE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMGLOBAL
      {
       PCODE_SYMVAR    derived;
       PCODE_VALUE     value;
      } PCODE_SYMGLOBAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMLOCAL
      {
       PCODE_SYMVAR    derived;
       int32           offset;
      } PCODE_SYMLOCAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMPROP
      {
       PCODE_SYMLOCAL  derived;
      } PCODE_SYMPROP;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_PROCDEF
      {
       PCODEREF          local;         // list ISYMBOL*
       PCODEREF          parm;          // list ISYMBOL*
       int8              is_standart;
       int8              v_type;        // VALTYPE
       PCODEREF          sym;           // SYMCLASS*
       PCODEENTRYPOINT   entry;         // PCODE_STATEMENT
      } PCODE_PROCDEF;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMPROC
      {
       PCODE_ISYMBOL     derived;
       PCODE_PROCDEF     proc;
       PCODEREF          sym; // SYMPROC*

       //PCODE_METHODCHUNK     m; // TMethodChunk
       PCODEREF          errSym; // SYMLOCAL*
       PCODEENTRYPOINT   entry; // list STATEMENT

       PCODEENTRYPOINT   stEnd; // STATEMENT*
      } PCODE_SYMPROC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMCLASS
      {
       PCODE_SYMPROC     derived;
       PCODEREF          parent; // list of parents
      } PCODE_SYMCLASS;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMEXTCLASS
      {
       PCODE_SYMPROC     derived;
       int32             info; // IRslTypeInfo or TGenClass if isClsInfo == true
       int32             provider; // TProviderInfo
       int8              canCreate;
       int8              isClsInfo;
      } PCODE_SYMEXTCLASS;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMMETHOD
      {
       PCODE_SYMPROC     derived;
       int8              override;
       int32             offset; // offset to TMethodChunk
      } PCODE_SYMMETHOD;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMINITCLASS
      {
       PCODE_ISYMBOL     derived;
       PCODEREF          classPtr; // SYMCLASS pointer
      } PCODE_SYMINITCLASS;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMMODULE
      {
       PCODE_SYMPROC     sym;
       PCODEREF          constant;         // list SYMGLOBAL
       PCODEREF          global;           // list ISYMBOL
       PCODEREF          provider;         // list = 0;

       PCODEREF          codeList;         // list = 0;

       int32             isBtrStream;      // bool
       int32             initCalled;       // bool

       PCODEREF          uniCls;           // list = 0;
       int32             uniClsRegistered; // bool, = false;

       PCODEREF          file;             // table of names
      } PCODE_SYMMODULE;

// -----------------------------------------------------------------------------
#ifndef NO_BTR
    // -----------------------------------------------------------------------------
    typedef struct tagPCODE_SYMSTRUC
          {
           PCODE_ISYMBOL   derived;
           PCODEREF        dicname;
           int32           ninst;
           int32           offset;
           int32           isDialog;
           PCODEREF        altDic;
          } PCODE_SYMSTRUC;

    // -----------------------------------------------------------------------------
    typedef struct tagPCODE_SYMFILE
          {
           PCODE_SYMSTRUC  derived;
           int32           mode;
           int32           path;
           uint8           useBlob;
          } PCODE_SYMFILE;
#endif // NO_BTR

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMTXTFILE
      {
       PCODE_ISYMBOL     derived;
       int32             offset;
       int8              mode;
       int32             recSize;
       PCODEREF          file;
      } PCODE_SYMTXTFILE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMDBFFILE
      {
       PCODE_ISYMBOL     derived;
       int32             offset;
       int8              write;
       PCODEREF          altDic;
       PCODEREF          file;
      } PCODE_SYMDBFFILE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMARRAY
      {
       PCODE_ISYMBOL     derived;
       int32             offset;
      } PCODE_SYMARRAY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_HEADER
      {
       PCODEREF          next;
       int8              type;
      } PCODE_HEADER;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPELEM
      {
       PCODE_HEADER      itemHeader;
       PCODEREF          prevItem;
       PCODEREF          exp; // EXPRESSION*
       int32             w;
       int32             p;
       int32             flag;
       PCODEREF          data; // void*
      } PCODE_EXPELEM;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STATEMENT
      {
       PCODE_HEADER itemHeader;
       int32             fStmt;
       int8              st_type;
       int32             line;
       PCODEREF          sym; // SYMPROC*

       int32             lastStmtOffs;
       int32             offsBegin;
       int32             offsEnd;
       int8              breakPoint;
   
       union
           {
            int32        nBreakPointData;
            void        *pBreakPointData;
           } u;
      } PCODE_STATEMENT;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STEXPRESSION
      {
       PCODE_STATEMENT   derived;
       PCODEREF          exp; // EXPRESSION*
      } PCODE_STEXPRESSION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STLOOP
      {
       PCODE_STATEMENT   derived;
       PCODEREF          st; // CONDITION*
      } PCODE_STLOOP;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STCONDITION
      {
       PCODE_STATEMENT   derived;
       PCODEREF          iflist; // list CONDITION
      } PCODE_STCONDITION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STRETURN
      {
       PCODE_STATEMENT   derived;
       PCODEREF          ret; // EXPRESSION*
      } PCODE_STRETURN;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STOUT
      {
       PCODE_STATEMENT   derived;
       PCODEREF          linelist; // list LINE_ELEM
       PCODEREF          parmlist; // list EXPLISTELEM
       int32             buff; // char*
      } PCODE_STOUT;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STSPEC
      {
       PCODE_STATEMENT   derived;
       PCODEREF          explist; // list EXPLISTSPEC
      } PCODE_STSPEC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STENDPROC
      {
       PCODE_STATEMENT   derived;
       int8              called;
      } PCODE_STENDPROC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_CONDITION
      {
       PCODE_STATEMENT   derived;
       PCODEREF          cond;          // EXPRESSION*
       PCODEREF          stmtlist;      // list STATEMENT
      } PCODE_CONDITION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPRESSION
      {
       PCODE_HEADER      itemHeader;
       int8              operation;
       int8              exp_type;
       int32             level;
      } PCODE_EXPRESSION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPBINARY
      {
       PCODE_EXPRESSION  derived;
       PCODEREF          left;          // EXPRESSION*
       PCODEREF          right;         // EXPRESSION*
      } PCODE_EXPBINARY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPUNARY
      {
       PCODE_EXPRESSION  derived;
       PCODEREF          oper;          // EXPRESSION*
      } PCODE_EXPUNARY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_ICLUST
      {
       PCODE_HEADER      itemHeader;
       int8              cl_type;
       int32             Id;
       int16             isProp;
       uint32            key;
      } PCODE_ICLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_NAMECLUST
      {
       PCODE_ICLUST      derived;
       PCODEREF          name;          // char*
      } PCODE_NAMECLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_NUMCLUST
      {
       PCODE_ICLUST      derived;
       PCODE_EXPELEM     elem;
       int16             num;
      } PCODE_NUMCLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_LINEELEM
      {
       PCODE_HEADER      itemHeader;
       PCODEREF          line;          // char*
      } PCODE_LINEELEM;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPVAL
      {
       PCODE_EXPRESSION  derived;
       PCODEREF          sym;           // ISYMBOL*
       PCODEREF          fld_list;      // ICLUST*
      } PCODE_EXPVAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPSPEC
      {
       PCODE_HEADER      itemHeader;
       PCODEREF          exp;           // EXPRESSION*
       PCODEREF          sym;           // SYMGLOBAL*
      } PCODE_EXPSPEC;

#endif // RSL_PCODE2
#endif // RSL_NO_PCODE

////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NO_BTR
struct  tagBTRSTRUCTURE;
#endif

typedef void (*RslStdProc)    (void);
typedef bool (*TRslActionProc)(void *UserData);

//backward compatibility
typedef TRslMesProc     JRslMsgProc;
typedef TRslActionProc  TRSLActionProc;

extern void            *UserData;


#ifdef ENUMINTSIZE
#pragma option -b-
#endif

// The types of RSL modules
#define  RSL_MOD_MAC   0
#define  RSL_MOD_RSM   1
#define  RSL_MOD_DLM   2
#define  RSL_MOD_STD   3

//------------------------ PROCDEF part ----------------------------------

// -----------------------------------------------------------------------------
typedef struct
      {
       CNTLIST               local;             // список ISYMBOL
       CNTLIST               parm;              // список ISYMBOL
       unsigned  char        is_standart;
       unsigned char         v_type;            // We need 1 byte size
       struct tagSYMCLASS   *sym;

       union
           {
            CNTLIST          stmtlist;          // список STATEMENT
            RslStdProc       proc;              // указатель на стандартную процедуру
            // add by A.Tokmashev (07.06.2000)
#ifndef RSL_PCODE2
    #ifndef RSL_NO_PCODE
            PCODEENTRYPOINT  entry;
    #endif // RSL_NO_PCODE
#endif // RSL_PCODE2
           } u;
      } PROCDEF;

// -----------------------------------------------------------------------------
#define  INTERPRET_PROC   0
#define  STANDART_PROC    1

// add by A.Tokmashev (07.06.2000)
#ifndef  RSL_NO_PCODE

#define  PCODE_PROC       2

#endif // RSL_NO_PCODE

//------------------ END of PROCDEF part ----------------------------------


//------------------- ISYMBOL part--------------------------------------
/*  Все поименованные объекты являются производными от ISYMBOL  */
typedef enum  // При изменениях , изменять массив размеров datasize
      {
       S_UNDEF,
       S_VGLOBAL,
       S_VLOCAL,
       S_MODULE,
       S_PROC,
#ifndef NO_BTR
       S_FILE,
       S_STRUC,
#endif
       S_ARRAY,
       S_TXTFILE,
       S_TMP,
       S_CLASS,
       S_EXTCLASS,
       S_METHOD,
       S_PROP,
       S_INITCLASS,
       S_DBFFILE
      } SYMTYPE;


//------------ Атрибуты элемента в таблице символов -------------------
// Определяют биты в v_attr структуры ISYMBOL
#define  vaCONST          1
#define  vaDECLARED       2
#define  vaCONVERTPARM    4 // Procedure have declared parameters
#define  vaINIT           8
#define  vaONERROR       16 // OnError Present
#define  vaPERSIST       32 // Don't call ValueClear on Close Run Time state
#define  vaSOURCE        64
#define  vaONERRORSYM   128
#define  vaPRIVATE      256
#define  vaHIDDEN       512
#define  vaINITOFFS    1024 // for offsets class (27.01.2001)
#define  vaSYSCONST    2048
#define  vaBYREF       4096

// -----------------------------------------------------------------------------
typedef struct tagISYMBOL
      {
       char                 *name;        // Имя символа
       unsigned char         s_type;      // We need 1 byte size
       int                   v_attr;      // атрибуты данного символа (поле битов)
       int                   level;       // уровень вложенности
       LOBJ                  symlist_elem;

       // add by A.Tokmashev (T74) - 22.05.2000
#ifndef RSL_NO_HASH
       SOBJ                  m_linkHash;
#endif // RSL_NO_HASH
       // add by A.Tokmashev (T74) - 23.05.2000
       struct tagSYMMODULE  *module;      // ссылка на модуль которому принадлежит этот символ
       // end (T74) - 23.05.2000
       // end (T74) - 22.05.2000
       char                 *name_origin; //исходное название символа (регистрозависимое)
      } ISYMBOL;

// -----------------------------------------------------------------------------
#define  P_SYM(ptr) ((ISYMBOL *)(ptr))

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL              derived;
       VALTYPE              v_type;  // Декларированный тип
       struct tagSYMCLASS  *sym;     // if v_type == V_GENOBJ
      } SYMVAR;


// -----------------------------------------------------------------------------
typedef void (DLMAPI *TVarInitProc)(ISYMBOL *symVar,ISYMBOL *symClass);

// -----------------------------------------------------------------------------
typedef struct tagSYMGLOBAL
      {
       SYMVAR        derived;
       TVarInitProc  initProc;  // DLM SDK can use this pointer to init global values.
       VALUE         value;
      } SYMGLOBAL;

// -----------------------------------------------------------------------------
typedef struct
      {
       SYMVAR  derived;
       int     offset;  // Смещение в стеке локальной переменной
      } SYMLOCAL;

// -----------------------------------------------------------------------------
typedef struct
      {
       SYMLOCAL derived;
      } SYMPROP;

// -----------------------------------------------------------------------------
typedef struct
      {
       struct tagTPROCREF  *replace;      // Pointer to current TPROCREF
       CNTLIST              replaceList;  // List of TPROCREF
      } TMethodChunk;

// -----------------------------------------------------------------------------
typedef struct
      {
       struct tagTPROCREF  *replace;      // Pointer to current TPROCREF
       CNTLIST              replaceList;  // List of TPROCREF
       struct tagSYMP      *original;
      } TMethodChunkEx;

// -----------------------------------------------------------------------------
typedef struct tagSYMP
      {
       ISYMBOL                     derived;
       PROCDEF                     proc;

       struct tagSYMP             *sym; // Процедура в которую включена данная процедура.
                                        // Для процедуры модуля NULL
       TMethodChunk                m;

       SYMLOCAL                   *errSym; // Error Object (can be NULL)

    // add by A.Tokmashev (05.07.2000)
    #if (!defined(RSL_NO_PCODE) && !defined(RSL_PCODE2))
       union
           {
            CNTLIST                stmtlist; // список STATEMENT
            PCODEENTRYPOINT        entry; // P-Code STATEMENT
           } u;
    #else
       union
           {
            CNTLIST                stmtlist;
           } u;
    #endif // RSL_NO_PCODE || RSL_PCODE2

#ifdef USEDEBUGER
    #if (!defined(RSL_NO_PCODE) && !defined(RSL_PCODE2))
       union
           {
            struct  tagSTATEMENT  *stEnd;
            PCODEENTRYPOINT        stEndEntry;
           } uEnd;
    #else
       union
           {
            struct tagSTATEMENT   *stEnd;
           } uEnd;
    #endif // RSL_NO_PCODE || RSL_PCODE2
#endif

    #ifdef USE_DBGHANDLE32
       RslHandle32                 key;
       LOBJ                        procList_elem;
    #endif
      } SYMPROC;

// -----------------------------------------------------------------------------
typedef struct tagSYMCLASS
      {
       SYMPROC              derived;
       struct tagSYMCLASS  *parent; // Next will be List of parents
      } SYMCLASS;

// -----------------------------------------------------------------------------
typedef struct
      {
       SYMPROC              derived;
       void                *info; // IRslTypeInfo or TGenClass if isClsInfo == true
       void                *provider; // TProviderInfo
       bool                 canCreate;

       bool                 isClsInfo;
      } SYMEXTCLASS;

// -----------------------------------------------------------------------------
typedef struct
      {
       SYMPROC              derived;
       bool                 override;
       int                  offset; // Offset to TMethodChunk
      } SYMMETHOD;

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL              derived;
       SYMCLASS            *classPtr;
      } SYMINITCLASS;

// -----------------------------------------------------------------------------
typedef struct tagTPROCREF
      {
       LOBJ                 listElem;
       SYMPROC             *sym;
      } TPROCREF;


// -----------------------------------------------------------------------------
void FreeProcRef(TPROCREF *ref);

// -----------------------------------------------------------------------------
typedef struct tagCCallIntf
      {
       // Common part of all RSL interfaces
       int   (DLMAPI *GetVersion)(void);
       char *(DLMAPI *GetName)(void);
      } TCCallIntf;

// -----------------------------------------------------------------------------
typedef struct tagSYMMODULE
      {
       SYMPROC               sym;
       CNTLIST               constant;    // списоки ISYMBOL
       CNTLIST               global;
       CNTLIST               provider;    // Object provider list
       DLMHAND               DllId;

       void                (*destruct)(void);    // ???

       struct tagSYMMODULE  *prev;  // Save Prev module for speed extraction
       bool                  isBlockedMem;
       bool                  isSourceFile;
       int                   sizeAllItems;

#ifdef USEDEBUGER
       CNTLIST               codeList;
#endif

       bool                  isBtrStream;
       bool                  initCalled;

       CNTLIST               uniCls;  // список TUniClass.
       bool                  uniClsRegistered;  // in UniProvider

       CNTLIST               importList;    // List of TImportRef
       time_t                modTime;    // time of last modification of file
       short int             modType;    // type of module (mac, rsm, dlm )

       TCCallIntf           *cCallIntf;
       uintptr_t             cookie;

#ifdef USE_DBGHANDLE32
       RslHandle32           key;
       CNTLIST               procList;
#endif

       char                 file[1];    // имя файла
      } SYMMODULE;

// -----------------------------------------------------------------------------
typedef struct tagTImportRef
      {
       LOBJ        elem;
       SYMMODULE  *mod;
       int         line;
       int         offs;
      } TImportRef;


// -----------------------------------------------------------------------------
typedef void  (*ClearModCtxProc)  (struct tagAddModContextBase *);
typedef void  (*SuspendModCtxProc)(struct tagAddModContextBase *);
typedef void  (*ResumeModCtxProc) (struct tagAddModContextBase *);

// -----------------------------------------------------------------------------
typedef struct tagAddModContextBase
      {
       LOBJ                elem;
       SYMMODULE          *sym;
       ClearModCtxProc     clrProc;
       SuspendModCtxProc   suspendProc;
       ResumeModCtxProc    resumeProc;

       CNTLIST             scopeList;

       int                 line; // for import
       int                 offs;
       bool                privateMode;
      } TAddModContextBase;


// -----------------------------------------------------------------------------
#ifndef NO_BTR
// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL                  derived;
       char                     dicname[25];    // Имя в словаре
       struct tagBTRSTRUCTURE  *str;            // Структура файла Инициализируется при необходимости
       int                      ninst;          // Количество копий
       int                      offset;         // Смещение в стеке копии локального объекта
       int                      isDialog;       // 1 - если это структура диалога
       char                    *altDic;         // Alternate dictionary name

       SINST                    inst;           // Указатель на буфер данных глобального объекта
       void                    *data;
      } SYMSTRUC;

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL                  derived;
       char                     dicname[25];    // Имя в словаре
       struct tagBTRSTRUCTURE  *str;            // Структура файла Инициализируется при необходимости
       int                      ninst;          // Количество копий
       int                      offset;         // Смещение в стеке копии локального объекта
       int                      isDialog;       // 1 - если это структура диалога
       char                    *altDic;         // Alternate dictionary name

       BINST                    inst;           // Данные глобального SYMFILE
       int                      mode;           // Режим открытия
       int                      path;           // Путь в индексе

       unsigned char            useBlob;

       // ... Список прилинкованных файлов
       // ... Установленный фильтр
      } SYMFILE;
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL      derived;
       int          offset;   // Смещение в стеке копии локального объекта
       TINST        inst;     // Указатель на буфер данных глобального объекта
       char         mode;     // 0 - read, 1 - write, 2 - append
       int          recSize;  // Размер записи (длина строки)
       char         file[1];  // Название текстового файла
      } SYMTXTFILE;

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL      derived;
       int          offset;   // Смещение в стеке копии локального объекта
       DINST        inst;     // Указатель на буфер данных глобального объекта
       char         write;    // Файл открывается для записи
       char        *altDic;
       char         file[1];  // Название dbf файла
      } SYMDBFFILE;

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL        derived;
       char           file[80];  // Название текстового файла или имя в словаре
       signed char    mode;
       int            path;
       char           what;
       int            isDialog;  // 1 - если указан спецификатор dialog
       int            recSize;
       unsigned char  useBlob;
       char           append;
       char           altDic[80];
      } SYMTMP;

// -----------------------------------------------------------------------------
typedef struct
      {
       ISYMBOL        derived;
       int            offset;  // Смещение в стеке копии локального объекта
       AINST          inst;    // Указатель на буфер данных глобального объекта
      } SYMARRAY;


// -----------------------------------------------------------------------------
#ifndef NO_BTR
void  ClearBinstance(BINST *iref);
#endif

SYMPROC *GetCurProc(void);

//------------------- END of ISYMBOL part------------------------------


//------------------  EXPRESSION  part ---------------------------------

// -----------------------------------------------------------------------------
typedef enum
      {
       ET_BINARY,
       ET_UNARY,
       ET_VAL,
       ET_FUNC
      } EXP_TYPES;

// -----------------------------------------------------------------------------
typedef struct
      {
       TOKKEN     operation;
       EXP_TYPES  exp_type;
       int        level;  // Глубина вложенности данного подвыражения
      } EXPRESSION;

// -----------------------------------------------------------------------------
#define  P_EXP(ptr)  ((EXPRESSION *)(ptr))

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ           elem;
       EXPRESSION    *exp;
       SYMPROC       *expContext;
       bool           onlyThisCtx;
       VALUE          result;
#ifdef USE_DBGHANDLE32
       RslHandle32    key;
#endif
       char          *newVal;
      } TExpElem;

// -----------------------------------------------------------------------------
typedef struct tagEXPLISTELEM
      {
       LOBJ           exp_elem;
       EXPRESSION    *exp;
       int            w, p, flag;      // Спецификаторы форматирования
       void          *data      ;      // Дополнительная информация для форматирования
      } EXPLISTELEM;

// -----------------------------------------------------------------------------
typedef struct  // Для спец. модуля см. iprspec.c и  STSPEC
      {
       LOBJ            exp_elem;
       EXPRESSION     *exp;
       SYMGLOBAL      *sym;  // Глобальный символ, в котором храним значение выражения
      } EXPLISTSPEC;


// -----------------------------------------------------------------------------
enum CLUST_TYPE
   {
    CL_NAME,
    CL_NUM
   };

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ              fld_elem;
       enum CLUST_TYPE   cl_type;
       long              Id;            // Индекс поля, если не равен -1
       short int         isProp;
       uintptr_t         key;           // For unic type identification
      } ICLUST;

// -----------------------------------------------------------------------------
typedef struct
      {
       ICLUST    derived;
       char      name[1];  // Имя поля
      } NAME_CLUST;

// -----------------------------------------------------------------------------
typedef struct
      {
       ICLUST       derived;
       EXPLISTELEM  elem;
       short int    num;
      } NUM_CLUST;

// -----------------------------------------------------------------------------
typedef struct
      {
       VALUE          *val;     // parametrest from C call
       unsigned char  *attr;
       int             npar;
      } CPARMS;

// -----------------------------------------------------------------------------
typedef struct tagCALLSTRUCT
      {
       SYMPROC       *sym;

       union
           {
            CNTLIST   parmlist;  // список EXPLISTELEM фактические параметры
            CPARMS    cParm;
            // add by A.Tokmashev (30.06.2000)
#if (!defined(RSL_NO_PCODE) && !defined(RSL_PCODE2))
            struct tagPCODE_EXPELEM  *pcodeParm;
#endif // RSL_NO_PCODE
            // end add (30.06.2000)
           } u;
      } CALLSTRUCT;

// -----------------------------------------------------------------------------
typedef struct
      {
       EXPRESSION   derived;
       EXPRESSION  *left;
       EXPRESSION  *right;
      } EXPBINARY;

// -----------------------------------------------------------------------------
typedef struct
      {
       EXPRESSION    derived;
       EXPRESSION   *oper;
      } EXPUNARY;

// -----------------------------------------------------------------------------
typedef struct tagEXPVAL
      {
       EXPRESSION   derived;
       ISYMBOL     *sym;
       CNTLIST      fld_list;   // Список ICLUST
      } EXPVAL;

// -----------------------------------------------------------------------------
typedef struct
      {
       EXPRESSION   derived;
       CALLSTRUCT   call;
      } EXPFUNC;

//------------------- END of EXPRESSION part ----------------------------


//-------------------- STATEMENT part -----------------------------------
typedef enum
      {
       ST_EXPRESSION,
       ST_LOOP,
       ST_CONDITION,
       ST_RETURN,
       ST_OUT,
       ST_SPEC,
       ST_ENDPROC,
       ST_CONDELEM,
       ST_FOR,
       ST_FOR2,
       ST_BREAK,
       ST_CONTINUE
      } STMTTYPE;

// -----------------------------------------------------------------------------
typedef struct  tagSTATEMENT
      {
       LOBJ         stmt_elem;
       STMTTYPE     st_type;
       int          line;       // Строка исходного текста
       SYMPROC     *sym;        // Процедура, которой принадлежит инструкция

#ifdef USEDEBUGER
       LOBJ         code_elem;
       int          lastStmtOffs;
       int          offsBegin;
       int          offsEnd;
       bool         breakPoint;
       void        *breakPointData;
#endif

#ifdef USE_DBGHANDLE32
       RslHandle32  key;
#endif
      } STATEMENT;


// -----------------------------------------------------------------------------
#define  P_ST(ptr)  ((STATEMENT *)(ptr))

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       bool            called;
      } STENDPROC;

//------------------------ STOUT -------------------------------------
typedef struct
      {
       LOBJ            line_elem;
       char            line[1];
      } LINE_ELEM;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       CNTLIST         linelist;  // список LINE_ELEM
       CNTLIST         parmlist;  // список EXPLISTELEM
       char           *buff;      // временный буфер
       bool            toString;  // вывести содержимое конструкции в строковый буфер buff
      } STOUT;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       EXPRESSION     *exp;
      } STEXPRESSION;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       EXPRESSION     *cond;
       CNTLIST         stmlist;   // список STATEMENT derived
      } CONDITION;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       CNTLIST         iflist;    // list of CONDITION
      } STCONDITION;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       CONDITION      *st;
      } STLOOP;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       EXPRESSION     *initExp;
       EXPRESSION     *termExp;
       EXPRESSION     *deltaExp;
       EXPRESSION     *loopVar;
       CNTLIST         stmlist;   // список STATEMENT derived
      } STFOR;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       EXPRESSION     *ret;
      } STRETURN;

// -----------------------------------------------------------------------------
typedef struct
      {
       STATEMENT       derived;
       CNTLIST         explist;   // Список EXPLISTSPEC
      } STSPEC;
//--------------------- END of STATEMENT part ----------------------------

//------------------------  TYPESTRUCT part ----------------------------

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned char   mixMode;
       VALTYPE         v_type;
       char           *v_name1;    // имя типа данных
       char           *v_nameAlt;  // name for Numeric mode
       short int       wt1;
       short int       wt2;        // wait for numeric mode

       // Процедуры обслуживания
       void          (*init) (VALUE *This);
       void          (*copy) (VALUE *This,VALUE *destin);
       void          (*clear)(VALUE *This);
       int           (*iseq) (VALUE *This,VALUE *destin);
       void          (*out)  (VALUE *This, EXPLISTELEM *exp );
       void          (*set)  (VALUE *This,const void *ptr);
       void          (*get)  (VALUE *This,void *ptr);
       //  .......
      } TYPESTRUCT;

// -----------------------------------------------------------------------------
// Этот массив индексируется значением VALTYPE
extern TYPESTRUCT  sym_type_array[];

TYPESTRUCT  *TypeStructGet(VALUE *val);


// -----------------------------------------------------------------------------
RSLLINK void    RSLAPI     ValueInit           (VALUE  *val, VALTYPE v_type);

DLMLINK void    RSLDLMAPIC ValueMake           (VALUE *val);
DLMLINK void    RSLDLMAPIC ValueClear          (VALUE *val);
DLMLINK void    RSLDLMAPIC ValueCopy           (VALUE *from, VALUE *to);
DLMLINK int     RSLDLMAPIC ValueIseq           (VALUE *This, VALUE *dest);
DLMLINK void    RSLDLMAPIC ValueMove           (VALUE *from, VALUE *to);
DLMLINK void    RSLDLMAPIC ValueSet            (VALUE *val, VALTYPE v_type, const void *ptr);
DLMLINK bool    RSLDLMAPIC ValueGet            (VALUE *val, VALTYPE v_type, void *ptr);
DLMLINK bool    RSLDLMAPIC ValueSetAs          (VALUE *val, VALTYPE needType, VALTYPE v_type, const void *ptr);

RSLLINK void    RSLAPI     ValueOut            (VALUE  *val, EXPLISTELEM *exp);
RSLLINK void    RSLAPI     RslValuetoStr       (VALUE  *val, int w, int p, int flag, char *buf, int sz);

RSLLINK int     RSLAPI     ValueMin            (VALUE *left, VALUE *right);
RSLLINK void    RSLAPI     ValueBinaryOperation(TOKKEN oper,VALUE *left, VALUE *right);

        void               ValueStrAddChars    (VALUE *strVal, int isReplace, const char *chars, int len);

        void               ValueClrObj         (bool enable);
        bool               AllowClearObj       (void);

DLMLINK bool    RSLDLMAPIC CnvType             (VALUE *val, VALTYPE v_type);
        bool               CnvTypeInternal     (VALUE *val, VALTYPE v_type, HRD inst);

//---------------------- END of TYPESTRUCT part ------------------------

//----------------------- SCOPE part ------------------------------------

typedef enum
      {
       SCOPE_AUTO,
       SCOPE_GLOBAL,
       SCOPE_CONST,
       SCOPE_LOCAL,
       SCOPE_PARM,
       SCOPE_PROVIDER,
       SCOPE_PROP,
       SCOPE_METHOD,
       SCOPE_ALLWAYSLOC,
       SCOPE_PRIVATE
      } SCOPE_TYPE;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         context_elem;
       SYMPROC     *sym;    // Текущая локальная процедура
       int          indloc;
       int          indpar;
       int          oldlevel;
       int          methodloc;
       int          OnlyDeclared;
       CNTLIST      withList;
       int          indprop;
      } SCOPE;


// -----------------------------------------------------------------------------
void PushScope    (PROCDEF *proc);
void PopScope     (HRD inst);
void ProcScope    (SYMPROC *sym, int indprop, int methodloc, HRD inst);
void DoneScope    (void);
void ScopeReset   (HRD);
void BuildScopeFor(SYMPROC *sym);


// -----------------------------------------------------------------------------
#define  RSL_WITH_TYPED         0
#define  RSL_WITH_NONTYPED      1
#define  RSL_WITH_NAMERESOLVER  2


// -----------------------------------------------------------------------------
typedef unsigned short  TWithType;


// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         elem;
       ISYMBOL     *sym;
       SYMPROC     *owner;
       TWithType    withType;
      } WITHPTR;

// -----------------------------------------------------------------------------
typedef struct
      {
       long            id;
       int             isProp;
       const char     *name;

       ISYMBOL        *sym;     // sym.name
       uintptr_t       key;     // class key
       int             level;
       TWithType       withType;
       SYMPROC        *owner;
       const char     *nameUp;
      } FndWithData;


// -----------------------------------------------------------------------------
typedef int  (* TRslGetChProc)(void *data);
typedef void (* TRslCloseProc)(void *data);
typedef int  (* TRslReadProc) (void *data, void *buff, size_t size);


// -----------------------------------------------------------------------------
typedef struct TExtSrcInfo
      {
       char            *name;
       TRslGetChProc    get_ch;
       TRslCloseProc    closePtr;
       void            *data;
      } TExtSrcInfo;

//----------------------- END of SCOPE part ----------------------------


//-------------------- Прототипы функций --------------------------------

// -----------------------------------------------------------------------------
typedef struct tagInspectSymData
      {
       int        (*proc)(ISYMBOL *sym, SYMMODULE *mod, void *UserData);
       SYMMODULE   *module;
       ISYMBOL     *sym;
      } InspectSymData;

// -----------------------------------------------------------------------------
typedef struct tagTNameSpaceData
      {
       char        *nameSpace;
      } TNameSpaceData;


// -----------------------------------------------------------------------------
RSLLINK bool       RSLAPI ModuleOpen        (TRslMesProc MsgFun);
RSLLINK void       RSLAPI ModuleClose       (void);
        void              PushNewModule     (const char *name, SYMMODULE **mod, unsigned char std, const char *file1, const char *file2);
RSLLINK void       RSLAPI DelModule         (SYMMODULE *mod);
RSLLINK void       RSLAPI RemoveModule      (const char *name);

        SYMMODULE        *FindModule        (const char *name);
        SYMMODULE        *FindModuleEx      (const char *name);
        bool              TestExistModule   (const char *file);

        SYMMODULE        *FindModuleByKey   (RslHandle32 key);
        STATEMENT        *FindStmtByKey     (RslHandle32  key);
        SYMPROC          *FindProcByKey     (RslHandle32  key);

        SYMMODULE        *FindModuleByCookie(uintptr_t cookie);
RSLLINK int RSLAPI        ModuleAssignCookie(const char *name, uintptr_t cookie);

        SYMMODULE        *FindAttachModule  (const char *name,bool *attached);


// -----------------------------------------------------------------------------
#define  RSL_MAXMODNAMELEN  80


// -----------------------------------------------------------------------------
        char                   *RslMkModuleName  (const char *name, char *module);

        void                    PopModule        (void);
        SYMMODULE              *TopModule        (void);
        SYMMODULE              *SetTopModuleExp  (SYMMODULE *newMod);

        void                    ModuleSetStdProc (SYMMODULE *sym, RslStdProc proc);
        bool                    MakeStdModule    (const char *modname, RslStdProc proc, RslStdProc init, bool);
        bool                    MakeStdModuleEx  (const char *modname, RslStdProc proc,
                                                  RslStdProc init, RslStdProc done, DLMHAND id, bool *newFlag, bool);

DLMLINK SYMPROC    *RSLDLMAPIC  AddStdProc       (VALTYPE type, const char *name, RslStdProc proc, int attr);
        SYMPROC                *AddDefProc       (VALTYPE type, const char *name, SCOPE_TYPE scope, bool override,int offset, bool thisPCode);
        SYMPROC                *AddDefProc2      (VALTYPE type, const char *name, SCOPE_TYPE scope, bool override,int offset, bool thisPCode, const char *name_origin);
        SYMPROC                *AddDefProcEx     (VALTYPE type, const char *name, SCOPE_TYPE scope, bool override,int offset, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMPROC                *AddDefProcEx2    (VALTYPE type, const char *name, SCOPE_TYPE scope, bool override,int offset, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);
        SYMCLASS               *AddRslClass      (const char *name);
        SYMCLASS               *AddRslClass2     (const char *name, const char *name_origin);
        SYMCLASS               *AddRslClassEx    (const char *name, MODIFIER modifier, bool thisPCode, SCOPE_TYPE scope, bool toUpper);
        SYMCLASS               *AddRslClassEx2   (const char *name, MODIFIER modifier, bool thisPCode, SCOPE_TYPE scope, bool toUpper, const char *name_origin);
        SYMCLASS               *FindAddRslClass  (bool inferit);
        ISYMBOL                *FindAddRslClassRT(const char *name);

#ifndef NO_BTR
        SYMFILE                *AddSymFile       (const char *name, SCOPE_TYPE scope, int delta);
        SYMFILE                *AddSymFile2      (const char *name, SCOPE_TYPE scope, int delta,const char *name_origin);
        SYMFILE                *AddSymFileEx     (const char *name, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMFILE                *AddSymFileEx2    (const char *name, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);
        SYMSTRUC               *AddSymStruc      (const char *name, SCOPE_TYPE sc, int delta);
        SYMSTRUC               *AddSymStruc2     (const char *name, SCOPE_TYPE sc, int delta, const char *name_origin);
        SYMSTRUC               *AddSymStrucEx    (const char *name, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMSTRUC               *AddSymStrucEx2   (const char *name, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);
#endif

RSLLINK void                    AllocSymArray    (AINST *r, int size);
RSLLINK void                    ArrayWriteValue  (AINST *r, VALUE *v, int Id);
        SYMARRAY               *AddSymArray      (const char *name, SCOPE_TYPE sc);
        SYMARRAY               *AddSymArray2     (const char *name, SCOPE_TYPE sc, const char *name_origin);
        SYMARRAY               *AddSymArrayEx    (const char *name, SCOPE_TYPE sc, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMARRAY               *AddSymArrayEx2   (const char *name, SCOPE_TYPE sc, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);
        SYMTXTFILE             *AddSymTxt        (const char *name, char *file, SCOPE_TYPE sc);
        SYMTXTFILE             *AddSymTxt2       (const char *name, char *file, SCOPE_TYPE sc, const char *name_origin);
        SYMTXTFILE             *AddSymTxtEx      (const char *name, char *file, SCOPE_TYPE sc, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMTXTFILE             *AddSymTxtEx2     (const char *name, char *file, SCOPE_TYPE sc, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);

#ifdef DBASE
        SYMDBFFILE             *AddSymDbf        (const char *name, char *file, SCOPE_TYPE sc, int delta);
        SYMDBFFILE             *AddSymDbf2       (const char *name, char *file, SCOPE_TYPE sc, int delta, const char *name_origin);
        SYMDBFFILE             *AddSymDbfEx      (const char *name, char *file, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper);
        SYMDBFFILE             *AddSymDbfEx2     (const char *name, char *file, SCOPE_TYPE sc, int delta, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);
#endif

DLMLINK ISYMBOL    *RSLDLMAPIC  AddSymGlobal     (VALTYPE type, const char *name);
DLMLINK ISYMBOL    *RSLDLMAPIC  AddSymGlobal2    (VALTYPE type, const char *name, const char *name_origin);
        ISYMBOL                *AddSymGlobalEx   (VALTYPE type, const char *name, bool thisPCode, MODIFIER modifier, bool toUpper);
        ISYMBOL                *AddSymGlobalEx2  (VALTYPE type, const char *name, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);

RSLLINK SYMGLOBAL  *RSLAPI      RslAddConst      (VALTYPE type, const char *name, void *ptr);
        ISYMBOL                *AddSymConstant   (void);
        ISYMBOL                *AddSymConstantEx (bool fPCode);
        ISYMBOL                *AddSymLocal      (VALTYPE type, const char *name, SCOPE_TYPE sc, SYMPROC **owner);
        ISYMBOL                *AddSymLocal2     (VALTYPE type, const char *name, SCOPE_TYPE sc, SYMPROC **owner, const char *name_origin);
        ISYMBOL                *AddSymLocalEx    (VALTYPE type, const char *name, SCOPE_TYPE sc, SYMPROC **owner, bool thisPCode, MODIFIER modifier, bool toUpper);
        ISYMBOL                *AddSymLocalEx2   (VALTYPE type, const char *name, SCOPE_TYPE sc, SYMPROC **owner, bool thisPCode, MODIFIER modifier, bool toUpper, const char *name_origin);

        ISYMBOL                *FindAllSymbolWith(SYMMODULE **module, bool curCntxt, bool onlyClass, SYMPROC **owner,
                                                  MODIFIER modifier, bool isVar, FndWithData *data);
        ISYMBOL                *FindAllSymbolEx  (SYMMODULE **module, bool curCntxt, bool onlyClass, SYMPROC **owner, MODIFIER modifier, bool isVar);
        ISYMBOL                *FindAllSymbol    (SYMMODULE **module, int is_local);
        ISYMBOL                *FindAddSymbol    (void);
        SYMGLOBAL              *FindAddConst     (void);
        ISYMBOL                *AddSymAuto       (SYMPROC **owner);
        ISYMBOL                *AddSymAutoEx     (SYMPROC **owner, MODIFIER modifier);
        void                    PrepareVarContext(SYMMODULE *mod);
        void                    DoneVarContext   (void);
        void                    RemSymbol        (ISYMBOL *sym, SCOPE_TYPE sc);

DLMLINK void       RSLDLMAPI    SetSymAttr       (ISYMBOL *sym, int attr);

DLMLINK ISYMBOL   *RSLDLMAPIC   FindSymbolProc   (const char *name, SYMPROC *sym);
RSLLINK ISYMBOL   *RSLAPI       FindSymbolProcEx (const char *name, SYMPROC *sym, bool onlyClass);
RSLLINK ISYMBOL   *RSLAPI       FindSymbolProcEx2(const char *name, SYMPROC *sym, bool onlyClass, const char *nameModule);


        void                    DoneExpression   (EXPRESSION *exp);
        void                    FreeExpList      (EXPLISTELEM *elist);
RSLLINK bool       RSLAPI       AddSystemModule  (void);
RSLLINK bool       RSLAPI       AddBtrieveModule (void);

        void                    FreeSymbol       (ISYMBOL *pS);
        void                    FreeDelayModule  (SYMMODULE *mod);
        void                    DoneModule       (SYMMODULE *mod);

        ISYMBOL                *NewSymbol        (const char *name, SYMTYPE s_type, int delta);
        ISYMBOL                *NewSymbol2       (const char *name, SYMTYPE s_type, int delta, const char *name_origin);
        ISYMBOL                *NewSymbolEx      (const char *name, SYMTYPE s_type, int delta, bool toUpper);
        ISYMBOL                *NewSymbolEx2     (const char *name, SYMTYPE s_type, int delta, bool toUpper, const char *name_origin);
        CNTLIST                *GetVarList       (SCOPE_TYPE scope, SYMPROC **sym, SCOPE_TYPE *resultScope);

        void                    InitProcDef      (VALTYPE type, PROCDEF *proc, unsigned char is_std);
//----------------------------------------------------------------------

//------------------ Обслуживание списка глобальных переменных ----------
#define  SF_SPECIAL      1  // Выбор спецпеременных
#define  SF_VAR          2  // Выбор обычных переменных
#define  SF_MACRO        4  // Выбор макросов
#define  SF_FILE         8  // Выбор файлов (объектов типа FILE)
#define  SF_STRUC       16  // Выбор объектов типа RECORD
#define  SF_ARRAY       32  // Выбор массивов
#define  SF_ALLMODULE   64  // Выбор из всех загруженных модулей
#define  SF_CONST      128  // Выбор констант


// -----------------------------------------------------------------------------
typedef int (*RslInspectProc)(ISYMBOL *sym, SYMMODULE *mod, void *UserData);


// -----------------------------------------------------------------------------
RSLLINK int     RSLAPI     InspectGlobals(RslInspectProc proc, int flag);
RSLLINK int     RSLAPI     InspectModule (RslInspectProc proc, int flag, const char *modname);

DLMLINK void    RSLDLMAPIC SymGlobalSet  (ISYMBOL *sym, VALTYPE v_type, void *ptr);
DLMLINK VALUE  *RSLDLMAPIC SymGlobalGet  (ISYMBOL *sym);


// -----------------------------------------------------------------------------
int      InitGlobals           (SYMMODULE *mod);
int      GlDone                (ISYMBOL *sym);
void     DoneRunState          (void);
int      DoneAddModuleRunState (SYMMODULE *mod, long *passNumber);

#ifndef NO_BTR
        int  InitSymFile       (SYMFILE *sym);
        void DoneSymFile       (SYMFILE *sym, BINST *iref);

        int  InitSymStruc      (SYMSTRUC *sym);
        void DoneSymStruc      (SYMSTRUC *sym, SINST *r);

        void ClearStrInst      (SYMSTRUC *sym, SINST *r);
#endif

int      InitSymArray          (SYMARRAY *sym);
void     DoneSymArray          (SYMARRAY *sym, AINST *r);
void     ClearArrayInst        (AINST *r);

int      InitSymTxt            (SYMTXTFILE *sym);
void     DoneSymTxt            (SYMTXTFILE *sym, TINST *r);
void     ClearTxtInst          (TINST *r);

RSLLINK void RSLAPI SetUserData(void *UserData);

VALTYPE  GetVarType            (char *buf);

WITHPTR *PushWith              (ISYMBOL *sym, SYMPROC *owner, HRD);
void     PopWith               (HRD);
ISYMBOL *FindWith              (FndWithData *data, TWithType tp, SYMPROC **owner);

void     ScopeListFree         (SCOPE *pC);

bool     ParseExpAtContext     (SYMPROC *sym, const char *mem, EXPRESSION **exp, SYMPROC **topContext, bool *topContextOnly);

void     DoneExpListElem       (TExpElem *el);
bool     ExpIsRefModule        (EXPRESSION *exp, SYMMODULE *mod);
void     RslValidateExpressions(SYMMODULE *remMod);


#ifdef ENUMINTSIZE
#pragma option -b
#endif

void RegisterTUniProvider(void);
void RegisterTUniProviderGlobal(void);
void RegisterRCWProviderGlobal(void);

void RegisterWrProviderGlobal(void);
void IsWrObect(void);
void CreateWrObect(void);
void StrongRef(void);

TGenObject   *TWeakRefGetObj(TGenObject *p);
uintptr_t     TWeakRefCLSID(void);

// May be called in IM_ADD_NAMERESOLVER
RSLLINK void RSLAPI RslAddNameResolver  (SYMVAR *sym);
RSLLINK void RSLAPI RslAddNameResolverEx(SYMVAR *sym, bool isTyped);

// add by A.Tokmashev (T74) 23.05.2000
#ifndef RSL_NO_HASH
    void AddToGlobalHash(ISYMBOL *pSym);
#endif // RSL_NO_HASH


#ifndef RSL_PCODE2
#ifndef RSL_NO_PCODE
    void           ExecPCodeProc     (HPCODE hPCode, struct tagCALLSTRUCT *call);
    void           ExecPCodeListStmt (HPCODE hPCode, PCODEENTRYPOINT *entry);
    int            ExecPCodeEndProc  (HPCODE hPCode, PCODEENTRYPOINT *entry);
    bool           PCodeSetParmEx    (HPCODE hPCode, struct tagProcProlog *runOn, struct tagProcProlog *bp, int n, VALUE *val);
    int            PCodeExpGetNitem  (PCODE_EXPELEM *expElem);
    int            PCodeClustGetNitem(PCODE_ICLUST *clust);
    PCODE_EXPELEM *PCodeGetAt        (PCODE_EXPELEM *expElem, int n);
#endif
#endif // RSL_PCODE2

#ifndef RSL_NO_PCODE
    HPCODE         CreatePCodeManager(void);
    void           DeletePCodeManager(HPCODE hPCode);
    bool           LoadPCode         (HPCODE hPCode, const char *importFile, const char *impName, void *pS);
    bool           SavePCodeModule   (HPCODE hPCode, SYMMODULE *mod);
RSLLINK bool RSLAPI SavePCode        (HPCODE hPCode); // for conintf.hpp
    int            GetModePCode      (void);
    bool           CheckActualPCode  (const char *name, char *fileRSL, char *fileMAC, int mode);
#endif // RSL_NO_PCODE

void ReadVal(VALTYPE v_type, void *ptr, int size, int decNum);

void InitValueInfoList(void);
void DoneValueInfoList(void);
void ValidateValueInfo(SYMMODULE *sym);


// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
            TGenObject *DLMAPI CreateObjectOfRslClass(ISYMBOL *pSym);
            ISYMBOL    *DLMAPI RslAddObjVar          (const char *name, int tp, int attr, const char *clsName, TGenClass *genCls, TVarInitProc proc);
            ISYMBOL    *DLMAPI RslAddVar             (const char *name, int tp, int attr, void *val);
            void        DLMAPI RslSetClientData      (void *ptr, const char *name, void *reserved);
            void       *DLMAPI RslGetClientData      (const char *name, int reserved);
#else
    RSLLINK TGenObject *RSLAPI CreateObjectOfRslClass(ISYMBOL *pSym);
    RSLLINK ISYMBOL    *RSLAPI RslAddObjVar          (const char *name, int tp, int attr, const char *clsName, TGenClass *genCls, TVarInitProc proc);
    RSLLINK ISYMBOL    *RSLAPI RslAddVar             (const char *name, int tp, int attr, void *val);
    RSLLINK void        RSLAPI RslSetClientData      (void *ptr, const char *name, void *reserved);
    RSLLINK void       *RSLAPI RslGetClientData      (const char *name, int reserved);
#endif


// -----------------------------------------------------------------------------
typedef struct tagStdModIntf
      {
       int         (DLMAPI  *GetVersion)      (void);
       char       *(DLMAPI  *GetName)         (void);

       int         (DLMAPI  *Print)           (const char *data);
       void        (DLMAPI  *ValueToString)   (VALUE  *val,int w,int p,int flag,char *buf,int sz);
       const char *(DLMAPI  *GetIncPath)      (void);
       int         (DLMAPI  *GetMoneyFormat)  (void);
       int         (DLMAPI  *IsParmVariable)  (int n);
       void        (DLMAPI  *SetUpExtObject)  (TGenObject *mainObj,TGenObject *obj); // in version 4

       // Version 5
       bool        (DLMAPI  *GetParmFrame)    (int level,int n,VALUE **val);
       int         (DLMAPI  *GetParmAttrFrame)(int level,int n);
       int         (DLMAPI  *GetNumParmFrame) (int level);
       bool        (DLMAPI  *PutParmFrame2)   (int level,int n,VALUE *val,bool onlyByRef);
       bool        (DLMAPI  *PutParmFrame)    (int level, int n,VALTYPE v_type,void *ptr,bool onlyByRef);
       bool        (DLMAPI  *IsEqClass)       (SYMCLASS *sym,TGenObject *obj);
       SYMCLASS   *(DLMAPI  *FindSymClass)    (const char *name);
       void        (DLMAPIC *RslErrorEx)      (int level,const char *fmt, ...);

       // Version 6
#ifndef NO_BTR
       bclient_h (DLMAPI *GetBtrClient)       (int *isSql);
#else
       void      (DLMAPI *Dummy1)             (void);
#endif

       // Version 7
       void      (DLMAPI *SetUserObj2ErrObject)(TGenObject *userObj);
      } TStdModIntf;


// -----------------------------------------------------------------------------
typedef struct tagRcwIntf
      {
       int          (DLMAPI *GetVersion)       (void);
       char        *(DLMAPI *GetName)          (void);

       void         (DLMAPI *SetRsComErrObj)   (int stat);
       void         (DLMAPI *RunRsComErr)      (const char *message, int stat);
       int          (DLMAPI *GetRSCOMerror)    (void);

       TGenObject  *(DLMAPI *CreateErrorObject)(void *info);

       void        *(DLMAPI *CreateRCWSink)    (TGenObject *rslObj, const char *name);
       void         (DLMAPI *DestoyRCWSink)    (void *obj);
       int          (DLMAPI *IsCOMObjWrapper)  (TGenObject *rslObj);
      } TRcwIntf;

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif   // isymbol_h

/* <-- EOF --> */