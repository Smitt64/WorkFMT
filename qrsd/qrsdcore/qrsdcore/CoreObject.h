#pragma once

#include <string>

class CCoreObject
{
public:
	CCoreObject(void);
	~CCoreObject(void);

	virtual void setLastError(const char *msg);
	virtual char const* what() const;

private:
	std::string m_ErrorString;
};

