#ifndef DATFILECONTENTTREEITEM_H
#define DATFILECONTENTTREEITEM_H

#include "filecontenttreeitem.h"

class DatFileContentTreeItem : public FileContentTreeItem
{
public:
    DatFileContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~DatFileContentTreeItem();

    virtual QVariant data(const int &column, const int &role) const;
};

#endif // DATFILECONTENTTREEITEM_H
