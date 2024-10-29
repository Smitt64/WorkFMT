#pragma once

#include "coreobject.h"
#include <fdecimal.h>
#include <RsdC.h>

class CCoreFld;
class CCoreCommand;
class CCoreRecords :
	public CCoreObject
{
public:
	static CCoreRecords *create(CCoreCommand *cmd, bool isForwardOnly);
	virtual ~CCoreRecords(void);

	bool isNull();
	int getCurPos();
	int getFldCount();
	int getRecCount();

	bool moveFirst();
	bool moveLast();
	bool moveNext();
	bool movePrev();
	bool move(const int &index, const int &dir);

	void getField(CCoreFld **fld, const int &index);

private:
	CCoreRecords();
	CRsdRecordset *pRec;
};

class CCoreFld :
	public CCoreObject
{
public:
	CCoreFld(CRsdField *fld);
	virtual ~CCoreFld();

	bool IsNull();
	int Type();
	const char *Name();

	char AsByte();
	short AsShort();
	unsigned short AsUShort();
	int AsLong();
	unsigned int AsULong();
	long long AsBigInt();
	float AsFloat();
	double AsDouble();
	const char *AsString();
	char AsChar();

	void AsDate(void *tmp);
	void AsTime(void *tmp);
	void AsDateTime(void *tmp);

	int bufferLen();
	void* AsBinary();
	void read(void *ptr, int size);

private:
	CRsdField *pfld; 
};