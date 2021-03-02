/*-----------------------------------------------------------------------*-

 File Name   : btrmac.h  

 Sybsystem   : RSL  
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 March 27,1996  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __BTRMAC_H
#define __BTRMAC_H

#include <nwbtrv.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <packpsh1.h>

// -----------------------------------------------
typedef struct
      {
       char       name[25];
       db_int16   chunk;
       db_int16   len;
       db_uint32  CRC;
       db_int16   dirCode;
       db_bdate   date;
       db_int16   flags;

       unsigned char  buff[4096];
      } BTRMACFILE;

// -----------------------------------------------
typedef struct
      {
       char      name[25];
       db_int16  chunk;
      } BTRMACFILEKEY;

// -----------------------------------------------
typedef struct
      {
       db_int16  dirCode;
       char      name[25];
      } BTRMACFILEKEY1;

// Описание  флагов  макроса
// 0 - чтение доступно          ( 1 ), недоступно          ( 0 )
// 1 - запись доступна          ( 1 ), недоступна          ( 0 )
// 2 - обновление из дистрибуции( 1 ), обновление ручное   ( 0 )
// 3 - макрос R-Style           ( 1 ), макрос пользователя ( 0 )
// 4 - макрос удален            ( 1 ), макрос в базе       ( 0 )

enum  MACRO_FLAGS 
   {
    MACRO_READ    =  1,
    MACRO_WRITE   =  2,
    MACRO_DISTRIB =  4,
    MACRO_RSTYLE  =  8,
    MACRO_DELETE  = 16
   };

#define  mRead(flags)     (flags & MACRO_READ)
#define  mWrite(flags)    (flags & MACRO_WRITE)
#define  mDistrib(flags)  (flags & MACRO_DISTRIB)
#define  mRstyle(flags)   (flags & MACRO_RSTYLE)
#define  mDelete(flags)   (flags & MACRO_DELETE)


#ifndef ICMP_DEFINED
    int  BtrMacCreate (char *name);
    int  BtrMacInsert (BTRVFILE *bf, char *flName, int dirCode, int flags);
    int  BtrMacCheck  (BTRVFILE *bf, char *name, int *dirCode, int *flags, bdate *dt);
    int  BtrMacExtract(BTRVFILE *bf, char *name, char *flName, bool doDel, bool full);
#endif

#include <packpop.h>

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */