/*@H*********************************************************************
*        File Name:   iexec.h          December 11,1993            
*       Written by:  S. Kubrin   VECsoft TM.
*      Description:   
*  Public routines:  
* Private routines:  
**********************************************************************@H*/
#if !defined(__IEXEC_H)
#define  __IEXEC_H

#ifndef __STDARG_H
#include <stdarg.h>
#endif // STDARG.H

#ifndef __RSTYPES_H
#include <rstypes.h>
#endif // __RSTYPES_H

#ifndef __RSCDEFS_H
#include <rscdefs.h>
#endif // __RSCDEFS_H

#include <packpshn.h>

#ifdef __cplusplus
extern "C" {
#endif


// -----------------------------------------------------------------------------   
// Для обслуживания сообщения IM_TESTEVENT
typedef struct
      {
       long  key;
       long  kbflags;
       long  scan;
       long  Xm;
       long  Ym;
       long  Butn;

       int   time;
      } RSL_EVENT_MES;

// -----------------------------------------------------------------------------   
// Для обслуживания сообщения IM_FORMATSTR и IM_GET_CMD_ARG
typedef struct
      {
       char   *str;
       int     newLen;
       int     v_type;
       int     w;
       int     flag;
       VALUE  *value;
      } RSL_FORMAT_STR;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_SYSTEM_CALL
typedef struct
      {
       int    id;
       int    system;
       char  *cmdArgs;
      } RSL_SYSTEM_CALL;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_GETUSERDATA 
typedef struct
      {
       void  *UserData;
      } TMesGetUserData;


// -----------------------------------------------------------------------------
#define TMESGETUSERDATA

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INIT_EXECUTOR
typedef struct
      {
       FILE         *outstream;
       TRslOutProc   customProc;
       // ......
      } INIT_EXECUTOR;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INPUT_DATA
typedef struct
      {
       unsigned char  v_type;  // Тип данного для ввода
       void          *ptr;     // Куда положить введенные данные
       int            size;    // Размер буфера под данные
       char          *prompt;  // Приглашение к вводу
       int            outlen;  // Ширина поля вывода
       char           align;   // 1 - выравнивание по правому краю
       int            passwd;
       int            decNum;
      } INPUT_DATA;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INPUT_DATAEX
typedef struct
      {
       unsigned char   v_type;        // Тип данного для ввода
       void           *ptr;           // Куда положить введенные данные
       int             size;          // Размер буфера под данные
       const char     *prompt;        // Приглашение к вводу
       int             outlen;        // Ширина поля вывода
       char            align;         // 1 - выравнивание по правому краю
       int             passwd;
       int             decNum;

       int             version;       // Must be 0
       int             handled;
       const char     *head;
       const char     *stLine;
      } INPUT_DATAEX;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_SET_STRUC
typedef struct
      {
       char   *name;     // Имя объекта
       char   *dicname;  // Имя в словаре
       void   *Buff;     // Буфер данных
       int     alloced;  // 1 == динамически распределен
      } SET_STRUC;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_RUN_PROG
typedef struct
      {
       char  *prog;
       char  *parm;
       char  *stmes;
       char  *fnmes;
      } RUN_PARM;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_MEM_USAGE
typedef struct
      {
       unsigned long  usageCount;
      } MEM_USAGE;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_MENU
typedef struct
      {
       VALUE       *array;
       int          nElem;
       int          width;
       int          height;
       long         result;
       char        *prompt;
       char        *head;
       int          x, y;
       int          curPos;
       TGenObject  *obj;
      } MENU_DATA;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INIT_PROGRESS
typedef struct
      {
       long   initVal;
       char  *statMes;
       char  *headMes;
      } ProgressData;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INIT_TRN IM_DONE_TRN
typedef struct
      {
       SYMPROC    *sym;      // Процедура выполняемая в транзакции
       int         processType;
       bool        isAbort;  // iTRUE если выполнен BtAbortTrn
#ifndef NO_BTR
       enum Locks  lock;
#endif
       int         trnFlag;  // Набор флагов RSL_TRN_... из bt.h
       int         endTrnStat;
       RslR2M      r2m;
       bclient_h   clientID;
       int         extstat;  // bfstat form client C++ programm
      } TRslProcessData;

// -----------------------------------------------------------------------------
// Для обслуживания сообщения IM_INIT_TRN IM_VIEW_FILE
typedef struct
      {
       char  *dic;
       char  *file;
       int    edit;
       int    isTxt;     // Text file flag
      } TRslViewFile;

// -----------------------------------------------------------------------------
// For handle IM_CHANGE_OUTPUT message
typedef struct
      {
       const char  *newFileName;   // If NULL restore original output name
       char        *oldFileName;   // Return by message procedure
       FILE        *outstream;     // Opened stream
       bool         isAppend;      // If true, Append to output file

       int          err_no;        // Результат открытия/создания файла
      } TChangeOutput;


// -----------------------------------------------------------------------------
// For handle IM_REWIND
#define  REWIND_BEGIN    0
#define  REWIND_END      1
#define  REWIND_CURPOS   2


// -----------------------------------------------------------------------------
typedef struct
      {
       long  offset;   // 0
       int   origin;   // REWIND_BEGIN, REWIND_END, REWIND_CURPOS
      } TRewindData;

// -----------------------------------------------------------------------------
//   Флаги форматирования
#define  F_LEFT              1   // 'l'
#define  F_RIGHT             2   // 'r'
#define  F_JUST              3   // 'c'
#define  F_APOS              4   // 'a'
#define  F_TRUNC             8   // 't'
#define  F_LONG             16   // 'd'
#define  F_EXPAND           24   //
#define  F_TAB              32   //
#define  F_MWORD            64   // 'm'
#define  F_WRAP            128   // 'w'
#define  F_HEX            2048   // 'x'
#define  F_ZERO            256   // 'z'
#define  F_EXTERN          512   // 'f'
#define  F_BYREF          1024   // 'i'
#define  F_BYREFVAR       2048   // 'v' если после 'i'
#define  F_BYVAL          2048   // 'v' если само по себе
#define  F_FILL           4096   // 'o'
#define  F_BYREFAUTO      8192   //

#define  F_BYREFSHORT    16384   // 's'
#define  F_UNDEFEMPTY    32768   // 'e'


//---------------------- Вычисление переменной -------------------------
#ifdef ENUMINTSIZE
#pragma option -b-
#endif

// -----------------------------------------------------------------------------
typedef enum
      {
       CV_VALUE,
       CV_FIELD,
       CV_FILE,
       CV_STRUC,
       CV_STRFIELD,
       CV_ARRAY,
       CV_ARFIELD,
       CV_TXTFILE,
       CV_TXTFIELD,
       CV_DBFFILE,
       CV_DBFFIELD
      } CV_TYPE;

#ifdef ENUMINTSIZE
#pragma option -b
#endif

// -----------------------------------------------------------------------------
typedef struct
      {
       CV_TYPE      cv_type;
       ISYMBOL     *sym;
       void        *inst;

       union
           {
            int     Id;
            VALUE  *vl;
           } u;
      } CALCVAL;


// -----------------------------------------------------------------------------
void CalcVariable(CALCVAL *cv, EXPVAL *val);

//====================================================================
typedef enum
      {
       CV_RSLVAL,
       CV_TMPVAL,
       CV_GENOBJ,
       CV_RSLOBJ_FILE,
       CV_RSLOBJ_STRUC,
       CV_RSLOBJ_ARRAY,
       CV_RSLOBJ_TXT,
       CV_RSLOBJ_DBF
      } CV_TYPE2;

// -----------------------------------------------------------------------------
struct FCV_RSLVALUE
     {
      ISYMBOL  *sym; // SYMGLOBAL or SYMLOCAL
      VALUE    *val;
     };

// -----------------------------------------------------------------------------
struct FCV_RSLOBJECT
     {
      ISYMBOL   *sym;  // SYMFILE,SYMSTRUC,SYMTXTFILE,SYMDBFFILE
      void      *inst; // Instance for sym
     };

// -----------------------------------------------------------------------------
struct FCV_GENOBJECT
     {
      CALLSTRUCT   call;
      void        *obj;  // GenObject
     };

// -----------------------------------------------------------------------------
typedef struct tagCALCVAL2
      {
       CV_TYPE2                    cv_type;

       union
           {
            struct FCV_RSLVALUE    rslval;
            struct FCV_RSLOBJECT   rslobj;
            struct FCV_GENOBJECT   genobj;
            CALLSTRUCT             rslfun;
           } inst;

       bool                        isNamed; // if true - cl point to NAME_CLUST
       ICLUST                     *cl;      // if NULL - no field
      } CALCVAL2;


void CalcVariableNew(CALCVAL2 *cv, EXPVAL *val, bool isAssign, HRD inst);
bool EnterProc(CALLSTRUCT *call, VALUE *ret, unsigned char flags, int delta, HRD inst);


//RSLLINK void RSLAPI ExecMethod (CALCVAL2 *cv,TGenObject *obj);
RSLLINK void RSLAPI ExecClassMethod(CALLSTRUCT *call);

void ExecClassMethodInternal(CALLSTRUCT *call, HRD inst);

bool PopProlog(HRD inst);
void PushProlog(CALLSTRUCT *call,void *obj, HRD inst);
void PushPrologParm(SYMPROC *proc,va_list ap, int usePar, VALUE *par, void *obj, HRD inst);
void PushPrologParmEx(SYMPROC *proc,va_list ap, int usePar, VALUE *par, void *obj, HRD inst, unsigned char *attr);

//====================================================================


//---@H------------- Exit Procedure ------------------------------------
//typedef void (*ExitProcType)(void);
//----------------------------------------------------------------------

RSLLINK bool     RSLAPI     ExecOpen            (void);
RSLLINK void     RSLAPI     ExecClose           (void);

RSLLINK bool     RSLAPI     ExecPrepear         (bool clearParserErr);
RSLLINK void     RSLAPI     ExecShut            (void);

DLMLINK SYMPROC *RSLDLMAPIC FindMacro           (const char *name);
RSLLINK SYMPROC *RSLAPI     FindMacroEx         (const char *name);

DLMLINK bool     RSLDLMAPIC RunMacro            (SYMPROC *sym, ...);
RSLLINK bool     RSLAPI     RunMacroName        (const char *name, ...);
RSLLINK bool     RSLAPI     DoRunProc           (SYMPROC *proc, va_list_ptr ap, int usePar, VALUE *par);
RSLLINK bool     RSLAPI     RunMacroName2       (const char *name, int nPar, VALUE *par);
RSLLINK bool     RSLAPI     RunMacro2           (SYMPROC *sym, int nPar, VALUE *par);

RSLLINK bool     RSLAPI     SetRecordBuff       (const char *name, void *adr);
RSLLINK VALUE   *RSLAPI     RunResult           (void);

        void                RunProc             (SYMPROC *sym, HRD inst);
        void                ExecExpression      (EXPRESSION *exp, HRD inst);
        int                 ExecStatement       (STATEMENT *stmt, HRD inst);
        bool                ExecModule          (int free_obj);
        int                 ExecModuleEx        (SYMMODULE *mod, HRD inst);
        bool                Execute             (void);
        void                ExecProc            (CALLSTRUCT *call, HRD inst);
        void                ExecExpList         (CNTLIST *parm, HRD inst);
        int                 ExecStmtList        (CNTLIST *stmt, HRD inst);
        void                SetUpRtInit         (void);


DLMLINK VALUE   *RSLDLMAPIC PushValue           (VALUE *val);
DLMLINK bool     RSLDLMAPIC PopValue            (void);

        VALUE              *RefValue            (VALUE *val);
        VALUE              *PushTmpVal          (void);
        bool                PushValRef          (VALUE *val);
        bool                AdvanceStack        (int size);
        void                Assign              (EXPVAL *dest, VALUE *from, bool abortOnConst, HRD inst);

        bool                AdvanceStackInternal(int size, HRD inst);
        bool                PopValueInternal    (HRD inst);
        bool                PopValueInternal    (HRD inst);
        VALUE              *PushValueInternal   (VALUE *val, HRD inst);

        void                ExecBinaryOperation (TOKKEN tok, HRD inst);
        void                ExecUnaryOperation  (TOKKEN tok, HRD inst);

        bool                GetConditionResult  (HRD inst);

DLMLINK int    RSLDLMAPIC   GetNumParm          (void);

DLMLINK bool   RSLDLMAPIC   GetParm             (int n, VALUE **val);

DLMLINK int    RSLDLMAPIC   GetParmAttr         (int n);
RSLLINK int    RSLAPI       IsParmVariable      (int n);
            

// -----------------------------------------------------------------------------
typedef void (DLMAPIC *RslRaiseEvProc)(void *data, char *evName);

// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
    bool DLMAPI PutParm (int n, VALTYPE v_type, void *ptr);
    bool DLMAPI PutParm2(int n, VALUE *v);

    #define  SetParm   PutParm
    #define  SetParm2  PutParm2
           
    void DLMAPI DoRaiseEvents(RslRaiseEvProc ptr, void *data);
#else
    RSLLINK bool RSLAPI SetParm (int n, VALTYPE v_type, void *ptr);
    RSLLINK bool RSLAPI SetParm2(int n, VALUE *val);

    #define  PutParm   SetParm
    #define  PutParm2  SetParm2

    void DoRaiseEvents(RslRaiseEvProc ptr, void *data);
#endif       

bool SetParmIfByRef(int n, VALUE *val);

DLMLINK VALUE *RSLDLMAPIC GetReturnVal(void);
DLMLINK bool   RSLDLMAPIC SetParmAs(int n, VALTYPE needType, VALTYPE v_type, const void *ptr);


bool GetParmFrame    (int level, int n, VALUE **val);
int  GetParmAttrFrame(int level, int n);
int  GetNumParmFrame (int level);
bool PutParmFrame2   (int level, int n, VALUE *val, bool onlyByRef);
bool PutParmFrame    (int level, int n, VALTYPE v_type, void *ptr, bool onlyByRef);

DLMLINK void RSLDLMAPIC ReturnVal (VALTYPE v_type, void *ptr);
DLMLINK void RSLDLMAPIC ReturnVal2(VALUE *from);

void   ExecOut(STOUT *pO, HRD inst);

RSLLINK void RSLAPI SwitchToProc(SYMPROC *sym, int n);
void SwitchToProcInternal(SYMPROC *sym, int n, HRD inst);


DLMLINK bool RSLDLMAPIC SwitchToMethod(SYMPROC *sym, int n);

#ifndef BUILD_DLM
    #define  SwitchToMacro  SwitchToMethod
#endif

// -----------------------------------------------------------------------------
#ifndef NO_BTR
    int  BtrFindField (SYMFILE *sym, const char *name);
    void BtrReadValue (SYMFILE *sym, BINST *bi, VALUE *v, int Id);
    void BtrWriteValue(SYMFILE *sym, BINST *bi, VALUE *v, int Id);

    RSLLINK void RSLAPI ConnectFile(SYMFILE  *sym, BINST *r);
    RSLLINK void RSLAPI ConnectBuff(SYMSTRUC *sym, SINST *r);

    RSLLINK int ConnectStructure(SYMSTRUC *sym);

    void StrReadValue (SYMSTRUC *sym, SINST *r, VALUE *v, int Id);
    void StrWriteValue(SYMSTRUC *sym, SINST *r, VALUE *v, int Id);
#endif

VALUE *GetLocalValue(int level, int offset, HRD inst);

RSLLINK bool RSLAPI Std_Execute(const char *mac, TRslMesProc Msg, bool (*proc)(void *), void *);

// -----------------------------------------------------------------------------
#ifndef RSL_NO_PCODE
    RSLLINK bool RSLAPI Std_Generation(const char *szFileName, TRslMesProc Msg, void *, bool forEach);
    RSLLINK bool RSLAPI Add_Generation(const char *szFileName, bool forEach); 
#endif // RSL_NO_PCODE

RSLLINK bool RSLAPI Add_Execute(const char *mac, bool (*ProcFun)(void *), void *AddData);
bool Add_ExecuteLocal(const char *szFileName, TRslActionProc ProcFun, void *AddData, bool abortOnParserErr);
bool Add_ExecuteModuleMem(const char *strModule, TRslActionProc ProcFun, void *AddData);

bool LoadModuleLocal(const char *module, bool abortOnParserErr);

// -----------------------------------------------------------------------------
#ifdef RSL_NO_HOST
    RSLLINK bool RSLAPI RslInExec(void);
#endif // RSL_NO_HOST

RSLLINK int  RSLAPI FindMacroFile(char *Buff, const char *file);
RSLLINK bool RSLAPI RslChangeOutput(char *newFile, bool isAppend, char *oldFile);
RSLLINK bool RSLAPI RslChangeOutputEx(char *newFile, bool isAppend, char *oldFile, int *err_no);
int                 FindRsmFile(char *Buff, const char *file);

bool StartM(void *data);

// -----------------------------------------------------------------------------
#define  EXIT_SHOWOUT  1
#define  EXIT_NOTSHOW  2
#define  EXIT_BREAK    3 
#define  EXIT_PRINT    4 


RSLLINK void RSLAPI SetExitFlag(int code);

RSLLINK bool   RSLAPI ArraySize(SYMARRAY *sym, int size);
RSLLINK bool   RSLAPI ArraySet (SYMARRAY *sym, VALUE *v, int Id);
RSLLINK VALUE *RSLAPI ArrayGet (SYMARRAY *sym, int Id);

void RslMsgBox (void);


// Эта процедура вызывается для прекращения транзакции на языке C
RSLLINK void RSLAPI RslAbortTrn  (void);
RSLLINK void RSLAPI RslAbortTrnEx(int stat);



// October 3,1995 Column output 
void ColumnInit(void);
void ColumnDone(void);

RSLLINK void RSLAPI ColumnSet(int col);
RSLLINK void RSLAPI ColumnFlush(void);
RSLLINK int  RSLAPI RslSetDefPrec(int newPrec);


// November 10,1995
//extern unsigned RslCurrentPrintLine;

// -----------------------------------------------------------------------------
// June 6,1996
#ifndef NO_BTR
    TBtDataBase *MakeDbForRslRes(TBtError *er);
#endif

// -----------------------------------------------------------------------------                  
#ifdef RSL_NO_HOST
    RSLLINK void *RSLAPI RslGetUserData(void);
#endif // RSL_NO_HOST

RSLLINK int RSLAPI ErCnv        (VALUE *val, VALTYPE v_type, int shower);
int                ErCnvInternal(VALUE *val, VALTYPE v_type, int shower, HRD inst);

void ExecClassCtr   (CALLSTRUCT *call, bool doCreate, HRD inst);
void ExecClassCtrExt(CALLSTRUCT *call, bool doCreate, HRD inst);

void ClearRslStack(PROCPROLOG *limit, VALUE *v);


void convert_ansi_oem(void);
void convert_oem_ansi(void);
void convert_ansi_utf8(void);
void convert_oem_utf8(void);
void convert_utf8_oem(void);
void convert_utf8_ansi(void);

VALUE *DeRef(VALUE *val);

TRslOutProc RslSetOutProc(TRslOutProc newProc);


DLMLINK ISYMBOL *RSLDLMAPI RslGetInstSymbol  (const char *pName);
DLMLINK ISYMBOL *RSLDLMAPI RslGetInstSymbolEx(const char *pName, const char *pModuleName);

DLMLINK bool RSLDLMAPI RslCallInstSymbol  (ISYMBOL *sym, int code, int nPar, VALUE *par, VALUE *propORret);
        bool           RslCallInstSymbolEx(ISYMBOL *sym, int code, int nPar, VALUE *par, VALUE *propORret, unsigned char *attr);

SYMPROC *RslGetPrevProc(PROCPROLOG *pr);

bool IsEqClass(SYMCLASS *sym, TGenObject *obj);
void AutoCreateObj(CALLSTRUCT *call, SYMPROC *sym, HRD inst);

SYMCLASS  *AddRslExtClass(const char *name, void *info, TProviderInfo *provider, bool canCreate, bool isCls, bool toUpper);

// -----------------------------------------------------------------------------
#ifdef RSL_PL_MS
    #define  RSL_FOPEN_MODE_READ    "rb"
    #define  RSL_FOPEN_MODE_WRITE   "wb"
    #define  RSL_FOPEN_MODE_APPEND  "ab"
    #define  RSL_NEWLINE            "\r\n"
#endif

void AddAltRubTostr(void);
void SetTxtDelim(const char *str, bool spaceDelimAsOther, bool someDelimAsOne);

RSLLINK void RSLAPI RslLogStack(void);

RSLLINK bool RSLAPI RslGetBuff(VALUE *val, char **buffPtr, int *size, char **name);

void FormatOutBuf(int w, int flag, int v_type, VALUE *val);

typedef void (*RslMakeSegProc)(void *);
int CalcNOfSegments(char *str, int len1, int len2, RslMakeSegProc proc, void *v);

DLMLINK int RSLDLMAPI ImportModule    (const char *name);
DLMLINK int RSLDLMAPI ImportModulePath(const char *name, const char *path);

RSLLINK void RSLAPI InitErrorsEx(HRD inst);

bool TryReallocOutBuff       (size_t newSize);
bool TryReallocOutBuffAndCopy(size_t newSize, size_t numToCopy);

// -----------------------------------------------------------------------------
// add A.Tokmashev
typedef struct
      {
       int    nseg;
       char  *pCur;
       char   segs[1];
      } WrapData;

void ColumnAddLine(char *line);
WrapData *CreateWrapData(char *str, int len);
void InsertWrap(char *buff, WrapData *wd, int w, int offs, int flag, int nseg);
int  InsertOut(char *buf, int offs, int flag);
void SwapStack(HRD inst);
int ExecFieldNew(ICLUST *fld, CALCVAL2 *cv, HRD inst);

void  ArrayReadValue(AINST *r, VALUE *v, int Id);
void  ArrayCopy(AINST *to, AINST *from);

BINST *ILOCAL GetBinstance(SYMFILE *f);
SINST *ILOCAL GetStrBuf   (SYMSTRUC *f);
AINST *ILOCAL GetArrayInst(SYMARRAY *f);
TINST *ILOCAL GetTxtInst  (SYMTXTFILE *f);
DINST *ILOCAL GetDbfInst  (SYMDBFFILE *f);

void  TxtWriteValue(SYMTXTFILE *sym, TINST *r, VALUE *v, int Id);
void  TxtReadValue (SYMTXTFILE *sym, TINST *r, VALUE *v, int Id);

int DLMAPIC  LocMsgBox (char *fmt, ...);

// -----------------------------------------------------------------------------
#ifndef BUILD_DLM
    #ifndef RSL_RMT_PROXY  // rsextt.d32 build
        #define  MsgBox  LocMsgBox
    #endif
#endif

void InitAltDB (HRD inst);
void FlushAltDB(HRD inst);
void DoneAltDB (HRD inst);

RSLLINK void RSLAPI ClearDBStruct(HRD inst);


// -----------------------------------------------------------------------------
#ifdef SQLBUILD
    //RSL_MAKE_HANDLE(BKMHANDLE);

    void AddBookMarkCls(void);
    TGenObject *RslCreateBookMarkCls(BKMHANDLE bkm);
    BKMHANDLE   RslGetBookMarkFrom(TGenObject *obj);
#endif

void AddRslTimerCls (void);

typedef int (RSLAPI *TWriteTagProc) (const char *tagName, const char *rslFile, const char *tagString, void *data);
RSLLINK bool RSLAPI BuildTags (const char *rslFile, TWriteTagProc procPtr, void *data);



/*
 * setlocale() is very time-consuming operation, so do it only on platforms
 * where it absolutely necessary.
 */
#ifdef RSL_PL_MS
    void RslSetLocale(int ct);
    void RslRestLocale(int ct);

    #ifdef USE_FDECIMAL   // Money version
        #define  SAVELOCALE_MN(k)
        #define  RESTLOCALE_MN(k)
    #else
        #define  SAVELOCALE_MN(k)  RslSetLocale(k)
        #define  RESTLOCALE_MN(k)  RslRestLocale(k)
    #endif

    #define  SAVELOCALE(k)  RslSetLocale(k)
    #define  RESTLOCALE(k)  RslRestLocale(k)

#else
    #define  SAVELOCALE(k)
    #define  RESTLOCALE(k)
    #define  SAVELOCALE_MN(k)
    #define  RESTLOCALE_MN(k)
#endif

int RslReadConfig (char *name, ProcKeyValue_f fun, void *userData);

// Used in rsexts.c
bool GetLocaleInfoRmt(TRslLocaleData *ptr);
long GetLangIdRmt(int id);

TGenObject *GetThisPtr(HRD inst);

bool RslIsEqClassImpl(const char *clsName, TGenObject *obj);

void PrintRefs(HRD inst, PROCPROLOG *st, TGenObject *obj);

// -----------------------------------------------------------------------------
#ifndef RSL_BUILD_DLL
    void ForceLinkRslStaticIniter();
#endif


#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif  //iexec_h

/* <-- EOF --> */