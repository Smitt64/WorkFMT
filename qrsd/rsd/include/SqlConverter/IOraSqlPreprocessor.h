#ifndef IORASQLPREPROCESSOR_H
#define IORASQLPREPROCESSOR_H

#include "OraGrammarType.h"

#include "SharedInterfaceAdapt.h"

class IOraSqlPreprocessor
{
public:
	virtual ~IOraSqlPreprocessor() {}
	virtual ustring_adapt run(const ustring_adapt& script) = 0;
};

#endif