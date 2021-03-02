/*-----------------------------------------------------------------------*-

 File Name   : idecimal.hpp

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: /RS-Tools/Interpreter RSL/rsl/idecimal.hpp $
               $Revision: 5 $
               $Date: 22.06.00 20:29 $
               $Author: Kubrin $
               $Modtime: 22.06.00 20:02 $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 May 29,2000  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/
#ifndef __IDECIMAL_HPP
#define __IDECIMAL_HPP


// -----------------------------------------------------------------------------
#ifdef USE_FDECIMAL
    #include "fdecimal.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    // -----------------------------------------------------------------------------
    #ifdef USE_NUMERIC
        Numeric_t                *AllocNumeric();
        void                      FreeNumeric(Numeric_t *val);
        RSLLINK Numeric_t *RSLAPI RslGetNumericFrom(VALUE *val);
        void                      RslReleaseNumeric(VALUE *val);


        void BtrDec2NumericRsl(void *data, Numeric_t *num, int dec);
        void Numeric2BtrDecRsl(void *ptr, const Numeric_t *num, int p);

        void Numeric2DB       (void *ptr, const Numeric_t *num);

        bool               RSL_numericFromDec(const FDecimal_t *val, Numeric_t *res);
        bool               RSL_DecFromNumeric(const Numeric_t *val, FDecimal_t *res);

        long               RSL_getInteger_Numeric      (Numeric_t *val);
        int64_t            RSL_getBigInteger_Numeric   (Numeric_t *val);
        void               RSL_numeric_round           (Numeric_t *val, int pos, Numeric_t *res);
        void               RSL_numeric_roundEx         (Numeric_t *val, int digits, int m, unsigned int roundDigit, Numeric_t *res);
        int                RSL_numeric_to_str          (Numeric_t *val, char *buff, int w, unsigned int p, int ljust);
        bool               RSL_str_to_numeric          (const char *s, unsigned int p, int size, Numeric_t *res);
        int                RSL_isZero_numeric          (Numeric_t *val);
        long double RSLAPI RSL_getLongDouble_Numeric_MN(Numeric_t *val);
        double             RSL_getDouble_Numeric       (Numeric_t *val);
        long double        RSL_getLongDouble_Numeric   (Numeric_t *val);
        double             RSL_getDouble_Numeric_MN    (Numeric_t *val);
        void               RSL_MulNumericInt           (Numeric_t *op1, long op2, Numeric_t *res);
        void               RSL_DivNumericInt           (Numeric_t *op1, long op2, Numeric_t *res);
        void               RSL_NumericFloor            (Numeric_t *op1, Numeric_t *res);
        void               RSL_int2Numeric             (long val, Numeric_t *res);
        void               RSL_bigint2Numeric          (int64_t val, Numeric_t *res);
        void               RSL_int2Numeric_MN          (long val, Numeric_t *res);
        void               RSL_bigint2Numeric_MN       (int64_t val, Numeric_t *res);
        void               RSL_Numeric2Money           (Numeric_t *res);
        void               RSL_doub2Numeric            (double val, Numeric_t *res);
        void               RSL_doubL2Numeric           (long double val, Numeric_t *res);
        void RSLAPI        RSL_doub2Numeric_MN         (double val, Numeric_t *res);
        void RSLAPI        RSL_doubL2Numeric_MN        (long double val, Numeric_t *res);
        void               RSL_doubLms2Numeric_MN      (void *data, Numeric_t *res);
        void               AbsNumeric                  (Numeric_t *val, Numeric_t *res);
        bool               EqNumeric                   (Numeric_t *op1, Numeric_t *op2);
        bool               LtNumeric                   (Numeric_t *op1, Numeric_t *op2);
        bool               GtNumeric                   (Numeric_t *op1, Numeric_t *op2);
        void               AddNumeric                  (Numeric_t *op1, Numeric_t *op2, Numeric_t *res);
        void               SubNumeric                  (Numeric_t *op1, Numeric_t *op2, Numeric_t *res);
        void               MulNumeric                  (Numeric_t *op1, Numeric_t *op2, Numeric_t *res);
        void               DivNumeric                  (Numeric_t *op1, Numeric_t *op2, Numeric_t *res);

        void               RSL_rubtostr_numeric        (Numeric_t *val, char *buff, int prec);
        void               RSL_SumToStr_numeric        (Numeric_t *val, char *str, char *kop, int prec, int code);
        void               RSL_LocalRubToStr_numeric   (Numeric_t *val, char *str, int prec);
        void               RSL_LocalCurToStr_numeric   (Numeric_t *val, char *str, int CurCode, int prec);

        void               ModNumeric                  (Numeric_t *op1, Numeric_t *op2, Numeric_t *res);

        void               RSL_NumToStr                (Numeric_t *val, char *ioutbuf, char *n1, char *n2, char *n3, int rod, int prec);
    #endif

    void                         SetFDecHandler         (bool set);

    int                          RSL_dmoney_to_str      (FDecimal_t *val, char *buff, int w, unsigned int p, int ljust);
    bool                         RSL_str_to_dmoney      (const char *s, unsigned int p, int size, FDecimal_t *res);
    bool                         RSL_str_to_decimal     (const char *s, unsigned int p, int size, FDecimal_t *res);

    int                          RSL_isZero             (FDecimal_t *val);
    void                         RSL_rubtostr           (FDecimal_t *val, char *buff);
    void                         RSL_SumToStr           (FDecimal_t *val, char *str, char *kop, int code);
    void                         RSL_LocalRubToStr      (FDecimal_t *val, char *str);
    void                         RSL_LocalCurToStr      (FDecimal_t *val, char *str, int CurCode);

    double                       RSL_getDoubleDec       (FDecimal_t *val);
    long double                  RSL_getLongDoubleDec   (FDecimal_t *val);

    RSLLINK double RSLAPI        RSL_getDouble          (FDecimal_t *val);
    RSLLINK long   RSLAPI double RSL_getLongDouble      (FDecimal_t *val);

    void                         RSL_MulFDecimalInt     (FDecimal_t *op1, long op2, FDecimal_t *res);
    void                         RSL_MulFDecimalBigInt  (FDecimal_t *op1, int64_t op2, FDecimal_t *res);
    void                         RSL_DivFDecimalInt     (FDecimal_t *op1, long op2, FDecimal_t *res);
    void                         RSL_DivFDecimalBigInt  (FDecimal_t *op1, int64_t op2, FDecimal_t *res);
    void                         RSL_MulFDecimalFDecimal(FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res); 
    void                         RSL_DivFDecimalFDecimal(FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res);

    void                         RSL_FDecimalFloor      (FDecimal_t *op1, FDecimal_t *res);

    void                         RSL_int2Decimal        (long val, FDecimal_t *res);
    void                         RSL_bigint2Decimal     (int64_t val, FDecimal_t *res);
    void                         RSL_doub2Decimal       (double val, FDecimal_t *res);
    void                         RSL_doubL2Decimal      (long double val, FDecimal_t *res);
    void                         RSL_Dec2Money          (FDecimal_t *res);

    void                         RSL_int2Money          (long intval, FDecimal_t *res);
    void                         RSL_bigint2Money       (int64_t intval, FDecimal_t *res);
    RSLLINK void RSLAPI          RSL_doub2Money         (double val, FDecimal_t *res);
    RSLLINK void RSLAPI          RSL_doubL2Money        (long double val, FDecimal_t *res);

    void                AbsFDecimal     (FDecimal_t *val, FDecimal_t *res);

    bool                EqFDecimal      (FDecimal_t *op1, FDecimal_t *op2);
    bool                LtFDecimal      (FDecimal_t *op1, FDecimal_t *op2);
    bool                GtFDecimal      (FDecimal_t *op1, FDecimal_t *op2);

    void                RSL_FDecimal2DB (FDecimal_t *op, DBFDecimal_t *res);
    void                RSL_DB2FDecimal (DBFDecimal_t *op, FDecimal_t *res);

    RSLLINK void RSLAPI RSL_GetLineSum  (int line, FDecimal_t *res);

    void                AddFDecimal     (FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res);
    void                SubFDecimal     (FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res);
    void                MulFDecimal     (FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res);
    void                DivFDecimal     (FDecimal_t *op1, FDecimal_t *op2, FDecimal_t *res);

    void                RSL_dmoney_round(FDecimal_t *val, int pos, FDecimal_t *res);

    void                RSL_ReadMoney   (FDecimal_t *res);

    #ifdef __cplusplus
    }
    #endif
#else
    #ifdef __cplusplus
    extern "C" {
    #endif

    RSLLINK void RSLAPI RSL_GetLineSum(int line, lmoney *res);

    #ifdef __cplusplus
    }
    #endif
#endif

// -----------------------------------------------------------------------------
#ifndef RSL_DLM
    lmoney GetLineSum(int line);
#endif

#ifdef __cplusplus
extern "C" {
#endif

RSLLINK bool RSLAPI RSL_GetLineSumEx(int line, VALTYPE v_type, void *res);

#ifdef __cplusplus
}
#endif

#endif

/* <-- EOF --> */