#include "fmtcontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QIcon>

FmtContentTreeItem::FmtContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem),
    m_Elem(ElementTable)
{
}

FmtContentTreeItem::~FmtContentTreeItem()
{

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
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
            return QObject::tr("create index");
    }

    return FileContentTreeItem::data(column, role);
}
