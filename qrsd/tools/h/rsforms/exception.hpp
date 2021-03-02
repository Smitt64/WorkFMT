/*-----------------------------------------------------------------------*-

 File Name   : rsforms\exception.hpp
 Programmer  : Leonid S. Pavlov

 Sybsystem   : rsforms library
 Description : 

 Source      : 

 Library(s)  : RSFSrv.d32

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Mar 28, 2002  Leonid S. Pavlov (LSP) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFORMSEXCEPTION_H__
#define __RSFORMSEXCEPTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class XRSFormsError : public XRSComError
{
public:
    XRSFormsError(TAppObj* a, TRsStat s = RSL_STAT_EX, const char* what = "RSForms exception") 
        : XRSComError(a, s, what) 
    {}
};

inline void checkErrorInfo(TAppObj* a) throw(XRSFormsError)
{
    TRsError err(a);
    if(err.getCount()) throw XRSFormsError(a);
}

/*-----------------------------------------------------------------------*/

#define RTM_ADDERROR(erCode, level, msg)                        \
    {                                                           \
        TRsError err(app);                                      \
        err.addError(erCode, level, msg);                       \
    }                                                           \

#define RTM_ADDERROR_1(erCode, level, msg, p1)                  \
    {                                                           \
        TRsError err(app);                                      \
        char buff[255]; sprintf(buff, msg, p1);                 \
        err.addError(erCode, level, buff);                      \
    }                                                           \


#define RTM_ERROR(erCode, level, msg, retVal)                   \
    {                                                           \
        RTM_ADDERROR(erCode, level, msg);                       \
        return retVal;                                          \
    }                                                           \
    
#define RTM_ERROR_1(erCode, level, msg, p1, retVal)             \
    {                                                           \
        RTM_ADDERROR_1(erCode, level, msg, p1);                 \
        return retVal;                                          \
    }                                                           \

#define RTM_CHECK(stat, erCode, level, msg)                     \
    if(stat != RSL_STAT_OK)                                     \
    {                                                           \
        TRsError err(app);                                      \
        if(stat != RSL_STAT_EX)                                 \
            err.addError(stat, RSCOM_SYS_LEVEL, NULL);          \
        err.addError(erCode, level, msg);                       \
        stat = RSL_STAT_EX;                                     \
    }                                                           \

#define RTM_CHECK_1(stat, erCode, level, msg, p1)               \
    if(stat != RSL_STAT_OK)                                     \
    {                                                           \
        TRsError err(app);                                      \
        if(stat != RSL_STAT_EX)                                 \
            err.addError(stat, RSCOM_SYS_LEVEL, NULL);          \
        char buff[255]; sprintf(buff, msg, p1);                 \
        err.addError(erCode, level, buff);                      \
        stat = RSL_STAT_EX;                                     \
    }                                                           \

#define RTM_CHECK_2(stat, erCode, level, msg, p1, p2)           \
    if(stat != RSL_STAT_OK)                                     \
    {                                                           \
        TRsError err(app);                                      \
        if(stat != RSL_STAT_EX)                                 \
            err.addError(stat, RSCOM_SYS_LEVEL, NULL);          \
        char buff[255]; sprintf(buff, msg, p1, p2);             \
        err.addError(erCode, level, buff);                      \
        stat = RSL_STAT_EX;                                     \
    }                                                           \

#define RTM_CHECK_EX(stat, erCode, level, msg, retVal)          \
    {                                                           \
        RTM_CHECK(stat, erCode, level, msg);                    \
        if(stat != RSL_STAT_OK) return retVal;                  \
    }                                                           \

#define RTM_CHECK_1_EX(stat, erCode, level, msg, p1, retVal)    \
    {                                                           \
        RTM_CHECK_1(stat, erCode, level, msg, p1);              \
        if(stat != RSL_STAT_OK) return retVal;                  \
    }                                                           \

/*-----------------------------------------------------------------------*/

//class XRSFormsError : public RSException
//{
//public:
//    XRSFormsError(TAppObj* a, TRsStat s = RSL_STAT_EX, const char* what = "RSForms exception") 
//        : RSException(what), app(a), stat(s) 
//    {}
//    
//    const TRsStat getStat() const { return stat; }
//    bool isExtended() const { return stat == RSL_STAT_EX; }
//    TAppObj* getErrApp() const { return app; }
//
//private:
//    TAppObj* app;
//    TRsStat stat;
//};

/*-----------------------------------------------------------------------*/
#endif // __RSFORMSEXCEPTION_H__