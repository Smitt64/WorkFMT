//***********************************************************************
//             Полноэкранный текстовый интерфейс для Borland C.
//                  Поддержка импорта из текстового файла.
//                           VECsoft  6. 3.93
//***********************************************************************

#ifndef __IMPTXT_H
#define __IMPTXT_H

#include <stdio.h>

#include "apptldef.h"


/* Описатель поля импорта                       */
typedef struct
      {
       unsigned char  IVt;     /* Тип значения                                 */
       void          *IValue;  /* Значение в поле                              */
       int            IVp;     /* Знаков после точки,признак wrap для строк    */
       coord          x,y,l,h; /* Координаты поля                              */
       char         **wlist;   /* Список для wrap                              */
      } IFIELD;

/* Строка импорта                       */
typedef struct
      {
       IFIELD         *IFList;        /* Массив полей строки импорта          */
       unsigned char   IFnum;         /* Количество полей строки импорта      */
       char            sep;           /* Символ-разделитель полей             */
       unsigned char   Field;         /* Номер поля импорта                   */
       coord           h;             /* Количество линий импорта в строке    */
       int             stat;          /* состояние выводимой строки           */
      } ILINE;


#define  NI(i)  sizeof(i)/sizeof(IFIELD)

/* Описатель импорта                    */
typedef struct
      {
       int      wide;                 /* Ширина импорта                       */
       char    *curchar;              /* Ссылка на текущий символ импорта     */
       int      Line;                 /* Номер линии импорта                  */
       char    *buff;                 /* Буфер ввода                          */
       char    *wsave;                /* Область сохранения статуса           */
       FILE    *impfile;              /* Файл для импорта                     */
       int      nl;                   // Число строк отчета.
       ILINE  **il;                   // Строки отчета.
       char     ch;                   /* Вводимый символ                      */
      } IMPORT;

#ifdef __cplusplus
 extern "C" {
#endif

int ImplineFlow                 /* Ввести строку импорта в потоке полей */
(
 IMPORT *im,                    /* Описатель импорта                    */
 ILINE  *rl                     /* Описатель строки импорта             */
);

_APPTLFUNC int Impline                     /* Ввести строку импорта                */
(
 IMPORT *im,                    /* Описатель импорта                    */
 ILINE  *rl                     /* Описатель строки импорта             */
);

int bimp                        /* Начать импорт                        */
(
 IMPORT *im,                    /* Описатель импорта                    */ 
 char   *fname,                 /* Название файла импорта               */
 int     wd                     /* Ширина импорта                       */
);

_APPTLFUNC IMPORT *begimp                  /* Начать импорт                        */
(
 char   *fname,                 /* Название файла импорта               */
 int     wd                     /* Ширина импорта                       */
);

_APPTLFUNC void endimp                     /* Кончить импорт                       */
(
 IMPORT *im                     /* Описатель импорта                    */ 
);

#ifdef __cplusplus
 } ;
#endif

#endif //__IMPTXT_H

/* <-- EOF --> */