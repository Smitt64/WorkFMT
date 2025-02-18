#ifndef FOLDERCONTENTTREEITEM_H
#define FOLDERCONTENTTREEITEM_H

#include "contenttreeitem.h"
#include <QDir>
#include <QFileInfo>

class QFileIconProvider;
class FileContentTreeItem;
class FmtContentTreeItem;
class DatFileContentTreeItem;
class FolderContentTreeItem : public ContentTreeItem
{
public:
    FolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem = nullptr);
    virtual ~FolderContentTreeItem();

    const QString &folder() const;
    virtual QVariant data(const int &column, const int &role) const Q_DECL_OVERRIDE;
    virtual MakeResult make(const MakeAction &action, QString &msg) const Q_DECL_OVERRIDE;

    FolderContentTreeItem *appendFolder(const QString &name);
    FileContentTreeItem *appendFile(const QString &name);
    FmtContentTreeItem *appendFmtItem(const QString &name);
    DatFileContentTreeItem *appendDatItem(const QString &name);

    template<class T>
    T *appendFolder(const QString &name)
    {
        QString path = m_Folder + "\\" + name;

        return (T*)appendChild(std::make_unique<T>(QDir::toNativeSeparators(path)));
    }

private:
    QString m_Folder;
    QFileInfo m_Info;
    QStringList m_Dirs;
};

class OraSqlFolderContentTreeItem : public FolderContentTreeItem
{
public:
    OraSqlFolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem = nullptr);
    virtual ~OraSqlFolderContentTreeItem();

    virtual MakeResult make(const MakeAction &action, QString &msg) const Q_DECL_OVERRIDE;

private:
    typedef QList<QStringList> PathChunks;
    PathChunks deserializeGroupsFromJson(const QByteArray& jsonString) const;
    PathChunks deserializeGroupsFromXml(const QByteArray& xmlString) const;

    QString getFileNameWithoutExtension(const QString& fullPath) const;
    QStringList findFileInPathChunks(const PathChunks& chunks, const QString& file) const;
    PathChunks GetDiffGroups(const QStringList &paths) const;
};

#endif // FOLDERCONTENTTREEITEM_H
