#ifndef __RESMGR_H_GUARD__
#define __RESMGR_H_GUARD__

#include "trnmgr.h"

/* Может ли ресурс использовать MS DTC */

class CRsResourceMgr: public RsResourceMgr
{
public:
	CRsResourceMgr()
	{
		vtbl = &MethTable;
		m_hResourceMgr = this;
	}
	virtual ~CRsResourceMgr() {}

protected:
	
	// Handles support
	HRSRESOURCEMGR m_hResourceMgr;
	static IRsResourceMgr MethTable;
	
	virtual TrnResult Begin(int attr) = 0;
	virtual TrnResult Commit() = 0;
	virtual TrnResult Abort() = 0;
	virtual TrnResult CanUseMSDTC() = 0;
	virtual TrnResult EnlistMSDTC(ITransaction*) = 0;
	virtual TrnResult GetModuleName(char*, int) = 0;
	virtual TrnResult Init() = 0;
	virtual TrnResult Done() = 0;

	static TrnResult pBegin(HRSRESOURCEMGR, int attr);
	static TrnResult pCommit(HRSRESOURCEMGR);
	static TrnResult pAbort(HRSRESOURCEMGR);
	static TrnResult pCanUseMSDTC(HRSRESOURCEMGR);
	static TrnResult pEnlistMSDTC(HRSRESOURCEMGR, ITransaction*);
	static TrnResult pGetModuleName(HRSRESOURCEMGR, char*, int);
	static TrnResult pInit(HRSRESOURCEMGR);
	static TrnResult pDone(HRSRESOURCEMGR);

};

#define DEFINE_RESMANAGER_METHODS()	\
	IRsResourceMgr CRsResourceMgr::MethTable = \
	{	\
CRsResourceMgr::pBegin,	\
CRsResourceMgr::pCommit,	\
CRsResourceMgr::pAbort,	\
CRsResourceMgr::pCanUseMSDTC,	\
CRsResourceMgr::pEnlistMSDTC,	\
CRsResourceMgr::pGetModuleName,	\
CRsResourceMgr::pInit,	\
CRsResourceMgr::pDone	\
};	\
TrnResult CRsResourceMgr::pBegin(HRSRESOURCEMGR hMgr, int attr)	\
{	\
	return static_cast<CRsResourceMgr*>(hMgr)->Begin(attr);	\
}	\
TrnResult CRsResourceMgr::pCommit(HRSRESOURCEMGR hMgr)	\
{	\
	return static_cast<CRsResourceMgr*>(hMgr)->Commit();	\
}	\
TrnResult CRsResourceMgr::pAbort(HRSRESOURCEMGR hMgr)	\
{	\
	return static_cast<CRsResourceMgr*>(hMgr)->Abort();	\
}	\
TrnResult CRsResourceMgr::pCanUseMSDTC(HRSRESOURCEMGR hMgr)	\
{	\
	return static_cast<CRsResourceMgr*>(hMgr)->CanUseMSDTC();	\
}	\
TrnResult CRsResourceMgr::pEnlistMSDTC(HRSRESOURCEMGR hMgr, ITransaction* pTrn)	\
{	\
	return static_cast<CRsResourceMgr*>(hMgr)->EnlistMSDTC(pTrn);	\
}	\
TrnResult CRsResourceMgr::pGetModuleName(HRSRESOURCEMGR hMgr, char* pBuffer, int len)	\
{	\
return static_cast<CRsResourceMgr*>(hMgr)->GetModuleName(pBuffer, len);	\
}	\
TrnResult CRsResourceMgr::pInit(HRSRESOURCEMGR hMgr)	\
{	\
return static_cast<CRsResourceMgr*>(hMgr)->Init();	\
}	\
TrnResult CRsResourceMgr::pDone(HRSRESOURCEMGR hMgr)	\
{	\
return static_cast<CRsResourceMgr*>(hMgr)->Done();	\
}	

#endif /* __RESMGR_H_GUARD__ */