#include "StdAfx.h"
#include "CoreObject.h"

CCoreObject::CCoreObject(void)
{
}


CCoreObject::~CCoreObject(void)
{
}


void CCoreObject::setLastError(const char *msg)
{
	m_ErrorString = msg;
}

char const* CCoreObject::what() const
{
	return m_ErrorString.c_str();
}