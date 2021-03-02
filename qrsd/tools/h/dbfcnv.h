/*-----------------------------------------------------------------------*-

 File Name   : dbfcnv.h  

 Sybsystem   : TOOLS  
 Description : Process dBase files

 Source      : dbf.c

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 July 25,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __DBFCNV_H
#define __DBFCNV_H

/************************* Дата и время в формате Btrieve **************/
#ifndef BDATE
#define BDATE

    #pragma pack(push, 1)

    typedef struct
          {
           unsigned char  day;      // День
           unsigned char  mon;      // Месяц
           short    int   year;     // Год
          } bdate;

    typedef struct
          {
           unsigned char  hundr;
           unsigned char  sec;
           unsigned char  min;
           unsigned char  hour;
          } btime;

    #pragma pack(pop)

#endif

typedef struct tagdBaseFile
      {
       char  *file;    // Объект DBC библиотеки
       char  *Buff;    // Буфер записи
       char  *ioutbuf;  
       char  *filename;
      }  dBaseFile;

#ifdef __cplusplus
 extern "C" {
#endif

extern void  (*errFieldProc)(char *dbfname,char *field);
extern void  (*errDbfFile)(char *dbfname, char *erType, int errCode);

double  GetMoney  (dBaseFile *db, char *name);
void    GetDate   (dBaseFile *db, char *name, bdate *bd);
void    GetZstring(dBaseFile *db, char *name, char *buff, int size);
double  GetDouble (dBaseFile *db, char *name);
long    GetLong   (dBaseFile *db, char *name);
int     GetInt    (dBaseFile *db, char *name);

int     dLoop     (char *dbfn,    int (*pr)(dBaseFile *db));
int     dLoop2    (char *dbfname, int (*pr)(dBaseFile *db));
int     dLoopEx   (char *dbfname, int (*pr)(dBaseFile *db), int *stt);

char   *dbern     (int rc);

extern   int         _dbcerr;

#ifdef __cplusplus
 }
#endif

#endif // __DBFCNV_H

/* <-- EOF --> */