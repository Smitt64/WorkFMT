#ifndef READMEFILECONTENTTREEITEM_H
#define READMEFILECONTENTTREEITEM_H

#include "filecontenttreeitem.h"
#include "rslexecutor.h"

class ReadmeFileContentTreeItem : public FileContentTreeItem
{
public:
    ReadmeFileContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~ReadmeFileContentTreeItem();

    virtual MakeResult make(const MakeAction &action, QString &msg, const MakeParams &params) const Q_DECL_OVERRIDE;
};

class BuildTxtFileContentTreeItem : public FileContentTreeItem
{
public:
    BuildTxtFileContentTreeItem(const QString &file, ContentTreeItem *parentItem = nullptr);
    virtual ~BuildTxtFileContentTreeItem();

    virtual MakeResult make(const MakeAction &action, QString &msg, const MakeParams &params) const Q_DECL_OVERRIDE;
};

class ParamsMap;
class ReadmeFileRslExecutor : public RslExecutor
{
    Q_OBJECT
public:
    ReadmeFileRslExecutor(ContentTreeItem *root, const MakeParams &params, QObject *parent = nullptr);
    virtual ~ReadmeFileRslExecutor();

protected:
    virtual void PlayRepProc() Q_DECL_OVERRIDE;
    void onBeginExec(const QString &modname) Q_DECL_OVERRIDE;

private:
    ContentTreeItem *_root;
    QScopedPointer<ParamsMap> _params;
};

#endif // READMEFILECONTENTTREEITEM_H
