/*-----------------------------------------------------------------------*-

 File Name   : rsforms\cmdgroup.hpp
 Programmer  : Leonid S. Pavlov

 Sybsystem   : RSForms library
 Description : 

 Source      : 

 Library(s)  : RSFSrv.d32

 Copyright (c) 1991 - 2003 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 Jul 03, 2003  Leonid S. Pavlov (LSP) - Create file
 
-*-----------------------------------------------------------------------*/

#ifndef __CMDGROUP_H__
#define __CMDGROUP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TCommandSetImpl : public TObjectImpl, public ICommandSet
{
public:
    TCommandSetImpl(TAppObj* app)  throw(XRSFormsError) : TObjectImpl(app) 
    {
        // создание локального серверного объекта
        TRsStat stat = fs_getServerObjectA(app, runtime_module, "CCommandSet", NULL, IID_COMMANDSET2, &impl, true, 0, NULL, 0);
        RTM_CHECK_1(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete CCommandSet from %s.", runtime_module);
        if(stat) throw(XRSFormsError(app, stat));
    }

    TRsStat RSCOMCALL finalConstruct(const char* objName, IModuleObj* pMod, void* uData)
    { 
        TRsStat stat = TObjectImpl::finalConstruct(objName, pMod, uData);
        impl->init(static_cast<ICommandSet*>(this));
        return stat; 
    }

RSCOM_INTF_BEGIN2(TObjectImpl)
    RSCOM_INTF(IID_COMMANDSET, ICommandSet)
RSCOM_INTF_END

public: // ICommandSet
    void RSCOMCALL setCommands(IDispObj* pCommands)
    { impl->setCommands(pCommands); }

    bool RSCOMCALL getCommands(IDispObj** ppCommands)
    { return impl->getCommands(ppCommands); }
    
    bool RSCOMCALL setCmdState(unsigned long cmdID, unsigned int newState)
    { return impl->setCmdState(cmdID, newState); }
    
    bool RSCOMCALL getCmdState(unsigned long cmdID, unsigned int* ptr_newState)
    { return impl->getCmdState(cmdID, ptr_newState); }
    
    bool RSCOMCALL resetCmdState(unsigned long cmdID)
    { return impl->resetCmdState(cmdID); }

    void RSCOMCALL setGroupState(ICommandGroup* pCmdGroup, unsigned int newState)
    { impl->setGroupState(pCmdGroup, newState); }

RSCOM_INVOKE_BEGIN(TCommandSetImpl,TObjectImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
    RSCOM_DISP_METHOD(RTM_SETGROUPSTATE, RsCracker(this, &TCommandSetImpl::_setGroupState))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TCommandSetImpl,TObjectImpl,TCommandSet)
    RSCOM_METHODEX("setGroupState", RTM_SETGROUPSTATE, NULL, RV_OBJ RV_UI4, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
    TRsStat _setGroupState(TValRef retVal, IRsObj* pCmdGroup, unsigned int newState)
    {
        setGroupState(TRsPtrX<ICommandGroup, IID_COMMANDGROUP>(pCmdGroup), newState);
        return RSL_STAT_OK;
    }

protected:
    TRsPtr<ICommandSet2> impl;
};

DECLARE_CLASS(TCommandSet, TCommandSetImpl);

/*-----------------------------------------------------------------------*/

class TCommandGroupImpl : public TDispStubImpl, public ICommandGroup
{
public:
    TCommandGroupImpl(TAppObj* App) throw(XRSFormsError) : TDispStubImpl(App), app(App)
    {
        setUseMixObj(true);
        
        // создание локального серверного объекта
        TRsStat stat = fs_getServerObjectA(app, runtime_module, "CCommandGroup", NULL, IID_COMMANDGROUP, &impl, true, 0, NULL, 0);
        RTM_CHECK_1(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete CCommandGroup from %s.", runtime_module);
        if(stat) throw(XRSFormsError(app, stat));
    }

RSCOM_INTF_BEGIN2(TDispStubImpl)
    RSCOM_INTF(IID_COMMANDGROUP, ICommandGroup)
RSCOM_INTF_END

public: // ICommandGroup
    long RSCOMCALL count()
    { return impl->count(); }

    void RSCOMCALL item(long i, unsigned long* ptr_cmdID)
    { impl->item(i, ptr_cmdID); }

    void RSCOMCALL addCommand(unsigned long cmdID)
    { impl->addCommand(cmdID); }

    void RSCOMCALL remCommand(unsigned long cmdID)
    { impl->remCommand(cmdID); }

RSCOM_INVOKE_BEGIN(TCommandGroupImpl, TDispStubImpl)
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
    RSCOM_DISP_METHOD(RTM_COUNT, RsCracker(this, &TCommandGroupImpl::_count))
    RSCOM_DISP_METHOD(RTM_ITEM, RsCracker(this, &TCommandGroupImpl::_item))    
    RSCOM_DISP_METHOD(RTM_ADDITEM, RsCracker(this, &TCommandGroupImpl::_addCommand))
    RSCOM_DISP_METHOD(RTM_REMITEM, RsCracker(this, &TCommandGroupImpl::_remCommand))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TCommandGroupImpl, TDispStubImpl, TCommandGroup)
    RSCOM_METHODEX("count", RTM_COUNT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("item", RTM_ITEM, RSCOM_UINT, RV_I4, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("addCommand", RTM_ADDITEM, NULL, RV_UI4, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("remCommand", RTM_REMITEM, NULL, RV_UI4, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
    TRsStat _count(TValRef retVal)
    {
        retVal = count();
        return RSL_STAT_OK;
    }

    TRsStat _item(TValRef retVal, long i)
    { 
        unsigned long cmdID;
        item(i, &cmdID); 
        retVal = cmdID;
        return RSL_STAT_OK;
    }

    TRsStat _addCommand(TValRef retVal, unsigned long cmdID)
    { 
        addCommand(cmdID); 
        return RSL_STAT_OK;
    }

    TRsStat _remCommand(TValRef retVal, unsigned long cmdID)
    { 
        remCommand(cmdID); 
        return RSL_STAT_OK;
    }

protected:
    TAppObj* app;
    TRsPtr<ICommandGroup> impl;
};

DECLARE_CLASS(TCommandGroup, TCommandGroupImpl);

/*-----------------------------------------------------------------------*/
#endif // __CMDGROUP_H__