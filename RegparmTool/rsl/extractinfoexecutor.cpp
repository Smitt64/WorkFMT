#include "extractinfoexecutor.h"
#include "rslmodule/richtext/rttable.h"
#include "rslmodule/iterableobjectbase.h"
#include "rsl/reginfoobj.h"
#include <QJsonObject>

ExtractInfoExecutor::ExtractInfoExecutor(QObject *parent) :
    RslExecutor(parent),
    m_pDocument(nullptr)
{
}

ExtractInfoExecutor::~ExtractInfoExecutor()
{

}

void ExtractInfoExecutor::setDocument(QTextDocument *pDocument)
{
    m_pDocument = pDocument;
}

void ExtractInfoExecutor::onSetStModuleAdd()
{
    RslExecutor::onSetStModuleAdd();
}

void ExtractInfoExecutor::PlayRepProc()
{
    RTTableList *DocumentTables = nullptr;
    RTGetDocumentTables(m_pDocument, &DocumentTables);

    QVariant retVal = call("ExtractRegInfo",
         {
             QVariant::fromValue<QObject*>(DocumentTables)
         });

    m_List.clear();

    QVariantList lst = retVal.toList();
    for (const QVariant &info : qAsConst(lst))
    {
        QJsonObject obj = info.toJsonObject();
        if (!obj.isEmpty())
        {
            QSharedPointer<RegInfoObj> ptr = RegInfoObj::fromJson(obj);
            m_List.append(ptr);
        }
    }

    if (DocumentTables)
        delete DocumentTables;
}

QList<QSharedPointer<RegInfoObj>> ExtractInfoExecutor::objList()
{
    return m_List;
}
