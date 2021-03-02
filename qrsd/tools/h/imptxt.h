//***********************************************************************
//             ������࠭�� ⥪�⮢� ����䥩� ��� Borland C.
//                  �����প� ������ �� ⥪�⮢��� 䠩��.
//                           VECsoft  6. 3.93
//***********************************************************************

#ifndef __IMPTXT_H
#define __IMPTXT_H

#include <stdio.h>

#include "apptldef.h"


/* ����⥫� ���� ������                       */
typedef struct
      {
       unsigned char  IVt;     /* ��� ���祭��                                 */
       void          *IValue;  /* ���祭�� � ����                              */
       int            IVp;     /* ������ ��᫥ �窨,�ਧ��� wrap ��� ��ப    */
       coord          x,y,l,h; /* ���न���� ����                              */
       char         **wlist;   /* ���᮪ ��� wrap                              */
      } IFIELD;

/* ��ப� ������                       */
typedef struct
      {
       IFIELD         *IFList;        /* ���ᨢ ����� ��ப� ������          */
       unsigned char   IFnum;         /* ������⢮ ����� ��ப� ������      */
       char            sep;           /* ������-ࠧ����⥫� �����             */
       unsigned char   Field;         /* ����� ���� ������                   */
       coord           h;             /* ������⢮ ����� ������ � ��ப�    */
       int             stat;          /* ���ﭨ� �뢮����� ��ப�           */
      } ILINE;


#define  NI(i)  sizeof(i)/sizeof(IFIELD)

/* ����⥫� ������                    */
typedef struct
      {
       int      wide;                 /* ��ਭ� ������                       */
       char    *curchar;              /* ��뫪� �� ⥪�騩 ᨬ��� ������     */
       int      Line;                 /* ����� ����� ������                  */
       char    *buff;                 /* ���� �����                          */
       char    *wsave;                /* ������� ��࠭���� �����           */
       FILE    *impfile;              /* ���� ��� ������                     */
       int      nl;                   // ��᫮ ��ப ����.
       ILINE  **il;                   // ��ப� ����.
       char     ch;                   /* ������� ᨬ���                      */
      } IMPORT;

#ifdef __cplusplus
 extern "C" {
#endif

int ImplineFlow                 /* ����� ��ப� ������ � ��⮪� ����� */
(
 IMPORT *im,                    /* ����⥫� ������                    */
 ILINE  *rl                     /* ����⥫� ��ப� ������             */
);

_APPTLFUNC int Impline                     /* ����� ��ப� ������                */
(
 IMPORT *im,                    /* ����⥫� ������                    */
 ILINE  *rl                     /* ����⥫� ��ப� ������             */
);

int bimp                        /* ����� ������                        */
(
 IMPORT *im,                    /* ����⥫� ������                    */ 
 char   *fname,                 /* �������� 䠩�� ������               */
 int     wd                     /* ��ਭ� ������                       */
);

_APPTLFUNC IMPORT *begimp                  /* ����� ������                        */
(
 char   *fname,                 /* �������� 䠩�� ������               */
 int     wd                     /* ��ਭ� ������                       */
);

_APPTLFUNC void endimp                     /* ������ ������                       */
(
 IMPORT *im                     /* ����⥫� ������                    */ 
);

#ifdef __cplusplus
 } ;
#endif

#endif //__IMPTXT_H

/* <-- EOF --> */