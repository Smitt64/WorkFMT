#include "fmtcontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QIcon>
#include <QFileInfo>

FmtContentTreeItem::FmtContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem),
    m_Elem(ElementTable)
{
}

FmtContentTreeItem::~FmtContentTreeItem()
{

}

void FmtContentTreeItem::setElementType(const FmtElement &type)
{
    m_Elem = type;
}

QVariant FmtContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
        {
            if (m_Elem == ElementTable)
                return QIcon("://img/fmtable.png");
            else
                return QIcon("://img/index.png");
        }
        else if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementTable)
            {
                QFileInfo fi(fileName());
                return fi.baseName().toUpper();
            }
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
        {
            if (m_Elem == ElementIndex)
                return QObject::tr("create index");
            else
            {
                if (svnAction() == "modified")
                    return "alter table";
                else
                    return "create table";
            }
        }
    }

    return FileContentTreeItem::data(column, role);
}
