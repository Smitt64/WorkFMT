//-*--------------------------------------------------------------------------*-
//
// File Name   : rssync.hpp
//
// Copyright (c) 1991 - 2006 by R-Style Softlab.
// All Rights Reserved.
//
//-*--------------------------------------------------------------------------*-
// October 18,2006  Sergei Kubrin (K78) - Create file
//-*--------------------------------------------------------------------------*-

#ifndef __RSSYNC_HPP
#define __RSSYNC_HPP

// -----------------------------------------------------------------------------
class TRsCriticalSection : public TRsPtr<IRsLock>
    {
     public:
       TRsCriticalSection(TAppObj *app, const char *name = 0)
       {
        TRsStat  stat = fs_getPSmgrA(app)->createCriticalSection(name, &p);

        if(stat != RSL_STAT_OK)
          throw XRSComError(app, stat);
       }
    };

// -----------------------------------------------------------------------------
class TRsMutex : public TRsPtr<IRsLock>
    {
     public:
       TRsMutex(TAppObj *app, bool initOwn = false, const char *name = 0)
       {
        TRsStat  stat = fs_getPSmgrA(app)->createMutex(initOwn, name, &p);

        if(stat != RSL_STAT_OK)
          throw XRSComError(app, stat);
       }
    };

// -----------------------------------------------------------------------------
class TRsEvent : public TRsPtr<IRsEvent>
    {
     public:
       TRsEvent(TAppObj *app, bool manual = false, bool signaled = false, const char *name = 0)
       {
        TRsStat  stat = fs_getPSmgrA(app)->createEvent(manual, signaled, name, &p);

        if(stat != RSL_STAT_OK)
          throw XRSComError(app, stat);
       }
    };

// -----------------------------------------------------------------------------
class TRsSemaphore : public TRsPtr<IRsSemaphore>
    {
     public:
       TRsSemaphore(TAppObj *app, long initCount = 1, long maxCount = 1, const char *name = 0)
       {
        TRsStat  stat = fs_getPSmgrA(app)->createSemaphore(initCount, maxCount, name, &p);

        if(stat != RSL_STAT_OK)
          throw XRSComError(app, stat);
       }
    };

#endif // __RSSYNC_HPP

/* <-- EOF --> */