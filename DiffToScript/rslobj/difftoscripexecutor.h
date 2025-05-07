#ifndef DIFFTOSCRIPEXECUTOR_H
#define DIFFTOSCRIPEXECUTOR_H

#include "taskoptions.h"
#include <rslexecutor.h>

class DbSpelling;
class SqlStringList;
class TaskOptionsContainer;
class JoinTables;
class DiffToScripExecutor : public RslExecutor
{
    Q_OBJECT
public:
    DiffToScripExecutor(QObject *parent = nullptr);
    virtual ~DiffToScripExecutor();

    void setTaskOptions(const TaskOptions *opts);
    void setSqlStringList(SqlStringList *opts);
    void setDbSpelling(DbSpelling *opts);
    void setJoinTables(JoinTables *opts);

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;
    virtual void onWriteOut(const QString &msg) Q_DECL_OVERRIDE;
    virtual void onInspectModuleSymbol(Qt::HANDLE sym) Q_DECL_OVERRIDE;

private:
    SqlStringList *m_pSqlList;
    DbSpelling *m_pDbSpelling;
    JoinTables *m_pJoinTables;

    QScopedPointer<TaskOptionsContainer> m_pTaskOptionsContainer;
};

#endif // DIFFTOSCRIPEXECUTOR_H
