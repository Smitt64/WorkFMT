
#include "pch.h"
#include "pgconvwrp.h"
#include "OraSqlParser.h"
#include <boost/locale.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// ------------------------------------------------------------------

std::wstring convertSqlOraToPgCommon(const std::wstring& Sql, std::wstring *Error, std::wstring* Tail)
{//e_pglib e_odbc
  OraSqlParser::sp_t opar = OraSqlParser::make(e_pglib, e_func_arg_out_as_procedure);

  boost::locale::localization_backend_manager my = boost::locale::localization_backend_manager::global();
  my.select("icu");

  boost::locale::generator gen(my);
  std::locale loc1251_ = gen("ru_RU.cp1251");
  std::locale loc866_ = gen("ru_RU.cp866");

  SOraPgParam* paramSp = opar->takeParam().get();
  paramSp->setUserParam(boost::locale::conv::to_utf<wchar_t>("USER", loc1251_).c_str(),
    boost::locale::conv::to_utf<wchar_t>("INDX", loc1251_).c_str(),
    boost::locale::conv::to_utf<wchar_t>("CHECK_DB_TEST_ZH", loc1251_).c_str()
  );

  paramSp->loadConvException("pgsql_cfg\\ora_to_pg_exception_replacer.txt", "ru_RU.cp866");
  paramSp->sysTableReplacer()->load("pgsql_cfg\\ora_to_pg_sys_table_replacer.txt", "ru_RU.cp866");
  paramSp->columnTypeReplacer()->load("pgsql_cfg\\ora_to_pg_column_type_replacer.txt", "ru_RU.cp866");
  paramSp->rowTypeReplacer()->load("pgsql_cfg\\ora_to_pg_row_type_replacer.txt", "ru_RU.cp866");
  paramSp->funcReplacer()->load("pgsql_cfg\\ora_to_pg_func_replacer.txt", "ru_RU.cp866");
  paramSp->wordReplacer()->load("pgsql_cfg\\ora_to_pg_key_word_replacer.txt", "ru_RU.cp866");
  paramSp->statementReplacer()->load("pgsql_cfg\\ora_to_pg_statement_replacer.txt", "ru_RU.cp866");
  paramSp->pgsqlAutoTypeCastMap()->load("pgsql_cfg\\ora_to_pg_postgre_auto_type_cast_map.txt", "ru_RU.cp866");
  paramSp->pgsqlTypeAliasFuncSign()->load("pgsql_cfg\\ora_to_pg_postgre_type_alias_func_signature.txt", "ru_RU.cp866");

  std::wstringstream of;
  ustring usql = Sql;
  opar->setText(usql.c_str());

  ustring_adapt err, tail;
  vector_ustring_adapt usql_res;

  try
  {
    usql_res = opar->convertSqlOraToPg(err, tail);
  }
  catch (const std::exception& e)
  {
    of << "EXCEPTION:" << std::endl << e.what() << std::endl;
  }
  catch (const std::string& e)
  {
    of << "EXCEPTION:" << std::endl << std::wstring(e.begin(), e.end()) << std::endl;
  }

  if (Error)
    *Error = err.c_str();

  if (Tail)
    *Tail = tail.c_str();

  BOOST_FOREACH(ustring_adapt & v, usql_res) {
    of << v.c_str();
  }
  of << std::endl;

  return of.str();
}

// ------------------------------------------------------------------

static void StdWSTR_ToWCharT(std::wstring &src, wchar_t** dst)
{
  if (!dst)
    return;

  int len = src.size() + 1;
  int size = sizeof(wchar_t) * len;

  *dst = (wchar_t*)malloc(size);
  memset(*dst, 0, size);

  if (!(*dst))
    return;

  wcscpy_s((*dst), len, src.c_str());
}

ORACONV_API void convertSqlOraToPg(const wchar_t* Sql, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail)
{
  if (!PgSql)
    return;

  std::wstring error;
  std::wstring tail;
  std::wstring result = convertSqlOraToPgCommon(Sql, &error, &tail);

  StdWSTR_ToWCharT(result, PgSql);
  StdWSTR_ToWCharT(error, Error);
  StdWSTR_ToWCharT(tail, Tail);
}