#ifndef FOLDERCONTENTTREEITEM_H
#define FOLDERCONTENTTREEITEM_H

#include "contenttreeitem.h"
#include <QDir>
#include <QFileInfo>

class QFileIconProvider;
class FileContentTreeItem;
class FmtContentTreeItem;
class FolderContentTreeItem : public ContentTreeItem
{
public:
    FolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem = nullptr);
    virtual ~FolderContentTreeItem();

    const QString &folder() const;
    virtual QVariant data(const int &column, const int &role) const Q_DECL_OVERRIDE;

    FolderContentTreeItem *appendFolder(const QString &name);
    FileContentTreeItem *appendFile(const QString &name);
    FmtContentTreeItem *appendFmtItem(const QString &name);

private:
    QString m_Folder;
    QFileInfo m_Info;
    QStringList m_Dirs;
};

#endif // FOLDERCONTENTTREEITEM_H
