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

    void setElementType(const FmtElement &type);
    void setWidth(const int &w);

    virtual MakeResult make(const MakeAction &action, QString &msg, const MakeParams &params) const Q_DECL_OVERRIDE;

private:
    FmtElement m_Elem;
    int m_Width;
};

#endif // FMTCONTENTTREEITEM_H
