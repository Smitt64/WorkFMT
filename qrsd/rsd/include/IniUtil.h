#ifndef INIUTIL_H
#define INIUTIL_H

#include "RSDUtlBuildOption.h"

DLL_DIRECTION bool getIniValue(const char* fname, const char *name, char *val, size_t size);

#endif