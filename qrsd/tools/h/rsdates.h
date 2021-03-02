/*******************************************************************************
 FILE         :   RSDATES.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   Инструментарий для работы с датами

 PROGRAMMED BY:   VECsoft 13.11.1992

 CREATION DATE:   16 Oct 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/rsdates.h $
                  $Revision: 3 $
                  $Date: 23.03.00 17:43 $
                  $Author: Lebedev $
                  $Modtime: 23.03.00 17:40 $
*******************************************************************************/

#ifndef __RSDATES_H
#define __RSDATES_H

#include <bdate.h>
#include <tooldef.h>

#ifdef __cplusplus
extern "C" {
#endif

_TOOLEXP bdate rsMaxDate(void);
_TOOLEXP btime rsMaxTime(void);
                         
_TOOLEXP bdate rsZeroDate(void);
_TOOLEXP btime rsZeroTime(void);

_TOOLEXP bdate rsFirstValidDate(void);

// -----------------------------------------------
// Сравнить даты 1- первая больше
_TOOLEXP int rsDateCmp(const bdate *rec1, const bdate *rec2);

// -----------------------------------------------
// Сравнить время
_TOOLEXP int rsTimeCmp(const btime *rec1, const btime *rec2);

// -----------------------------------------------
// Вернуть количество дней от Р.Х. до заданной даты по календарю
_TOOLEXP long rsLDays(const bdate *bdD);

// -----------------------------------------------
// Перевести значение long в дату
_TOOLEXP void rsVLDateToBDate(long lDs, bdate *bdDate);

// -----------------------------------------------
// Вернуть количество дней от Р.Х. до заданной даты по календарю
_TOOLEXP long rsNDays(const bdate *b);

// -----------------------------------------------
// Вернуть количество дней от Р.Х. до заданной даты, считая, что в месяце 30 дней
_TOOLEXP long rsNDaysp(const bdate *b);

// -----------------------------------------------
// Вернуть последнее число месяца
_TOOLEXP int rsLastDayInMonth(const bdate *Date);

// -----------------------------------------------
// Найти завтрашний день
_TOOLEXP void rsTomorrow(const bdate *Today, bdate *Tomorrow);

// -----------------------------------------------
// Найти вчерашний день
_TOOLEXP void rsYesterday(const bdate *today, bdate *yesterday);

// -----------------------------------------------
// Получить наименование месяца заданной даты
_TOOLEXP char *rsMonthName(const bdate *bd, char *buf);

// -----------------------------------------------
// Получить номер дня недели (0 - воскресенье, 1 - понедельник и т.д.)
_TOOLEXP int rsGetDayOfWeek(const bdate *dt);

#ifdef __cplusplus
 } ;
#endif

/* В прикладном коде появились свои аналоги
#ifdef __cplusplus
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // Общая информация для операторов с датами:
    // 1. Действия операторов распространяются на диапазон дат [BDATE_FIRST_VALID, BDATE_MAX]
    // 2. Если в результате действий дата выходит за границы диапазона, ее значение
    //    устанавливается равным BDATE_FIRST_VALID либо BDATE_MAX в зависимости от операции.
    // -----------------------------------------------------------------------------
    // Оператор ++ для даты
    _TOOLEXP bdate &operator ++ (bdate &d);

    // -----------------------------------------------------------------------------
    // Оператор -- для даты
    _TOOLEXP bdate &operator -- (bdate &d);

    // -----------------------------------------------------------------------------
    // Оператор + int для даты
    _TOOLEXP bdate &operator + (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // Оператор - int для даты
    _TOOLEXP bdate &operator - (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // Оператор += int для даты
    _TOOLEXP bdate &operator += (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // Оператор -= int для даты
    _TOOLEXP bdate &operator -= (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // Оператор вычитания дат. Возвращает количество дней между двумя датами.
    _TOOLEXP int operator - (const bdate &ld, const bdate &rd);
#endif
*/

/*******************************************************************************
                        Old names
*******************************************************************************/

#define  DateCmp(d1, d2)        rsDateCmp((d1), (d2))
#define  TimeCmp(t1, t2)        rsTimeCmp((t1), (t2))
#define  lDays(d)               rsLDays(d)
#define  vLDateToBDate(ld, bd)  rsVLDateToBDate((ld), (bd))
#define  ndays(d)               rsNDays(d)
#define  ndaysp(d)              rsNDaysp(d)
#define  LastDayInMonth(d)      rsLastDayInMonth(d)
#define  Tomorrow(d1, d2)       rsTomorrow((d1), (d2))
#define  Yesterday(d1, d2)      rsYesterday((d1), (d2))
#define  month(d ,s)            rsMonthName((d), (s))
#define  GetDayOfWeek(d)        rsGetDayOfWeek(d)

/*******************************************************************************
                        Max, min and zero values
*******************************************************************************/

#define  BDATE_MAX          (rsMaxDate())
#define  BDATE_MIN          (rsZeroDate())
#define  BDATE_ZERO         (rsZeroDate())
#define  BDATE_FIRST_VALID  (rsFirstValidDate())

#define  BTIME_MAX   (rsMaxTime())
#define  BTIME_MIN   (rsZeroTime())
#define  BTIME_ZERO  (rsZeroTime())

/*******************************************************************************
 db_bdate
 db_btime
 db_btimestamp
*******************************************************************************/
#if defined(DB_DATE_TYPES)
    #ifdef __cplusplus

        // ---------------------------------------
        inline int rsDateCmp(const db_bdate *d1, const db_bdate *d2)
        {
         bdate  b1 = *d1, b2 = *d2;
         
         return rsDateCmp(&b1, &b2);
        }

        // ---------------------------------------
        inline int rsTimeCmp(const db_btime *t1, const db_btime *t2)
        {
         btime  b1 = *t1, b2 = *t2;

         return rsTimeCmp(&b1, &b2);
        }

        // ---------------------------------------
        inline long rsLDays(const db_bdate *d)
        {
         bdate  b = *d;
         
         return rsLDays(&b);
        }

        // ---------------------------------------
        inline void rsVLDateToBDate(long l, db_bdate *d)
        {
         bdate  b;
         
         rsVLDateToBDate(l, &b);
         
         *d = b;
        }

        // ---------------------------------------
        inline long rsNDays(const db_bdate *d)
        {
         bdate  b = *d;
         
         return rsNDays(&b);
        }

        // ---------------------------------------
        inline long rsNDaysp(const db_bdate *d)
        {
         bdate  b = *d;
         
         return rsNDaysp(&b);
        }

        // ---------------------------------------
        inline int rsLastDayInMonth(const db_bdate *d)
        {
         bdate  b = *d;
         
         return rsLastDayInMonth(&b);
        }

        // ---------------------------------------
        inline char *rsMonthName(const db_bdate *d, char *buf)
        {
         bdate  b = *d;
         
         return rsMonthName(&b, buf);
        }

        // ---------------------------------------
        inline int rsGetDayOfWeek(const db_bdate *d)
        {
         bdate  b = *d;
         
         return rsGetDayOfWeek(&b);
        }
    #endif
#endif //defined(DB_DATE_TYPES)

#endif // __RSDATES_H

/* <-- EOF --> */