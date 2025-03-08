#include "releasechangelogcontenttreeitem.h"
#include "datfilecontenttreeitem.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>
#include <QProcess>
#include <QFileInfo>

QString getCurrentUsername()
{
    QProcess process;
#ifdef Q_OS_WIN
    process.start("whoami");
#else
    process.start("whoami");
#endif
    process.waitForFinished();
    QString username = process.readAllStandardOutput().trimmed();

    // Если имя доменное, извлекаем часть после '\'
    int backslashIndex = username.lastIndexOf('\\');
    if (backslashIndex != -1) {
        username = username.mid(backslashIndex + 1); // Извлекаем часть после '\'
    }

    return username;
}

ReleaseChangelogContentTreeItem::ReleaseChangelogContentTreeItem(const QString &file, ContentTreeItem *parentItem) :
    FileContentTreeItem(file, parentItem)
{

}


ReleaseChangelogContentTreeItem::~ReleaseChangelogContentTreeItem()
{

}

void ReleaseChangelogContentTreeItem::addChangeSet(QDomDocument &doc, QDomElement &root, const MakeParams &params) const
{
    QStringList WRITABLE_TABLES;
    QDomElement changeSet = doc.createElement("changeSet");
    changeSet.setAttribute("id", params[PARAM_HOTFIX_NAME].toString());
    changeSet.setAttribute("author", getCurrentUsername());
    changeSet.setAttribute("context", "upg");

    ContentTreeItem *pgitem = parentItem();
    ContentTreeItem *sqlfolder = pgitem->findItemByData(params[PARAM_HOTFIX_NAME], 0, Qt::DisplayRole);

    if (sqlfolder)
    {
        for (int i = 0; i < sqlfolder->childCount(); i++)
        {
            DatFileContentTreeItem *item = sqlfolder->child<DatFileContentTreeItem>(i);

            if (item)
            {
                if (!item->isChunksEmpty())
                {
                    QDomElement sqlFile = doc.createElement("sqlFile");
                    sqlFile.setAttribute("encoding", "cp866");
                    sqlFile.setAttribute("endDelimiter", "/");
                    sqlFile.setAttribute("splitStatements", "true");
                    sqlFile.setAttribute("stripComments", "false");

                    sqlFile.setAttribute("path", "./" + item->getPathToAncestor(pgitem));
                    changeSet.appendChild(sqlFile);
                }

                QFileInfo fi(item->fileName());
                WRITABLE_TABLES.append(fi.baseName().toUpper());
            }
        }
    }

    if (!WRITABLE_TABLES.isEmpty())
    {
        QDomElement comentwrite = doc.createElement("comment");
        QDomText textNode = doc.createTextNode(WRITABLE_TABLES.join("; ") + ";");
        comentwrite.appendChild(textNode);

        changeSet.appendChild(comentwrite);
    }

    root.appendChild(changeSet);
}

MakeResult ReleaseChangelogContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake)
    {
        QFile file(":/res/release-changelog.xml");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QDomDocument doc;
        doc.setContent(&file);
        file.close();

        QDomElement root = doc.documentElement();
        addChangeSet(doc, root, params);

        QFile outputFile(fileName());
        if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            msg = QString("Не удалось открыть файл <b>%1</b> для записи. <br>")
                    .arg(fileName(), doc.toString(4));
            return ResultFail;
        }

        QTextStream stream(&outputFile);
        stream << doc.toString();
        outputFile.close();
    }
    return ResultSuccess;
}
