#include "hotfixcontentmodel.h"
#include "model/contenttreeitem.h"
#include "model/foldercontenttreeitem.h"
#include "model/filecontenttreeitem.h"
#include "model/fmtcontenttreeitem.h"
#include "model/datfilecontenttreeitem.h"
#include "projectloader.h"
#include "errorsmodel.h"
#include <QDebug>
#include <QFileInfo>
#include <QDir>

bool ExecMakeAction(const MakeAction &action, ContentTreeItem *root, ErrorsModel *logs, bool logonlyerror)
{
    QString msg;

    MakeResult result = root->make(action, msg);
    if (result == ResultSuccess)
    {
        if (!logonlyerror && !msg.isEmpty())
            logs->addMessage(msg);
    }
    else if (result == ResultFail)
    {
        if (!msg.isEmpty())
            logs->addError(msg);

        return false;
    }
    else
    {
        if (!msg.isEmpty())
            logs->appendError(msg, ErrorsModel::TypeWarning);
    }

    return true;
}

void HotfixContentModel::makeHotFix(ErrorsModel *logs, UsrMakeHandle usr)
{
    std::function<void(ContentTreeItem *root)> func = [&usr, &func, &logs](ContentTreeItem *root)
    {
        if (usr)
            usr(UsrMakeBegin, root);

        bool result = ExecMakeAction(ActionPrepare, root, logs, false);

        if (result)
            result = ExecMakeAction(ActionMake, root, logs, false);

        if (usr)
            usr(UsrMakeEnd, root);

        if (result)
        {
            for (int i = 0; i <root->childCount(); i++)
                func(root->child(i));
        }

        result = ExecMakeAction(ActionEnd, root, logs, false);
    };

    for (int i = 0; i <rootItem->childCount(); i++)
    {
        FolderContentTreeItem *folder = dynamic_cast<FolderContentTreeItem*>(rootItem->child(i));

        if (folder)
        {
            QDir d(folder->folder());
            if (d.removeRecursively())
                logs->addMessage(QString("Каталог уже <b>%1</b> существует и был удален").arg(folder->folder()));
            else
                logs->appendError(QString("Каталог уже <b>%1</b> существует и не был удален").arg(folder->folder()), ErrorsModel::TypeWarning);
        }
        func(rootItem->child(i));
    }
}
