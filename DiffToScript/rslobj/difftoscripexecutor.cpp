#include "difftoscripexecutor.h"
#include "task.h"
#include "rsscript/registerobjlist.hpp"
#include "rslobj/taskoptionscontainer.h"
#include "rslobj/sqlstringlist.h"
#include "dbspelling.h"
#include "difftableinfo.h"
#include "join.h"
#include "recordparser.h"
#include "sqlscriptmain.h"
#include <QDebug>
#include <rslmodule/sql/sqldatabase.h>

static void Rsl_diffCreateTableForSqlite()
{
    if (GetFuncParamCount() != 1)
        ThrowParamCountError(1);

    DiffTable *table = GetFuncParam<DiffTable*>(0);

    if (!table)
        ThrowParamTypeError<DiffTable>(0);

    QString sql = diffCreateTableForSqlite(table);
    SetReturnVal(sql);
}

static void Rsl_diffLoadDatToSqlite()
{
    enum
    {
        prm_filename = 0,
        prm_Connection,
        prm_table,
        prm_changes,

        prm_Max
    };

    if (GetFuncParamCount() != prm_Max)
        ThrowParamCountError(prm_Max);

    QString filename = GetFuncParam<QString>(prm_filename);
    SqlDatabase *Connection = GetFuncParam<SqlDatabase*>(prm_Connection);
    DiffTable *table = GetFuncParam<DiffTable*>(prm_table);
    bool changes = GetFuncParam<bool>(prm_changes);

    if (filename.isEmpty())
        ThrowParamTypeError<DiffTableInfo>(prm_filename);

    if (!Connection)
        ThrowParamTypeError<SqlDatabase>(prm_Connection);

    if (!table)
        ThrowParamTypeError<DiffTable>(prm_table);

    bool hr = diffLoadDatToSqlite(filename, Connection, table, changes);
    SetReturnVal(hr);
}

static void Rsl_Padding()
{
    int prmcount = GetFuncParamCount();

    QString result;
    if (prmcount)
        result = Padding();
    else
    {
        int depth = GetFuncParam<int>(0);
        result = Padding(depth);
    }

    SetReturnVal(result);
}

// ----------------------------------------------------------------------

DiffToScripExecutor::DiffToScripExecutor(QObject *parent) :
    RslExecutor(parent),
    m_pJoinTables(nullptr)
{

}

DiffToScripExecutor::~DiffToScripExecutor()
{

}

void DiffToScripExecutor::setGlobalsVariables(const RslGlobalsMap &globals)
{
    m_GlobalsVariables = globals;
}

void DiffToScripExecutor::onWriteOut(const QString &msg)
{
    RslExecutor::onWriteOut(msg);
    //qDebug() << "onWriteOut:" << msg;
}

void DiffToScripExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{
    QString name = getSymbolName(sym);

    auto it = std::find_if(m_GlobalsVariables.begin(), m_GlobalsVariables.end(),
                           [name](const auto &pair)
    {
        return name.compare(pair.first, Qt::CaseInsensitive) == 0;
    });

    if (it != m_GlobalsVariables.end())
        globalSet(sym, it->second);
}

void DiffToScripExecutor::onSetStModuleAdd()
{
    addRslObjects();
    RegisterObjList::inst()->AddStdProc("diffCreateTableForSqlite", Rsl_diffCreateTableForSqlite);
    RegisterObjList::inst()->AddStdProc("diffLoadDatToSqlite", Rsl_diffLoadDatToSqlite);
    RegisterObjList::inst()->AddStdProc("Padding", Rsl_Padding);
}

void DiffToScripExecutor::setJoinTables(JoinTables *opts)
{
    m_pJoinTables = opts;
}

void DiffToScripExecutor::PlayRepProc()
{
    call("GenSqlScrpt",
         {
             QVariant::fromValue<QObject*>(m_pJoinTables)
         });
}
