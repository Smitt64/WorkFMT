#pragma once

#include "CoreObject.h"
#include "CoreError.h"
#include <fdecimal.h>
#include <RsdC.h>

class CCoreConnection;
class CCoreCommand : public CCoreObject
{
public:
	CCoreCommand(CCoreConnection *connection);
	~CCoreCommand(void);

	bool setCmdText(const char *cmd);
	void clearParams();
	void setDisableOraToPgConverter(bool value);
	int execute();

	void addParam(const char* name, 
		RSDValType_t type, 
		void* value = 0, 
		long* indlen = 0, 
		long len = 0, 
		RSDBindParamIO_t dir = RSDBP_IN, 
		bool isWide = false);

	void insertParam(int index, 
		const char* name,
		RSDValType_t type, 
		void* value = 0, 
		long* indlen = 0, 
		long len = 0, 
		RSDBindParamIO_t dir = RSDBP_IN, 
		bool isWide = false);

	CRsdCommand *command();

private:
	CRsdCommand *pCmd;
};

