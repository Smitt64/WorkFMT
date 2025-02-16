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

void HotfixContentModel::makeHotFix(ErrorsModel *logs, UsrMakeHandle usr)
{
    std::function<void(ContentTreeItem *root)> func = [&usr, &func, &logs](ContentTreeItem *root)
    {
        QString msg;

        if (usr)
            usr(UsrMakeBegin, root);

        MakeResult result = root->make(msg);

        if (result == ResultSuccess)
            logs->addMessage(msg);
        else if (result == ResultFail)
            logs->addError(msg);
        else
            logs->appendError(msg, ErrorsModel::TypeWarning);

        if (usr)
            usr(UsrMakeEnd, root);

        if (result != ResultFail)
        {
            for (int i = 0; i <root->childCount(); i++)
                func(root->child(i));
        }
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
