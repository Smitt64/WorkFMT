 // Profile reader
 // by Mike Denisov
 // Copyright (C) JV RStyle 1994.

#ifndef _PROFILE_H_
#define _PROFILE_H_

#include "na/nadef.h"

// -----------------------------------------------------------------------------
enum _PRSAY_
   {
    PR_OPEN = 1,
    PR_ALL
   };

#ifdef __cplusplus
 extern "C" {
#endif

_NALGFUNC FILE *OpenProfile(char *pszName, int iSay);
_NALGFUNC void  CloseProfile(FILE *f);
_NALGFUNC int   RslGetProfileString(FILE *f, char *pszName, char *szKey, char *pszValue, int iLen, int iSay);

#ifdef __cplusplus
  }
#endif

#endif

/* <-- EOF --> */