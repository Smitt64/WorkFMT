//-*--------------------------------------------------------------------------*-
//
// File Name   : rsthread.hpp
//
// Copyright (c) 1991 - 2008 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// May 22,2009  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef _RSTHREAD_HPP_
#define _RSTHREAD_HPP_

#include "btrv.h"
#include "hcom/threadex.hpp"
#include "rscom/rscom.hpp"


// -----------------------------------------------------------------------------
class ToolsThread : public TThread, public IRsLock, public IWinHandle
    {
     public:

       operator IRsLock * ()
       {
        return this;
       }

     protected:
       ToolsThread(bool useWait = true, bool autoDelete = false) : TThread (useWait, autoDelete)
       {
       }

       virtual int exit(int status)
       {
        ToolsClearTLS();

        return TThread::exit(status);
       }

       virtual bool RSCOMCALL lock(int timeOut)
       {
        return wait2((unsigned)timeOut);
       }

       virtual bool RSCOMCALL unlock()
       {
        return true;
       }

       virtual unsigned RSCOMCALL addRef()
       {
        return 1;
       }

       virtual unsigned RSCOMCALL release()
       {
        return 1;
       }

       virtual bool RSCOMCALL getInterface(int id, void **ptr)
       {
        if(id == IID_RSOBJECT || id == IID_RSLOCK)
         {
          *ptr = (IRsLock *)this;

          return true;
         }
        else if (id == IID_IWINNHANDLE)
         {
          *ptr = (IWinHandle *)this;

          return true;
         }

        return false;
       }

       virtual const char *RSCOMCALL getClassName()
       {
        return "ToolsThread";
       }

       virtual TAppObj *RSCOMCALL getAppObj()
       {
        return NULL;
       }

       virtual uintptr_t RSCOMCALL getHandle()
       {
        return hd;
       }
    };

#endif // _RSTHREAD_HPP_

/* <-- EOF --> */