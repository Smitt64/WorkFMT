/*******************************************************************************
 FILE         :   CALENDAR.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Work witch 'calendar'

 PROGRAMMED BY:   Merkulovitch M.V.

 CREATION DATE:   12 Jan 1993

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/Tools/H/calendar.h $
                  $Revision: 2 $
                  $Date: 5.11.98 12:43 $
                  $Author: Lebedev $
                  $Modtime: 5.11.98 12:41 $
*******************************************************************************/

#ifndef __CALENDAR_H
#define __CALENDAR_H

#ifdef __cplusplus
 extern "C" {
#endif

#define KOLYEAR 3

#include <packpsh1.h>

 typedef struct
       {
        int     Key;
        int     Num[366];
       } CALENDAR;

#include <packpop.h>

extern char CName[];

int get_cal(void);                   /* ������ ����� �� 䠩��    */
int RazDay(bdate *c1, bdate *c2);    /* ������� ����� �� ��������� */
int RazDay(bdate *c1, bdate *c2);    /* ������� ����� �� ��������� */
int ord_nday(bdate *c1);             /* ���浪��� ����� 1-��� ��� ����� � ���� ( 1-366) */

#ifdef __cplusplus
 } ;
#endif

#endif // __CALENDAR_H

/* <-- EOF --> */