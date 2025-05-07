#include "difftoscripexecutor.h"
#include "task.h"
#include "rsscript/registerobjlist.hpp"
#include "rslobj/taskoptionscontainer.h"
#include "rslobj/sqlstringlist.h"
#include "dbspelling.h"
#include "join.h"
#include <QDebug>

DiffToScripExecutor::DiffToScripExecutor(QObject *parent) :
    RslExecutor(parent)
{

}

DiffToScripExecutor::~DiffToScripExecutor()
{

}

void DiffToScripExecutor::setTaskOptions(const TaskOptions *opts)
{
    m_pTaskOptionsContainer.reset(new TaskOptionsContainer(*opts));
}

void DiffToScripExecutor::setSqlStringList(SqlStringList *opts)
{
    m_pSqlList = opts;
}

void DiffToScripExecutor::onWriteOut(const QString &msg)
{
    RslExecutor::onWriteOut(msg);
    qDebug() << "onWriteOut:" << msg;
}

void DiffToScripExecutor::onInspectModuleSymbol(Qt::HANDLE sym)
{
    QString name = getSymbolName(sym);

    if (!name.compare("{Options}", Qt::CaseInsensitive))
        globalSet(sym, QVariant::fromValue<QObject*>(m_pTaskOptionsContainer.data()));
    else if (!name.compare("{SqlStrings}", Qt::CaseInsensitive))
        globalSet(sym, QVariant::fromValue<QObject*>(m_pSqlList));
    else if (!name.compare("{Spelling}", Qt::CaseInsensitive))
        globalSet(sym, QVariant::fromValue<QObject*>(m_pDbSpelling));
}

void DiffToScripExecutor::setDbSpelling(DbSpelling *opts)
{
    m_pDbSpelling = opts;
}

void DiffToScripExecutor::setJoinTables(JoinTables *opts)
{
    m_pJoinTables = opts;
}

void DiffToScripExecutor::PlayRepProc()
{
    addRslObjects();

    call("GenSqlScrpt",
         {
             QVariant::fromValue<QObject*>(m_pJoinTables)
         });
}
