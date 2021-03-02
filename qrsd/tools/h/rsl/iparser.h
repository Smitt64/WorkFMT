/*@H*********************************************************************
*        File Name:   iparser.h          December 11,1993
*       Written by:  S. Kubrin   VECsoft TM.
*      Description:
*  Public routines:
* Private routines:
**********************************************************************@H*/
#ifndef  __IPARSER_H
#define  __IPARSER_H

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------
extern TOKKEN decl_list[];
extern TOKKEN decl_term[];

extern TOKKEN stmt_list[];
extern TOKKEN stmt_term[];

extern TOKKEN parm_list[];

extern TOKKEN parm_term[];
extern TOKKEN expr_list[];
extern TOKKEN expr_term[];


// -----------------------------------------------------------------------------
RSLLINK bool RSLAPI   ParserOpen         (void);
RSLLINK void RSLAPI   ParserClose        (void);

void                 *NewStatement       (CNTLIST *stlist, STMTTYPE st_type);
void                 *NewStatementEx     (CNTLIST *stlist, STMTTYPE st_type, bool isPCode, HRD inst, TScanerData *sc);
void                  NewExpression      (EXPRESSION **exp, EXP_TYPES exp_type, HRD inst);
NAME_CLUST           *AddNameClustEx     (EXPVAL *exp, char *name, bool isPCode, HRD inst);
NUM_CLUST            *AddNumClustEx      (EXPVAL *exp, int num, CNTLIST *lst, bool isPCode, HRD inst, TScanerData *sc);
EXPLISTELEM          *NewExpListEx       (CNTLIST *list, bool isPCode, HRD inst, TScanerData *sc);
bool                  NewExpListSpecEx   (CNTLIST *list, bool isPCode, EXPLISTSPEC **spec, HRD inst, TScanerData *sc);

RSLLINK bool  RSLAPI  ParseModule        (char *name);
void                  ParseStatement     (CNTLIST *stlist, TScanerData *sc);
char                 *ParseBracketString (TScanerData *sc);
RSLLINK bool  RSLAPI  ParseModuleSpec    (char *name);
RSLLINK bool  RSLAPI  ParseBlock         (char *name, char *file);
bool                  ParseModuleDeclList(SYMPROC *S, int *userBreak, TScanerData *sc);
bool                  ImportRslModule    (HRD inst, const char *file, TExtSrcInfo *info);

STEXPRESSION         *ParseAssignment    (CNTLIST *stlist, ISYMBOL *sym, TScanerData *sc);


void                  Variable           (EXPRESSION **exp, ISYMBOL *sym, TScanerData *sc);
void                  ParseExpression    (EXPRESSION **left, TScanerData *sc);

void                  ParseExpList       (CNTLIST *explist, TScanerData *sc);
void                  ParseCallStruct    (CALLSTRUCT *st, SYMPROC *name);
void                  DoneCallStruct     (CALLSTRUCT *st);
void                  FreeStatement      (STATEMENT *st);
void                  FreeExpListSpec    (EXPLISTSPEC *elist);
void                  ParseFile          (int is_struc, MODIFIER modifier, TScanerData *sc);
void                  ParseStruc         (void);

// add A.Tokmashev (14.06.2000)
SYMINITCLASS         *AddInitClass       (SYMCLASS *classPtr, bool thisPCode);
// end (14.06.2000)

// Установить флаги форматирования в соответствии с заданными спецификаторами
void                  ParseSetFormat     (int &flag, const char *fmt);

void                  ParseCp            (unsigned char codePage, TScanerData *sc);

void                  ParseDeclaration   (CNTLIST *stlist, int isconst, MODIFIER modifier, TScanerData *sc);
bool                  ParseTypeDecl      (ISYMBOL *sym, bool useByRef, TScanerData *sc);

#ifdef __cplusplus
}
#endif

#endif // iparser_h

/* <-- EOF --> */