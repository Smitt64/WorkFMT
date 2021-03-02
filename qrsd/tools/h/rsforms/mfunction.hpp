/*-----------------------------------------------------------------------*-

  File Name   : mfunction.hpp
  Programmer  : Leonid S. Pavlov
  
  Sybsystem   : RSForms C++ library header
  Description : 
    
  Source      : 
      
  Library(s)  : RSForms C++ library header
        
  Copyright (c) 1991 - 2002 by R-Style Software Lab.
  All Rights Reserved.
          
-*- History -------------------------------------------------------------*-
  Sep 23, 2002  Leonid S. Pavlov (LSP) - Create file
            
-*-----------------------------------------------------------------------*/

#ifndef __MFUNCTION_H__
#define __MFUNCTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*-----------------------------------------------------------------------*/

class MemberFunctionBaseImpl : public MemberFunction
{
    virtual unsigned RSCOMCALL addRef() { return 1; }
    virtual unsigned RSCOMCALL release() { return 1; }
    virtual bool RSCOMCALL getInterface(int id, void**) { return false; }
    virtual const char* RSCOMCALL getClassName() { return NULL; }
    virtual TAppObj* RSCOMCALL getAppObj() { return NULL; }
    virtual TRsStat RSCOMCALL getId(const char* name, RSDISPID* id) { return RSL_STAT_NOTIMPL; }
    virtual TRsStat RSCOMCALL getMetaInfo(int mask, IMetaInfo** info) { return RSL_STAT_NOTIMPL; } 
}; 

/*-----------------------------------------------------------------------*/

template<class Object>
class MemberFunctionImpl : public MemberFunctionBaseImpl
{
public:
    typedef void (Object::*TMemberFun)(IParmInfo* pInfo);
    MemberFunctionImpl(Object* ob,TMemberFun mem) : obj(ob), member(mem) {}
    virtual TRsStat RSCOMCALL invoke (RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
    {       
        try
        {
            (obj->*member)(pInfo);
            return RSL_STAT_OK;
        }
        catch(XRSComError& err)
        {
            TRsError errInfo(pInfo->getAppObj());
            TRsStat stat = err.getStat();
            if(stat != RSL_STAT_EX)
                errInfo.addError(stat, RSCOM_SYS_LEVEL, NULL);
            errInfo.addError(ERR_HANDLER, LEVEL_RSFRUNTIME, "Error in user handler.");
            return RSL_STAT_EX;
        }
    }

private:
    Object        *obj;
    TMemberFun  member;
};

// Special case for menu command handlers
template<class Object>
class CmdHandlerImpl : public MemberFunctionBaseImpl
{
public:
    typedef void (Object::*TMemberFun)(ICmdSet* pSender, unsigned long id, ICmdTarget* pFocusedObj, IFrameWindow *wnd, const char *parms, IDispObj* pCmdItem, bool& cancel);

    CmdHandlerImpl(Object* ob,TMemberFun mem) : obj(ob), member(mem) {}
    virtual TRsStat RSCOMCALL invoke (RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
    {           
        TParmArray prm (pInfo);
        try
        {                      
            TRsPtr<IFrameWindow> wnd;
            TRsPtrX<ICmdSet,IID_CMDSET> cmdSet ((IRsObj*)prm [0]);
            TRsPtrX<ICmdTarget,IID_CMDTARGET> focusObj ((IRsObj*)prm [1]);
            const char *parms = prm [2];
            TRsPtrX<IDispObj,IID_RSDISP> cmdItem ((IRsObj*)prm [3]);
            bool cancel = prm [4];
            ICmdTarget *cmdTarget = focusObj;
       		while (cmdTarget && !cmdTarget->getInterface (IID_FRAME_WND,(void**)&wnd))
          		cmdTarget = cmdTarget->getParentTarget ();
            (obj->*member)(cmdSet,id,focusObj,wnd,parms,cmdItem,cancel);
            prm [4] = cancel;
            prm.retVal() = !cancel;
            return RSL_STAT_OK;
        }
        catch(XRSComError& err)
        {
            TRsError errInfo(pInfo->getAppObj());
            TRsStat stat = err.getStat();
            if(stat != RSL_STAT_EX)
                errInfo.addError(stat, RSCOM_SYS_LEVEL, NULL);
            errInfo.addError(ERR_HANDLER, LEVEL_RSFRUNTIME, "Error in user handler.");
            return RSL_STAT_EX;
        }
    }

private:
    Object        *obj;
    TMemberFun  member;
};

/*---------------------------------------------------------------------------*/
#endif // __MFUNCTION_H__
