/*******************************************************************************
 FILE         :   RSDATES.H

 COPYRIGHT    :   R-Style Software Lab, 1992, 1998

 DESCRIPTION  :   �����㬥��਩ ��� ࠡ��� � ��⠬�

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
// �ࠢ���� ���� 1- ��ࢠ� �����
_TOOLEXP int rsDateCmp(const bdate *rec1, const bdate *rec2);

// -----------------------------------------------
// �ࠢ���� �६�
_TOOLEXP int rsTimeCmp(const btime *rec1, const btime *rec2);

// -----------------------------------------------
// ������ ������⢮ ���� �� �.�. �� �������� ���� �� ���������
_TOOLEXP long rsLDays(const bdate *bdD);

// -----------------------------------------------
// ��ॢ��� ���祭�� long � ����
_TOOLEXP void rsVLDateToBDate(long lDs, bdate *bdDate);

// -----------------------------------------------
// ������ ������⢮ ���� �� �.�. �� �������� ���� �� ���������
_TOOLEXP long rsNDays(const bdate *b);

// -----------------------------------------------
// ������ ������⢮ ���� �� �.�. �� �������� ����, ����, �� � ����� 30 ����
_TOOLEXP long rsNDaysp(const bdate *b);

// -----------------------------------------------
// ������ ��᫥���� �᫮ �����
_TOOLEXP int rsLastDayInMonth(const bdate *Date);

// -----------------------------------------------
// ���� �����譨� ����
_TOOLEXP void rsTomorrow(const bdate *Today, bdate *Tomorrow);

// -----------------------------------------------
// ���� ���譨� ����
_TOOLEXP void rsYesterday(const bdate *today, bdate *yesterday);

// -----------------------------------------------
// ������� ������������ ����� �������� ����
_TOOLEXP char *rsMonthName(const bdate *bd, char *buf);

// -----------------------------------------------
// ������� ����� ��� ������ (0 - ����ᥭ�, 1 - �������쭨� � �.�.)
_TOOLEXP int rsGetDayOfWeek(const bdate *dt);

#ifdef __cplusplus
 } ;
#endif

/* � �ਪ������ ���� ������ ᢮� �������
#ifdef __cplusplus
    // -----------------------------------------------------------------------------
    // -----------------------------------------------------------------------------
    // ���� ���ଠ�� ��� �����஢ � ��⠬�:
    // 1. ����⢨� �����஢ �����࠭����� �� �������� ��� [BDATE_FIRST_VALID, BDATE_MAX]
    // 2. �᫨ � १���� ����⢨� ��� ��室�� �� �࠭��� ���������, �� ���祭��
    //    ��⠭���������� ࠢ�� BDATE_FIRST_VALID ���� BDATE_MAX � ����ᨬ��� �� ����樨.
    // -----------------------------------------------------------------------------
    // ������ ++ ��� ����
    _TOOLEXP bdate &operator ++ (bdate &d);

    // -----------------------------------------------------------------------------
    // ������ -- ��� ����
    _TOOLEXP bdate &operator -- (bdate &d);

    // -----------------------------------------------------------------------------
    // ������ + int ��� ����
    _TOOLEXP bdate &operator + (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // ������ - int ��� ����
    _TOOLEXP bdate &operator - (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // ������ += int ��� ����
    _TOOLEXP bdate &operator += (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // ������ -= int ��� ����
    _TOOLEXP bdate &operator -= (bdate &d, int shift);

    // -----------------------------------------------------------------------------
    // ������ ���⠭�� ���. �����頥� ������⢮ ���� ����� ���� ��⠬�.
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