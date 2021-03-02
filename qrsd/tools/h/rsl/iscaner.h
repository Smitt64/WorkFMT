/*@H*********************************************************************
*        File Name:   iscaner.h          November 25,1993            
*      Description:   
*  Public routines:  
* Private routines:  
**********************************************************************@H*/

#ifndef  __ISCANER_H
#define  __ISCANER_H

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
#define  MAX_STRING   2048

#ifdef RSL_MD_FLAT     
    #define MAX_TOKKEN   256
#else
    #define MAX_TOKKEN    80
#endif

#define  EOF_CHAR   -1
#define  EOL_CHAR  -10


//------------------   Типы лексем -----------------------------------
#ifdef ENUMINTSIZE
    #pragma option -b-
#endif

// -----------------------------------------------------------------------------
typedef enum tokkens
      {
       SC_NOTOKKEN,          //
       SC_ERROR,             //
       SC_IDENTIFIER,        //
       SC_NUMBER,            // Число
       SC_STRING,            // Строка
       SC_LPAREN,            // (
       SC_RPAREN,            // )
       SC_COMMA,             // ,
       SC_SEMICOLON,         // ;
       SC_MACRO,             // macro
       SC_END,               // end
       SC_IF,                // if
       SC_ELIF,              // elif
       SC_ELSE,              // else
       SC_WHILE,             // while
       SC_PLUS,              // +
       SC_MINUS,             // -
       SC_ASSIGN,            // =
       SC_GETREF,            // @          ?
       SC_STAR,              // *
       SC_SLASH,             // /
       SC_LBRACKET,          // [
       SC_RBRACKET,          // ]
       SC_EQ,                // ==         ?
       SC_NOTEQ,             // !=         ?

       SC_CLASS,             // class
       SC_WITHST,            // with

       SC_LT,                // <
       SC_GT,                // >
       SC_LE,                // <=
       SC_GE,                // >=
       SC_AND,               // and
       SC_OR,                // or
       SC_RETURN,            // return
       SC_NOT,               // not
       SC_COLON,             //
       SC_PERIOD,            //
       SC_FILE,              //
       SC_IMPORT,            //
       SC_SPDEL,             //
       SC_STRUC,             //
       SC_DECLARE,           //
       SC_CONST,             // const
       SC_ARRAY,             // Array
       SC_ONERROR,           // OnError
       SC_LOCAL,             // local
       SC_PRIVATE,           // private
       SC_FOR,               // for
       SC_CPDOS,             // cpdos
       SC_CPWIN,             // cpwin
       SC_BREAK,             // break
       SC_CONTINUE,          // continue
       SC_ENUM,              // enum


       // MUST be the last constants!
       SC_NORMAL,            //
       SC_KEY,               // key
       SC_TXT,               // txt
       SC_MEM,               //
       SC_APPEND,            // append
       SC_WRITE,             // write
       SC_SORT,              // sort
       SC_DBF,               // dbf
       SC_DIALOG,            //
       SC_BTR,               //
       SC_BLOB,              //

       SC_END_STREAM
      } TOKKEN;

// -----------------------------------------------------------------------------
typedef enum tag_modifier
      {
       MDF_NONE,
       MDF_LOCAL,
       MDF_PRIVATE
      } MODIFIER;

#define  SPEC_NUMBER  11   // Количество спецификаторов mem,key,sort... in sc_def_table (iscaner.c)

#ifdef ENUMINTSIZE
    #pragma option -b
#endif

// -----------------------------------------------
typedef struct 
      {
       char   *string;
       TOKKEN  code;
      } KEYWORD;

// -----------------------------------------------
typedef struct
      {
       int  (*next_ch)(void *UserData);  
       KEYWORD  *rw_table;      // Таблица ключевых слов
       int       nkeyword;      // Количество ключевых слов в таблице
       // ....... 
       void     *data;
       int  (*setFileEncode)(int enc, void *data);      // Pointer to user function
      } INIT_SCANER;

// -----------------------------------------------
typedef struct
      {
       FILE  *fp;  // Текущий обрабатываемый поток
      } TDosStream;

// -----------------------------------------------
typedef struct
      {
       short int  chunk;
       int        offset;
      } TBtrStream;


// -----------------------------------------------------------------------------
typedef struct
      {
       int  sc_line, sc_offset, sc_ch, sc_backch;
       int  sc_tokken;
       int  tok_lbeg, tok_obeg, tok_lend, tok_oend, ptok_l, ptok_o;

       int  (*old_next_ch)(void *UserData);  
       void *oldData;

       #ifdef USEDEBUGER
           int  lastStmtOffs;
           int  curOffs;

           int  curTokBegOffs;
           int  curTokEndOffs;
           int  prevTokEndOffs;
       #endif

       TStrCp  codePage;
       int  (*setFileEncode)(int enc, void *data);
      } SCSTATE;

// -----------------------------------------------------------------------------
typedef struct
      {
       TOKKEN      sc_tokken;            // Current tokken
       char       *tokken_buf;           // Value of current tokken placed here
       char       *tokken_bufUp;         // Value of current tokken in uppercase placed here
       VALUE       constval;             // Constant value

       int         sc_line;              // Current input line number
       int         sc_offset;            // Позиция лексемы в строке
       int         iNestOut;             // Количество вложений '[' в инструкции вывода

       int         tok_lbeg, tok_obeg, tok_lend, tok_oend;
       int         ptok_l, ptok_o;

       char       *strconst;             // Буфер для строковой константы
       int         sc_ch;                 // Current input character
       int         sc_backch;
       int         (*next_ch)(void *data);      // Pointer to user function
       int         ncomment;

       KEYWORD    *rw_table;
       int         nkeyword;

       #ifdef DEBUG
           FILE   *dbg;
       #endif

       void       *data;   // data for next_ch

       #ifdef USEDEBUGER
           int     lastStmtOffs;
           int     curOffs;

           int     curTokBegOffs;
           int     curTokEndOffs;
           int     prevTokEndOffs;
       #endif

       char       *oldLocale;
       bool        ordinalBracket;  

       bool        new_ln_flag;
       TStrCp      codePage;
                           
       #if defined(USE_NUMERIC) // [!!!:x64] && !defined(RSL_PL_WIN64)
           Numeric_t   numVal;
       #endif

       HRD         inst;

       int         (*setFileEncode)(int enc, void* data);      // Pointer to user function
      } TScanerData;

// -----------------------------------------------------------------------------
RSLLINK void RSLAPI   ScanerSuspend  (SCSTATE *st);
RSLLINK void RSLAPI   ScanerResume   (SCSTATE *st);

void                  ScanerGetInfo  (INIT_SCANER *scInfo);
void                  ScanerSetInfo  (INIT_SCANER *scInfo);

RSLLINK void   RSLAPI ScanerClose    (void);
RSLLINK bool   RSLAPI ScanerOpen     (void);
TOKKEN                ScanerGetTokken(TScanerData *sc);
char                 *Read_out_line  (int off, int prevOffs, TScanerData *sc);
RSLLINK void   RSLAPI ScanerReset    (void);

int                   sc_get         (TScanerData *sc);
void                  sc_getch       (TScanerData *sc);

bool                  sc_tokken_in   (TOKKEN *list1, TScanerData *sc);
bool                  ValidTokken    (TOKKEN *list1, TOKKEN *list2, TOKKEN *list3, TScanerData *sc);
void                  NeedTokken     (TOKKEN tok, TScanerData *sc);

// -----------------------------------------------------------------------------
#define  SCD         ((TScanerData *)(RslInst->scanerData))
#define  SCD2(inst)  ((TScanerData *)(inst->scanerData))

// -----------------------------------------------------------------------------
#ifdef RSL_ANSI_STR
    #define  sc_stricmp2(s1, s2, inst)  (stricmpREx(s1, s2, (TStrCp)(inst->GUIclient ? RSL_CPWIN : RSL_CPDOS)))
#else
    #define  sc_stricmp2(s1, s2, inst)  (stricmpR(s1, s2))
#endif

#ifdef __cplusplus
}
#endif

#endif   // scaner_h

/* <-- EOF --> */