/*******************************************************************************
 FILE         :   RSROUND.H

 COPYRIGHT    :   R-Style SoftLab, 2004

 DESCRIPTION  :   ‘-ции дл€ округлени€ и получени€ абсолютного значени€.

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   03 Aug 2004

 VERSION INFO :   

*******************************************************************************/

#ifndef __RSROUND_H
#define __RSROUND_H

#include "rstypes.h"
#include "tooldef.h"

// Overloaded functions
#ifdef __cplusplus

#if defined(USE_FDECIMAL)
    _TOOLFUNC FDecimal rs_abs     (const FDecimal &);
    _TOOLFUNC FDecimal rs_round   (const FDecimal &, int decimals);
    _TOOLFUNC FDecimal rs_truncate(const FDecimal &, int decimals);

    _TOOLFUNC FMoney   rs_abs     (const FMoney &);
    _TOOLFUNC FMoney   rs_round   (const FMoney &, int decimals);
    _TOOLFUNC FMoney   rs_truncate(const FMoney &, int decimals);
#endif

#if defined(USE_NUMERIC)
    _TOOLFUNC Numeric  rs_abs     (const Numeric &);
    _TOOLFUNC Numeric  rs_round   (const Numeric &, int decimals);
    _TOOLFUNC Numeric  rs_truncate(const Numeric &, int decimals);
#endif

#endif //__cplusplus

#endif //__RSROUND_H

/* <-- EOF --> */