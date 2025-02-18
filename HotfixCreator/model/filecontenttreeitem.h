#ifndef FILECONTENTTREEITEM_H
#define FILECONTENTTREEITEM_H

#include "contenttreeitem.h"

class FileContentTreeItem : public ContentTreeItem
{
public:
    FileContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~FileContentTreeItem();

    void setSvnAction(const QString &action);
    void setFullFileName(const QString &action);
    virtual QVariant data(const int &column, const int &role) const Q_DECL_OVERRIDE;
    virtual MakeResult make(const MakeAction &action, QString &msg) const Q_DECL_OVERRIDE;

    static QIcon getIconForExtension(const QString &ext);
    const QString &fileName() const;
    const QString &svnAction() const;

private:
    QString m_FileName, m_SvnAction, m_FullFileName;
    QStringList m_Dirs;
};

#endif // FILECONTENTTREEITEM_H
