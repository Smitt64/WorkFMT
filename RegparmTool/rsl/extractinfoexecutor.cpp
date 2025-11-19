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
    /*RegisterObjList::inst()->AddStdProc("diffCreateTableForSqlite", Rsl_diffCreateTableForSqlite);
    RegisterObjList::inst()->AddStdProc("diffLoadDatToSqlite", Rsl_diffLoadDatToSqlite);
    RegisterObjList::inst()->AddStdProc("Padding", Rsl_Padding);*/
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

    //qDebug() << retVal;

    if (DocumentTables)
        delete DocumentTables;
}

// Оператор вывода в QDebug (добавить в public секцию)
