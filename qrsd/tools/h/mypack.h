/************************************************************************
 *  MYPACK.H                                                            *
 *                                                                      *
 *  Определения поддержки упаковки/разупаковки                          *
 *                                                                      *
 *  Copyright (c) 1992 Компьютерные и Информационные Технологии         *
 *                1993 JV R-Style                                       *
 *                                                                      *
 *  VERSION INFO :  $Archive: /RS-Tools/Old_tools/Tools/H/mypack.h $
 *                  $Revision: 3 $
 *                  $Date: 15.10.98 18:34 $
 *                  $Author: Markovitch $
 *                  $Modtime: 14.10.98 23:59 $
 *  History:                                                            *
 *  ---------------------------                                         *
 *  06.04.93    AI      Created                                         *
 *  28.10.93    ED      Changed for BTRVFILE                            *
 ************************************************************************/

#ifndef __MYPACK_H
#define __MYPACK_H

#ifndef _MYPACKH_
#define _MYPACKH_

#include <platform.h>
#include <tooldef.h>

#include <packpsh1.h>

#ifdef __cplusplus
extern "C" {
#endif

#define  TABLE_SIZE    512

// -----------------------------------------------------------------------------
typedef struct
      {
       unsigned int   iCodeValue;
       unsigned int   iPrefixCode;
       unsigned char  cAppendChar;
      } code_type;

// -----------------------------------------------------------------------------
typedef struct
      {
       int             iInputByteCount;
       int             iInputBufferSize;
       unsigned char  *pInputBuffer;
       int             iOutputByteCount;
       int             iOutputBufferSize;
       unsigned char  *pOutputBuffer;
       unsigned int    iNextCode;
       unsigned int    iInputBitCount;
       unsigned long   lInputBitBuffer;
       unsigned int    iOutputBitCount;
       unsigned long   lOutputBitBuffer;
       code_type       codeEntry[TABLE_SIZE];
       unsigned char   cDecodeStack[TABLE_SIZE];
      } workdata_type;


typedef int (RSL_CDECL *cmpgetbyte_t)(workdata_type *);
typedef int (RSL_CDECL *cmpputbyte_t)(workdata_type *, int);

_TOOLFUNC void Compress(workdata_type *workData, cmpgetbyte_t CMPgetByte, cmpputbyte_t CMPputByte);
_TOOLFUNC void Extract (workdata_type *workData, cmpgetbyte_t CMPgetByte, cmpputbyte_t CMPputByte);

_TOOLFUNC int BufferUnpk(const char *in, char *out, int inlen, int outlen);
_TOOLFUNC int BufferPack(const char *in, char *out, int inlen, int outlen);

#ifdef __cplusplus
}
#endif

#include <packpop.h>

#endif

#endif // __MYPACK_H

/* <-- EOF --> */