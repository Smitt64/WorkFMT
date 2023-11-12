#include "baseoperation.h"
#include "baseloghighlighter.h"
#include <QFile>

BaseOperation::BaseOperation(DumpToolWizard *wizzard) :
    QObject(),
    QRunnable(),
    m_pWizzard()
{
    m_pWizzard = wizzard;
    setAutoDelete(true);
}

BaseOperation::~BaseOperation()
{

}

DumpToolWizard *BaseOperation::wizzard()
{
    return m_pWizzard;
}

void BaseOperation::highlighter(BaseLogHighlighter **obj, QTextDocument *parent)
{
    *obj = new BaseLogHighlighter(parent);
}

bool BaseOperation::UnpackFile(const QString &res, const QString &dst)
{
    bool hr = true;
    QFile resFile(res);
    QFile dstFile(dst);

    if (resFile.open(QIODevice::ReadOnly) && dstFile.open(QIODevice::WriteOnly))
        dstFile.write(resFile.readAll());

    resFile.close();
    dstFile.close();

    return hr;
}
