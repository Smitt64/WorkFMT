//-*-----------------------------------------------------------------------*-
//
// File Name   : delegate.hpp
//
// Sybsystem   :   
// Description : 
//
// Copyright (c) 1991 - 2002 by R-Style Softlab.
// All Rights Reserved.
//
//-*- History -------------------------------------------------------------*-
// September 16,2002  Sergei Kubrin (K78) - Create file
//-*-----------------------------------------------------------------------*-

#ifndef __DELEGATE_HPP
#define __DELEGATE_HPP

#define RSL_DELEGATE(tpname, parmlist,calllist)          \
class tpname                                             \
{                                                        \
public:                                                  \
   virtual void RSCOMCALL runEvent parmlist = 0;         \
};                                                       \
template <class Object>                                  \
class tpname##Impl : public tpname                       \
{                                                        \
public:                                                  \
   typedef void (Object::*TMemberFun) parmlist;          \
   tpname##Impl (Object* ob,TMemberFun mem) :            \
                   obj (ob), member (mem) {}             \
   void RSCOMCALL runEvent parmlist { (obj->*member) calllist; }   \
   /*virtual ~##tpname##Impl () {}*/                         \
private:                                                 \
   Object        *obj;                                   \
   TMemberFun  member;                                   \
} 

#define RSL_DELEGATE_(tpname,retval,parmlist,calllist)          \
class tpname                                                    \
{                                                               \
public:                                                         \
   virtual retval RSCOMCALL runEvent parmlist = 0;              \
};                                                              \
template <class Object>                                         \
class tpname##Impl : public tpname                              \
{                                                               \
public:                                                         \
   typedef retval (Object::*TMemberFun) parmlist;               \
   tpname##Impl (Object* ob,TMemberFun mem) :                   \
                   obj (ob), member (mem) {}                    \
   retval RSCOMCALL runEvent parmlist { return (obj->*member) calllist; } \
   /*virtual ~##tpname##Impl () {} */                               \
private:                                                        \
   Object        *obj;                                          \
   TMemberFun  member;                                          \
}


//RSL_DELEGATE(TEventCall, (TObj *sender, TEvent *ev), (sender,ev) );
//RSL_DELEGATE(TDelegate, (),() );
//RSL_DELEGATE_(TEventCall,bool, (TObj *sender, TEvent *ev), (sender,ev) );

#endif  // __DELEGATE_HPP
