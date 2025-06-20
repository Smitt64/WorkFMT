#ifndef PGSQLGENERATOR_H
#define PGSQLGENERATOR_H

#include "Types.h"
#include "SqlConverterBuildParm.h"

#include "SharedInterfaceAdapt.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>


class __SHARED_DIRECTION IPgSqlGenerator
{
public:
  virtual vector_ustring_adapt genJarLoadScriptAndHttpShare(const ustring_adapt & path, const ustring_adapt & name) = 0;
};


#endif