/*-----------------------------------------------------------------------*-

 File Name   : idata.h

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 6,1998  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __IDATA_H
#define __IDATA_H

// add by A.Tokmashev (T74) - 22.05.2000
#ifndef RSL_NO_HASH
   #ifndef __IRSLHASH_C_H
      #include "rsl/irhash_c.h"
   #endif // __IRSLHASH_C_H
#endif // RSL_NO_HASH
// end (T74) - 22.05.2000

RSL_MAKE_HANDLE(HRESFILE);

RSL_MAKE_HANDLE(HDBG);

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
typedef struct tagClntData
      {
       LOBJ     listElem;
       void    *data;
       char     name[1];
      } TClntData;

// -----------------------------------------------------------------------------
typedef struct tagAddExecData
      {
       int      (*iMsgFun)(IMESSAGE, void *, void *);   // Старая функция сообщений
       void      *UserData;                             // Указывает на предыдущую UserData
       CNTLIST    addModList;                           // Список TAddModule
       bool       oldUseBlockMem;
       bool       abortOnParserErrors;

       struct tagAddExecData  *prev;
      } TAddExecData;

// -----------------------------------------------------------------------------
typedef struct tagISTREAMDATA
      {
       CNTLIST         modCtxList;    // List of TAddModContextBase
       CNTLIST         std_list;      // List of STDTABLE


       CNTLIST        *scopeListPtr;  // Current SCOPE list
       int             CurLevel;      // Текущий уровынь вложенности процедур
       int             ParserLevel;   // Общий уровень вложенности процедур
       int             OnlyDeclared;  // Если == 1 - все имена должны явно декларир.
       int             DebugInfo;     // Если == 1 - добавляется отладочная инфо.

       struct tagTRD  *inst;

       bool            setExpContext;
       SYMPROC        *topContext;
       bool            topContextOnly;

    // =======================================
    // For InitIstreamDataRt

       int (*iMsgFun)(IMESSAGE, void *, void *); // Старая функция сообщений
       void           *UserData;                 // Старые данные пользователя

       bool            needCloseParser;

       int  (*old_next_ch)(void *UserData);
       void           *oldData;
       bool            oldUseBlockMem;
    // =======================================

       int  (*setFileEncode)(int enc, void *data);
      } ISTREAMDATA;

//------------  Стек ----------------------------------------------------
typedef struct tagProcProlog  PROCPROLOG;

// Возвращаемое значение помещаем перед прологом
struct tagProcProlog
     {
      PROCPROLOG      *dinlink;   // Динамическая связь с предыдущей процедурой
      PROCPROLOG      *statlink;  // Статическая связь
      CALLSTRUCT       call;      // We must use COPY of CALLSTRUCT here !!!!
      VALUE           *loc_base;  // База локальных переменных и параметров
      VALUE           *ret;       // Возвращаемое значение
      STATEMENT       *cur;       // Statement выполнявшийся до вызова

      TPROCREF        *replacePtrProc;
      TPROCREF        *replacePtr;
      TMethodChunkEx  *m;

      short int        level;     // Уровень вложенности данной процедуры
      short int        delta;

#ifdef USE_DBGHANDLE32
      RslHandle32      key;
#endif

      unsigned char    flags;

      uintptr_t        address;   // Address in C stack for this RSL stack frame. Need for Active Debugging.
     };

// -----------------------------------------------------------------------------
#ifndef NO_BTR
    typedef struct
          {
           TBtrStream  parent;
           char        keyName[25];
           BF         *fl;
          } TBMac;
#endif

// -----------------------------------------------------------------------------
// flags values
#define  RSL_PROLOG_METHOD          1
#define  RSL_PROLOG_CCALL           2
#define  RSL_PROLOG_INTERPRET       4

// add A.Tokmashev (15.06.2000)
#ifndef RSL_NO_PCODE
    #define  RSL_PROLOG_PCODE       8
#endif // RSL_NO_PCODE

#define  RSL_PROLOG_PCODE_CALL     16  // The call made from p-code to any proc

#define  RUNSTACKSIZE            2000


// -----------------------------------------------------------------------------
#ifndef RSL_USE_DLL

    #ifdef RSL_PL_MS
        typedef void (__cdecl *TFPEHandler)(int);

        typedef void (*TRslFpuHandler)(int, int);
    #else
       typedef void (*TFPEHandler)(int);
       typedef void (*TRslFpuHandler)(int);
    #endif

    // -----------------------------------------------------------------------------
    typedef struct
          {
           LOBJ       colomn_elem;
           short int  number;      // Ordinal colomn number
           short int  width;       // Max colomn width
           short int  nLines;      // Number of lines in this colom
           CNTLIST    lines;       // List of LINE_ELEM
          } TColumnData;

    // -----------------------------------------------------------------------------
    typedef struct
          {
           bool  dos;

           union
               {
                FILE  *f;
    #ifndef NO_BTR
                TBMac  bm;
    #endif
               } u;

    #ifdef USE_DBGHANDLE32
           LOBJ          listElem;
           RslHandle32   key;
    #endif
          } TSRC;

    // -----------------------------------------------------------------------------
    // А это основная структура RSL'я
    struct tagTRD
         {
          void                  *UserData;           // Пользовательские данные (из STD_USERDATA)

       // iscaner.c
          void                  *scanerData;         // Динамическая память для нужд сканера

       // isymbol
          CNTLIST                imodlist;           // Глобальный список модулей
          CNTLIST                delayModList;       // Отложенные модули (содержащие поставщиков объектов)

          CNTLIST                ProviderList;       // Список провайдеров (поставщиков) объектов

          unsigned char          iModuleOpen;        // Установлен в 1, если открыт какой-либо модуль
          unsigned char          iParserOpen;        // Установлен в 1, если RSL находится в состоянии разбора
          unsigned char          iExecOpen;          // Установлен в 1, если RSL находится в состоянии выполнения

          TRslOutProc            RslOutProcPtr;      // Процедура вывода RSL
          int                    RslDefPrec;         // Точность по-умолчанию для дробных чисел
          STATEMENT             *CurStatement;       // Текущий выполняемый оператор (м.б. NULL)

          VALUE                  TmpVal;             // Временное значение

          TRslMesProc            iMsgFun;            // Процедура обработки сообщений

          //ierror
          int                    iNerrors;           // Количество ошибок
          CNTLIST                ErrList;            // Список ошибок
          ERRLISTELEM           *CurErr;             // Текущая ошибка



          unsigned               RslCurrentPrintLine;  // Номер текущей строки вывода (суть осталась неясна)

          ExitProcType           ExitProc;           // Процедура выхода

          // iexec.c
          VALUE                 *RunStack;           // Стек локальных переменных,параметров, прологов ф-ий
                                                     // и указатели на данные для вычисления выражений
          int                    RunStackSize;       // Размер стека (м.б. отличен от RUNSTACKSIZE)

          struct tagProcProlog  *BP_ptr;             // Указатель текущего кадра стека
          VALUE                 *SP_ptr;             // Указатель на первую свободную ячейку стека
          char                  *ioutbuf;

          FILE                  *ioutstream;         // Куда выводить информацию из RSL (?)

          struct tagProcProlog  *BP_top;             // Самый последний кадр стека. С него начинается
                                                     // откат в случае ошибки

          unsigned char          iPrepeared;         // [???] Используется только во время выполнения
          int                    ExitFlag;           // Не 0 если вызвана функция exit

          unsigned               fgOldCtrlWord;      // Сохраняет старые флаги FPU

          // ibfile.c

       #ifndef NO_BTR
          struct tagBtDataBase  *mainDB;             // Словарь по-умолчанию
          CNTLIST                AltDbDic;           // Список альтернативных словарей
          struct tagBtDataBase  *trnDB;              // Used for transaction prcessing (понятно, что в транзакциях, но хотелось-бы подробностей)
          unsigned               inTrnCount;         // Счетчик повторов транзакций (не нашел, где инициализируется)
          struct tagBtDataBase  *lastErrDB;          // Последний dbt-файл, на котором произошла ошибка
       #endif

          CNTLIST                RSLCatchList;       // Список обработчиков TRY/CATCH
          bool                   RSLCatchInited;     // Проинициализирован или нет список обработчик (можно и без него обойтись)

          // iutil.c
          bool                   RslUseBlockMem;     // Выделение памяти через AllocBlockMem или через calloc.
                                                     // Есть подозрение, что AllocBlockMem не очень нужно.
       #ifdef DEBUG
          unsigned long          MemUsage;           // Отслеживание парности вызовов RslAlloc/RslFree (отлов утечек памяти)
       #endif

          // imemory.c
          unsigned               PageSize;           // Размер страницы памяти (по-умолчанию 4096) (см. AllocBlockMem)
          CNTLIST                memList;            // Список страниц памяти

          #ifdef MEMCHECK
          int                    BlockMemInUse;      // Отлов утечек памяти при блочном выделении
          #endif

          // iprout.c
          CNTLIST                colomn_list;        // Список колонок при табулированном выводе
          TColumnData           *curCol;             // Текущая колонка (м.б. NULL)

          // itxtfile
          char                   delim[10];          // Список разделителей для текстового файла (по-умолчанию пробел, табуляция и запятая)

          //rslobj.c
          CNTLIST                RslObjectList;      // Список объектов вида TRslObject


          CNTLIST                AltRes;             // Список альтернативных файлов ресурсов (lbr)
          HRESFILE               rslRes;             // Дефолтный файл ресурсов

       #ifndef NO_BTR
          TBtDataBase           *dummy;              // Пустышка для возможности работы с файлом ресурсов как со словарем
       #else
          void                  *dummy;
       #endif

          bool                   needDelete;         // Освобождать инстанс при закрытии, или нет
          void                  *instData;           // Данные инстанса (кроме NULL ничего не нашел)
          int                    jmper;              // Используется в связке с RSLCatchList

          TRslOutProc            oldOutProc;         // Используется для SetOutHandler
          SYMPROC               *outSymProc;         // --//-

       // May 19,1998
          unsigned               NumUserErrHandler;  // Используется для вывода ошибок
          TGenObject            *errObject;          // Содержит объект с описанием ошибки

          unsigned               refCount;           // Счетчик ссылок на инстанс RSL

          int                    startingLine;       // Используется сканером (см. так же startingOffs)

          bool                   useDeclared;        // Режим обязательной декларации переменных

       #ifdef USEDEBUGER
          int                    globalTrace;        // Нужно для отладчика RSL
          int                    traceState;         // --//--

          struct tagProcProlog  *traceProc;          // --//--
          bool                   useDbg;             // --//--

          CNTLIST                expresion_list;     // List of TExpElem
          CNTLIST                valInfo_list;       //

          bool                   DbgMode;            // --//--
       #endif

          void                  *idisp;              // TRslAutoProg -> IDispatch (не нашел чего-либо, отличного от NULL)

       #ifndef NO_BTR
          CNTLIST                BtrObjList;         // Список объектов типа TBtrieveFile и TRecordHandler
       #endif

          CNTLIST                ArrayObjList;       // Список объектов TArrayObj

          CNTLIST                UniObjList;         // Список объектов TUniObj (т.е. объектов из самого RSL или DLM)
          CNTLIST                UniClsList;         // Список классов TUniClass

          void                  *AutoObjList;        // [???] Не используется ?

          TRslFpuHandler         oldHandler;         // Предыдущий обработчик ошибок FPU

       #ifndef NO_BTR
          TBtDataBase           *BtrMacdb;           // Для работы с btrmac.ddf
          BF                    *BtrMacfl;           // --//--
          char                  *BtrMacName;         // --//--
       #endif

          // add by A.Tokmashev (T74) - 22.05.2000
       #ifndef RSL_NO_HASH
          HHASH                  m_globalHash;       // Хендл класса TRSLHash
       #endif // RSL_NO_HASH
          // end (T74) - 22.05.2000
          int                    ioutbufSize;        // Размер буфера вывода

       #ifdef USE_FDECIMAL
          FDecimal_Error_Proc    oldFdecHandler;     // [???] Не нашел. где используется
       #endif

          // add by A.Tokmashev (T74) - 13.06.2000
       #ifndef RSL_NO_PCODE
          HPCODE                 hPCode;             // Менеджер прекомпиляции
          bool                   isManualGen;        //
          char                  *ioutbufPCode;       // size (_MAX_PATH * 2)
       #endif // RSL_NO_PCODE

       #ifdef USEDEBUGER
          HDBG                   hdbg;               // Используется для нужд отладчика
          SYMMODULE             *traceMod;           // --//--
       #endif

          ISTREAMDATA           *data;               // Под нужды интерпретатора

          unsigned               disableErrors;      // Блокировка вывода сообщений об ошибках (при копировании полей и т.п.)

          void                  *objInstIntf;        // Указатель на интерфейс вида TRSLInstIntf_impl
          bool                   privateMode;        // Приватный режим работы RSL

          const char            *findModuleName;     // Имя текущего модуля
          SYMMODULE             *activeModule;       // Текущий модуль

          bool                   useLoopInTrn;       // Флаг, устанавливаемый через LoopInTrn

          // for special mode func PlayRepEx()
          bool                   accumulateModule;     // Режим MODE_ADDMODULE (?)
          bool                   accumulateBlockMem;   // Двойник RslUseBlockMem

          CNTLIST                InstObjList;        // Список объектов TRslInstanceObj
          void                  *hostInstObj;        // Указатель на TRslInstObjHost


          const char            *ToolsDef;           // Дефолтный словарь для работы с dbt-файлами
          const char            *ToolsDbt;           // Каталог dbt-файлов
          const char            *ToolsMac;           // Каталог макрофайлов
          const char            *ToolsTxt;           // Каталог текстовых файлов

       #ifdef DBASE
          const char            *ToolsDbf;           // Каталог dbf-файлов
       #endif
          const char            *ToolsLbr;           // Дефолтная библиотека ресурсов
          const char            *ToolsRsm;           // Каталог для rsm-файлов

          bool                   GUIclient;          // Кодировка строк ANSI/OEM

       #ifndef NO_BTR
          bclient_h              clientID;           // Клиентский хендл (но не настоящий - настоящий в extClientID) для работы с СУБД
       #else
          short                  clientID;
       #endif

          bool                   persistStr;         // Удерживать открытые структуры (описания файлов в словаре)
          char                  *oldLc;              // Данные предыдущей локали

          bool                   useByRef;           // Передача параметров по ссылке (используется в rcw)

          void                  *AxObjList;          // Указатель на объект TAxInstData

          CNTLIST                clntDataList;       // Список объектов TClntData

          void                  *rcwHost;            // Указатель на интерфейс IRCW

          unsigned short         extendedFormats;    // Расширенные флаги форматирования

          CNTLIST                WkRefList;          // Список "слабых" ссылок

          HTRACE                 hTrace;             // Хендл сервиса трассировки

          bool                   needCheckBtrMac;    // [???] Проверка макросов в btrmac.ddf
          bool                   AutoMoneyFloor;     // Режим округления денег (SetAutoMoneyFloor в RSL)
          bool                   cacheActivexId;     // Кеширование объектов ActiveX (CacheActivexIds из RSL::rslx)

          int                    BtrMacError;        // Для работы с btrmac.ddf
          bool                   isSQL;              // Режим работы инстанса
          bool                   NeedFreeDB;         // Отпустить словари после завершения работы

       #ifndef NO_BTR
          btrvem_h               hBtrvem;            // Хендл эмулятора BTrieve
       #else
          short                  hBtrvem;
       #endif

       #ifndef NO_BTR
          BTClientID             extClientID;        // Расширение clientID (задает реальный хендл клиента)
       #else
          char                   extClientID[16];
       #endif

          int                    startingOffs;       // Используется сканером (см. так же startingLine)
          int                    errorThrowCode;     // Код исключения (вида JMP_*)
          bool                   restrictMode;       // Работа в урезанном режиме

          bool                   moneyDecimal;       // Режим конвертации денег: если true, то $1.00 == 1.00
          unsigned char          mixMode;            // 1 - decimal, 2 - numeric.

       #if defined (USE_NUMERIC) && !defined(VALUE_NUMERIC)
          Numeric_t             *freeNumPtr;         // Что-то вроде временного буфера для NUMERIC
       #endif

       #ifdef USE_DBGHANDLE32
          CNTLIST                srcList;            // Для нужд отладчика RSL

          RslHandle32            keyModule;          // --//--
          RslHandle32            keyStmt;            // --//--
          RslHandle32            keyProc;            // --//--
          RslHandle32            keyExpctx;          // --//--
          RslHandle32            keyValue;           // --//--
          RslHandle32            keyVinfo;           // --//--
          RslHandle32            keySrc;             // --//--
       #endif

          SYMMODULE             *expTopModule;       // Верхний модуль

          int                    RslDefPrecMoney;    // Точность для денег (SetDefMoneyPrec в RSL)

          bool                   ctrlBreakPressed;           // Признак нажатия Ctrl-Break
          bool                   ctrlBreakCallBackInstalled; // Признак установки обработчика Ctrl-Break

          bool                   showDictErrors;     // Показывать ошибки словаря
          bool                   brk2Dbg;            // Вызывать отладчик при наличии обработчика OnError или нет

       #ifndef NO_GC
          unsigned short         generation;         // Used to collect cyclic objects
          unsigned short         marknumber;         // Used to report references
       #endif

          TAddExecData          *curExecData;        // Дополнительные данные RSL в режиме выполнения

       #ifndef NO_BTR
          char                  *extDbMessage;       // Расширенное сообщение при работе с СУБД
       #endif

          bool                   spaceDelimAsOther;  // Для функции SetDelim в RSL
          bool                   someDelimAsOne;     // Для функции SetDelim в RSL

          char                  *lastErrMessage;     // Последнее сообщение об ошибке
          int                    lastErrCode;        // Последний код ошибки (IER_*)
          unsigned short         interactExpStatus;  // Содержит значение вида INTR_EXP_*

          bool                   disableCvtFuncs;    // Будут работать в RSL функции ToOEM/ToANSI или нет

          int                    modeTraceMessage;   // Режим работы трассировочных сообщений
          int                    modeExistDbtFiles;  // Режим проверки существования dbt-файлов
         }; //  TRD, *HRD;


    // -----------------------------------------------------------------------------
    #ifdef RSL_MD_MT
        #define  RslInst ((TRD *)RslGetThreadInst())

        #define  RSL_LOCK(sect)   RslEnterCritSect(sect)
        #define  RSL_UNLOCK(sect) RslLeaveCritSect(sect)
    #else
        extern TRD  *RslInst;

        #define RSL_LOCK(sect)
        #define RSL_UNLOCK(sect)
    #endif

    extern char  *iRslDic;
    extern char  *iRSLDataDir;
    extern char  *iRSLTxtDir;
    extern char  *IncPath;
    extern char  *resFileName;
    extern char  *PCodePath;

    #ifdef DBASE
        extern char  *iRSLDbfDir;
    #endif

    extern void  *UserData;

    #ifndef BUILD_DLM
    # define InstBuf RslInst->ioutbuf
    #endif
#else
    #ifndef BUILD_DLM
        #define InstBuf RslOutBuf ()
    #endif
#endif

// -----------------------------------------------------------------------------
RSLLINK void *RSLAPI RslSetInstPtr(HRD inst, void *instData);
bool IsModuleOpen(HRD inst);
bool IsModuleExec(HRD inst);
unsigned int RslGetRefCount(HRD inst);
RSLLINK unsigned int RSLAPI RslSetCurrentPrintLine(unsigned nLine); // for conintf.hpp
CNTLIST *RslGetModList(HRD inst);
RSLLINK bool RSLAPI RslSetPrivateMode(bool mode);  // for conintf.hpp
RSLLINK const char *RSLAPI RslSetCurModuleName(const char *nameModule); // for conintf.hpp

#ifndef RSL_NO_PCODE
    RSLLINK void   RSLAPI RslSetPCodeData(int genPCode, bool forEach, bool isGenMode); // for conintf.hpp
    RSLLINK HPCODE RSLAPI RslGetHPCode(void); // for conintf.hpp
#endif // RSL_NO_PCODE

void  RslSetObjInstIntf(HRD inst, void *data);
void *RslGetObjInstIntf(HRD inst);
void *RslGetObjInstIntf2(void);

RSLLINK HRD        RSLAPI    RslAllocInstance(void *instData);
RSLLINK void       RSLAPI    RslFreeInstance(HRD inst);
RSLLINK void      *RSLAPI    RslInstPtr(HRD inst);

// Установка режима отправки трассировочных сообщений
RSLLINK int        RSLAPI    RslTraceMessageMode(int mode, HRD inst);

// Установка режима проверки наличия dbt-файлов
RSLLINK int        RSLAPI    RslExistDbtFileMode(int mode, HRD inst);

RSLLINK unsigned   RSLAPI    RslSetRestrictMode(HRD inst, unsigned mode);

DLMLINK HRD        RSLDLMAPI RslGetInstance(void);
DLMLINK HRD        RSLDLMAPI RslSetInstance(HRD newInst);
RSLLINK bool       RSLAPI    CheckInstance(void);
RSLLINK void       RSLAPI    FreeAutoInstance(void);
RSLLINK void      *RSLAPI    RslInstPtr2(void);

DLMLINK int        RSLDLMAPI RslSendMes(IMESSAGE mes, void *ptr);
RSLLINK char      *RSLAPI    RslOutBuf(void);

RSLLINK void       RSLAPI    RslOnlyDeclMode(void);


DLMLINK unsigned   RSLDLMAPI RslLockInstance(HRD inst);
DLMLINK unsigned   RSLDLMAPI RslUnLockInstance(HRD inst);
DLMLINK int        RSLDLMAPI RslInstGetNumModule(HRD inst);

RSLLINK void       RSLAPI    RslStartLine(int startingLine);
RSLLINK void       RSLAPI    RslParseStartPos(int startingLine, int offs);

RSLLINK BACKENTRY *RSLAPI    RslGetBackEntryTable(void);
RSLLINK bool       RSLAPI    RslAccumulateMode(HRD inst, bool fBeginEnd); // true -> begin, false -> end

RSLLINK void      *RSLAPI    SobjNextItemSys(SOBJ *sobj);
RSLLINK void       RSLAPI    SobjUnlinkSys  (SOBJ *obj);
RSLLINK void       RSLAPI    SobjInitSys    (SOBJ *obj);


// -----------------------------------------------------------------------------
// March 28,1996
// Btrieve stream
bool ConnectBtrMacro   (HRD dt);
void DisconnectBtrMacro(HRD dt);

void SetErrHandler(bool set);

void ConnectModule(SYMMODULE *sym);
void MakeNewModule(HRD inst, const char *name, SYMMODULE **mod, unsigned char std, const char *file1, const char *file2);
bool BuildStdModule(HRD inst, SYMMODULE **mod, const char *modname, RslStdProc proc, RslStdProc init,
                    RslStdProc done, bool *newFlag, const char *file);

bool         InitIstreamData   (HRD inst, ISTREAMDATA *data);
void         DoneIstreamData   (HRD inst, ISTREAMDATA *data);
bool         ParseStdModules   (HRD inst);
TRslMesProc  RslSetMsgProc     (HRD inst, TRslMesProc uMsgProc);
void        *RslSetHostInstance(void *host);
void        *RslGetHostInstance(void);


RSLLINK unsigned long RSLAPI RslGetStack(HRD inst);

void InitRslBtrClient(HRD inst);
void DoneRslBtrClient(HRD inst);

void OpenRslBtrClient(HRD inst, bool useMacDef);
void CloseBtrClient  (HRD inst);

#ifdef RSL_BUILD_DLL
    RSLLINK char *RSLAPI BfileSetDictionary(char *name);
    RSLLINK char *RSLAPI BfileSetDataDir   (char *dir);
    RSLLINK char *RSLAPI SetIncDir         (char *dirlist);
    RSLLINK char *RSLAPI SetTextDir        (char *dir);
    RSLLINK char *RSLAPI SetDbfDir         (char *dir);
    RSLLINK char *RSLAPI SetRslResFile     (char *fileName);
#endif

RSLLINK void RSLAPI InitRubToStr(const char *cnfFile);

void RslRestLocale(int ct);

#ifndef NO_BTR
    RSLLINK int RSLAPI RslGetBtID(BTClientID *id);
#endif

RSLLINK unsigned RSLAPI RslGetVersion(void);

void RegisterRCW(ISTREAMDATA *dt);
void *GetRCWhost(void);
void InitRCWhost(HRD);
void DoneRCWhost(HRD);
void ClearFantomsOnRCWhost(HRD);

void ClearNumeric(HRD inst);

void RslCatchInit2(TCatchData *dt, HRD inst);
void RslCatchProcess2(TCatchData *dt, HRD inst);

ISYMBOL *FindSymbolProcInternal(const char *name, SYMPROC *sym, bool onlyClass, const char *nameModule, HRD inst);

void CloseTrnDataBase(HRD inst);

#ifdef __cplusplus
}
#endif

#endif // __IDATA_H

/* <-- EOF --> */