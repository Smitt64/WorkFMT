//-*-----------------------------------------------------------------------*-
//
// File Name   : rsdbhd.hpp
//
// Description : 
//
// Source      : 
//
// Copyright (c) 1991 - 2002 by R-Style Softlab.
// All Rights Reserved.
//
//-*- History -------------------------------------------------------------*-
// June 23,2004  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-
#ifndef __RSDBHD_HPP
#define __RSDBHD_HPP

// -----------------------------------------------------------------------------
typedef unsigned long  RslHandle32;   

#ifdef RSL_PL_WIN64
    typedef unsigned __int64  RslHandle64;
#else

    struct  RslHandle64
         {
          unsigned long  lo;
          unsigned long  hi;

          #ifdef __cplusplus
              RslHandle64() {}
              RslHandle64(unsigned long val) { lo = val; hi = 0; }

              operator unsigned long () const { return lo; }
              RslHandle64 &operator = (unsigned long dt) { lo = dt; hi = 0; return *this; }
          #endif
         };


    #ifdef __cplusplus
        inline bool operator == (const RslHandle64 &val1, const RslHandle64 &val2)
        {
         return val1.lo == val2.lo && val1.hi == val2.hi;
        }     
    #endif

#endif


#ifdef __cplusplus

    // -----------------------------------------------------------------------------
    template<class T>
    class HandleCvt64
        {
         public:
         #ifdef RSL_PL_WIN64
             static RslHandle64 RslMakeHandle(T val)
             {
              return(RslHandle64)val;
             }

             static T RslMakePtr(RslHandle64 val)
             {
              return (T)val;
             }
         #else
             static RslHandle64 RslMakeHandle(T val)
             {
              return RslHandle64((unsigned long)val);
             }

             static T RslMakePtr(RslHandle64 val)
             {
              return (T)val.lo;
             }
         #endif
        };

    // -----------------------------------------------------------------------------
    template<class T>
    class HandleCvt32
        {
         public:
           static RslHandle32 RslMakeHandle(T val)
           {
            return (RslHandle32)val;
           }

           static T RslMakePtr(RslHandle32 val)
           {
           #ifdef RSL_PL_WIN64
               return (T)(unsigned __int64)val;
           #else
               return (T)val;
           #endif
           }
        };

    // -----------------------------------------------------------------------------
    template <class Type> class db_handle
    {
     public:
       operator Type () const
       {
        Type  val;

        memcpy(&val, data, sizeof(Type));

        return val;
       }

       db_handle<Type> operator = (Type val)
       {
        memcpy(data, &val, sizeof(Type));

        return *this;
       }

    protected:
       char data [sizeof (Type)];
    };

    // -----------------------------------------------------------------------------
    #if defined (RSL_PL_WIN32) && !defined (RSL_PL_WIN64)
        RSL_TPL_SPECIALIZATION class db_handle<RslHandle32>
        {
         public:
           operator RslHandle32() const
           {
            return data;
           }

           db_handle<RslHandle32> operator = (RslHandle32 val)
           {
            data = val;

            return *this;
           }

         protected:
           RslHandle32  data;
        };
    #endif

    // -----------------------------------------------------------------------------
    template <class Type, class SaveType, class CvtType> 
    class db_handleEx : public db_handle<SaveType>
    {
     public:
       operator Type () const
       {
        SaveType  h64 = db_handle<SaveType>::operator SaveType();

        return CvtType::RslMakePtr(h64);
       }

       
       db_handleEx<Type, SaveType, CvtType> operator = (Type val)
       {
        db_handle<SaveType>::operator = (CvtType::RslMakeHandle(val));

        return *this;
       }
       
       
       db_handleEx<Type, SaveType, CvtType> operator = (SaveType val)
       {
        db_handle<SaveType>::operator = (val);

        return *this;
       }
    };

    // -----------------------------------------------------------------------------
    template <class Type> 
    class db_handle64 : public db_handleEx<Type, RslHandle64, HandleCvt64<Type>>
        {
         public:
           db_handle64<Type> operator = (Type val)
           {
            db_handleEx<Type, RslHandle64, HandleCvt64<Type>>::operator = (val);

            return *this;
           }
          
           db_handle64<Type> operator = (RslHandle64 val)
           {
            db_handleEx<Type, RslHandle64, HandleCvt64<Type>>::operator = (val);

            return *this;
           }
        };

    // -----------------------------------------------------------------------------
    template <class Type> 
    class db_handle32: public db_handleEx<Type, RslHandle32, HandleCvt32<Type>>
        {
         public:
           db_handle32<Type> operator = (Type val)
           {
            db_handleEx<Type, RslHandle32, HandleCvt32<Type>>::operator = (val);

            return *this;
           }
           
           db_handle32<Type> operator = (RslHandle32 val)
           {
            db_handleEx<Type, RslHandle32, HandleCvt32<Type>>::operator = (val);

            return *this;
           }
        };
#endif // __cplusplus

#endif

/* <-- EOF --> */