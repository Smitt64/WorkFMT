#include "difftoscripexecutor.h"
#include "task.h"
#include "rsscript/registerobjlist.hpp"
#include "rslobj/taskoptionscontainer.h"

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

void DiffToScripExecutor::PlayRepProc()
{
    addRslObjects();

    rslAddGlobal("{Options}", QVariant::fromValue<QObject*>(m_pTaskOptionsContainer.data()));
    call("GenSqlScrpt", {});
}
