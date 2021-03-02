/*-----------------------------------------------------------------------*-

 File Name   : safelist.hpp

 Sybsystem   :   
 Description : 

 Source      : 

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 June 27,1997  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __SAFELIST_HPP
#define __SAFELIST_HPP

// -----------------------------------------------------------------------------
template<class T>
class  TSafeListBaseEx : public TSmartListBase<T>
{
public:
   TSafeListBaseEx (int delta,TClearObj f,unsigned char flags,TCriticalSection& sec)
      { clr = f; InitCntListEx (&base,(unsigned char)(flags|LST_SMART),delta,(LPCRITICAL_SECTION)sec); }
};

// -----------------------------------------------------------------------------
template<class T>
class TSafeListBase : public TSafeListBaseEx<T>
{
public:
   TSafeListBase (int delta,TClearObj f,unsigned char flags = LST_DEFAULT) :
      TSafeListBaseEx<T> (delta,f,flags,sec) {}
private:
   TCriticalSection  sec;
};

// -----------------------------------------------------------------------------
template<class T, int M>
class TSafeList : public TSafeListBase<T>
    {
     public:
       TSafeList(unsigned char flags = LST_DEFAULT) : TSafeListBase(M, clearObj, flags) {}
       TSafeList(int, TClearObj, unsigned char flags = LST_DEFAULT) : TSafeListBase(M, clearObj, flags) {}

     private:
       static void clearObj(T *obj)
       {
        delete obj;
       }
    };

#endif // __SAFELIST_HPP

/* <-- EOF --> */