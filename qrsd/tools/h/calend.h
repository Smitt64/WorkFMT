/*************************************************************************
   File Name      : calend.h
   Programmer     : Elena Lebedeva

   Creation       : 8 Apr 2004
   Description    : Work with calendar-list
*************************************************************************/

#ifndef _CALEND_H_
#define _CALEND_H_

#include <getbdt.h>

#ifdef __cplusplus
 extern "C" {
#endif


typedef int (*FillWorkFlagsArr)(
                                int n_year,     /* номер года в календаре  */
                                int *ar_year    /* календарь         */
                               );

_APPTLFUNC void SetFillWorkFlags(FillWorkFlagsArr);

_APPTLFUNC int UseCalendar(bdate BaseDate, bdate *SelectDate);

#ifdef __cplusplus
 } ;
#endif

#endif // _CALEND_H_

/* <-- EOF --> */