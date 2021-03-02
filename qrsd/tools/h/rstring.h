//-*--------------------------------------------------------------------------*-
//
// File Name   : rstring.h
//
// Copyright (c) 1991 - 2005 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// March 5, 1994 Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

//
// Char and string case conversion and string/char
// comparison routines using national locale
//

#ifndef __RSTRING_H
#define __RSTRING_H

#include <stddef.h>
#include <string.h>

#include <rscomplr.h>
#include <platform.h>
#include "tooldef.h"
#include "rsfname.h"


#ifdef RSL_NO_STRCASE_CONV

    #define  strupr(s)  struprR(s)
    #define  strlwr(s)  strlwrR(s)

#endif // RSL_NO_STRCASE_CONV


#define  toupzs(s)  (struprR(s))

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 Conversion/comparison functions
*******************************************************************************/
typedef unsigned char  TStrCp;

#define  RSL_CPDOS  0
#define  RSL_CPWIN  1


#ifdef RSL_CHAR_ASCII

    #define  ccmpR(c1, c2)              ((unsigned char)(c1) - (unsigned char)(c2))
    #define  cicmpR(c1, c2)             ((unsigned char)(toupc(c1)) - (unsigned char)(toupc(c2)))
    #define  strcmpR(str1, str2)        (strcmp((str1), (str2)))
    #define  strncmpR(str1, str2, len)  (strncmp((str1), (str2), (len)))

#else
    //
    // Compare characters using unique-weight collating sequence for current locale
    //
    _TOOLEXP int ccmpR(char c1, char c2);

    //
    // Compare characters using shared-weight collating sequence for current locale
    // (case-independent comparison)
    //
    _TOOLEXP int cicmpR(char c1, char c2);

    //
    // Compare strings using unique-weight collating sequence for current locale
    //
    _TOOLEXP int strcmpR(const char *str1, const char *str2);

    //
    // Compare strings up to given length using unique-weight collating
    // sequence for current locale
    //
    _TOOLEXP int strncmpR(const char *str1, const char *str2, size_t len);

#endif // RSL_CHAR_ASCII

//
// Convert char to uppercase using current charset
//
_TOOLEXP int toupc  (int ch);
_TOOLEXP int toupcEx(int ch, TStrCp codePage);

//
// Convert char to lowercase using current charset
//
_TOOLEXP int toDownc  (int ch);
_TOOLEXP int toDowncEx(int ch, TStrCp codePage);

//
// Compare strings using shared-weight collating sequence for current locale
// (case-independent comparison)
//
_TOOLEXP int stricmpR  (const char *str1, const char *str2);
_TOOLEXP int stricmpREx(const char *str1, const char *str2, TStrCp codePage);

//
// Compare strings up to given length using shared-weight collating sequence
// for current locale (case-independent comparison)
//
_TOOLEXP int strnicmpR  (const char *str1, const char *str2, size_t len);
_TOOLEXP int strnicmpREx(const char *str1, const char *str2, size_t len, TStrCp codePage);

//
// Поиск подстроки без учета регистра
//
_TOOLEXP const char *stristrR  (const char *str1, const char *str2);
_TOOLEXP const char *stristrREx(const char *str1, const char *str2, TStrCp codePage);

//
// Convert string to uppercase using current charset
//
_TOOLEXP char * struprR (char *str);
_TOOLEXP char *struprREx(char *str, TStrCp codePage);

//
// Convert string to uppercase up to given length using current charset
//
_TOOLEXP char * strnuprR (char *str, size_t len);
_TOOLEXP char *strnuprREx(char *str, size_t len, TStrCp codePage);

//
// Convert string to lowercase using current charset
//
_TOOLEXP char *strlwrR  (char *str);
_TOOLEXP char *strlwrREx(char *str, TStrCp codePage);

//
// Convert string to lowercase up to given length using current charset
//
_TOOLEXP char *strnlwrR  (char *str, size_t len);
_TOOLEXP char *strnlwrREx(char *str, size_t len, TStrCp codePage);

_TOOLEXP int isalphaR  (int ch);
_TOOLEXP int isalphaREx(int ch, TStrCp codePage);
_TOOLEXP int isalnumR  (int ch);
_TOOLEXP int isalnumREx(int ch, TStrCp codePage);

/*******************************************************************************
 String/memory copy functions
*******************************************************************************/

_TOOLEXP char *rsmemstrstripcpy (char *dest, const char *src, size_t sizetMaxD, size_t sizetMaxS);

_TOOLEXP char *rsmempadcpy(char *dest, const char *src, size_t sizetMaxD, size_t sizetMaxS);

#define  szTrunCpy(dest, src, sizetMaxD, sizetMaxS)  rsmemstrstripcpy((dest), (src), (sizetMaxD), (sizetMaxS))
#define  TxtSCpy(dest, src, sizetMaxD, sizetMaxS)    rsmempadcpy((dest), (src), (sizetMaxD), (sizetMaxS))

//
// Remove trailing spaces
// dest and src can overlap (and can be the same)
//
_TOOLEXP char *rstrimr(char *dest, const char *src);
_TOOLEXP char *remrb(char *c);

//
// Remove leading spaces
// dest and src can overlap (and can be the same)
//
_TOOLEXP char *rstriml(char *dest, const char *src);

#define  remlb(src, dest)  rstriml((dest), (src))


/*******************************************************************************
 Oem <-> Ansi conversion functions
*******************************************************************************/

_TOOLEXP char *RslToOEM     (char *buff, const char *ansi);
_TOOLEXP char *RslToANSI    (char *buff, const char *oem);
_TOOLEXP char *RslToOEMBuff (char *buff, const char *ansi, size_t len);
_TOOLEXP char *RslToANSIBuff(char *buff, const char *oem,  size_t len);

_TOOLEXP char *RslCvtChar  (unsigned dstCp, char *dst, int dstSz, unsigned srcCp, const char *src, int srcSz);
_TOOLEXP int   RslWide2Char(char *outStr, int outSz, const wchar_t *inStr, int inSz, unsigned cp);
_TOOLEXP int   RslChar2Wide(wchar_t *outStr, int outSz, const char *inStr, int inSz, unsigned cp);

_TOOLEXP bool     IsRsLocaleUsed(void);
_TOOLEXP unsigned SetRslDefProgEncode(unsigned newEncode);


#ifdef __cplusplus
}
#endif

#endif //#ifndef __RSTRING_H

/* <-- EOF --> */