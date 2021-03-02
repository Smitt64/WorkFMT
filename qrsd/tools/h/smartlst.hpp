/*-----------------------------------------------------------------------*-

 File Name   : smartlst.hpp

 Sybsystem   :
 Description :

 Source      :

 Library(s)  :


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 June 23,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __SMARTLST_HPP
#define __SMARTLST_HPP

#include "listobj.h"

// -----------------------------------------------
#ifdef BUILD_DLM
    #define  InitCntList(a, b, c)  LobjInitList(a, c); ADD_FLAGS(a, b)
    #define  DoneCntList           LobjDoneList
#endif

// -----------------------------------------------------------------------------
#ifndef RSTPNAME
    #ifdef _MSC_VER
        #define  RSTPNAME  typename
    #else
        #define  RSTPNAME
    #endif
#endif

// -----------------------------------------------------------------------------
class TSmartObj : public SOBJ
{
 public:
   // --------------------------------------------
   TSmartObj()
   {
    SobjInit(this);
   }

   // --------------------------------------------
  ~TSmartObj()
   {
    SobjUnlink(this);
   }
};

// -----------------------------------------------------------------------------
#ifndef __RSLDLL_H
// -----------------------------------------------------------------------------
    template<class T> class TSmartListBaseImpl;

// -----------------------------------------------------------------------------
    template<class T>
    class TSmartLstIter
    {
     public:
   // --------------------------------------------
   TSmartLstIter()
   {
   }

   // --------------------------------------------
       TSmartLstIter(TSmartListBaseImpl<T> &lst, bool begin = false)
       {
        obj = begin ? (SOBJ *)_LobjFirst(&lst.base) : NULL;
       }

   // --------------------------------------------
       TSmartLstIter<T> operator ++ (int)
       {
        TSmartLstIter<T>  tmp = *this;


        obj = (SOBJ *)_LobjNext(obj->owner, (LOBJ *)obj);

        return tmp;
       }

   // --------------------------------------------
       TSmartLstIter<T> operator ++ ()
       {
        obj = (SOBJ *)_LobjNext(obj->owner, (LOBJ *)obj);

        return *this;
       }

   // --------------------------------------------
       T &operator * ()
       {
        return *(T *)_LobjGetObj(obj->owner, (LOBJ *)obj);
       }

   // --------------------------------------------
       bool operator == (const TSmartLstIter<T> &ob) const
       {
        return obj == ob.obj;
       }

   // --------------------------------------------
       bool operator != (const TSmartLstIter<T> &ob) const
       {
        return obj != ob.obj;
       }

     private:
   // --------------------------------------------
   // --------------------------------------------
       SOBJ *obj;
    };
#endif

// -----------------------------------------------------------------------------
template<class T>
class TSmartListBaseImpl
    {
     public:

       #ifndef __RSLDLL_H
           typedef TSmartLstIter<T>  iterator;
       #endif

       typedef int  (*TIterFunc)(T *, void *);
       typedef int  (*TSortFunc)(T *, T *, void *);
       typedef void (*TClearObj)(T *);
       typedef T objType;

       TSmartListBaseImpl() {}
       TSmartListBaseImpl(int delta, TClearObj f, unsigned char flags = LST_DEFAULT) : clr(f) {}
      ~TSmartListBaseImpl() {}

   // --------------------------------------------
       void init(int delta, TClearObj f, unsigned char flags = LST_DEFAULT)
       {
        clr = f;

        InitCntList(&base, (unsigned char)(flags | LST_SMART), delta);
       }

   // --------------------------------------------
       void done()
       {
        DoneCntList(&base, (LCLRFUNC)clr);
       }

       T    *first    ()                     { return (T *)LobjFirstItem(&base); }
       T    *last     ()                     { return (T *)LobjLastItem(&base); }
       T    *next     (T *obj)               { return (T *)LobjNextItem(&base, obj); }
       T    *prev     (T *obj)               { return (T *)LobjPrevItem(&base, obj); }
       void  add      (T *obj)               { LobjInsert(&base,obj); }
       int   nItems   ()                     { return LobjNitems(&base); }
       int   forEach  (TIterFunc f, void *d) { return LobjForEach(&base, (LITRFUNC)f, d); }
       T    *firstThat(TIterFunc f, void *d) { return (T *)LobjFirstThat(&base, (LCMPFUNC)f, d); }
       int   forEachL (TIterFunc f, void *d) { return LobjForEachL(&base,(LITRFUNC)f, d); }
       T    *lastThat (TIterFunc f, void *d) { return (T *)LobjLastThat(&base, (LCMPFUNC)f, d); }
       void  remove   (T *obj)               { LobjRemove(&base, obj, (LCLRFUNC)clr); }

   // --------------------------------------------
       T *operator [] (int id)            { return (T *)LobjAt(&base, id); }

       #ifndef __RSLDLL_H
           T        *extractFirst()                             { return (T *)LobjExtractFirst(&base); }
           void      addBefore   (T *obj,T *targ)               { LobjInsertBefore(&base, obj, targ); }
           void      addAfter    (T *obj,T *after)              { LobjInsertAfter(&base, obj, after); }
           void      addAt       (T *obj, int at)               { LobjInsertAt(&base, obj, at); }
           void      addSort     (T *obj, TSortFunc f, void *d) { LobjInsertSort(&base, obj, (LSRTFUNC)f, d); }
           void      detach      (T *obj)                       { LobjDetach(&base, obj); }
           void      sort        (TSortFunc f, void *d)         { SortCntList(&base, (LSRTFUNC)f, d); }
           int       indexOf     (T *obj)                       { return LobjIndexOf(&base, obj); }
           iterator  begin       ()                             { return TSmartLstIter<T>(*this, true);}
           iterator  end         ()                             { return TSmartLstIter<T>(*this);}
       #endif

     protected:
   // --------------------------------------------
   // --------------------------------------------
       CNTLIST      base;
       TClearObj     clr;

       #ifndef __RSLDLL_H
           friend class TSmartLstIter<T>;
       #endif
    };

// -----------------------------------------------------------------------------
template<class T>
class TSmartListBase : public TSmartListBaseImpl<T>
    {
     public:
   // --------------------------------------------
       typedef TSmartListBaseImpl<T>::TClearObj  TClearObj;

   // --------------------------------------------
       TSmartListBase() {}

   // --------------------------------------------
       TSmartListBase(int delta, TClearObj f, unsigned char flags = LST_DEFAULT)
       {
        init(delta, f, flags);
       }

   // --------------------------------------------
      ~TSmartListBase()
       {
        done();
       }
    };

// -----------------------------------------------------------------------------
template<class T, int M>
class TSmartList : public TSmartListBase<T>
    {
     public:
   // --------------------------------------------
       typedef TSmartListBase<T>::TClearObj  TClearObj;

   // --------------------------------------------
       TSmartList(unsigned char flags = LST_DEFAULT) : TSmartListBase<T>(M, clearObj, flags) {}

   // --------------------------------------------
       TSmartList(int, TClearObj, unsigned char flags = LST_DEFAULT) : TSmartListBase<T>(M, clearObj, flags) {}

     private:
   // --------------------------------------------
       static void clearObj(T *obj)
       {
        delete obj;
       }
    };

// -----------------------------------------------------------------------------
template<class T, int M>
class TSmartListManual : public TSmartListBaseImpl<T>
    {
     public:
   // --------------------------------------------
       typedef TSmartListBaseImpl<T>  Parent;
       typedef Parent::TClearObj      TClearObj;

   // --------------------------------------------
       TSmartListManual(unsigned char aFl = LST_DEFAULT) : delta(M), flags(aFl), inited(false) {}

   // --------------------------------------------
       TSmartListManual(int, TClearObj, unsigned char aFl = LST_DEFAULT) : delta(M), flags(aFl), inited(false) {}

   // --------------------------------------------
       void init()
       {
        if(!inited)
         {
          inited = true;
      
          TSmartListBaseImpl<T>::init(delta, clearObj, flags);
         }
       }

   // --------------------------------------------
       void done()
       {
        if(inited)
         {
          inited = false;
      
          TSmartListBaseImpl<T>::done();
         }
       }

     private:
   // --------------------------------------------
   // --------------------------------------------
       int            delta;
       unsigned char  flags;
       bool           inited;

   // --------------------------------------------
       static void clearObj(T *obj)
       {
        delete obj;
       }
    };

// -----------------------------------------------------------------------------
template<class T>
class TRslStack : public T
    {
     public:
   // --------------------------------------------
       TRslStack(int delta, RSTPNAME T::TClearObj f, bool ownElem = true) :
               T(delta, f, (unsigned char)(LST_ADDBEG | (ownElem ? 0 : LST_NOTOWN))) {}

   // --------------------------------------------
       void push(RSTPNAME T::objType *obj)
       {
        add(obj);
       }

   // --------------------------------------------
       RSTPNAME T::objType *pop()
       {
        return extractFirst();
       }
    };

// -----------------------------------------------------------------------------
template<class T>
class TRslQueue : public T
    {
     public:
   // --------------------------------------------
       TRslQueue(int delta, RSTPNAME T::TClearObj f, bool ownElem = true) :
               T(delta, f, (unsigned char)(LST_DEFAULT | (ownElem ? 0 : LST_NOTOWN))) {}

   // --------------------------------------------
       void push(RSTPNAME T::objType *obj)
       {
        add(obj);
       }

   // --------------------------------------------
       RSTPNAME T::objType *pop()
       {
        return extractFirst();
       }
    };

#endif  // __SMARTLST_HPP

/* <-- EOF --> */