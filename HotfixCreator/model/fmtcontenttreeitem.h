#ifndef FMTCONTENTTREEITEM_H
#define FMTCONTENTTREEITEM_H

#include "filecontenttreeitem.h"

class FmtContentTreeItem : public FileContentTreeItem
{
public:
    enum FmtElement
    {
        ElementTable,
        ElementIndex
    };
    FmtContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~FmtContentTreeItem();

    virtual QVariant data(const int &column, const int &role) const;

private:
    FmtElement m_Elem;
};

#endif // FMTCONTENTTREEITEM_H
