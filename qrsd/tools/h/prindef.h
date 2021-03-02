// Комплекс:   RS-TOOLS 3.5 JSC R-Style 1993-1994.
// Модуль:     Подсистема просмотра файлов.
// Информация:
// История: ED 09/11/93
//
// VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/prindef.h $
//                  $Revision: 17 $
//                  $Date: 18.02.00 19:51 $
//                  $Author: Lebedev $
//                  $Modtime: 18.02.00 18:01 $
//
// 07/10/94 ED Добавил в PRINTPARMS PageSize.

#ifndef __PRINTDEF_H
#define __PRINTDEF_H


#include <stdio.h>

#include "rstypes.h"
#include "tooldef.h"
#include "pexpdef.h"
#include "apptldef.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <packpsh1.h>

typedef struct PRINTPARMS_t
      {
       unsigned char  iDbl;
       char           frmFeed;
       char           szPrn[80];
       char           szNmSeq[15];
       char           szSeq[160];
       char           szHFN[80];
       db_int16       PageSize;
       char           szNmExtVeiwer[160];
       char           bannerDir[160];
      } PRINTPARMS;

extern PRINTPARMS _TOOLEXP  printparmsV;

void _TOOLEXP SAVE_printparmsV(const PRINTPARMS *printparmsV);
void _TOOLEXP REST_printparmsV(PRINTPARMS *printparmsV);

int _TOOLEXP IsModif_printparmsV(PRINTPARMS *printparmsV);

typedef struct SEQNS_t
      {
       char  szNmSeq[15];
       char  szComment[60];
       char  szSeq[160];
      } SEQNS;

#include <packpop.h>

int _RSPNLEXP DinamicPPV(PRINTPARMS *ppV);

void _APPTLFUNC EditEscSeq(char *szNmSeq, char *szSeq);

int _APPTLFUNC iOpenSEQ(int mode);

typedef int (*altprintfn_t)(char *filename, FILE *out, int MaxLines);

_RSPNLEXP extern void (* ep)(char *szNmSeq, char *szSeq);
_RSPNLEXP extern altprintfn_t  AltPrint;
_RSPNLEXP extern void(* sp)(const PRINTPARMS *ppV);

#ifdef __cplusplus
}
#endif

#endif // __PRINTDEF_H

/* <-- EOF --> */