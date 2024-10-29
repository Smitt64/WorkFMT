#pragma once

#include "CoreObject.h"
#include "CoreError.h"
#include <fdecimal.h>
#include <RsdC.h>

class CCoreConnection : public CCoreObject
{
public:
	CCoreConnection(void);
	~CCoreConnection(void);

	bool open(const char *dsn, const char *user, const char *pswd, const char *options);
	void close();

	CCoreError *error();
	bool isUnicode() const;
	bool isPostgres() const;
	bool isOpen() const;

	bool beginTransaction();
	bool commitTransaction();
	bool rollbackTransaction();

	CRsdConnection *connection();

private:
	void freeConnection();
	char m_RDDrvO[256], m_RDDrvODll[256];
    char db866[256], user866[256], password866[256];

	CRsdEnvironment *pEnv;
	CRsdConnection *pConnection;

	CCoreError *pLastError;
};

