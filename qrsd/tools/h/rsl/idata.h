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
       int      (*iMsgFun)(IMESSAGE, void *, void *);   // ���� �㭪�� ᮮ�饭��
       void      *UserData;                             // ����뢠�� �� �।����� UserData
       CNTLIST    addModList;                           // ���᮪ TAddModule
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
       int             CurLevel;      // ����騩 �஢�� ���������� ��楤��
       int             ParserLevel;   // ��騩 �஢��� ���������� ��楤��
       int             OnlyDeclared;  // �᫨ == 1 - �� ����� ������ � �������.
       int             DebugInfo;     // �᫨ == 1 - ���������� �⫠��筠� ���.

       struct tagTRD  *inst;

       bool            setExpContext;
       SYMPROC        *topContext;
       bool            topContextOnly;

    // =======================================
    // For InitIstreamDataRt

       int (*iMsgFun)(IMESSAGE, void *, void *); // ���� �㭪�� ᮮ�饭��
       void           *UserData;                 // ���� ����� ���짮��⥫�

       bool            needCloseParser;

       int  (*old_next_ch)(void *UserData);
       void           *oldData;
       bool            oldUseBlockMem;
    // =======================================

       int  (*setFileEncode)(int enc, void *data);
      } ISTREAMDATA;

//------------  �⥪ ----------------------------------------------------
typedef struct tagProcProlog  PROCPROLOG;

// �����頥��� ���祭�� ����頥� ��। �஫����
struct tagProcProlog
     {
      PROCPROLOG      *dinlink;   // �������᪠� ��� � �।��饩 ��楤�ன
      PROCPROLOG      *statlink;  // ����᪠� ���
      CALLSTRUCT       call;      // We must use COPY of CALLSTRUCT here !!!!
      VALUE           *loc_base;  // ���� �������� ��६����� � ��ࠬ��஢
      VALUE           *ret;       // �����頥��� ���祭��
      STATEMENT       *cur;       // Statement �믮���訩�� �� �맮��

      TPROCREF        *replacePtrProc;
      TPROCREF        *replacePtr;
      TMethodChunkEx  *m;

      short int        level;     // �஢��� ���������� ������ ��楤���
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
    // � �� �᭮���� ������� RSL'�
    struct tagTRD
         {
          void                  *UserData;           // ���짮��⥫�᪨� ����� (�� STD_USERDATA)

       // iscaner.c
          void                  *scanerData;         // �������᪠� ������ ��� �㦤 ᪠���

       // isymbol
          CNTLIST                imodlist;           // �������� ᯨ᮪ ���㫥�
          CNTLIST                delayModList;       // �⫮����� ���㫨 (ᮤ�ঠ騥 ���⠢騪�� ��ꥪ⮢)

          CNTLIST                ProviderList;       // ���᮪ �஢����஢ (���⠢騪��) ��ꥪ⮢

          unsigned char          iModuleOpen;        // ��⠭����� � 1, �᫨ ����� �����-���� �����
          unsigned char          iParserOpen;        // ��⠭����� � 1, �᫨ RSL ��室���� � ���ﭨ� ࠧ���
          unsigned char          iExecOpen;          // ��⠭����� � 1, �᫨ RSL ��室���� � ���ﭨ� �믮������

          TRslOutProc            RslOutProcPtr;      // ��楤�� �뢮�� RSL
          int                    RslDefPrec;         // ��筮��� ��-㬮�砭�� ��� �஡��� �ᥫ
          STATEMENT             *CurStatement;       // ����騩 �믮��塞� ������ (�.�. NULL)

          VALUE                  TmpVal;             // �६����� ���祭��

          TRslMesProc            iMsgFun;            // ��楤�� ��ࠡ�⪨ ᮮ�饭��

          //ierror
          int                    iNerrors;           // ������⢮ �訡��
          CNTLIST                ErrList;            // ���᮪ �訡��
          ERRLISTELEM           *CurErr;             // ������ �訡��



          unsigned               RslCurrentPrintLine;  // ����� ⥪�饩 ��ப� �뢮�� (���� ��⠫��� ���᭠)

          ExitProcType           ExitProc;           // ��楤�� ��室�

          // iexec.c
          VALUE                 *RunStack;           // �⥪ �������� ��६�����,��ࠬ��஢, �஫���� �-��
                                                     // � 㪠��⥫� �� ����� ��� ���᫥��� ��ࠦ����
          int                    RunStackSize;       // ������ �⥪� (�.�. �⫨祭 �� RUNSTACKSIZE)

          struct tagProcProlog  *BP_ptr;             // �����⥫� ⥪�饣� ���� �⥪�
          VALUE                 *SP_ptr;             // �����⥫� �� ����� ᢮������ �祩�� �⥪�
          char                  *ioutbuf;

          FILE                  *ioutstream;         // �㤠 �뢮���� ���ଠ�� �� RSL (?)

          struct tagProcProlog  *BP_top;             // ���� ��᫥���� ���� �⥪�. � ���� ��稭�����
                                                     // �⪠� � ��砥 �訡��

          unsigned char          iPrepeared;         // [???] �ᯮ������ ⮫쪮 �� �६� �믮������
          int                    ExitFlag;           // �� 0 �᫨ �맢��� �㭪�� exit

          unsigned               fgOldCtrlWord;      // ���࠭�� ���� 䫠�� FPU

          // ibfile.c

       #ifndef NO_BTR
          struct tagBtDataBase  *mainDB;             // ������� ��-㬮�砭��
          CNTLIST                AltDbDic;           // ���᮪ ����ୠ⨢��� ᫮��३
          struct tagBtDataBase  *trnDB;              // Used for transaction prcessing (����⭮, �� � �࠭������, �� �⥫���-�� ���஡���⥩)
          unsigned               inTrnCount;         // ���稪 ����஢ �࠭���権 (�� ��襫, ��� ���樠���������)
          struct tagBtDataBase  *lastErrDB;          // ��᫥���� dbt-䠩�, �� ���஬ �ந��諠 �訡��
       #endif

          CNTLIST                RSLCatchList;       // ���᮪ ��ࠡ��稪�� TRY/CATCH
          bool                   RSLCatchInited;     // �ந��樠����஢�� ��� ��� ᯨ᮪ ��ࠡ��稪 (����� � ��� ���� �������)

          // iutil.c
          bool                   RslUseBlockMem;     // �뤥����� ����� �१ AllocBlockMem ��� �१ calloc.
                                                     // ���� �����७��, �� AllocBlockMem �� �祭� �㦭�.
       #ifdef DEBUG
          unsigned long          MemUsage;           // ��᫥������� ��୮�� �맮��� RslAlloc/RslFree (�⫮� ��祪 �����)
       #endif

          // imemory.c
          unsigned               PageSize;           // ������ ��࠭��� ����� (��-㬮�砭�� 4096) (�. AllocBlockMem)
          CNTLIST                memList;            // ���᮪ ��࠭�� �����

          #ifdef MEMCHECK
          int                    BlockMemInUse;      // �⫮� ��祪 ����� �� ���筮� �뤥�����
          #endif

          // iprout.c
          CNTLIST                colomn_list;        // ���᮪ ������� �� ⠡㫨஢����� �뢮��
          TColumnData           *curCol;             // ������ ������� (�.�. NULL)

          // itxtfile
          char                   delim[10];          // ���᮪ ࠧ����⥫�� ��� ⥪�⮢��� 䠩�� (��-㬮�砭�� �஡��, ⠡���� � ������)

          //rslobj.c
          CNTLIST                RslObjectList;      // ���᮪ ��ꥪ⮢ ���� TRslObject


          CNTLIST                AltRes;             // ���᮪ ����ୠ⨢��� 䠩��� ����ᮢ (lbr)
          HRESFILE               rslRes;             // ��䮫�� 䠩� ����ᮢ

       #ifndef NO_BTR
          TBtDataBase           *dummy;              // �����誠 ��� ���������� ࠡ��� � 䠩��� ����ᮢ ��� � ᫮��६
       #else
          void                  *dummy;
       #endif

          bool                   needDelete;         // �᢮������� ���⠭� �� �����⨨, ��� ���
          void                  *instData;           // ����� ���⠭� (�஬� NULL ��祣� �� ��襫)
          int                    jmper;              // �ᯮ������ � �離� � RSLCatchList

          TRslOutProc            oldOutProc;         // �ᯮ������ ��� SetOutHandler
          SYMPROC               *outSymProc;         // --//-

       // May 19,1998
          unsigned               NumUserErrHandler;  // �ᯮ������ ��� �뢮�� �訡��
          TGenObject            *errObject;          // ����ন� ��ꥪ� � ���ᠭ��� �訡��

          unsigned               refCount;           // ���稪 ��뫮� �� ���⠭� RSL

          int                    startingLine;       // �ᯮ������ ᪠��஬ (�. ⠪ �� startingOffs)

          bool                   useDeclared;        // ����� ��易⥫쭮� ������樨 ��६�����

       #ifdef USEDEBUGER
          int                    globalTrace;        // �㦭� ��� �⫠�稪� RSL
          int                    traceState;         // --//--

          struct tagProcProlog  *traceProc;          // --//--
          bool                   useDbg;             // --//--

          CNTLIST                expresion_list;     // List of TExpElem
          CNTLIST                valInfo_list;       //

          bool                   DbgMode;            // --//--
       #endif

          void                  *idisp;              // TRslAutoProg -> IDispatch (�� ��襫 祣�-����, �⫨筮�� �� NULL)

       #ifndef NO_BTR
          CNTLIST                BtrObjList;         // ���᮪ ��ꥪ⮢ ⨯� TBtrieveFile � TRecordHandler
       #endif

          CNTLIST                ArrayObjList;       // ���᮪ ��ꥪ⮢ TArrayObj

          CNTLIST                UniObjList;         // ���᮪ ��ꥪ⮢ TUniObj (�.�. ��ꥪ⮢ �� ᠬ��� RSL ��� DLM)
          CNTLIST                UniClsList;         // ���᮪ ����ᮢ TUniClass

          void                  *AutoObjList;        // [???] �� �ᯮ������ ?

          TRslFpuHandler         oldHandler;         // �।��騩 ��ࠡ��稪 �訡�� FPU

       #ifndef NO_BTR
          TBtDataBase           *BtrMacdb;           // ��� ࠡ��� � btrmac.ddf
          BF                    *BtrMacfl;           // --//--
          char                  *BtrMacName;         // --//--
       #endif

          // add by A.Tokmashev (T74) - 22.05.2000
       #ifndef RSL_NO_HASH
          HHASH                  m_globalHash;       // ����� ����� TRSLHash
       #endif // RSL_NO_HASH
          // end (T74) - 22.05.2000
          int                    ioutbufSize;        // ������ ���� �뢮��

       #ifdef USE_FDECIMAL
          FDecimal_Error_Proc    oldFdecHandler;     // [???] �� ��襫. ��� �ᯮ������
       #endif

          // add by A.Tokmashev (T74) - 13.06.2000
       #ifndef RSL_NO_PCODE
          HPCODE                 hPCode;             // �������� �४������樨
          bool                   isManualGen;        //
          char                  *ioutbufPCode;       // size (_MAX_PATH * 2)
       #endif // RSL_NO_PCODE

       #ifdef USEDEBUGER
          HDBG                   hdbg;               // �ᯮ������ ��� �㦤 �⫠�稪�
          SYMMODULE             *traceMod;           // --//--
       #endif

          ISTREAMDATA           *data;               // ��� �㦤� ��������

          unsigned               disableErrors;      // �����஢�� �뢮�� ᮮ�饭�� �� �訡��� (�� ����஢���� ����� � �.�.)

          void                  *objInstIntf;        // �����⥫� �� ����䥩� ���� TRSLInstIntf_impl
          bool                   privateMode;        // �ਢ��� ०�� ࠡ��� RSL

          const char            *findModuleName;     // ��� ⥪�饣� �����
          SYMMODULE             *activeModule;       // ����騩 �����

          bool                   useLoopInTrn;       // ����, ��⠭��������� �१ LoopInTrn

          // for special mode func PlayRepEx()
          bool                   accumulateModule;     // ����� MODE_ADDMODULE (?)
          bool                   accumulateBlockMem;   // ������� RslUseBlockMem

          CNTLIST                InstObjList;        // ���᮪ ��ꥪ⮢ TRslInstanceObj
          void                  *hostInstObj;        // �����⥫� �� TRslInstObjHost


          const char            *ToolsDef;           // ��䮫�� ᫮���� ��� ࠡ��� � dbt-䠩����
          const char            *ToolsDbt;           // ��⠫�� dbt-䠩���
          const char            *ToolsMac;           // ��⠫�� ����䠩���
          const char            *ToolsTxt;           // ��⠫�� ⥪�⮢�� 䠩���

       #ifdef DBASE
          const char            *ToolsDbf;           // ��⠫�� dbf-䠩���
       #endif
          const char            *ToolsLbr;           // ��䮫⭠� ������⥪� ����ᮢ
          const char            *ToolsRsm;           // ��⠫�� ��� rsm-䠩���

          bool                   GUIclient;          // ����஢�� ��ப ANSI/OEM

       #ifndef NO_BTR
          bclient_h              clientID;           // ������᪨� 奭�� (�� �� �����騩 - �����騩 � extClientID) ��� ࠡ��� � ����
       #else
          short                  clientID;
       #endif

          bool                   persistStr;         // ���ন���� ������ �������� (���ᠭ�� 䠩��� � ᫮���)
          char                  *oldLc;              // ����� �।��饩 ������

          bool                   useByRef;           // ��।�� ��ࠬ��஢ �� ��뫪� (�ᯮ������ � rcw)

          void                  *AxObjList;          // �����⥫� �� ��ꥪ� TAxInstData

          CNTLIST                clntDataList;       // ���᮪ ��ꥪ⮢ TClntData

          void                  *rcwHost;            // �����⥫� �� ����䥩� IRCW

          unsigned short         extendedFormats;    // ����७�� 䫠�� �ଠ�஢����

          CNTLIST                WkRefList;          // ���᮪ "᫠���" ��뫮�

          HTRACE                 hTrace;             // ����� �ࢨ� ����஢��

          bool                   needCheckBtrMac;    // [???] �஢�ઠ ����ᮢ � btrmac.ddf
          bool                   AutoMoneyFloor;     // ����� ���㣫���� ����� (SetAutoMoneyFloor � RSL)
          bool                   cacheActivexId;     // ���஢���� ��ꥪ⮢ ActiveX (CacheActivexIds �� RSL::rslx)

          int                    BtrMacError;        // ��� ࠡ��� � btrmac.ddf
          bool                   isSQL;              // ����� ࠡ��� ���⠭�
          bool                   NeedFreeDB;         // ������� ᫮��� ��᫥ �����襭�� ࠡ���

       #ifndef NO_BTR
          btrvem_h               hBtrvem;            // ����� ����� BTrieve
       #else
          short                  hBtrvem;
       #endif

       #ifndef NO_BTR
          BTClientID             extClientID;        // ����७�� clientID (������ ॠ��� 奭�� ������)
       #else
          char                   extClientID[16];
       #endif

          int                    startingOffs;       // �ᯮ������ ᪠��஬ (�. ⠪ �� startingLine)
          int                    errorThrowCode;     // ��� �᪫�祭�� (���� JMP_*)
          bool                   restrictMode;       // ����� � �१����� ०���

          bool                   moneyDecimal;       // ����� �������樨 �����: �᫨ true, � $1.00 == 1.00
          unsigned char          mixMode;            // 1 - decimal, 2 - numeric.

       #if defined (USE_NUMERIC) && !defined(VALUE_NUMERIC)
          Numeric_t             *freeNumPtr;         // ��-� �த� �६������ ���� ��� NUMERIC
       #endif

       #ifdef USE_DBGHANDLE32
          CNTLIST                srcList;            // ��� �㦤 �⫠�稪� RSL

          RslHandle32            keyModule;          // --//--
          RslHandle32            keyStmt;            // --//--
          RslHandle32            keyProc;            // --//--
          RslHandle32            keyExpctx;          // --//--
          RslHandle32            keyValue;           // --//--
          RslHandle32            keyVinfo;           // --//--
          RslHandle32            keySrc;             // --//--
       #endif

          SYMMODULE             *expTopModule;       // ���孨� �����

          int                    RslDefPrecMoney;    // ��筮��� ��� ����� (SetDefMoneyPrec � RSL)

          bool                   ctrlBreakPressed;           // �ਧ��� ������ Ctrl-Break
          bool                   ctrlBreakCallBackInstalled; // �ਧ��� ��⠭���� ��ࠡ��稪� Ctrl-Break

          bool                   showDictErrors;     // �����뢠�� �訡�� ᫮����
          bool                   brk2Dbg;            // ��뢠�� �⫠�稪 �� ����稨 ��ࠡ��稪� OnError ��� ���

       #ifndef NO_GC
          unsigned short         generation;         // Used to collect cyclic objects
          unsigned short         marknumber;         // Used to report references
       #endif

          TAddExecData          *curExecData;        // �������⥫�� ����� RSL � ०��� �믮������

       #ifndef NO_BTR
          char                  *extDbMessage;       // ����७��� ᮮ�饭�� �� ࠡ�� � ����
       #endif

          bool                   spaceDelimAsOther;  // ��� �㭪樨 SetDelim � RSL
          bool                   someDelimAsOne;     // ��� �㭪樨 SetDelim � RSL

          char                  *lastErrMessage;     // ��᫥���� ᮮ�饭�� �� �訡��
          int                    lastErrCode;        // ��᫥���� ��� �訡�� (IER_*)
          unsigned short         interactExpStatus;  // ����ন� ���祭�� ���� INTR_EXP_*

          bool                   disableCvtFuncs;    // ���� ࠡ���� � RSL �㭪樨 ToOEM/ToANSI ��� ���

          int                    modeTraceMessage;   // ����� ࠡ��� ����஢���� ᮮ�饭��
          int                    modeExistDbtFiles;  // ����� �஢�ન ����⢮����� dbt-䠩���
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

// ��⠭���� ०��� ��ࠢ�� ����஢���� ᮮ�饭��
RSLLINK int        RSLAPI    RslTraceMessageMode(int mode, HRD inst);

// ��⠭���� ०��� �஢�ન ������ dbt-䠩���
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