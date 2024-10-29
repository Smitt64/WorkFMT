#include "StdAfx.h"
#include "CoreError.h"

CCoreError::CCoreError(XRsdError &cError)
{
	CRsdEnvironment *env = const_cast<CRsdEnvironment*>(cError.getEnv());

	if (!env)
        setLastError(cError.what());
	else
	{
		int errcount = env->getErrorCount();

		for (int i = 0; i < errcount; i++)
        {
			const CRsdError &error = env->getError(i);
            const char *descr = error.getDescr();

			m_Messages.push_back(descr);
		}
	}
}

CCoreError::CCoreError(CCoreError &other)
{
	m_Messages = other.m_Messages;
}

CCoreError::~CCoreError(void)
{
}

int CCoreError::count()
{
	return (int)m_Messages.size();
}

char const* CCoreError::text(const int &index) const
{
	return m_Messages[index].c_str();
}