/*************************************************************************
 *  FILEXT.H                                                             *
 *                                                                       *
 *  Поддержка расширенных операций с файлами (Novell Netware)            *
 *                                                                       *
 *  Copyright (c) 1992 Компьютерные и Информационные Технологии          *
 *                                                                       *
 *  History:                                                             *
 *  ---------------------------                                          *
 *  30.09.93    AI      Created                                          *
 *************************************************************************/

#ifndef _FILEXTH_
#define _FILEXTH_

#include "platform.h"
#include "apptldef.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define  _NOVELL_TTSFILE     0x0010
#define  _NOVELL_NDXFILE     0x0020
#define  _NOVELL_RDAUDIT     0x0040
#define  _NOVELL_WRAUDIT     0x0080

int _cdecl  FEgetFileAttr(char RSL_FAR *, int *);
int _cdecl  FEsetFileAttr(char RSL_FAR *, int);
int _cdecl  FEgetRemoteStatus(int);
int _cdecl  FEgetDrive(char *);
int _cdecl  FEunlinkFile(char *);
int _cdecl  FEmakeDir(char *);
int _cdecl  FEfindFile(char *);

#ifdef __cplusplus
 } ;
#endif

#endif

/* <-- EOF --> */