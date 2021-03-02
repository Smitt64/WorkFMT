/*-----------------------------------------------------------------------*-

 File Name   : getmni.h  

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
 March 15,2002  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#include "rsl/getmoney.h"

#ifndef USE_FDECIMAL
    #undef  RSL_DECIMAL_CONST
    #include "fdecimal.h"
#endif

#ifndef RSL_RESTRICT_LDBL
    #define RSL_RESTRICT_LDBL
#endif

#include "rsldoubl.h"

// -----------------------------------------------------------------------------
#ifdef USE_FDECIMAL
    static void RSL_doub2MoneyLoc(double val, FDecimal_t *res)
    {
     FMoney  result(val);

     result.get(res);
    }

    static double RSL_getDoubleLoc(FDecimal_t *val)
    {
     double  ret;
     FMoney  par(*val);

     ret = RSL_CAST_MON_TO_DBL(par);

     return ret;
    } 

    #ifndef _MSC_VER
        static void RSL_doubL2MoneyLoc(long double val, FDecimal_t *res)
        {
         FMoney  result(val);

         result.get(res);
        }

        static long double RSL_getLongDoubleLoc(FDecimal_t *val)
        {
         long double  ret;
         FMoney  par(*val);

         ret = RSL_CAST_LMON_TO_LDBL(par);

         return ret;
        }
    #endif
#endif // USE_FDECIMAL

// -----------------------------------------------------------------------------
static int IsMoneyDoubleInEXE(void)
{
 static int mode = -1;

 if(mode == -1)
  {
   TGetCIntf    data;
   TStdModIntf *ptr;

   data.intfName = "system";
   data.intf     = NULL;

   RslSendMes(IM_GET_C_INTF, &data);

   ptr = (TStdModIntf *)data.intf;

   if(ptr && ptr->GetVersion() > 1)
     mode = ptr->GetMoneyFormat();
   else
     mode = 1;  // Assume long double 
  }

 return mode;
}

// -----------------------------------------------------------------------------
#ifdef USE_FDECIMAL

    #ifdef BUILD_DLM
        bool GetMoneyFromValue(VALUE *val, FDecimal_t *ptr)
    #else
        bool GetMoneyFromValueTools(VALUE *val, FDecimal_t *ptr)
    #endif
        {
         if(val->v_type == V_MONEY || val->v_type == V_MONEY_DBL10)
          {
           if(!IsMoneyDoubleInEXE())
             memmove(ptr, &val->value.monval, sizeof(FDecimal_t));
           else if(val->v_type == V_MONEY)
             RSL_doub2MoneyLoc(val->value.doubval, ptr);
           else
             #ifdef _MSC_VER
                 RSL_doub2MoneyLoc(ToDouble(&val->value.doubvalL), ptr);
             #else 
                 RSL_doubL2MoneyLoc(val->value.doubvalL, ptr);
             #endif

           return true;
          }

         return false;
        }

    // -------------------------------------------------------------------------
    #ifdef BUILD_DLM
        bool GetMoneyFromValue (VALUE *val,lmoney& mn)                   
    #else
        bool GetMoneyFromValueTools (VALUE *val,lmoney& mn)                   
    #endif
        {
         FDecimal_t FDec;

         #ifdef BUILD_DLM
             if(GetMoneyFromValue(val, &FDec))
         #else
             if(GetMoneyFromValueTools(val, &FDec))
         #endif
              {
               mn = lmoney(FDec);

               return true;
              }

           return false;   
        }

    // -------------------------------------------------------------------------
    #ifdef BUILD_DLM
        bool SetMoneyToValue(VALUE *val, FDecimal_t *ptr)
    #else
        bool SetMoneyToValueTools(VALUE *val, FDecimal_t *ptr)
    #endif
        {
         val->v_type = V_MONEY;

         if(!IsMoneyDoubleInEXE())
           memmove(&val->value.monval, ptr, sizeof(FDecimal_t));
         else if(IsMoneyDoubleInEXE() == 2)
            val->value.doubval = RSL_getDoubleLoc(ptr);
         else if(IsMoneyDoubleInEXE() == 1) // long double
          {
           val->v_type = V_MONEY_DBL10;

           #ifdef _MSC_VER
               ToLDouble(RSL_getDoubleLoc(ptr), &val->value.doubval);
           #else
               val->value.doubvalL = RSL_getLongDoubleLoc(ptr);
           #endif
          }

         return true;
        }

    // -------------------------------------------------------------------------
    #ifdef BUILD_DLM
        bool SetMoneyToValue(VALUE *val, lmoney &mn)
    #else
        bool SetMoneyToValueTools(VALUE *val, lmoney &mn)
    #endif
        {
         FDecimal_t  FDec;

         mn.get(&FDec);

         #ifdef BUILD_DLM
             return SetMoneyToValue(val, &FDec);
         #else
             return SetMoneyToValueTools(val, &FDec);
         #endif
        }

    // -------------------------------------------------------------------------
    #ifdef USE_NUMERIC
        #ifdef BUILD_DLM
            bool GetMoneyFromValue(VALUE *val, Numeric &mn)
        #else
            bool GetMoneyFromValueTools(VALUE *val, Numeric &mn)
        #endif
            {
             mn = *((Numeric *)RslGetNumericFrom(val));

             return true;
            }

        // -------------------------------------------------------------------------
        #ifdef BUILD_DLM
            bool SetMoneyToValue(VALUE *val, const Numeric &mn)
        #else
            bool SetMoneyToValueTools(VALUE *val, const Numeric &mn)
        #endif
            {
             *RslGetNumericFrom(val) = mn;

             val->v_type = V_NUMERIC;

             return true;
            }
    #endif // USE_NUMERIC
#endif //  USE_FDECIMAL

// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
    bool GetMoneyFromValue(VALUE *val, double *ptr)
    {
     if(val->v_type == V_MONEY || val->v_type == V_MONEY_DBL10)
      {
       if(!IsMoneyDoubleInEXE())
        {
         FMoney  *p = (FMoney *)&val->value.monval;

         *ptr = p->castDouble();
        }
       else if(val->v_type == V_MONEY)
         *ptr = val->value.doubval;
       else
         #ifdef _MSC_VER
             *ptr = ToDouble(&val->value.doubval);
         #else
             *ptr = val->value.doubvalL;
         #endif

        return true;
      }

     return false;
    }
#else
    bool GetMoneyFromValueTools(VALUE *val, double &ptr)
    {
     if(val->v_type == V_MONEY || val->v_type == V_MONEY_DBL10)
      {
       if(!IsMoneyDoubleInEXE())
        {
         FMoney  *p = (FMoney *)&val->value.monval;

         ptr = p->castDouble();
        }
       else if(val->v_type == V_MONEY)
         ptr = val->value.doubval;
       else
         #ifdef _MSC_VER
             ptr = ToDouble(&val->value.doubval);
         #else
             ptr = val->value.doubvalL;
         #endif

        return true;
      }

     return false;
    }
#endif

// -----------------------------------------------------------------------------
#ifdef BUILD_DLM
    bool SetMoneyToValue(VALUE *val, double *ptr)
    {
     val->v_type = V_MONEY;

     if(!IsMoneyDoubleInEXE())
       *((FMoney *)&val->value.monval) = FMoney(*ptr);
     else
       val->value.doubval = *ptr;

     return true;
    }
#else
    bool SetMoneyToValueTools(VALUE *val, double ptr)
    {
     val->v_type = V_MONEY;

     if(!IsMoneyDoubleInEXE())
       *((FMoney *)&val->value.monval) = FMoney(ptr);
     else
       val->value.doubval = ptr;

     return true;
    }
#endif

/* <-- EOF --> */