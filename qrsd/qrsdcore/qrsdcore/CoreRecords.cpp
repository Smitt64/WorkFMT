#include "StdAfx.h"
#include "CoreRecords.h"
#include "CoreCommand.h"

CCoreRecords::CCoreRecords(void) :
	pRec(NULL)
{
}


CCoreRecords::~CCoreRecords(void)
{
	if (pRec)
	{
		delete pRec;
		pRec = NULL;
	}
}

bool CCoreRecords::isNull()
{
	return !pRec;
}

CCoreRecords *CCoreRecords::create(CCoreCommand *cmd, bool isForwardOnly)
{
	CCoreRecords *ptr = NULL;
	CRsdCommand *command = cmd->command();
	CRsdRecordset *rs = new CRsdRecordset(*command, RSDVAL_CLIENT, isForwardOnly ? RSDVAL_FORWARD_ONLY : RSDVAL_STATIC);

	try
	{
		rs->open();
	}
	catch (XRsdError& e)
    {
        delete rs;
		rs = NULL;
    }
    catch (...)
    {
        delete rs;
		rs = NULL;
    }

	if (rs)
	{
		ptr = new CCoreRecords();
		ptr->pRec = rs;
	}

	return ptr;
}

int CCoreRecords::getCurPos()
{
	return pRec->getCurPos();
}

int CCoreRecords::getFldCount()
{
	return pRec->getFldCount();
}

int CCoreRecords::getRecCount()
{
	return pRec->getRecCount();
}

bool CCoreRecords::moveFirst()
{
	return pRec->moveFirst();
}

bool CCoreRecords::move(const int &index, const int &dir)
{
	return pRec->move(index, (RSDOrientation_t)dir);
}

bool CCoreRecords::moveLast()
{
	return pRec->moveLast();
}

bool CCoreRecords::moveNext()
{
	return pRec->moveNext();
}

bool CCoreRecords::movePrev()
{
	return pRec->movePrev();
}

void CCoreRecords::getField(CCoreFld **fld, const int &index)
{
	*fld = new CCoreFld(&pRec->getFld(index));
}


// ----------------------------------------------------------------------------

CCoreFld::CCoreFld(CRsdField *fld)
{
	pfld = fld;
}

CCoreFld::~CCoreFld()
{
	pfld = NULL;
}

bool CCoreFld::IsNull()
{
	return pfld->isNull();
}

int CCoreFld::Type()
{
	return pfld->getType();
}

const char *CCoreFld::Name()
{
	return pfld->getName();
}

char CCoreFld::AsByte()
{
	return pfld->AsRSDBYTE();
}

short CCoreFld::AsShort()
{
	return pfld->AsRSDSHORT();
}

unsigned short CCoreFld::AsUShort()
{
	return pfld->AsRSDUSHORT();
}

int CCoreFld::AsLong()
{
	return pfld->AsRSDLONG();
}

unsigned int CCoreFld::AsULong()
{
	return pfld->AsRSDULONG();
}

long long CCoreFld::AsBigInt()
{
	return pfld->AsRSDBIGINT();
}

float CCoreFld::AsFloat()
{
	return pfld->AsRSDFLOAT();
}

double CCoreFld::AsDouble()
{
	if (pfld->getType() == RSDPT_NUMERIC)
	{
		Numeric _rsNumeric = pfld->AsRSDNUMERIC();
		return Numeric96::convertToDouble(_rsNumeric);
	}

	return pfld->AsRSDDOUBLE();
}

const char *CCoreFld::AsString()
{
	return pfld->AsRSDLPSTR();
}

void CCoreFld::AsDate(void *tmp)
{
	bdate val = pfld->AsRSDDATE();
	memcpy(tmp, &val, sizeof(bdate));
}

void CCoreFld::AsTime(void *tmp)
{
	btime val = pfld->AsRSDTIME();
	memcpy(tmp, &val, sizeof(btime));
}

void CCoreFld::AsDateTime(void *tmp)
{
	btimestamp val = pfld->AsRSDTIMESTAMP();
	memcpy(tmp, &val, sizeof(btimestamp));
}

char CCoreFld::AsChar()
{
	return pfld->AsRSDCHAR();
}

int CCoreFld::bufferLen()
{
	return pfld->getBufferLen();
}

void* CCoreFld::AsBinary()
{
	return pfld->AsRSDBINARY();
}

void CCoreFld::read(void *ptr, int size)
{
	pfld->read(ptr, size);
}