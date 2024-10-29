#include "StdAfx.h"
#include "CoreConnection.h"

CCoreConnection::CCoreConnection(void) :
	CCoreObject(),
	pEnv(NULL),
	pConnection(NULL),
	pLastError(NULL)
{
	_putenv("NLS_LANG=AMERICAN_CIS.RU8PC866");

	strcpy(m_RDDrvO, "RDDrvO");
	strcpy(m_RDDrvODll, "RDDrvO.dll");
}


CCoreConnection::~CCoreConnection(void)
{
	freeConnection();
}

void CCoreConnection::freeConnection()
{
	if (pConnection)
	{
		delete pConnection;
		pConnection = NULL;
	}

	if (pEnv)
	{
		delete pEnv;
		pEnv = NULL;
	}
}

CCoreError *CCoreConnection::error()
{
	return pLastError;
}

bool CCoreConnection::isUnicode() const
{
	return !strcmp(m_RDDrvO, "RDDrvOu");
}

bool CCoreConnection::isPostgres() const
{
	if (pConnection)
		return pConnection->isPostgres();
	return false;
}

bool CCoreConnection::isOpen() const
{
	if (pConnection)
		return pConnection->IsOpened();

	return false;
}

bool CCoreConnection::beginTransaction()
{
	bool result = true;

	try
    {
        pConnection->beginTrans();
    }
    catch (XRsdError& e)
    {
        pLastError = new CCoreError(e);
        result = false;
    }
    catch(...)
    {
        setLastError("Transaction failed error");
        result = false;
    }

	return result;
}

bool CCoreConnection::commitTransaction()
{
	bool result = true;

	try
    {
        pConnection->commitTrans();
    }
    catch (XRsdError& e)
    {
        pLastError = new CCoreError(e);
        result = false;
    }
    catch(...)
    {
        setLastError("Transaction failed error");
        result = false;
    }

	return result;
}

bool CCoreConnection::rollbackTransaction()
{
	bool result = true;

	try
    {
        pConnection->rollbackTrans();
    }
    catch (XRsdError& e)
    {
        pLastError = new CCoreError(e);
        result = false;
    }
    catch(...)
    {
        setLastError("Transaction failed error");
        result = false;
    }

	return result;
}

void CCoreConnection::close()
{
	if (!pConnection)
		return;

	try
    {
		pConnection->close();
		freeConnection();
	}
	catch (XRsdError& e)
	{
		pLastError = new CCoreError(e);
	}
	catch (...)
	{
		setLastError("Unintended conclusion");
	}
}

bool CCoreConnection::open(const char *dsn, const char *user, const char *pswd, const char *options)
{
	bool hr = true;
	setLastError("");

	if (pLastError)
	{
		delete pLastError;
		pLastError = NULL;
	}

	if (strstr(options, "RSD_UNICODE"))
	{
		strcpy(m_RDDrvO, "RDDrvOu");
		strcpy(m_RDDrvODll, "RDDrvOu.dll");
	}

	pEnv = new CRsdEnvironment(m_RDDrvO, m_RDDrvODll);
	pEnv->SetOdbcInterface(NULL);
    pEnv->setClientEncoding(RSDENC_OEM);

	strcpy(db866, dsn);
    strcpy(user866, user);
    strcpy(password866, pswd);

	pConnection = new CRsdConnection(*pEnv, db866, user866, password866);
	pConnection->setClientEncoding(RSDENC_OEM);
    pConnection->setServerEncoding(RSDENC_OEM);

	try
	{
		pConnection->open();
	}
	catch (XRsdError& e)
    {
		pLastError = new CCoreError(e);
		freeConnection();
        hr = false;
    }
	catch (...)
	{
		freeConnection();
		setLastError("Connection error");
		hr = false;
	}
	
	return hr;
}

CRsdConnection *CCoreConnection::connection()
{
	return pConnection;
}