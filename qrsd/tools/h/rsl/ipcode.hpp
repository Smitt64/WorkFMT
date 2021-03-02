/*************************************************************************
        File Name               : ipcode.hpp                    25.05.2000
        Programmer              : A.Tokmashev

        Subsystem               : Interpreter RSL
        Description             : Generator P-code.
        Source                  : ipcode.cpp

        Library(s)              : ---

        Copyright(C) 1991 - 2000 by R-Style Software Lab.
        All Rights Reserved.

*************************************************************************/

#ifndef __IPCODE_HPP
#define __IPCODE_HPP

#ifndef RSL_NO_PCODE

#include <rsassert.h>

#include <packpsh1.h>

//////////////////////////////////////////////////////////////////////////
// Begin RSL_PCODE2
//////////////////////////////////////////////////////////////////////////

#ifdef RSL_PCODE2

//////////////////////////////////////////////////////////////////////////
// PCode Items
//////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
typedef struct
      {
       int8               type;
      } ITEM_HEADER;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_ISYMBOL
      {
       ITEM_HEADER        hdr;
       int16              name;   // table of names
       int8               s_type; // SYMTYPE
       int16              v_attr;
       int16              level;
       int16              module; // table of names
      } PCODE_ISYMBOL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMVAR
      {
       PCODE_ISYMBOL      derived;

       int8               v_type; // VALTYPE
       int16              sym;    // SYMCLASS *
      } PCODE_SYMVAR;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_VALUE
      {
       int8               v_type;

       union
           {
            int8          value[10]; // sizeof(VALDATA)
            int16         name;
           } u;
      } PCODE_VALUE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMGLOBAL
      {
       PCODE_SYMVAR       derived;
      } PCODE_SYMGLOBAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMLOCAL
      {
       PCODE_SYMVAR       derived;

       int16              offset;
      } PCODE_SYMLOCAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMPROP
      {
       PCODE_SYMLOCAL     derived;
      } PCODE_SYMPROP;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_PROCDEF
      {
       int16              local;    // list ISYMBOL *
       int16              parm;     // list ISYMBOL *
       int8               v_type;   // VALTYPE
       int16              sym;      // SYMCLASS *
       int16              stmtlist; // PCODE_STATEMENT
      } PCODE_PROCDEF;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMPROC
      {
       PCODE_ISYMBOL      derived;

       PCODE_PROCDEF      proc;
       int16              sym;      // SYMPROC *
       int16              stmtlist; // list STATEMENT
       int8               stEnd;    // STATEMENT *
      } PCODE_SYMPROC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMCLASS
      {
       PCODE_SYMPROC      derived;

       int16              parent; // list of parents
      } PCODE_SYMCLASS;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMMETHOD
      {
       PCODE_SYMPROC      derived;

       int8               override;
       int16              offset; // offset to TMethodChunk
      } PCODE_SYMMETHOD;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMINITCLASS
      {
       PCODE_ISYMBOL      derived;

       int16              classPtr; // SYMCLASS pointer
      } PCODE_SYMINITCLASS;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMMODULE
      {
       PCODE_SYMPROC      sym;
       int16              constant; // list SYMGLOBAL
       int16              global;   // list ISYMBOL
       int16              file;     // table of names
      } PCODE_SYMMODULE;

#ifndef NO_BTR
// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMSTRUC
      {
       PCODE_ISYMBOL      derived;

       int16              dicname;
       int16              offset;
       int8               isDialog;
       int16              altDic;
      } PCODE_SYMSTRUC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMFILE
      {
       PCODE_SYMSTRUC     derived;

       int8               mode;
       int8               path;
       uint8              useBlob;
  } PCODE_SYMFILE;
#endif // NO_BTR

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMTXTFILE
      {
       PCODE_ISYMBOL      derived;

       int16              offset;
       int8               mode;
       int16              recSize;
       int16              file;
      } PCODE_SYMTXTFILE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMDBFFILE
      {
       PCODE_ISYMBOL      derived;

       int16              offset;
       int8               write;
       int16              altDic;
       int16              file;
      } PCODE_SYMDBFFILE;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_SYMARRAY
      {
       PCODE_ISYMBOL      derived;

       int16              offset;
      } PCODE_SYMARRAY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPLISTELEM
      {
       ITEM_HEADER        hdr;
       int8               exp; // EXPRESSION *
       int16              w;
       int8               p;
       int16              flag;
      } PCODE_EXPLISTELEM;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STATEMENT
      {
       ITEM_HEADER        hdr;
       int8               st_type;
       int16              line;
       int16              sym; // SYMPROC *

       int32              lastStmtOffs;
       int32              offsBegin;
       int32              offsEnd;
      } PCODE_STATEMENT;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STEXPRESSION
      {
       PCODE_STATEMENT    derived;

       int8               exp; // EXPRESSION *
      } PCODE_STEXPRESSION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STLOOP
      {
       PCODE_STATEMENT    derived;

       int8               st; // CONDITION *
      } PCODE_STLOOP;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STFOR
      {
       PCODE_STATEMENT    derived;

       int8               initExp;
       int8               termExp;
       int8               deltaExp;
       int8               loopVar;
       int16              stmtlist; // list STATEMENT
      } PCODE_STFOR;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STCONDITION
      {
       PCODE_STATEMENT    derived;

       int16              iflist; // list CONDITION
      } PCODE_STCONDITION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STRETURN
      {
       PCODE_STATEMENT    derived;

       int8               ret; // EXPRESSION *
      } PCODE_STRETURN;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STOUT
      {
       PCODE_STATEMENT    derived;

       int16              linelist; // list LINE_ELEM
       int16              parmlist; // list EXPLISTELEM
      } PCODE_STOUT;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STSPEC
      {
       PCODE_STATEMENT    derived;

       int16              explist; // list EXPLISTSPEC
      } PCODE_STSPEC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_STENDPROC
      {
       PCODE_STATEMENT    derived;
      } PCODE_STENDPROC;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_CONDITION
      {
       PCODE_STATEMENT    derived;

       int8               cond;     // EXPRESSION *
       int16              stmtlist; // list STATEMENT
      } PCODE_CONDITION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPRESSION
      {
       ITEM_HEADER        hdr;
       int8               operation;
       int8               exp_type;
       int16              level;
      } PCODE_EXPRESSION;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPBINARY
      {
       PCODE_EXPRESSION   derived;
      } PCODE_EXPBINARY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPUNARY
      {
       PCODE_EXPRESSION   derived;
      } PCODE_EXPUNARY;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_ICLUST
      {
       ITEM_HEADER        hdr;
       int8               cl_type;
      } PCODE_ICLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_NAMECLUST
      {
       PCODE_ICLUST       derived;

       int16              name; // char *
      } PCODE_NAMECLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_NUMCLUST
      {
       PCODE_ICLUST       derived;

       PCODE_EXPLISTELEM  elem;
       int16              num;
      } PCODE_NUMCLUST;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_LINEELEM
      {
       ITEM_HEADER        hdr;
       int16              line; // char *
      } PCODE_LINEELEM;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPVAL
      {
       PCODE_EXPRESSION   derived;

       int16              sym;      // ISYMBOL *
       int16              fld_list; // ICLUST *
      } PCODE_EXPVAL;

// -----------------------------------------------------------------------------
typedef struct tagPCODE_EXPSPEC
      {
       ITEM_HEADER        hdr;
       int8               exp; // EXPRESSION *
       int16              sym; // SYMGLOBAL *
      } PCODE_EXPLISTSPEC;

//////////////////////////////////////////////////////////////////////////

typedef union
      {
       PCODE_SYMGLOBAL       pcodeVGlobal;
       PCODE_VALUE           pcodeValue;
       PCODE_SYMLOCAL        pcodeVLocal;
       PCODE_SYMMODULE       pcodeModule;
       PCODE_SYMPROC         pcodeProc;
       PCODE_SYMARRAY        pcodeArray;
       PCODE_SYMTXTFILE      pcodeTxtFile;
       PCODE_SYMCLASS        pcodeClass;
       PCODE_SYMMETHOD       pcodeMethod;
       PCODE_SYMPROP         pcodeProp;
       PCODE_SYMINITCLASS    pcodeInitClass;
#ifdef DBASE
       PCODE_SYMDBFFILE      pcodeDbfFile;
#endif

#ifndef NO_BTR
       PCODE_SYMFILE         pcodeFile;
       PCODE_SYMSTRUC        pcodeStruc;
#endif
       PCODE_STEXPRESSION    pcodeStExp;
       PCODE_STLOOP          pcodeStLoop;
       PCODE_STCONDITION     pcodeStCond;
       PCODE_STRETURN        pcodeStReturn;
       PCODE_STOUT           pcodeStOut;
       PCODE_STSPEC          pcodeStSpec;
       PCODE_STENDPROC       pcodeStEndProc;
       PCODE_CONDITION       pcodeCondition;
       PCODE_EXPBINARY       pcodeExpBinary;
       PCODE_EXPUNARY        pcodeExpUnary;
       PCODE_EXPVAL          pcodeExpVal;
       PCODE_NAMECLUST       pcodeNameClust;
       PCODE_NUMCLUST        pcodeNumClust;
       PCODE_LINEELEM        pcodeLineElem;
       PCODE_EXPLISTELEM     pcodeExpElem;
       PCODE_EXPLISTSPEC     pcodeExpSpec;

       PCODE_STFOR           pcodeStFor;
       PCODE_STATEMENT       pcodeStBreak;
       PCODE_STATEMENT       pcodeStContinue;
      } ALL_PCODE_ITEM;

//////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
enum
   {
    PCODE_TYPE_STEXPRESSION, PCODE_TYPE_STLOOP,      PCODE_TYPE_STCONDITION,
    PCODE_TYPE_STRETURN,     PCODE_TYPE_STOUT,       PCODE_TYPE_STSPEC,
    PCODE_TYPE_STENDPROC,    PCODE_TYPE_CONDITION,

    PCODE_TYPE_EXPBINARY,    PCODE_TYPE_EXPUNARY,    PCODE_TYPE_EXPVAL,

    PCODE_TYPE_NAMECLUST,    PCODE_TYPE_NUMCLUST,

    PCODE_TYPE_LINEELEM,     PCODE_TYPE_EXPLISTELEM, PCODE_TYPE_EXPLISTSPEC,

    PCODE_TYPE_VGLOBAL,      PCODE_TYPE_VLOCAL,      PCODE_TYPE_MODULE,    PCODE_TYPE_PROC,
    PCODE_TYPE_ARRAY,        PCODE_TYPE_TXTFILE,     PCODE_TYPE_CLASS,     PCODE_TYPE_METHOD,
    PCODE_TYPE_PROP,         PCODE_TYPE_INITCLASS,   PCODE_TYPE_DBFFILE,   PCODE_TYPE_FILE,
    PCODE_TYPE_STRUC,        PCODE_TYPE_STFOR,       PCODE_TYPE_STBREAK,   PCODE_TYPE_STCONTINUE
   };

// -----------------------------------------------------------------------------
enum
   {
    IMPORT_GLOBAL = 1,
    IMPORT_PROP,
    IMPORT_EXTCLASS
   };

// -----------------------------------------------------------------------------
enum
    {
     VERSION_MAJOR = 1,
     VERSION_MINOR = 4
    };

// -----------------------------------------------------------------------------
typedef struct
      {
       uint32       signature;
       int8         verMajor;
       int8         verMinor;

       int32        countItems;
       int16        countTableOfSymbols;
       int16        countTableOfNames;    // Размер таблицы имен (число элементов массива)
       int32        sizeTableOfNames;     // Размер таблицы имен (в байтах)
       int16        countTableOfModule;
       int16        countTableOfImport;
       int32        sizePCode;
       int32        sizeAllItems;         // for interpretator RSL

       int32        offsTableOfNames;     // Смещение до таблицы имен
       int32        offsTableOfModule;
       int32        offsTableOfImport;
       int32        offsPCode;
      } PCODE_HEADER_FILE;

// -----------------------------------------------------------------------------
typedef struct
      {
       int16        num;
       int8         flag;
       int16        name;
       int16        owner;
      } IMPORT_DATA;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         listElem;

       ISYMBOL     *sym;
      } SYMBOL_ELEM;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         listElem;

       char        *name;
       char        *name_origin;
      } NAME_ELEM;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         listElem;

       IMPORT_DATA  data;
      } IMPORT_ELEM;

// -----------------------------------------------------------------------------
typedef struct
      {
       LOBJ         listElem;

       const char  *module;
       int16        line;
       int16        offs;
      } MODULE_ELEM;

// -----------------------------------------------------------------------------
class TPCodeManager;
class TPCodeTree;

// -----------------------------------------------------------------------------
typedef struct
      {
       TPCodeTree      *tree;
       ALL_PCODE_ITEM  *allItem;
      } SAVE_DATA;

// -----------------------------------------------------------------------------
class TPCodeTree
{
 public:
   TPCodeTree() {};
  ~TPCodeTree() {};

   friend class TPCodeManager;
   void SetManager(TPCodeManager *mgr)
   {
    manager = mgr;
   }

 protected:
   void Init  (bool genMode);
   void UnInit(bool genMode);

   /////////////////////////////////
   // method for SavePCode
   /////////////////////////////////
   void SaveListSymbols         (CNTLIST          *list,      ALL_PCODE_ITEM *allItem, bool fDirect);
   void SaveListSymbolsVarProc  (CNTLIST          *list,      ALL_PCODE_ITEM *allItem, bool fDirect, bool varOrProc);
   void SaveListStatements      (CNTLIST          *list,      ALL_PCODE_ITEM *allItem);
   void SaveListClusts          (CNTLIST          *list,      ALL_PCODE_ITEM *allItem);
   void SaveListLineElem        (CNTLIST          *list,      ALL_PCODE_ITEM *allItem);
   void SaveListExpElem         (CNTLIST          *list,      ALL_PCODE_ITEM *allItem);
   void SaveListExpSpec         (CNTLIST          *list,      ALL_PCODE_ITEM *allItem);

   void SaveSymbol              (ISYMBOL          *sym,       ALL_PCODE_ITEM *allItem);
   void SaveStatement           (STATEMENT        *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveExpression          (EXPRESSION       *exp,       ALL_PCODE_ITEM *allItem);
   void SaveClust               (ICLUST           *clst,      ALL_PCODE_ITEM *allItem);

   void SaveStmtExp             (STEXPRESSION     *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtLoop            (STLOOP           *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtFor             (STFOR            *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtBreak           (STATEMENT        *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtContinue        (STATEMENT        *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtCond            (STCONDITION      *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtReturn          (STRETURN         *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtOut             (STOUT            *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtSpec            (STSPEC           *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtEndProc         (STENDPROC        *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveStmtCondElem        (CONDITION        *stmt,      ALL_PCODE_ITEM *allItem);
   void SaveExpBinary           (EXPBINARY        *exp,       ALL_PCODE_ITEM *allItem);
   void SaveExpUnary            (EXPUNARY         *exp,       ALL_PCODE_ITEM *allItem);
   void SaveExpVal              (EXPVAL           *exp,       ALL_PCODE_ITEM *allItem);
   void SaveClustName           (NAME_CLUST       *clst,      ALL_PCODE_ITEM *allItem);
   void SaveClustNum            (NUM_CLUST        *clst,      ALL_PCODE_ITEM *allItem);
   void SaveLineElem            (LINE_ELEM        *lineElem,  ALL_PCODE_ITEM *allItem);
   void SaveExpElem             (EXPLISTELEM      *expElem,   ALL_PCODE_ITEM *allItem);
   void SaveExpSpec             (EXPLISTSPEC      *expSpec,   ALL_PCODE_ITEM *allItem);
   void SaveVGlobal             (SYMGLOBAL        *sym,       ALL_PCODE_ITEM *allItem);
   void SaveVLocal              (SYMLOCAL         *sym,       ALL_PCODE_ITEM *allItem);
   void SaveModule              (SYMMODULE        *sym,       ALL_PCODE_ITEM *allItem);
   void SaveProc                (SYMPROC          *sym,       ALL_PCODE_ITEM *allItem);
   void SaveArray               (SYMARRAY         *sym,       ALL_PCODE_ITEM *allItem);
   void SaveTxtFile             (SYMTXTFILE       *sym,       ALL_PCODE_ITEM *allItem);
   void SaveClass               (SYMCLASS         *sym,       ALL_PCODE_ITEM *allItem);
   void SaveMethod              (SYMMETHOD        *sym,       ALL_PCODE_ITEM *allItem);
   void SaveProp                (SYMPROP          *sym,       ALL_PCODE_ITEM *allItem);
   void SaveInitClass           (SYMINITCLASS     *sym,       ALL_PCODE_ITEM *allItem);
#ifdef DBASE
   void SaveDbfFile             (SYMDBFFILE       *sym,       ALL_PCODE_ITEM *allItem);
#endif
#ifndef NO_BTR
   void SaveFile                (SYMFILE          *sym,       ALL_PCODE_ITEM *allItem);
   void SaveStruc               (SYMSTRUC         *sym,       ALL_PCODE_ITEM *allItem);
   void SaveValue               (VALUE            *val,       ALL_PCODE_ITEM *allItem);
#endif // NO_BTR

   void CopyStatement           (PCODE_STATEMENT  *pcodeStmt, STATEMENT      *stmt);
   void CopyExpression          (PCODE_EXPRESSION *pcodeExp,  EXPRESSION     *exp);
   void CopyClust               (PCODE_ICLUST     *pcodeClst, ICLUST         *clst);
   void CopyISymbol             (PCODE_ISYMBOL    *pcodeSym,  ISYMBOL        *sym);
   void CopySymVar              (PCODE_SYMVAR     *pcodeSym,  SYMVAR         *sym);
   void CopySymProcBefore       (PCODE_SYMPROC    *pcodeSym,  SYMPROC        *sym);
   void CopySymProcAfter        (SYMPROC          *sym,       ALL_PCODE_ITEM *allItem);
   void CopyProcDefBefore       (PCODE_PROCDEF    *pcodeDef,  PROCDEF        *def);
   void CopyProcDefAfter        (PROCDEF          *def,       ALL_PCODE_ITEM *allItem);
#ifndef NO_BTR
   void CopySymStruc            (PCODE_SYMSTRUC   *pcodeSym,  SYMSTRUC       *sym);
#endif // NO_BTR

   static int saveSym           (ISYMBOL     *sym,      SAVE_DATA *data);
   static int saveSymVar        (ISYMBOL     *sym,      SAVE_DATA *data);
   static int saveSymProc       (ISYMBOL     *sym,      SAVE_DATA *data);
   static int saveStmt          (STATEMENT   *stmt,     SAVE_DATA *data);
   static int saveClst          (ICLUST      *clst,     SAVE_DATA *data);
   static int saveLnEm          (LINE_ELEM   *lineElem, SAVE_DATA *data);
   static int saveExpEm         (EXPLISTELEM *expElem,  SAVE_DATA *data);
   static int saveExpSc         (EXPLISTSPEC *expSpec,  SAVE_DATA *data);

   /////////////////////////////////
   // method for LoadPCode
   /////////////////////////////////
   ITEM_HEADER *LoadListItems(CNTLIST *list, ITEM_HEADER* item, int16 size, CNTLIST *lstClust,
                              STATEMENT **endStmt, void **elem, bool fGlobal);
   ITEM_HEADER *LoadItem(ITEM_HEADER *pcodeItem, CNTLIST *list, void **elem, CNTLIST *lstClust,
                              STATEMENT **endStmt, bool fGlobal);

   ITEM_HEADER *LoadStmtExp     (PCODE_STEXPRESSION *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtLoop    (PCODE_STLOOP       *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtFor     (PCODE_STFOR        *pcodeStmt, CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtBreak   (PCODE_STATEMENT    *pcodeStmt, CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtContinue(PCODE_STATEMENT    *pcodeStmt, CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtCond    (PCODE_STCONDITION  *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtReturn  (PCODE_STRETURN     *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtOut     (PCODE_STOUT        *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtSpec    (PCODE_STSPEC       *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadStmtEndProc (PCODE_STENDPROC    *stmt,      CNTLIST *list, void **elem, STATEMENT **endStmt);
   ITEM_HEADER *LoadStmtCondElem(PCODE_CONDITION    *stmt,      CNTLIST *list, void **elem);
   ITEM_HEADER *LoadExpBinary   (PCODE_EXPBINARY    *exp,       CNTLIST *list, void **elem);
   ITEM_HEADER *LoadExpUnary    (PCODE_EXPUNARY     *exp,       CNTLIST *list, void **elem);
   ITEM_HEADER *LoadExpVal      (PCODE_EXPVAL       *exp,       CNTLIST *list, void **elem);
   ITEM_HEADER *LoadClustName   (PCODE_NAMECLUST    *clst,      CNTLIST *list);
   ITEM_HEADER *LoadClustNum    (PCODE_NUMCLUST     *clst,      CNTLIST *list, CNTLIST *lstClust);
   ITEM_HEADER *LoadLineElem    (PCODE_LINEELEM     *lineElem,  CNTLIST *list);
   ITEM_HEADER *LoadExpElem     (PCODE_EXPLISTELEM  *expElem,   CNTLIST *list);
   ITEM_HEADER *LoadExpSpec     (PCODE_EXPLISTSPEC  *expSpec,   CNTLIST *list);
   ITEM_HEADER *LoadVGlobal     (PCODE_SYMGLOBAL    *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadVLocal      (PCODE_SYMLOCAL     *sym,       CNTLIST *list, void **elem);
   ITEM_HEADER *LoadModule      (PCODE_SYMMODULE    *sym);
   ITEM_HEADER *LoadProc        (PCODE_SYMPROC      *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadArray       (PCODE_SYMARRAY     *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadTxtFile     (PCODE_SYMTXTFILE   *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadClass       (PCODE_SYMCLASS     *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadMethod      (PCODE_SYMMETHOD    *sym,       CNTLIST *list);
   ITEM_HEADER *LoadProp        (PCODE_SYMPROP      *sym,       CNTLIST *list);
   ITEM_HEADER *LoadInitClass   (PCODE_SYMINITCLASS *sym,       CNTLIST *list);

#ifdef DBASE
   ITEM_HEADER *LoadDbfFile     (PCODE_SYMDBFFILE   *sym,       CNTLIST *list, bool fGlobal);
#endif // DBASE

#ifndef NO_BTR
   ITEM_HEADER *LoadFile        (PCODE_SYMFILE      *sym,       CNTLIST *list, bool fGlobal);
   ITEM_HEADER *LoadStruc       (PCODE_SYMSTRUC     *sym,       CNTLIST *list, bool fGlobal);
#endif // NO_BTR

   ITEM_HEADER *CreateProcSym   (SYMPROC            *proc,      PCODE_SYMPROC    *pcodeProc, ITEM_HEADER *nextItem );
   ITEM_HEADER *CreateProcDef   (PROCDEF            *def,       PCODE_PROCDEF    *pcodeDef,  ITEM_HEADER *nextItem, void **elem);

   void         CreateSymVar    (SYMVAR             *var,       PCODE_SYMVAR     *pcodeVAR);
   void         CreateISymbol   (ISYMBOL            *sym,       PCODE_ISYMBOL    *pcodeSym);
   void         CreateStatement (STATEMENT          *stmt,      PCODE_STATEMENT  *pcodeStmt);
   void         CreateExpression(EXPRESSION         *exp,       PCODE_EXPRESSION *pcodeExp);
   void         CreateClust     (ICLUST             *clst);

   /////////////////////////////////
   // Tools methods
   /////////////////////////////////
   bool        MakePCodeModule    (TAddModContextRsm *pS, const char *importFile, const char *impName);
   int16       AddExternSymbol    (ISYMBOL *sym);
   int16       AddInTableOfSymbols(ISYMBOL *sym);
   int16       AddInTableOfNames  (char *name, char *name_origin = NULL);
   int16       SymbolPtrToIndex   (ISYMBOL *sym);
   ISYMBOL    *IndexToSymbolPtr   (int16 index);
   void        BeforeImportSymbol ();
   void        AddModuleImport    (SYMMODULE *module);
   bool        LoadImportModule   (TOpenData *dt, PCODE_HEADER_FILE *hdrFile);
   int32       GetSizeTableOfNames();
   const char *GetName            (int16 index, bool isCS = false);
   bool        GenerationPCode    ();
   void        SavePCodeSegment   (SYMMODULE *mod);
   void        SaveTuningData     ();
   void        SaveTableOfNames   ();
   void        SaveTableOfImport  ();
   void        SaveTableOfModule  ();
   bool        LoadPCodeFile      (const char *imporFile, const char *impName, TAddModContextRsm *pS);
   bool        FindWorkDir        (char *dir, const char *path, const char *fileName, const char *fileExt);
   void        ZeroProp           ();
   void        BeginTuningData    (PCODE_HEADER_FILE *hdrFile);

   // --------------------------------------------
   void ReserveMemForMac(int32)
   {
   }

   // --------------------------------------------
   void SetModuleName(const char *name)
   {
    nameFile = name;
   }

   // --------------------------------------------
   bool CheckVersion(uint8 verMajor, uint8 verMinor);
   bool CheckVersionGE(PCODE_HEADER_FILE *hdr, uint8 verMajor, uint8 verMinor);

   // --------------------------------------------
   size_t   getSizeTuningDataMem(PCODE_HEADER_FILE *hdr);

   void     FillListSymbols       (CNTLIST   *list, bool fDirect);
   void     FillListSymbolsVarProc(CNTLIST   *list, bool fDirect, bool varOrProc);
   void     FindAddSymbols        (ISYMBOL   *sym);
   void     BeginFillListSymbols  (SYMMODULE *mod);
   void     FillProcDefSymbol     (PROCDEF   *procDef);
   int      GetGlobalNitems       (CNTLIST   *list);

   // --------------------------------------------
   static int fillVector       (ISYMBOL     *sym,    TPCodeTree *tree);
   static int fillVectorVar    (ISYMBOL     *sym,    TPCodeTree *tree);
   static int fillVectorProc   (ISYMBOL     *sym,    TPCodeTree *tree);
   static int addModule        (TImportRef  *modRef, TPCodeTree *tree);
   static int genPCode         (SYMMODULE   *mod,    void       *data);
   static int saveTableOfNames (NAME_ELEM   *elem,   TPCodeTree *tree);
   static int saveTableOfImport(IMPORT_ELEM *elem,   TPCodeTree *tree);
   static int saveTableOfModule(MODULE_ELEM *elem,   TPCodeTree *tree);

   // --------------------------------------------
   SYMCLASS *GetOwnerSym()
   {
    return m_ownerSym;
   }

   // --------------------------------------------
   SYMCLASS *SetOwnerSym(SYMCLASS *sym)
   {
    SYMCLASS  *old = m_ownerSym;


    m_ownerSym = sym;

    return old;
   }

   /////////////////////////////////
   // prop for LoadPCode
   /////////////////////////////////
   ISYMBOL             **tableOfSymbols;
   char                **tableOfNames;
   char                **tableOfNamesCS;
   IMPORT_DATA          *tableOfImport;
   void                 *poolPCode;
   void                 *tuningData;
   int16                 numMakeItem;
   int16                 numImportSymbol;

   //////////////////////////////////
   // prop for SavePCode
   //////////////////////////////////
   CNTLIST               listOfSymbols;
   CNTLIST               listOfNames;
   CNTLIST               listOfImport;
   CNTLIST               listOfModule;

   FILE                 *streamPCode;
   const char           *nameFile;
   PCODE_HEADER_FILE     header;
   CNTLIST              *allStmt;
   TPCodeManager        *manager;
   TAddModContextRsm    *cnxtRsm;
   SYMCLASS             *m_ownerSym;
};

//////////////////////////////////////////////////////////////////////////
// Begin - class TPCodeManager
//////////////////////////////////////////////////////////////////////////
class TPCodeManager
    {
     public:
       // ----------------------------------------
       // ----------------------------------------
       TPCodeManager();
      ~TPCodeManager();

       // ----------------------------------------
       void *operator new (size_t size)
       {
        return iNewMemNull(size);
       }

       // ----------------------------------------
       void operator delete (void *ptr)
       {
        iDoneMem(ptr);
       }

       bool LoadPCode(const char *importFile, const char *impName, TAddModContextRsm *pS);
       bool SavePCode();
       bool SavePCode(SYMMODULE *mod);

     protected:
       // ----------------------------------------
       // ----------------------------------------
       TPCodeTree  treePCode; // Used for load p-code and manual generation p-code

       friend class TPCodeTree;

       char *GetOutBuff(void);

     private:
       // ----------------------------------------
       // ----------------------------------------
       char       *m_pv_iOutBuf;  // Аналог inst->ioutbuf для записи ошибок
   };

//////////////////////////////////////////////////////////////////////////
// The End - class TPCodeManager
//////////////////////////////////////////////////////////////////////////
#endif // RSL_PCODE2

#include <packpop.h>

#endif // RSL_NO_PCODE

#endif // __IPCODE_HPP

/* <-- EOF --> */