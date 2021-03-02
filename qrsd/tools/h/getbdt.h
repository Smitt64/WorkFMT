// Комплекс:     TOOLS 3.2  (c) VECsoft 1992-1993, (c) R-Style 1993.
// Модуль:       Получение даты и времени DOS.
// Программист:  Эмиль Довидович.
// Информация:
// История:      03/10/93
//

#ifndef __GETBDT_H
#define __GETBDT_H

#include "tooldef.h"
#include "bdate.h"

#ifdef __cplusplus
 extern "C" {
#endif

_TOOLEXP void getbdate(bdate *bd);

_TOOLEXP void getbtime(btime *bt);

_TOOLEXP void getbtimestamp(btimestamp *btstamp);

_TOOLEXP void getbtimeEx(btime *bt);
_TOOLEXP void getbtimestampEx(btimestamp *btstamp);

#ifdef __cplusplus
 } ;
#endif


_TOOLEXP void getbtime_cpp(btime *timebtr);
_TOOLEXP void getbdate_cpp(bdate *datebtr);
_TOOLEXP void getbtimestamp_cpp(btimestamp *btstamp);


#if defined(DB_DATE_TYPES) 
    #ifdef __cplusplus

        inline void getbdate_cpp(db_bdate *d)
        {
         bdate  b;
         
         getbdate_cpp(&b);

         *d = b;
        }

        inline void getbtime_cpp(db_btime *d)
        {
         btime  b;
         
         getbtime(&b);
         
         *d = b;
        }

        inline void getbtimestamp_cpp(db_btimestamp *d)
        {
         btimestamp  b;
         
         getbtimestamp(&b);
         
         *d = b;
        }

        #ifndef GETBDT_COMPILE
            #define  getbdate(p)       getbdate_cpp(p)
            #define  getbtime(p)       getbtime_cpp(p)
            #define  getbtimestamp(p)  getbtimestamp_cpp(p)
        #endif 

    #endif // cpp
#endif //defined(DB_DATE_TYPES)

#endif // __GETBDT_H

/* <-- EOF --> */