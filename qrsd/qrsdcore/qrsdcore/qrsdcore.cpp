// qrsdcore.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "qrsdcore.h"
#include "CoreConnection.h"
#include "CoreCommand.h"
#include "CoreRecords.h"

#define CHKHNDL(handle,magic) CheckHandle(handle,magic)

bool CheckHandle(void *handle, const char *magic)
{
	return !memcmp(handle, magic, 6);
}

static void coreZeroHandle(void *handle)
{
	RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
	memset(pHandle, 0, sizeof(RSDCOREHANDLE));
}

void makeHandle(void **handle, CCoreObject *pObject)
{
	RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)malloc(sizeof(RSDCOREHANDLE));

	pHandle->ptr = pObject;
	
	if (dynamic_cast<CCoreConnection*>(pObject))
		memcpy(pHandle->magic, RSD_CON, 6);
	else if (dynamic_cast<CCoreError*>(pObject))
		memcpy(pHandle->magic, RSD_ERR, 6);
	else if (dynamic_cast<CCoreRecords*>(pObject))
		memcpy(pHandle->magic, RSD_REC, 6);
	else if (dynamic_cast<CCoreCommand*>(pObject))
		memcpy(pHandle->magic, RSD_CMD, 6);
	else if (dynamic_cast<CCoreFld*>(pObject))
		memcpy(pHandle->magic, RSD_FLD, 6);

	*handle = pHandle;
}

void coreFreeHandle(void **handle)
{
	if (!handle)
		return;

	if (!(*handle))
		return;
	
	RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)*handle;
	CCoreConnection *pconn = dynamic_cast<CCoreConnection*>(pHandle->ptr);
	CCoreError *perr = dynamic_cast<CCoreError*>(pHandle->ptr);
	CCoreRecords *prec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
	CCoreCommand *pcmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
	CCoreFld *pfld = dynamic_cast<CCoreFld*>(pHandle->ptr);

	if (pconn)
	{
		delete pconn;
		coreZeroHandle(*handle);
	}
	else if (perr)
	{
		delete perr;
		coreZeroHandle(*handle);
	}
	else if (prec)
	{
		delete prec;
		coreZeroHandle(*handle);
	}
	else if (pcmd)
	{
		delete pcmd;
		coreZeroHandle(*handle);
	}
	else if (pcmd)
	{
		delete pfld;
		coreZeroHandle(*handle);
	}
}

bool coreConnOpen(const char *dsn, const char *user, const char *pswd, const char *options, void **handle)
{
	bool stat = 0;

	CCoreConnection *pConn = new CCoreConnection();
	stat = pConn->open(dsn, user, pswd, options);
	makeHandle(handle, pConn);

	return stat;
}

void coreConnClose(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		pConn->close();
	}
}

bool coreConnIsUnicode(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->isUnicode();
	}

	return false;
}

bool coreConnIsPostgres(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->isPostgres();
	}

	return false;
}

bool coreConnIsOpen(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->isOpen();
	}

	return false;
}

bool coreConnBeginTrn(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->beginTransaction();
	}

	return false;
}

bool coreConnCommitTrn(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->commitTransaction();
	}

	return false;
}

bool coreConnRollbackTrn(void *handle)
{
	if (CHKHNDL(handle, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreConnection *pConn = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		return pConn->rollbackTransaction();
	}

	return false;
}

// errors ----------------------------------------------------------------------------

void coreGetLastError(void *handle, char *buffer, int len)
{
	if (CHKHNDL(handle, RSD_CON) 
		|| CHKHNDL(handle, RSD_REC) 
		|| CHKHNDL(handle, RSD_ENV)
		|| CHKHNDL(handle, RSD_ERR)
		|| CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		strncpy_s(buffer, len, pHandle->ptr->what(), _TRUNCATE);
	}
}

void coreGetErrorHandle(void *connection, void **handle)
{
	if (CHKHNDL(connection, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)connection;
		CCoreConnection *pConnection = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		if (pConnection && pConnection->error())
		{
			CCoreError *error = new CCoreError(*pConnection->error());
			makeHandle(handle, error);
		}
	}
}

int coreGetErrorCount(void *handle)
{
	if (CHKHNDL(handle, RSD_ERR))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreError *pError = dynamic_cast<CCoreError*>(pHandle->ptr);

		return pError->count();
	}

	return -1;
}

void coreGetErrorText(void *handle, int index, char *buffer, int len)
{
	if (CHKHNDL(handle, RSD_ERR))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreError *pError = dynamic_cast<CCoreError*>(pHandle->ptr);

		strncpy_s(buffer, len, pError->text(index), _TRUNCATE);
	}
}

// Cmd --------------------------------------------------------

void coreCmdInit(void **handle, void *connection)
{
	if (CHKHNDL(connection, RSD_CON))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)connection;
		CCoreConnection *pConnection = dynamic_cast<CCoreConnection*>(pHandle->ptr);

		CCoreCommand *pCmd = new CCoreCommand(pConnection);
		makeHandle(handle, pCmd);
	}
}

bool coreCmdSetCmdText(void *handle, const char *cmd)
{
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		return pCmd->setCmdText(cmd);
	}

	return false;
}

void coreCmdClearParams(void *handle)
{
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		pCmd->clearParams();
	}
}

void coreCmdAddParam(void *handle, const char* name, 
		int type, void* value, long* indlen, 
		long len, int dir, bool isWide)
{
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		pCmd->addParam(name, (RSDValType_t)type, value, indlen,
			len, (RSDBindParamIO_t)dir, isWide);
	}
}

void coreCmdInsertParam(void *handle, int index, const char* name, 
		int type, void* value, long* indlen, 
		long len, int dir, bool isWide)
{
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		pCmd->insertParam(index, name, (RSDValType_t)type, value, indlen,
			len, (RSDBindParamIO_t)dir, isWide);
	}
}

void coreCmdDisableConverter(void *handle, bool value)
{
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		pCmd->setDisableOraToPgConverter(value);
	}
}

int coreCmdExecute(void *handle)
{
	int stat = -1;
	if (CHKHNDL(handle, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreCommand *pCmd = dynamic_cast<CCoreCommand*>(pHandle->ptr);
		stat = pCmd->execute();
	}

	return stat;
}

// Rec --------------------------------------------------------

void coreRecInit(void **handle, void *cmd, bool isForwardOnly)
{
	if (CHKHNDL(cmd, RSD_CMD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)cmd;
		CCoreCommand *pCommand = dynamic_cast<CCoreCommand*>(pHandle->ptr);

		CCoreRecords *pRec = CCoreRecords::create(pCommand, isForwardOnly);
		makeHandle(handle, pRec);
	}
}

bool coreRecIsNull(void *handle)
{
	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		return pRec->isNull();
	}

	return true;
}

int coreRecCurPos(void *handle)
{
	int stat = -1;
	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->getCurPos();
	}

	return stat;
}

int coreRecFldCount(void *handle)
{
	int stat = -1;
	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->getFldCount();
	}

	return stat;
}

int coreRecCount(void *handle)
{
	int stat = -1;
	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->getRecCount();
	}

	return stat;
}

bool coreRecMove(void *handle, int index, int dir)
{
	bool stat = false;

	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->move(index, dir);
	}

	return stat;
}

bool coreRecFirst(void *handle)
{
	bool stat = false;

	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->moveFirst();
	}

	return stat;
}

bool coreRecLast(void *handle)
{
	bool stat = false;

	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->moveLast();
	}

	return stat;
}

bool coreRecNext(void *handle)
{
	bool stat = false;

	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->moveNext();
	}

	return stat;
}

bool coreRecPrev(void *handle)
{
	bool stat = false;

	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		stat = pRec->movePrev();
	}

	return stat;
}

void coreRecGetFld(void *handle, int index, void **field)
{
	if (CHKHNDL(handle, RSD_REC))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreRecords *pRec = dynamic_cast<CCoreRecords*>(pHandle->ptr);
		
		CCoreFld *pFld = NULL;
		pRec->getField(&pFld, index);
		makeHandle(field, pFld);
	}
}

const char *coreFldName(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->Name();
	}

	return NULL;
}

int coreFldType(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->Type();
	}

	return -1;
}

bool coreFldIsNull(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->IsNull();
	}

	return true;
}

char coreFldAsByte(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsByte();
	}

	return '\0';
}

short coreFldAsShort(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsShort();
	}

	return 0;
}

unsigned short coreFldAsUShort(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsUShort();
	}

	return 0;
}

int coreFldAsLong(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsLong();
	}

	return 0;
}

unsigned int coreFldAsULong(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsULong();
	}

	return 0;
}

long long coreFldAsBigInt(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsBigInt();
	}

	return 0;
}

float coreFldAsFloat(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsFloat();
	}

	return 0.0f;
}

double coreFldAsDouble(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsDouble();
	}

	return 0.0;
}

const char *coreFldAsString(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsString();
	}

	return NULL;
}

void coreFldAsDate(void *handle, void *tmp)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		pFld->AsDate(tmp);
	}
}

void coreFldAsTime(void *handle, void *tmp)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		pFld->AsTime(tmp);
	}
}

void coreFldAsDateTime(void *handle, void *tmp)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		pFld->AsDateTime(tmp);
	}
}

char coreFldAsChar(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsChar();
	}

	return '\0';
}

int coreFldBufferLen(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->bufferLen();
	}

	return 0;
}

void *coreFldAsBinary(void *handle)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->AsBinary();
	}

	return nullptr;
}

void coreFldRead(void *handle, void *ptr, int size)
{
	if (CHKHNDL(handle, RSD_FLD))
	{
		RSDCOREHANDLE *pHandle = (RSDCOREHANDLE*)handle;
		CCoreFld *pFld = dynamic_cast<CCoreFld*>(pHandle->ptr);

		return pFld->read(ptr, size);
	}
}