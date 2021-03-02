/*-----------------------------------------------------------------------*-

 File Name   : getmoney.h

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 

 VERSION INFO: $Archive: $
               $Revision: $
               $Date: $
               $Author: $
               $Modtime: $

 Copyright (c) 1991 - 1998 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 January 31,2001  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifdef __cplusplus

#ifdef BUILD_DLM
    #ifdef USE_FDECIMAL
        bool GetMoneyFromValue(VALUE *val, FDecimal_t *ptr);
        bool GetMoneyFromValue(VALUE *val, FMoney     &mn);
        bool GetMoneyFromValue(VALUE *val, FDecimal   &mn);
        bool SetMoneyToValue  (VALUE *val, FDecimal_t *ptr);
        bool SetMoneyToValue  (VALUE *val, FMoney     &mn);
        bool SetMoneyToValue  (VALUE *val, FDecimal   &mn);
    #endif

    #ifdef USE_NUMERIC
        bool GetMoneyFromValue(VALUE *val,       Numeric &mn);
        bool SetMoneyToValue  (VALUE *val, const Numeric &mn);
    #endif

    bool GetMoneyFromValue(VALUE *val, double *ptr);
    bool SetMoneyToValue  (VALUE *val, double *ptr);
#else // !BUILD_DLM
    #ifdef USE_FDECIMAL
        bool GetMoneyFromValueTools(VALUE *val, FDecimal_t *ptr);
        bool GetMoneyFromValueTools(VALUE *val, FMoney     &mn);
        bool GetMoneyFromValueTools(VALUE *val, FDecimal   &mn);
        bool SetMoneyToValueTools  (VALUE *val, FDecimal_t *ptr);
        bool SetMoneyToValueTools  (VALUE *val, FMoney     &mn);
        bool SetMoneyToValueTools  (VALUE *val, FDecimal   &mn);
    #endif

    #ifdef USE_NUMERIC
        bool GetMoneyFromValueTools(VALUE *val,       Numeric &mn);
        bool SetMoneyToValueTools  (VALUE *val, const Numeric &mn);
    #endif

    bool GetMoneyFromValueTools(VALUE *val, double &ptr);
    bool SetMoneyToValueTools  (VALUE *val, double ptr);
#endif // BUILD_DLM

#endif

/* <-- EOF --> */