#include "foldercontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include "filecontenttreeitem.h"
#include "fmtcontenttreeitem.h"
#include <QFileIconProvider>
#include <QDebug>

FolderContentTreeItem::FolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem) :
    ContentTreeItem(parentItem)
{
    m_Folder = folder;
    m_Info = QFileInfo(folder);

    m_Dirs = m_Folder.split('\\');
}

FolderContentTreeItem::~FolderContentTreeItem()
{

}

const QString &FolderContentTreeItem::folder() const
{
    return m_Folder;
}

QVariant FolderContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DisplayRole)
            return m_Dirs.back();
        else if (role == Qt::DecorationRole)
            return iconProvider()->icon(QFileIconProvider::Folder);
    }

    return ContentTreeItem::data(column, role);
}

FolderContentTreeItem *FolderContentTreeItem::appendFolder(const QString &name)
{
    QString path = m_Folder + "\\" + name;

    return (FolderContentTreeItem*)appendChild(std::make_unique<FolderContentTreeItem>(QDir::toNativeSeparators(path)));
}

FileContentTreeItem *FolderContentTreeItem::appendFile(const QString &name)
{
    QString path = m_Folder + "\\" + name;
    return (FileContentTreeItem*)appendChild(std::make_unique<FileContentTreeItem>(QDir::toNativeSeparators(path)));
}

FmtContentTreeItem *FolderContentTreeItem::appendFmtItem(const QString &name)
{
    QString path = m_Folder + "\\" + name;
    return (FmtContentTreeItem*)appendChild(std::make_unique<FmtContentTreeItem>(QDir::toNativeSeparators(path)));
}
