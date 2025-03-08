#ifndef RELEASECHANGELOGCONTENTTREEITEM_H
#define RELEASECHANGELOGCONTENTTREEITEM_H

#include "filecontenttreeitem.h"

class QDomElement;
class QDomDocument;
class ReleaseChangelogContentTreeItem : public FileContentTreeItem
{
public:
    ReleaseChangelogContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~ReleaseChangelogContentTreeItem();

    virtual MakeResult make(const MakeAction &action, QString &msg, const MakeParams &params) const Q_DECL_OVERRIDE;

private:
    void addChangeSet(QDomDocument &doc, QDomElement &root, const MakeParams &params) const;
};

#endif // RELEASECHANGELOGCONTENTTREEITEM_H
