#ifndef __RSDMGR_H_GUARD__
#define __RSDMGR_H_GUARD__

#include "resmgr.h"
#include "RSDataDrv.h"

#include "rsdc.h"

class CRsdResourceMgr: public CRsResourceMgr
{
public:
	CRsdResourceMgr(IRSCon* iCon, RSDHCN hCon) : m_iCon(iCon), m_hCon(hCon) {}
	CRsdResourceMgr(CRsdConnection* pCon = 0) : m_pCon(pCon)
	{
		if( pCon )
		{
			m_iCon = pCon->GetIntf();
			m_hCon = pCon->GetHandle();
		}
		else
		{
			m_iCon = 0;
			m_hCon = 0;
		}
	}
	
	void SetConnection(CRsdConnection* pCon)
	{
		m_pCon = pCon;
		if( pCon )
		{
			m_iCon = pCon->GetIntf();
			m_hCon = pCon->GetHandle();
		}
		else
		{
			m_iCon = 0;
			m_hCon = 0;
		}
	}

	~CRsdResourceMgr() {}
	
protected:

	virtual TrnResult Begin(int attr)
	{ 
		if( m_pCon!=0 && m_iCon==0 )
		{
			m_pCon->open();
			m_iCon = m_pCon->GetIntf();
			m_hCon = m_pCon->GetHandle();
		}
		if( m_iCon && RSD_SUCCEEDED(m_iCon->pBeginTransaction(m_hCon)) )
			return TRN_OK;
		else
			return TRN_FAIL;
	}
	virtual TrnResult Commit()
	{
		if( m_iCon && RSD_SUCCEEDED(m_iCon->pCommitTransaction(m_hCon)) )
			return TRN_OK;
		else
			return TRN_FAIL;
	}
	virtual TrnResult Abort()
	{ 
		if( m_iCon && RSD_SUCCEEDED(m_iCon->pAbortTransaction(m_hCon)) )
			return TRN_OK;
		else
			return TRN_FAIL;
	}
	virtual TrnResult CanUseMSDTC() { return TRN_FAIL; }
	virtual TrnResult EnlistMSDTC(ITransaction*) {  return TRN_FAIL; }
	
	virtual TrnResult GetModuleName(char*, int) { return TRN_FAIL; }
	virtual TrnResult Init()  { return TRN_FAIL; }
	virtual TrnResult Done()  { return TRN_FAIL; }

	CRsdConnection* m_pCon;
	IRSCon*		m_iCon;
	RSDHCN		m_hCon;
};

class CRsdMsDtcResourceMgr: public CRsdResourceMgr
{
public:
	CRsdMsDtcResourceMgr(IRSCon* iCon, RSDHCN hCon) 
		: CRsdResourceMgr(iCon, hCon) {}
	CRsdMsDtcResourceMgr(CRsdConnection* Con)
		: CRsdResourceMgr(Con) {}
	~CRsdMsDtcResourceMgr() {}
	
protected:
	
	virtual TrnResult CanUseMSDTC() { return TRN_OK; }
	virtual TrnResult EnlistMSDTC(ITransaction* iTrn)
	{
		if( m_iCon && RSD_SUCCEEDED(m_iCon->pSetProperty(m_hCon, RSDPID_ENLIST_IN_DTC, RSDPT_BINARY, iTrn)) )
			return TRN_OK;
		else
			return TRN_FAIL;
	}
};

#endif