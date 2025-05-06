#ifndef DIFFTOSCRIPEXECUTOR_H
#define DIFFTOSCRIPEXECUTOR_H

#include "taskoptions.h"
#include <rslexecutor.h>

class TaskOptionsContainer;
class DiffToScripExecutor : public RslExecutor
{
    Q_OBJECT
public:
    DiffToScripExecutor(QObject *parent = nullptr);
    virtual ~DiffToScripExecutor();

    void setTaskOptions(const TaskOptions *opts);

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;

private:
    QScopedPointer<TaskOptionsContainer> m_pTaskOptionsContainer;
};

#endif // DIFFTOSCRIPEXECUTOR_H
