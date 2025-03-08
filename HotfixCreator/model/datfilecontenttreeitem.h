#ifndef DATFILECONTENTTREEITEM_H
#define DATFILECONTENTTREEITEM_H

#include "filecontenttreeitem.h"

class DatFileContentTreeItem : public FileContentTreeItem
{
public:
    DatFileContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~DatFileContentTreeItem();

    virtual QVariant data(const int &column, const int &role) const Q_DECL_OVERRIDE;
    virtual MakeResult make(const MakeAction &action, QString &msg, const MakeParams &params) const Q_DECL_OVERRIDE;

    void setChunks(const QStringList &lst);
    bool isChunksEmpty() const;

private:
    QStringList m_Chunks;
};

#endif // DATFILECONTENTTREEITEM_H
