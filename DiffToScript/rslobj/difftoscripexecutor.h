#ifndef DIFFTOSCRIPEXECUTOR_H
#define DIFFTOSCRIPEXECUTOR_H

#include "taskoptions.h"
#include <rslexecutor.h>

using RslGlobalsMap = std::map<QString, QVariant>;

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

    void setJoinTables(JoinTables *opts);
    void setGlobalsVariables(const RslGlobalsMap &globals);

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;
    virtual void onSetStModuleAdd() Q_DECL_OVERRIDE;
    virtual void onWriteOut(const QString &msg) Q_DECL_OVERRIDE;
    virtual void onInspectModuleSymbol(Qt::HANDLE sym) Q_DECL_OVERRIDE;

private:
    JoinTables *m_pJoinTables;
    RslGlobalsMap m_GlobalsVariables;
};

#endif // DIFFTOSCRIPEXECUTOR_H
