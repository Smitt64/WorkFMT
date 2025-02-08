#include "datfilecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QIcon>
#include <QFileInfo>

DatFileContentTreeItem::DatFileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}

DatFileContentTreeItem::~DatFileContentTreeItem()
{

}

QVariant DatFileContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DecorationRole)
            return QIcon("://img/ext_sql.png");
        else if (role == Qt::DisplayRole)
        {
            QFileInfo fi(fileName());
            return fi.baseName().toLower() + ".sql";
        }
    }
    else if (column == HotfixContentModel::ColumnAction)
    {
        if (role == Qt::DisplayRole)
            return "diff to script";
    }

    return FileContentTreeItem::data(column, role);
}
