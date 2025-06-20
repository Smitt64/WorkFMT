#ifndef ORASQLPARSER_H
#define ORASQLPARSER_H

#include "TypesOraPgParam.h"
#include <vector>

#include "FileTextLoader.h"
#include "PgSqlGenerator.h"

#include "SharedInterfaceAdapt.h"

enum OraSqlParserType {
  e_odbc,
  e_pglib
};

enum OraSqlParserFuncArgOutType {
  e_func_arg_out_as_function,
  e_func_arg_out_as_procedure,
  e_func_arg_out_as_auto,
  e_func_arg_out_as_unknown
};

class __SHARED_DIRECTION OraSqlParser : public virtual FileTextLoader
{
public:
   typedef boost::shared_ptr<OraSqlParser> sp_t;

   static OraSqlParser::sp_t make(OraSqlParserType type, OraSqlParserFuncArgOutType func_arg_out_type);

   virtual boost::shared_ptr<SOraPgParam> takeParam() = 0;
   virtual vector_ustring_adapt getSessionPrepareScripts() = 0;
   virtual vector_ustring_adapt checkNotFoundInDbView() = 0;
   virtual void assignPlaceholderTypes(vector_ustring_icase_adapt && v_ph_types) = 0;
   virtual vector_ustring_adapt convertSqlOraToPg(ustring_adapt &err, ustring_adapt &tail) = 0;
   virtual void reset() = 0;

   virtual IPgSqlGenerator & getPgSqlGenerator() = 0;

   virtual void outInternalLog() = 0;
};

#endif