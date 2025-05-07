#include <rsscript/registerobjlist.hpp>
#include "iterableobjectbase.h"
#include "sqlstringlist.h"
#include "join.h"
#include "scripttable.h"
#include "dbspellingoracle.h"
#include "dbspellingpostgres.h"
#include "taskoptionscontainer.h"
#include "sqlstringlist.h"
#include "sqlscriptmain.h"

#define REGISTER_DIFF_OBJ(Type) RegisterObjList::inst()->RegisterRslObject<Type>(GenInfoUseParentProps | GenInfoUseParentMeths)
#define ADDTORSL_DIFF_OBJ(Type, CanCreate) RegisterObjList::inst()->AddObject<Type>(CanCreate)

void registerRslObjects()
{
    REGISTER_DIFF_OBJ(IterableObjectBase);
    REGISTER_DIFF_OBJ(SqlStringList);
    REGISTER_DIFF_OBJ(JoinIndex);
    REGISTER_DIFF_OBJ(JoinList);
    REGISTER_DIFF_OBJ(Join);
    REGISTER_DIFF_OBJ(JoinTable);
    REGISTER_DIFF_OBJ(JoinTableList);
    REGISTER_DIFF_OBJ(JoinTables);

    REGISTER_DIFF_OBJ(IndexField);
    REGISTER_DIFF_OBJ(IndexFields);
    REGISTER_DIFF_OBJ(DiffField);
    REGISTER_DIFF_OBJ(DiffFields);
    REGISTER_DIFF_OBJ(DatIndex);
    REGISTER_DIFF_OBJ(DatIndexes);

    REGISTER_DIFF_OBJ(DiffTableInfo);
    REGISTER_DIFF_OBJ(DiffTable);
    REGISTER_DIFF_OBJ(ScriptTable);

    REGISTER_DIFF_OBJ(DbSpellingOracle);
    REGISTER_DIFF_OBJ(DbSpellingPostgres);

    REGISTER_DIFF_OBJ(DatRecord);
    REGISTER_DIFF_OBJ(DatRecords);
    REGISTER_DIFF_OBJ(TaskOptionsContainer);
    REGISTER_DIFF_OBJ(SqlStringList);
    REGISTER_DIFF_OBJ(SqlScriptMain);
}

void addRslObjects()
{
    rslAddConstant("ExceptDupValOnIndex", DbSpelling::ExceptDupValOnIndex);
    rslAddConstant("FunctionBeforeBlocks", DbSpelling::FunctionBeforeBlocks);
    rslAddConstant("FunctionInDeclareBlock", DbSpelling::FunctionInDeclareBlock);
    rslAddConstant("FunctionInBeginBlock", DbSpelling::FunctionInBeginBlock);

    rslAddConstant("ltInsert", ltInsert);
    rslAddConstant("ltDelete", ltDelete);
    rslAddConstant("ltUpdate", ltUpdate);
    rslAddConstant("ltTable", ltTable);
    rslAddConstant("ltLast", ltLast);

    rslAddConstant("lutNone", lutNone);
    rslAddConstant("lutOld", lutOld);
    rslAddConstant("lutNew", lutNew);
    rslAddConstant("lutLast", lutLast);

    rslAddModule("Fmt");

    ADDTORSL_DIFF_OBJ(IterableObjectBase, false);
    ADDTORSL_DIFF_OBJ(SqlStringList, false);
    ADDTORSL_DIFF_OBJ(JoinIndex, false);
    ADDTORSL_DIFF_OBJ(JoinList, false);
    ADDTORSL_DIFF_OBJ(Join, false);
    ADDTORSL_DIFF_OBJ(JoinTable, false);
    ADDTORSL_DIFF_OBJ(JoinTableList, false);
    ADDTORSL_DIFF_OBJ(JoinTables, false);

    ADDTORSL_DIFF_OBJ(IndexField, false);
    ADDTORSL_DIFF_OBJ(IndexFields, false);
    ADDTORSL_DIFF_OBJ(DiffField, false);
    ADDTORSL_DIFF_OBJ(DiffFields, false);
    ADDTORSL_DIFF_OBJ(DatIndex, false);
    ADDTORSL_DIFF_OBJ(DatIndexes, false);

    ADDTORSL_DIFF_OBJ(DiffTableInfo, false);
    ADDTORSL_DIFF_OBJ(DiffTable, false);
    ADDTORSL_DIFF_OBJ(ScriptTable, false);

    ADDTORSL_DIFF_OBJ(DbSpellingOracle, false);
    ADDTORSL_DIFF_OBJ(DbSpellingPostgres, false);

    ADDTORSL_DIFF_OBJ(DatRecord, false);
    ADDTORSL_DIFF_OBJ(DatRecords, false);

    ADDTORSL_DIFF_OBJ(TaskOptionsContainer, false);
    ADDTORSL_DIFF_OBJ(SqlStringList, false);
    ADDTORSL_DIFF_OBJ(SqlScriptMain, false);
}
