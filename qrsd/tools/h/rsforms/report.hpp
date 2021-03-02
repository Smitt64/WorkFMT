/*-----------------------------------------------------------------------*-

 File Name   : rsforms_report.hpp
 Programmer  : Nechaev Pavel D.

 Sybsystem   : RSRepSrv library header
 Description : 

 Source      : 

 Library(s)  : RSRepSrv.d32

 Copyright (c) 1991 - 2002 by R-Style Software Lab.
 All Rights Reserved.

-*- History -------------------------------------------------------------*-
 17/06/2002  Nechaev Pavel D. (NPD) - Create file

-*-----------------------------------------------------------------------*/

#ifndef __RSFREPSRV_H__
#define __RSFREPSRV_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*-----------------------------------------------------------------------*/

class TReportsGroupImpl : public TObjectImpl, public IReportsGroup
{
public:
    TReportsGroupImpl(TAppObj* app) throw(XRSFormsError) : TObjectImpl(app)
    {
        // создание локального серверного объекта
        TRsStat stat = fs_getServerObjectA( app, report_module, "CReportsGroup", NULL, IID_REPORTSGROUP2, &impl, true, 0, NULL, RSDISP_DEFAULT );
        RTM_CHECK_1(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete CReportsGroup from %s.", report_module);
        if(stat) throw(XRSFormsError(app, stat));
    }

RSCOM_INTF_BEGIN2( TObjectImpl )
    RSCOM_INTF( IID_REPORTSGROUP, IReportsGroup )
RSCOM_INTF_END

public:
    // деинициализация независимого объекта
    void deinit() { impl->deinit(); }

public: // ILifeTime
    virtual TRsStat RSCOMCALL finalConstruct(const char* namePtr, IModuleObj* pMod, void* data)
    {
        TRsStat stat = TObjectImpl::finalConstruct(namePtr, pMod, data);
        impl->init(this);
        return stat;
    }

    virtual void RSCOMCALL finalRelease()
    { 
        deinit();
        impl.Release();
        TObjectImpl::finalRelease();
    }

public: // IReportsGroup
    void RSCOMCALL RefUserObj(IRsObj* pObj)
    { impl->RefUserObj(pObj); }
    void RSCOMCALL SetPreviewPagesLimit(long limit)
    { impl->SetPreviewPagesLimit(limit); }
    void RSCOMCALL SetReportCopies(long limit)
    { impl->SetReportCopies(limit); }

    bool RSCOMCALL Print(bool preview, printmode mode, bool edit)
    { return( impl->Print(preview, mode, edit) ); }

RSCOM_INVOKE_BEGIN(TReportsGroupImpl, TObjectImpl)
RSCOM_INVOKE_LEVEL(RSCOM_SYS_LEVEL)
    RSCOM_DISP_METHOD(RTM_DEINIT, RsCracker(this, &TReportsGroupImpl::_deinit))
//    RSCOM_DISP_METHOD(RTM_REFUSEROBJ, RsCracker(this, &TReportsGroupImpl::_RefUserObj))
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
    RSCOM_DISP_METHOD(RTM_SETPREVPAGLIMIT, RsCracker(this, &TReportsGroupImpl::_SetPreviewPagesLimit))
    RSCOM_DISP_METHOD(RTM_SETREPORTCOPIES, RsCracker(this, &TReportsGroupImpl::_SetReportCopies))
    RSCOM_DISP_METHOD(RTM_PRINT, RsCracker(this, &TReportsGroupImpl::_Print))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TReportsGroupImpl, TObjectImpl, TReportsGroup)
    RSCOM_METHODEX("SetPreviewPagesLimit", RTM_SETPREVPAGLIMIT, RSCOM_UNDEF, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("SetReportCopies", RTM_SETREPORTCOPIES, RSCOM_UNDEF, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("Print", RTM_PRINT, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
    TRsStat _deinit(TValRef retVal)
    { return( deinit(), RSL_STAT_OK ); }
    
//    TRsStat _RefUserObj(TValRef retVal, IRsObj* pObj)
//    { return( RefUserObj( pObj ), RSL_STAT_OK ); }
    
    TRsStat _SetPreviewPagesLimit(TValRef retVal, long limit)
    { return( SetPreviewPagesLimit(limit), RSL_STAT_OK ); }

    TRsStat _SetReportCopies(TValRef retVal, long limit)
    { return( SetReportCopies(limit), RSL_STAT_OK ); }

    TRsStat _Print(TValRef retVal, bool preview, int mode, bool edit)
    { return( retVal = Print( preview, ( printmode )mode, edit ), RSL_STAT_OK ); }

protected:
    TRsPtr<IReportsGroup2> impl;
};

DECLARE_CLASS( TReportsGroup, TReportsGroupImpl );

/*-----------------------------------------------------------------------*/

class TReportImpl : public TContainerImpl, public IReport
{
public:
    TReportImpl(TAppObj* app) throw(XRSFormsError) :
        TContainerImpl(app), throwExceptOnPutAbort(true)
    {
        // создание локального серверного объекта
        TRsStat stat = fs_getServerObjectA( app, report_module, "CReport", NULL, IID_REPORT2, &impl, true, 0, NULL, RSDISP_DEFAULT );
        RTM_CHECK_1(stat, ERR_FSCREATE, LEVEL_RSFRUNTIME, "Can't craete CReport from %s.", report_module);
        if(stat) throw(XRSFormsError(app, stat));
    }

RSCOM_INTF_BEGIN2( TContainerImpl )
    RSCOM_INTF( IID_REPORT, IReport )
RSCOM_INTF_END

    TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
    {
        TRsStat stat = TRsPtrX<IDispObj, IID_RSDISP>(impl)->invoke(id, cmdType, pInfo, level);
        if(stat == RSL_STAT_UNKNOWNID)
            stat = TContainerImpl::invoke(id, cmdType, pInfo, level);
        return stat;
    }

public:
    void enableThrowExceptOnPutAbort(bool enable)
    {
        throwExceptOnPutAbort = enable;
    }

    // инициализация независимого объекта
    bool init(IReportsGroup * p_group, IRsObj * p_userRefObj, bool bOptimize)
    {
        return( impl->init(p_group, p_userRefObj, bOptimize) );
    }

    // деинициализация независимого объекта
    void deinit() { impl->deinit(); }

public: // ILifeTime
    virtual TRsStat RSCOMCALL finalConstruct(const char* namePtr, IModuleObj* pMod, void* data)
    {
        TRsStat stat = TContainerImpl::finalConstruct(namePtr, pMod, data);
        impl->construct(this, TRsPtrX<IEventHandler, IID_EVENTHANDLER>(TControlImpl::impl));
        return stat;
    }

    virtual void RSCOMCALL finalRelease()
    { 
        deinit();
        TContainerImpl::finalRelease();
        impl.Release();
    }

public: // IEventHandler
    TRsStat RSCOMCALL onEvent(RSDISPID id, IParmInfo* pInfo, int level)
    {
        impl->chkevtprm(pInfo);
        return TRsPtrX<IEventHandler, IID_EVENTHANDLER>(impl)->onEvent(id, pInfo, level);
    }

public: // IReport
    void RSCOMCALL setTemplate(const char* szLib, const char* szRes)
    { impl->setTemplate(szLib, szRes); }
    void RSCOMCALL SetPreviewPagesLimit(long limit)
    { impl->SetPreviewPagesLimit(limit); }
    void RSCOMCALL SetReportCopies(long limit)
    { impl->SetReportCopies(limit); }
    bool RSCOMCALL Print(bool preview, printmode mode, bool edit)
    { return( impl->Print(preview, mode, edit) ); }

    bool RSCOMCALL putElement(const char * p_name, IHostObject* p_pObj = NULL, bool* p_aborted = NULL)
    {
        bool l_aborted;
        bool l_res = impl->putElement(p_name, p_pObj, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        return( l_res );
    }
    
    bool RSCOMCALL putElement(IControl* pCtl, IHostObject* p_pObj = NULL, bool* p_aborted = NULL)
    {
        bool l_aborted;
        bool l_res = impl->putElement(pCtl, p_pObj, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        return( l_res );
    }

    bool RSCOMCALL testElement(const char * p_name, long* p_outsize, bool* p_aborted = NULL)
    {
        bool l_aborted;
        long l_outSize;
        bool l_res = impl->testElement(p_name, &l_outSize, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        if( p_outsize ) *p_outsize = l_outSize;
        return( l_res );
    }

    bool RSCOMCALL testElement(IControl* pCtl, long* p_outsize, bool* p_aborted = NULL)
    {
        bool l_aborted;
        long l_outSize;
        bool l_res = impl->testElement(pCtl, &l_outSize, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        if( p_outsize ) *p_outsize = l_outSize;
        return( l_res );
    }

    bool RSCOMCALL putFormByRecordset(const char * pFormName, const char* szBindName, IRsObj* p_ds, bool* p_aborted = NULL)
    {
        bool l_aborted;
        bool l_res = impl->putFormByRecordset(pFormName, szBindName, p_ds, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        return( l_res );
    }
    
    bool RSCOMCALL putFormByRecordset(IForm* pForm, const char* szBindName, IRsObj* p_ds, bool* p_aborted = NULL)
    {
        bool l_aborted;
        bool l_res = impl->putFormByRecordset(pForm, szBindName, p_ds, &l_aborted);
        
        if( throwExceptOnPutAbort && l_aborted )
            throw;
        
        if( p_aborted ) *p_aborted = l_aborted;
        return( l_res );
    }

RSCOM_INVOKE_BEGIN(TReportImpl, TContainerImpl)
RSCOM_INVOKE_LEVEL(RSCOM_SYS_LEVEL)
    RSCOM_DISP_METHOD(RTM_CONTROLINIT_1, RsCracker(this, &TReportImpl::_init))
    RSCOM_DISP_METHOD(RTM_DEINIT, RsCracker(this, &TReportImpl::_deinit))
RSCOM_INVOKE_LEVEL(LEVEL_RSFRUNTIME)
    RSCOM_DISP_METHOD(RTM_SETTEMPLATE, RsCracker(this, &TReportImpl::_setTemplate))
    RSCOM_DISP_METHOD(RTM_PUTELEMENTBYNAME, RsCracker(this, &TReportImpl::_putElementByName))
    RSCOM_DISP_METHOD(RTM_PUTELEMENT, RsCracker(this, &TReportImpl::_putElement))
    RSCOM_DISP_METHOD(RTM_PUTFORMBYRECORDSETBYNAME, RsCracker(this, &TReportImpl::_putFormByRecordsetByName))
    RSCOM_DISP_METHOD(RTM_PUTFORMBYRECORDSET, RsCracker(this, &TReportImpl::_putFormByRecordset))
    RSCOM_DISP_METHOD(RTM_SETPREVPAGLIMIT, RsCracker(this, &TReportImpl::_SetPreviewPagesLimit))
    RSCOM_DISP_METHOD(RTM_SETREPORTCOPIES, RsCracker(this, &TReportImpl::_SetReportCopies))
    RSCOM_DISP_METHOD(RTM_PRINT, RsCracker(this, &TReportImpl::_Print))
    RSCOM_DISP_METHOD(RTM_TESTELEMENT, RsCracker(this, &TReportImpl::_testElement))
    RSCOM_DISP_METHOD(RTM_TESTELEMENTBYNAME, RsCracker(this, &TReportImpl::_testElementByName))
RSCOM_INVOKE_END

RSCOM_TYPE_BEGIN_IMP(TReportImpl, TContainerImpl, TReport)
    RSCOM_METHODEX("setTemplate", RTM_SETTEMPLATE, RSCOM_UNDEF, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("putElementByName", RTM_PUTELEMENTBYNAME, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("putElement", RTM_PUTELEMENT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("putFormByRecordsetByName", RTM_PUTFORMBYRECORDSETBYNAME, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("putFormByRecordset", RTM_PUTFORMBYRECORDSET, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("SetPreviewPagesLimit", RTM_SETPREVPAGLIMIT, RSCOM_UNDEF, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("SetReportCopies", RTM_SETREPORTCOPIES, RSCOM_UNDEF, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("Print", RTM_PRINT, RSCOM_BOOL, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("testElement", RTM_TESTELEMENT, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
    RSCOM_METHODEX("testElementByName", RTM_TESTELEMENTBYNAME, RSCOM_INT, NULL, LEVEL_RSFRUNTIME)
RSCOM_TYPE_END

private:
    TRsStat _init(TValRef retVal, IRsObj * group, IRsObj * userRefObj, bool bOptimize)
    { return( init(TRsPtrX<IReportsGroup, IID_REPORTSGROUP>(group), userRefObj, bOptimize) ? RSL_STAT_OK : RSL_STAT_EX ); }
    
    TRsStat _deinit(TValRef retVal)
    { return( deinit(), RSL_STAT_OK ); }

    TRsStat _setTemplate(TValRef retVal, const char* szLib, const char* szRes)
    { return( setTemplate(szLib, szRes), RSL_STAT_OK ); }

    TRsStat _putElementByName(TValRef retVal, const char * p_name, IRsObj * elemRes)
    {
        retVal = 0;
        
        bool l_aborted;
        if( !putElement( p_name, TRsPtrX<IHostObject, IID_HOSTOBJECT>(elemRes), &l_aborted ) )
            retVal = !l_aborted ? 1 : 2;
        return( RSL_STAT_OK );
    }

    TRsStat _putElement(TValRef retVal, IRsObj * elem, IRsObj * elemRes)
    {
        retVal = 0;

        bool l_aborted;
        if( !putElement( TRsPtrX<IControl, IID_CONTROL>(elem), TRsPtrX<IHostObject, IID_HOSTOBJECT>(elemRes), &l_aborted ) )
            retVal = !l_aborted ? 0 : 2;
        return( RSL_STAT_OK );
    }

    TRsStat _testElementByName(TValRef retVal, const char * p_name, TIntRef p_outSize)
    {
        retVal = 0;
        
        bool l_aborted;
        long l_outSize;
        if( !testElement( p_name, &l_outSize, &l_aborted ) )
            retVal = !l_aborted ? 1 : 2;
        p_outSize = l_outSize;
        return( RSL_STAT_OK );
    }

    TRsStat _testElement(TValRef retVal, IRsObj * elem, TIntRef p_outSize)
    {
        retVal = 0;

        bool l_aborted;
        long l_outSize;
        if( !testElement( TRsPtrX<IControl, IID_CONTROL>(elem), &l_outSize, &l_aborted ) )
            retVal = !l_aborted ? 0 : 2;
        p_outSize = l_outSize;
        return( RSL_STAT_OK );
    }

    TRsStat _putFormByRecordsetByName(TValRef retVal, const char * szFormName, const char* szBindName, IRsObj* ds)
    {
        retVal = 0;

        bool l_aborted;
        if( !putFormByRecordset( szFormName, szBindName, ds, &l_aborted ) )
            retVal = !l_aborted ? 0 : 2;
        return( RSL_STAT_OK );
    }

    TRsStat _putFormByRecordset(TValRef retVal, IRsObj * form, const char* szBindName, IRsObj* ds)
    {
        retVal = 0;

        bool l_aborted;
        if( !putFormByRecordset( TRsPtrX<IForm, IID_FORM>(form), szBindName, ds, &l_aborted ) )
            retVal = !l_aborted ? 0 : 2;
        return( RSL_STAT_OK );
    }

    TRsStat _SetPreviewPagesLimit(TValRef retVal, long limit)
    { return( SetPreviewPagesLimit(limit), RSL_STAT_OK ); }

    TRsStat _SetReportCopies(TValRef retVal, long limit)
    { return( SetReportCopies(limit), RSL_STAT_OK ); }

    TRsStat _Print(TValRef retVal, bool preview, int mode, bool edit)
    { return( retVal = Print(preview, ( printmode )mode, edit), RSL_STAT_OK ); }

protected:
    TRsPtr<IReport2>    impl;
    bool                throwExceptOnPutAbort;
};

DECLARE_CLASS( TReport, TReportImpl );

/*-----------------------------------------------------------------------*/

class TReportTableImpl : public TControlImpl
{
public:
    TReportTableImpl(TAppObj* app) :
        TControlImpl(app), throwExceptOnPutAbort(true)
    {}

    void enableThrowExceptOnPutAbort(bool enable)
    {
        throwExceptOnPutAbort = enable;
    }

    TRsStat RSCOMCALL invoke(RSDISPID id, int cmdType, IParmInfo* pInfo, int level)
    {
        enum { RSREPTBL_E_ADDCONTENTSABORT = 0x80041111 };

        TRsStat stat = TControlImpl::invoke(id, cmdType, pInfo, level);
        if( throwExceptOnPutAbort )
        {
            TRsError errInfo(pInfo->getAppObj());
            int l_nErr = errInfo.getCount();
            if( l_nErr ) if( errInfo.getLevel( l_nErr - 1 ) == RSL_STAT_EX ) if( errInfo.getErrCode( l_nErr - 1 ) == RSREPTBL_E_ADDCONTENTSABORT )
                throw;
        }
        return stat;
    }

RSCOM_TYPE_BEGIN_IMP(TReportTableImpl,TControlImpl,TReportTable)
RSCOM_TYPE_END

protected:
    bool        throwExceptOnPutAbort;
};

DECLARE_CLASS(TReportTable, TReportTableImpl);

/*-----------------------------------------------------------------------*/
#endif // __RSFREPSRV_H__
