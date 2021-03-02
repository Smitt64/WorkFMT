/*-----------------------------------------------------------------------*-

 File Name   : cracker.hpp

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
 April 2,2002  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __CRACKER_HPP
#define __CRACKER_HPP

// -----------------------------------------------------------------------------
template<class T>
class TRef
{ 
   // --------------------------------------------
   // --------------------------------------------
   TValRef  val;
 
 public:
   // --------------------------------------------
   TRef(TValRef &v) : val(v)
   {
   }


   TRef &operator = (T p)
   {
    val = p;

    return *this;
   }

   // --------------------------------------------
   operator T ()
   {
    return val;
   }
};

// -----------------------------------------------------------------------------
template<class T, int iid>
class TObjPtr : public TRsPtrX<T, iid>
    { 
     public:
       TObjPtr(IRsObj *p) : TRsPtrX<T, iid>(p) {}

   // --------------------------------------------
       TObjPtr(TValRef &v) : TRsPtrX<T, iid>((IRsObj *)v)
       {
        IRsObj  *ptr = v;


        if(ptr && !p)
          throw XRSComError(ptr->getAppObj(), RSL_STAT_NOINTF);
       }
    };

// -----------------------------------------------------------------------------
class TParmPtr : public TParmArray
    { 
     public:
   // --------------------------------------------
   TParmPtr(TValRef &v) : TParmArray((IParmInfo *)v)
   {
   }
    };

// -----------------------------------------------------------------------------
template<class T, int iid>
class TObjRefX : public TObjPtr<T, iid>
    { 
   // --------------------------------------------
   // --------------------------------------------
       TValRef val;

     public:
   // --------------------------------------------
   TObjRefX(TValRef &v) : TObjPtr<T, iid>(v), val(v)
   {
   }

   // --------------------------------------------
       T *operator = (T* lp)
       {
        T  *ptr = TRsPtrX<T, iid>::operator = (lp);


        val = ptr;

        return ptr;
       }

   // --------------------------------------------
       T *operator = (IRsObj *lp)
       {
        T  *ptr = TRsPtrX<T, iid>::operator = (lp);


        val = ptr;

        return ptr;
       }
    };

// -----------------------------------------------
typedef TRef<bool>                  TBoolRef;

typedef TRef<signed char>           TChrRef;
typedef TRef<unsigned char>         TUChrRef;

typedef TRef<short int>             TShortRef;
typedef TRef<unsigned short int>    TUShortRef;

typedef TRef<int>                   TIntRef;
typedef TRef<unsigned int>          TUIntRef;

typedef TRef<long>                  TLongRef;
typedef TRef<unsigned long>         TULongRef;

typedef TRef<double>                TDoubleRef;
typedef TRef<const char*>           TStrRef;

typedef TRef<IRsObj*>               TObjRef;

typedef TRef<bdate>                 TDateRef;
typedef TRef<btime>                 TTimeRef;
typedef TRef<btimestamp>            TDtTmRef;

#ifdef USE_FDECIMAL
    typedef TRef<FMoney>            TMoneyRef;
    typedef TRef<FDecimal>          TDecimalRef;
#endif

#ifdef USE_NUMERIC
    typedef TRef<Numeric>           TNumericRef;
#endif

// -----------------------------------------------                           
template <class T, class P1>
class Cracker1
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par);

   // --------------------------------------------
   Cracker1(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    TParmArray  prm(ptr);
    TRsStat     stat = (obj->*proc)(prm.retVal());

    return stat;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   pmf_t  proc;
   T     *obj;
};

// -----------------------------------------------------------------------------
template<class T, class P1>
inline Cracker1<T, P1> RsCracker(T *obj, TRsStat (T::*f)(P1 par))
{
 return Cracker1<T, P1>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2>
class Cracker2
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2);

   // --------------------------------------------
   Cracker2(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    TParmArray  prm(ptr);


    if(prm.getCount() < 1)
       throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

    TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0]);

    return stat;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   pmf_t  proc;
   T     *obj;
};

// -----------------------------------------------------------------------------
template<class T, class P1, class P2>
inline Cracker2<T, P1, P2> RsCracker(T *obj, TRsStat (T::*f)(P1 par, P2 par2))
{
 return Cracker2<T, P1, P2>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3>
class Cracker3
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3);

   // --------------------------------------------
   Cracker3(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    TParmArray  prm(ptr);


    if(prm.getCount() < 2)
      throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

    TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0], prm[1]);

    return stat;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   pmf_t  proc;
   T     *obj;
};

// -----------------------------------------------------------------------------
template<class T, class P1, class P2, class P3>
inline Cracker3<T, P1, P2, P3> RsCracker(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3))
{
 return Cracker3<T, P1, P2, P3>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4>
class Cracker4
{
 public:
   typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4);

   // --------------------------------------------
   Cracker4(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
   TRsStat operator () (IParmInfo *ptr)
   {
    TParmArray  prm(ptr);


    if(prm.getCount() < 3)
       throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

    TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0], prm[1], prm[2]);

    return stat;
   }

 private:
   // --------------------------------------------
   // --------------------------------------------
   pmf_t  proc;
   T     *obj;
};

// -----------------------------------------------------------------------------
template<class T, class P1, class P2, class P3, class P4>
inline Cracker4<T, P1, P2, P3, P4> RsCracker(T *obj, TRsStat(T::*f)(P1 par, P2 par2, P3 par3, P4 par4))
{
 return Cracker4<T, P1, P2, P3, P4>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5>
class Cracker5
    {
     public:
       typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5);

   // --------------------------------------------
   Cracker5(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
       TRsStat operator () (IParmInfo *ptr)
       {
        TParmArray  prm(ptr);


        if(prm.getCount() < 4)
          throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

        TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0], prm[1], prm[2], prm[3]);

        return stat;
       }

     private:
   // --------------------------------------------
   // --------------------------------------------
       pmf_t  proc;
       T     *obj;
    };

// -----------------------------------------------------------------------------
template<class T, class P1, class P2, class P3, class P4, class P5>
inline Cracker5<T, P1, P2, P3, P4, P5> RsCracker(T *obj, TRsStat(T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5))
{
 return Cracker5<T, P1, P2, P3, P4, P5>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6>
class Cracker6
    {
     public:
       typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6);

   // --------------------------------------------
   Cracker6(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
       TRsStat operator () (IParmInfo *ptr)
       {
        TParmArray  prm(ptr);


        if(prm.getCount() < 5)
          throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

        TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0], prm[1], prm[2], prm[3], prm[4]);

        return stat;
       }

     private:
   // --------------------------------------------
   // --------------------------------------------
       pmf_t  proc;
       T     *obj;
    };

// -----------------------------------------------------------------------------
template<class T, class P1, class P2, class P3, class P4, class P5, class P6>
inline Cracker6<T, P1, P2, P3, P4, P5, P6> RsCracker(T *obj, TRsStat(T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6))
{
 return Cracker6<T, P1, P2, P3, P4, P5, P6>(obj, f);
}

// -----------------------------------------------------------------------------
template <class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
class Cracker7
    {
     public:
       typedef TRsStat (T::*pmf_t)(P1 par1, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6, P7 par7);

   // --------------------------------------------
   Cracker7(T *objPtr, pmf_t p) : obj(objPtr), proc(p)
   {
   }

   // --------------------------------------------
       TRsStat operator () (IParmInfo *ptr)
       {
        TParmArray  prm(ptr);


        if(prm.getCount() < 6)
          throw XRSComError(ptr->getAppObj(), RSL_STAT_PRMCOUNT);

        TRsStat  stat = (obj->*proc)(prm.retVal(), prm[0], prm[1], prm[2], prm[3], prm[4], prm[5]);

        return stat;
       }

     private:
   // --------------------------------------------
   // --------------------------------------------
       pmf_t  proc;
       T     *obj;
    };

// -----------------------------------------------------------------------------
template<class T, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
inline Cracker7<T, P1, P2, P3, P4, P5, P6, P7> RsCracker(T *obj, TRsStat (T::*f)(P1 par, P2 par2, P3 par3, P4 par4, P5 par5, P6 par6, P7 par7))
{
 return Cracker7<T, P1, P2, P3, P4, P5, P6, P7>(obj, f);
}

// -----------------------------------------------------------------------------
template<class P1>
inline TRsStat RsMethod(TAppObj *app, IDispObj *obj, RSDISPID id, P1 par1)
{
 TParmArray  prm(app, 1, false);


 prm[0] = par1;

 return obj->invoke(id, RS_DISP_METHOD, prm, 0);
}

// -----------------------------------------------------------------------------
template<class P1, class P2>
inline TRsStat RsMethod(TAppObj *app, IDispObj *obj, RSDISPID id, P1 par1, P2 par2)
{
 TParmArray  prm(app, 2, false);


 prm[0] = par1;
 prm[1] = par2;

 return obj->invoke(id, RS_DISP_METHOD, prm, 0);
}

// -----------------------------------------------------------------------------
template<class P1, class P2, class P3>
inline TRsStat RsMethod(TAppObj *app, IDispObj *obj, RSDISPID id, P1 par1, P2 par2, P3 par3)
{
 TParmArray  prm(app, 2, false);


 prm[0] = par1;
 prm[1] = par2;
 prm[2] = par3;

 return obj->invoke(id, RS_DISP_METHOD, prm, 0);
}

// -----------------------------------------------------------------------------
template<class P1>
inline TRsStat RsPutProp(TAppObj *app, IDispObj *obj, RSDISPID id, P1 par1)
{
 TParmArray  prm(app, 0, false);

 prm.putVal() = par1;

 return obj->invoke(id, RS_DISP_PROPERTYPUT, prm, 0);
}

// -----------------------------------------------------------------------------
template<class P1>
inline TRsStat RsGetProp(TAppObj *app, IDispObj *obj, RSDISPID id, P1 *par1)
{
 TParmArray  prm(app, 0, true);
 TRsStat     stat = obj->invoke(id, RS_DISP_PROPERTYGET | RS_DISP_METHOD, prm, 0);


 if(stat == RSL_STAT_OK)
   *par1 = prm.retVal();

 return stat;
}

// -----------------------------------------------------------------------------
inline TRsStat RsGetStrProp(TAppObj *app, IDispObj *obj, RSDISPID id, char *buff, int sz)
{
 TParmArray  prm(app, 0, true);
 TRsStat     stat = obj->invoke(id, RS_DISP_PROPERTYGET | RS_DISP_METHOD, prm, 0);


 if(stat == RSL_STAT_OK)
  {
   const char  *p = prm.retVal();

   strncpy(buff, p, sz);
   buff[sz - 1] = '\0';
  }

 return stat;
}

// -----------------------------------------------------------------------------
inline TRsStat RsGetObjProp(TAppObj *app, IDispObj *obj, RSDISPID id, int iid, void **ret)
{
 TParmArray  prm(app, 0, true);
 TRsStat     stat = obj->invoke(id, RS_DISP_PROPERTYGET | RS_DISP_METHOD, prm, 0);


 if(stat == RSL_STAT_OK)
  {
   IRsObj  *obj = prm.retVal();


   if(!obj || !obj->getInterface(iid, ret))
     stat = RSL_STAT_NOINTF;
  }

 return stat;
}

#endif // __CRACKER_HPP

/* <-- EOF --> */