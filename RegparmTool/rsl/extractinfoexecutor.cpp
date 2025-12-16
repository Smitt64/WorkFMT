#include "extractinfoexecutor.h"
#include "rslmodule/richtext/rttable.h"
#include "rslmodule/iterableobjectbase.h"
#include "rsscript/registerobjlist.hpp"
#include "rsl/reginfoobj.h"
#include <QDebug>

#define REGISTER_OBJ(Type) RegisterObjList::inst()->RegisterRslObject<Type>(GenInfoUseParentProps | GenInfoUseParentMeths)
#define ADDTORSL_OBJ(Type, CanCreate) RegisterObjList::inst()->AddObject<Type>(CanCreate)

ExtractInfoExecutor::ExtractInfoExecutor(QObject *parent) :
    RslExecutor(parent),
    m_pDocument(nullptr)
{
    REGISTER_OBJ(RegInfoObj);
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
    ADDTORSL_OBJ(RegInfoObj, true);
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
    //
    qDebug() << retVal;

    QVariantList lst = retVal.toList();
    for (auto info : lst)
    {
        QObject *obj = info.value<QObject*>();
        RegInfoObj *InfoObj = dynamic_cast<RegInfoObj*>(obj);

        if (InfoObj)
        {
            QSharedPointer<RegInfoObj> ptr(new RegInfoObj(*InfoObj));
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
