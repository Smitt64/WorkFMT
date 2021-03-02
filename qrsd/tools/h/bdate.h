/*******************************************************************************
 FILE         :   BDATE.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   bdate & btime structures definitions and corresponding stuff

 PROGRAMMED BY:   Yakov E. Markovitch

 CREATION DATE:   30 Jun 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/bdate.h $
                  $Revision: 27 $
                  $Date: 7.09.00 12:22 $
                  $Author: Kormukhin $
                  $Modtime: 7.09.00 12:05 $
*******************************************************************************/

#ifndef __BDATE_H
#define __BDATE_H

#include <platform.h>

#define  BDATE


#if defined(RSL_MD_SUN)
    #include <rsdigit.h>
#endif //defined(RSL_MD_SUN)

/************************* Дата и время в формате Btrieve **************/

#include <packpsh1.h>


typedef struct bdate
      {
       unsigned char      day;     // День
       unsigned char      mon;     // Месяц

       #if !defined(RSL_MD_SUN)
           unsigned short year;    // Год
       #else
           db_ubaseint16  year;    // Год
       #endif
      } bdate;

typedef struct btime
      {
       unsigned char  hundr;       // Сотые доли секунды
       unsigned char  sec;         // Секунды
       unsigned char  min;         // Минуты
       unsigned char  hour;        // Часы
      } btime;

#include <packpop.h>

//   Macros for bdate & btime static initialisation
#if !defined(RSL_MD_SUN)
    #define  RSL_SCTR_BDATE(y, m, d)  { (d), (m), (y) }
#else
    #define  RSL_SCTR_BDATE(y, m, d)  { (d), (m), (db_uint16)(y) }
#endif

#define  RSL_SCTR_BTIME(h, m, s, hs)  { (hs), (s), (m), (h) }
/*
   Macros for convert bdate & btime to long
   Max and zero value
*/

#ifndef RSL_MD_SUN

    #define  RSL_BDATE_AS_ULONG(bd)  (*(unsigned long *)(&(bd)))
    #define  RSL_BTIME_AS_ULONG(bt)  (*(unsigned long *)(&(bt)))

    #define  BDATE_AS_ULONG_MAX      ((unsigned long)-1L)
    #define  BTIME_AS_ULONG_MAX      ((unsigned long)-1L)

    #define  BDATE_AS_ULONG_ZERO     ((unsigned long)0L)
    #define  BTIME_AS_ULONG_ZERO     ((unsigned long)0L)

#else

    #define  RSL_BDATE_AS_ULONG(bd)  (*(db_uint32 *)(&(bd)))
    #define  RSL_BTIME_AS_ULONG(bt)  (*(db_uint32 *)(&(bt)))

    #define  BDATE_AS_ULONG_MAX      ((db_uint32)-1L)
    #define  BTIME_AS_ULONG_MAX      ((db_uint32)-1L)

    #define  BDATE_AS_ULONG_ZERO     ((db_uint32)0L)
    #define  BTIME_AS_ULONG_ZERO     ((db_uint32)0L)

#endif

/*
   Checking bdate & btime for zero values
*/
#define  RSL_IS_NULL_BDATE(bd) (!RSL_BDATE_AS_ULONG(bd))
#define  RSL_IS_NULL_BTIME(bt) (!RSL_BTIME_AS_ULONG(bt))

#include <packpsh1.h>

typedef struct btimestamp
      {
       bdate  date;
       btime  time;
      } btimestamp;

#include <packpop.h>


#ifdef __cplusplus

// -----------------------------------------------------------------------------
//   bdate operators
inline bool operator < (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) < RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator > (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) > RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator == (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) == RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator <= (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) <= RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator >= (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) >= RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator != (const bdate &d1, const bdate &d2)
{
 return RSL_BDATE_AS_ULONG(d1) != RSL_BDATE_AS_ULONG(d2);
}

// -----------------------------------------------
inline bool operator ! (const bdate &d)
{
 return RSL_IS_NULL_BDATE(d);
}

// -----------------------------------------------------------------------------
//   btime operators
inline bool operator < (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) < RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator > (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) > RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator == (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) == RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator <= (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) <= RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator >= (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) >= RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator != (const btime &t1, const btime &t2)
{
 return RSL_BTIME_AS_ULONG(t1) != RSL_BTIME_AS_ULONG(t2);
}

// -----------------------------------------------
inline bool operator ! (const btime &t)
{
 return RSL_IS_NULL_BTIME(t);
}

// -----------------------------------------------------------------------------
//   btimestamp operators
inline bool operator < (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) < RSL_BDATE_AS_ULONG(st2.date))
   return true;

 if(RSL_BDATE_AS_ULONG(st1.date) > RSL_BDATE_AS_ULONG(st2.date))
   return false;

 return RSL_BTIME_AS_ULONG(st1.time) < RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator > (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) > RSL_BDATE_AS_ULONG(st2.date))
   return true;

 if(RSL_BDATE_AS_ULONG(st1.date) < RSL_BDATE_AS_ULONG(st2.date))
   return false;

 return RSL_BTIME_AS_ULONG(st1.time) > RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator == (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) != RSL_BDATE_AS_ULONG(st2.date))
   return false;

 return RSL_BTIME_AS_ULONG(st1.time) == RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator <= (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) < RSL_BDATE_AS_ULONG(st2.date))
   return true;

 if(RSL_BDATE_AS_ULONG(st1.date) > RSL_BDATE_AS_ULONG(st2.date))
   return false;

 return RSL_BTIME_AS_ULONG(st1.time) <= RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator >= (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) > RSL_BDATE_AS_ULONG(st2.date))
   return true;

 if(RSL_BDATE_AS_ULONG(st1.date) < RSL_BDATE_AS_ULONG(st2.date))
   return false;

 return RSL_BTIME_AS_ULONG(st1.time) >= RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator != (const btimestamp &st1, const btimestamp &st2)
{
 if(RSL_BDATE_AS_ULONG(st1.date) != RSL_BDATE_AS_ULONG(st2.date))
   return true;

 return RSL_BTIME_AS_ULONG(st1.time) != RSL_BTIME_AS_ULONG(st2.time);
}

// -----------------------------------------------
inline bool operator ! (const btimestamp &st)
{
 if(!RSL_IS_NULL_BDATE(st.date))
   return false;

 return RSL_IS_NULL_BTIME(st.time);
}

#endif // __cplusplus

/*******************************************************************************
 db_bdate
 db_btime
 db_btimestamp
*******************************************************************************/
/* [!!!:x64]
#if defined(RSL_PL_WIN64)

    #ifdef __cplusplus

        #include <rsdbtype.hpp>

        #define DB_DATE_TYPES

        typedef db_type<bdate>        db_bdate;
        typedef db_type<btime>        db_btime;
        typedef db_type<btimestamp>   db_btimestamp;

    #endif // __cplusplus

#else //!defined(RSL_PL_WIN64)
*/
    typedef bdate        db_bdate;
    typedef btime        db_btime;
    typedef btimestamp   db_btimestamp;
/*
#endif //!defined(RSL_PL_WIN64)
*/

#ifdef __cplusplus
extern "C" {
#endif

void RslSetBtimeWin32(btime *bt);

#ifdef __cplusplus
}
#endif

#endif // __BDATE_H

/* <-- EOF --> */