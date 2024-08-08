#include "checksaveexecutor.h"
#include "fmttable.h"
#include "errorsmodel.h"
#include "rsscript/registerobjlist.hpp"
#include <QTemporaryFile>

CheckSaveExecutor::CheckSaveExecutor(FmtTable *pTable, ErrorsModel *errors)
    : RslExecutor{}
{
    m_pTable = pTable;
    m_pErrors = errors;
}

void CheckSaveExecutor::PlayRepProc()
{
    QVariantList params;
    params << QVariant::fromValue<QObject*>((QObject*)m_pTable);
    params << QVariant::fromValue<QObject*>((QObject*)m_pErrors);

    m_Result = call("CheckSave", params);
}

QVariant CheckSaveExecutor::check()
{
    QScopedPointer<QTemporaryFile> report(new QTemporaryFile());
    report->setAutoRemove(false);
    report->open();
    playRep("check_save.mac", report->fileName().toLocal8Bit().data(), nullptr);

    QStringList err = errors();
    std::for_each(err.begin(), err.end(), [=](const QString &text)
    {
        m_pErrors->addError(text);
    });

    if (m_pErrors->hasErrors())
        return false;

    return m_Result;
}
