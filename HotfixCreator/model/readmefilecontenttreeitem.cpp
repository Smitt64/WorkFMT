#include "readmefilecontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <rsscript/registerobjlist.hpp>
#include "paramsmap.h"
#include "QVariant"

ReadmeFileRslExecutor::ReadmeFileRslExecutor(ContentTreeItem *root, const MakeParams &params, QObject *parent) :
    RslExecutor(parent),
    _params(new ParamsMap(std::cref(params)))
{
    _root = root;
}

ReadmeFileRslExecutor::~ReadmeFileRslExecutor()
{

}

void ReadmeFileRslExecutor::onBeginExec(const QString &modname)
{
    RegisterObjList::inst()->AddObject<ContentTreeItem>();
    RegisterObjList::inst()->AddObject<ParamsMap>();
}

void ReadmeFileRslExecutor::PlayRepProc()
{
    call("ReadmeFile", { QVariant::fromValue<QObject*>(_root),  QVariant::fromValue<QObject*>(_params.data()) });
}

// ---------------------------------------------------------------------------------

ReadmeFileContentTreeItem::ReadmeFileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}

ReadmeFileContentTreeItem::~ReadmeFileContentTreeItem()
{

}

MakeResult ReadmeFileContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake)
    {
        QDir d(params[PARAM_HOTFIX_DIR].toString());
        d.cd(params[PARAM_HOTFIX_NAME].toString());
        QString filename = d.absoluteFilePath(data(0, Qt::DisplayRole).toString());

        ContentTreeItem *root = rootItem();

        if (!root)
        {
            msg = tr("Не найден корневой элемент");
            return ResultWarning;
        }

        ReadmeFileRslExecutor executor(root, std::cref(params));
        executor.setDebugMacroFlag(true);
        executor.playRep("readmemake.mac", filename);

        QStringList err = executor.errors();
        if (!err.isEmpty())
        {
            msg = err.join("\r\n");
            return ResultFail;
        }
    }
    return ResultSuccess;
}

// ---------------------------------------------------------------------------------

BuildTxtFileContentTreeItem::BuildTxtFileContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}

BuildTxtFileContentTreeItem::~BuildTxtFileContentTreeItem()
{

}

MakeResult BuildTxtFileContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake)
    {
        QDir d(params[PARAM_HOTFIX_DIR].toString());
        d.cd(params[PARAM_HOTFIX_NAME].toString());
        QString filename = d.absoluteFilePath(data(0, Qt::DisplayRole).toString());

        QFile f(filename);
        if (!f.open(QIODevice::WriteOnly))
        {
            msg = QString("Не удалось сохранить рекомендации по сборке в файл <b>%1</b>")
                    .arg(QDir::toNativeSeparators(filename));
            return ResultWarning;
        }

        QTextStream stream(&f);
        stream.setCodec("IBM 866");
        stream << params[PARAM_BUILDINSTRUCTION].toString();

        msg = QString("Рекомендации по сборке успешно сохранены в файл <b>%1</b>")
                .arg(QDir::toNativeSeparators(filename));

        f.close();
    }

    return ResultSuccess;
}
