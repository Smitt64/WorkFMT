/*-----------------------------------------------------------------------*-

 File Name   : threadex.hpp

 Sybsystem   :   
 Description : Thread with wait and exit code support
    Использование стандартных процедур _beginthread и _beginthreadNT
    не позволяет получить код завершения потока, не позволяет родительскому
    потоку ожидать завершения дочернего потока. Хотя указанные процедуры
    и возвращают хэндл потока, использовать его нельзя, т.к. при завершении
    потока этот хэндл закрывается. Поэтому создаем дубликат этого хэндла.
    Для синхронизации потоков используем семафор.       

    Конструктор:

       TThread (BOOL useWait = TRUE);

    Если useWait равен FLASE, отключается возможность ожидания завершения
    дочернего потока и получения кода возврата.

    Для создания нового потока необходимо создать дочерний класс от
    TThread и переопределить виртуальную функциию threadProc.


 Source      : threadex.cpp

 Library(s)  : 


 Copyright (c) 1991 - 1994 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 October 31,1995  Sergei Kubrin (K78) - Create file
-*-----------------------------------------------------------------------*/

#ifndef __THREADEX_H
#define __THREADEX_H

#include "tooldef.h"

// -----------------------------------------------------------------------------
#ifndef INFINITE
    #define  INFINITE  0xFFFFFFFF  // Infinite timeout
#endif

// -----------------------------------------------------------------------------
class _TOOLEXP TThread
{
 public:
   enum ThreadState
      {
       NotActive,
       Running,
       Stopped,
       Suspended
      };

   virtual ~TThread();

   int getExitCode();

   int  wait (unsigned timeOut = INFINITE);
   bool wait2(unsigned timeOut = INFINITE);

   bool start();
   void terminate();

   int  terminateAndWait(unsigned timeOut = INFINITE);

   bool shouldTerminate()
   {
    return terminationRequest;
   }

   bool suspend();
   bool resume();

   ThreadState getState()
   {
    return state;
   }

   unsigned getThreadID()
   {
    return threadID;
   }

 protected:
   TThread(bool useWait = true, bool autoDelete = false);

   virtual int exit(int status);

   virtual int threadProc() = 0;

   virtual void setUp();

#if defined (_MSC_VER)
   static unsigned __stdcall entryPoint(void *d);
#else
   static void               entryPoint(void *);
#endif

#ifndef _MSC_VER
   int            retval;   // Thread termination staus
#endif

   uintptr_t      hd;   // Thread handle duplicate

   uintptr_t      sem;   // Semaphore handle

   bool           useWait;   // Use exit code and wait support
   bool           terminationRequest;
   bool           autoDelete;

   ThreadState    state;

   unsigned       threadID;
};

#endif // __THREADEX_H

/* <-- EOF --> */