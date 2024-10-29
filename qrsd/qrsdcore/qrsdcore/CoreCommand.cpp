#include "StdAfx.h"
#include "CoreCommand.h"
#include "qrsdcore.h"
#include "CoreConnection.h"

CCoreCommand::CCoreCommand(CCoreConnection *connection) :
	CCoreObject(),
	pCmd(NULL)
{
	pCmd = new CRsdCommand(*connection->connection());
}


CCoreCommand::~CCoreCommand(void)
{
	if (pCmd)
	{
		if (pCmd->IsOpened())
			pCmd->close();

		delete pCmd;
		pCmd = NULL;
	}
}

CRsdCommand *CCoreCommand::command()
{
	return pCmd;
}

bool CCoreCommand::setCmdText(const char *cmd)
{
	return pCmd->setCmdText(cmd);
}

void CCoreCommand::clearParams()
{
	pCmd->clearParams();
}

void CCoreCommand::addParam(const char* name, 
	RSDValType_t type, 
	void* value, 
	long* indlen, 
	long len, 
	RSDBindParamIO_t dir, 
	bool isWide)
{
	pCmd->addParam(name, type, value, indlen, len, dir, isWide);
}

void CCoreCommand::insertParam(int index, 
	const char* name,
	RSDValType_t type, 
	void* value, 
	long* indlen, 
	long len, 
	RSDBindParamIO_t dir, 
	bool isWide)
{
	pCmd->insertParam(index, name, type, value, indlen, len, dir, isWide);
}

void CCoreCommand::setDisableOraToPgConverter(bool value)
{
	pCmd->setDisableOraToPgConverter(value);
}

int CCoreCommand::execute()
{
	bool result = false;

	try
	{
		result = pCmd->execute();
	}
	catch(XRsdError& e)
	{
		std::string str;
		CCoreError error(e);

		int count = error.count();
		for (int i = 0; i < count; i++)
		{
			
			str += error.text(i);
			str += "\n";
		}

		result = false;
		setLastError(str.c_str());
	}

	return result;
}