#pragma once

#define ORACONV_EXPORTS

#ifdef _WIN32
#ifdef ORACONV_EXPORTS
#define ORACONV_API __declspec(dllexport)
#else
#define ORACONV_API __declspec(dllimport)
#endif
#else
#define ORACONV_API __attribute__((visibility("default")))
#endif

#include <string>

#ifdef __cplusplus
extern "C" {
#endif
ORACONV_API void convertSqlOraToPg(const wchar_t* Sql, const wchar_t* User, wchar_t** PgSql, wchar_t** Error, wchar_t** Tail);
#ifdef __cplusplus
}
#endif