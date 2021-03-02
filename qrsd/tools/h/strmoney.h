/*******************************************************************************
 FILE         :   STRMONEY.H

 COPYRIGHT    :   R-Style Software Lab, 1998

 DESCRIPTION  :   Money<->string conversion

 PROGRAMMED BY:   Alexey Lebedev

 CREATION DATE:   18 Dec 1998

 VERSION INFO :   $Archive: /RS-Tools/Old_tools/tools/h/strmoney.h $
                  $Revision: 14 $
                  $Date: 1.09.00 19:27 $
                  $Author: Kormukhin $
                  $Modtime: 30.08.00 17:10 $
*******************************************************************************/

#ifndef __STRMONEY_H
#define __STRMONEY_H

#include "platform.h"
#include "tooldef.h"
#include "rstypes.h"


#ifdef __cplusplus
extern "C" {
#endif

/* Втавить в строку десятичную точку      */
_TOOLEXP int insdecp(
                     char        *s,        /* Строка для обработки            */
                     unsigned int p         /* Знаков после точки           */
                    );                        

/* Удалить из строки десятичную точку     */
_TOOLEXP char *remdecp(
                       char        *s,      /* Строка для обработки            */
                       unsigned int p,      /* Знаков после точки           */
                       int          size    /* Размер выделенной памяти для строки      */
                      );

// Втавить в строку разделители 1000
_TOOLEXP int AddSumGrouping(char *s);

// Удалить из строки завершающие нули
_TOOLEXP int TrimSumZeros(char *s);

// Convert string -> double, long double
_TOOLEXP double      str_to_double (const char *s, int p, int size);
_TOOLEXP long double str_to_ldouble(const char *s, int p, int size);

// Convert double, long double -> string
_TOOLEXP int         double_to_str (double val, char *buff, int w, int p, int ljust);
_TOOLEXP int         ldouble_to_str(long double val, char *buff, int w, int p, int ljust);

#ifdef __cplusplus
}
#endif


/*******************************************************************************
   dmoney, lmoney, decimal <-> string conversion
*******************************************************************************/

#if defined(__cplusplus) && !(defined(USE_FDECIMAL) && defined(RSL_PL_MS))
extern "C" {
#endif

    #if !defined(NUMERIC_AS_MONEY)

        // Convert string -> dmoney, lmoney, decimal
        _TOOLEXP dmoney  str_to_dmoney (const char *s, int p, int size);
        _TOOLEXP lmoney  str_to_lmoney (const char *s, int p, int size);
        _TOOLEXP decimal str_to_decimal(const char *s, unsigned int p, int size);

        // Convert dmoney, lmoney, decimal -> string
        _TOOLEXP int     dmoney_to_str (dmoney val, char *buff, int w, int p, int ljust);
        _TOOLEXP int     lmoney_to_str (lmoney val, char *buff, int w, int p, int ljust);
        _TOOLEXP int     decimal_to_str(decimal val, char *buff, int w, unsigned int p, int ljust);

    #endif //!defined(NUMERIC_AS_MONEY)

#if defined(__cplusplus) && !(defined(USE_FDECIMAL) && defined(RSL_PL_MS))
}
#endif

#if defined(USE_FDECIMAL)

    #ifdef __cplusplus
    extern "C" {
    #endif

        // ------------------------------------------------------------------------------
        //  These are versions for old money, made on double and long double

        _TOOLEXP double      str_to_dmoney_old(const char *s, int p, int size);
        _TOOLEXP long double str_to_lmoney_old(const char *s, int p, int size);

        _TOOLEXP int         dmoney_to_str_old(double val, char *buff, int w, int p, int ljust);
        _TOOLEXP int         lmoney_to_str_old(long double val, char *buff, int w, int p, int ljust);

    #ifdef __cplusplus
    }
    #endif

#endif //USE_FDECIMAL


/*******************************************************************************
   numeric <-> string conversion
*******************************************************************************/

#if defined(USE_NUMERIC)

    // Only for C++
    #ifdef __cplusplus

        _TOOLEXP void    str_to_numeric(Numeric *val, const char *s, int p, int size);
        _TOOLEXP int     numeric_to_str(const Numeric &val, char *buff, int w, int p, int ljust);

        // For backward compatible
        _TOOLEXP Numeric str_to_numeric(const char *s, int p, int size);

    #endif //__cplusplus

    // Special for C only
    #ifdef __cplusplus
    extern "C" {
    #endif

        _TOOLEXP void    str_to_numericT(Numeric_t *val, const char *s, int p, int size);
        _TOOLEXP int     numericT_to_str(const Numeric_t *val, char *buff, int w, int p, int ljust);

    #ifdef __cplusplus
    }
    #endif

#endif //USE_NUMERIC

#if defined(NUMERIC_AS_MONEY)

    // Convert string -> dmoney, lmoney, decimal
    #define  str_to_dmoney(s, p, size)   str_to_numeric((s), (p), (size))
    #define  str_to_lmoney(s, p, size)   str_to_numeric((s), (p), (size))
    #define  str_to_decimal(s, p, size)  str_to_numeric((s), (p), (size))

    // Convert dmoney, lmoney, decimal -> string
    #define  dmoney_to_str(val, buff, w, p, ljust)   numeric_to_str((val), (buff), (w), (p), (ljust))
    #define  lmoney_to_str(val, buff, w, p, ljust)   numeric_to_str((val), (buff), (w), (p), (ljust))
    #define  decimal_to_str(val, buff, w, p, ljust)  numeric_to_str((val), (buff), (w), (p), (ljust))

#endif //defined(NUMERIC_AS_MONEY)

#endif // __STRMONEY_H

/* <-- EOF --> */