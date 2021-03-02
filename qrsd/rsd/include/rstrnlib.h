#ifndef __RSTRNLIB_H_GUARD__
#define __RSTRNLIB_H_GUARD__

#include "trnmgr.h"
#include "resmgr.h"
#include <windows.h>

#define TOOLS_RSCMGR_NAME_SQL "SqlRscMgr"
#define TOOLS_RSCMGR_NAME_BTR "BtrieveRscMgr"

#ifdef SQLBUILD 
#define TOOLS_RSCMGR_NAME TOOLS_RSCMGR_NAME_SQL
#else
#define TOOLS_RSCMGR_NAME TOOLS_RSCMGR_NAME_BTR
#endif


class CRsTrnManager
{
public:
	CRsTrnManager(bool bLoad=true) : m_hTrnLib(0), m_Mgr(0) /* Инициализация rstrnmgr.dll */
	{
		if( !bLoad ) return;
		
		m_hTrnLib = LoadLibrary("rstrnmgr.dll");
		if( m_hTrnLib )
		{
			GetTrnMgr_t Get = (GetTrnMgr_t)GetProcAddress(m_hTrnLib, "GetTrnMgr");
			if( Get )
				m_Mgr = Get();
		}

	}

	bool Connect() /* подключиться к загруженному TrnMgr */
	{
        if(m_Mgr)
			return true;
		m_hTrnLib = GetModuleHandle("rstrnmgr.dll");
		if( !m_hTrnLib )
			return false;

		GetTrnMgr_t Get = (GetTrnMgr_t)GetProcAddress(m_hTrnLib, "GetTrnMgr");
		// пытаемся увеличить счетчик ссылок для избежания отгрузки до того, 
		// как прекратим пользоваться
		
		CHAR modulename[MAX_PATH];
		if (!GetModuleFileName(m_hTrnLib, modulename, MAX_PATH)
			|| !LoadLibrary(modulename) )
		{
			m_hTrnLib = 0;
			return false;
		}
		if( Get )
		{
			m_Mgr = Get();
			return true;
		}
		else
			return false;
	}

	~CRsTrnManager()
	{
		if( m_hTrnLib )
			FreeLibrary(m_hTrnLib);
	}

	TrnResult Begin(int attr)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pBegin(m_Mgr, attr);
	}
	TrnResult Commit()
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pCommit(m_Mgr);
	}
	TrnResult Abort()
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pAbort(m_Mgr);
	}

	TrnResult AddResourceMgr(HRSRESOURCEMGR pResMgr)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pAddResourceMgr(m_Mgr, pResMgr);
	}
	TrnResult RemResourceMgr(HRSRESOURCEMGR pResMgr)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pRemResourceMgr(m_Mgr, pResMgr);
	}
	TrnResult UseMsDTC(bool fUse)
	{
		return m_Mgr->vtbl->pUseMsDTC(m_Mgr, fUse);
	}
	bool InTransaction()
	{
		return TRN_SUCCESS(m_Mgr->vtbl->pInTransaction(m_Mgr));
	}
	HRSRESOURCEMGR GetResMgr(const char* name)
	{
		if( !m_Mgr )
			return 0;

		HRSRESOURCEMGR mgr = NULL;
		m_Mgr->vtbl->pGetResMgr(m_Mgr, name, &mgr);
		return mgr;
	}
	TrnResult InsertBefore(HRSRESOURCEMGR pResMgr, const char* name, const char* relative)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pInsertBefore(m_Mgr, pResMgr, name, relative);
	}
	TrnResult InsertAfter(HRSRESOURCEMGR pResMgr, const char* name, const char* relative)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pInsertAfter(m_Mgr, pResMgr, name, relative);
	}
	TrnResult SetCommitDirection(int fwd)
	{
		if( !m_Mgr ) return TRN_FAIL;
		return m_Mgr->vtbl->pSetCommitDirection(m_Mgr, fwd);
	}
	int GetResMgrCount()
	{
		if( !m_Mgr )
			return -1;
		int count = 0;
		if( TRN_SUCCESS(m_Mgr->vtbl->pGetResMgrCount(m_Mgr, &count)) )
			return count;
		else
			return -1;
	}
	HRSRESOURCEMGR GetResMgrByIndex(int index)
	{
		if( !m_Mgr )
			return NULL;
		HRSRESOURCEMGR pMgr = 0;
		if( TRN_SUCCESS(m_Mgr->vtbl->pGetResMgrByIndex(m_Mgr, index, &pMgr)) )
			return pMgr;
		else
			return 0;
	}
	const char* GetResMgrName(HRSRESOURCEMGR pMgr)
	{
		if( !m_Mgr )
			return NULL;
		const char* name = 0;
		if( TRN_SUCCESS(m_Mgr->vtbl->pGetResMgrName(m_Mgr, pMgr, &name)) )
			return name;
		else
			return 0;
	}

protected:
	HINSTANCE m_hTrnLib;

	HRSTRNMGR m_Mgr;
};

#endif
