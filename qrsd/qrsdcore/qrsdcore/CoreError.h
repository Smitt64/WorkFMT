#pragma once

#include "CoreObject.h"
#include <fdecimal.h>
#include <RsdC.h>
#include <vector>
#include <string>

class CCoreError : public CCoreObject
{
public:
	CCoreError(XRsdError &cError);
	CCoreError(CCoreError &other);
	~CCoreError(void);

	int count();
	char const* text(const int &index) const;

private:
	std::vector<std::string> m_Messages;
};

