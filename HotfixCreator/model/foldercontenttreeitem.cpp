#include "foldercontenttreeitem.h"
#include "hotfixcontentmodel.h"
#include "filecontenttreeitem.h"
#include "fmtcontenttreeitem.h"
#include "datfilecontenttreeitem.h"
#include "toolsruntime.h"
#include <QFileIconProvider>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDomDocument>
#include <QDomElement>
#include <QJsonObject>
#include <QTextCodec>

FolderContentTreeItem::FolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem) :
    ContentTreeItem(parentItem)
{
    m_Folder = folder;
    m_Info = QFileInfo(folder);

    m_Dirs = m_Folder.split('\\');
}

FolderContentTreeItem::~FolderContentTreeItem()
{

}

const QString &FolderContentTreeItem::folder() const
{
    return m_Folder;
}

QVariant FolderContentTreeItem::data(const int &column, const int &role) const
{
    if (column == HotfixContentModel::ColumnName)
    {
        if (role == Qt::DisplayRole)
            return m_Dirs.back();
        else if (role == Qt::DecorationRole)
            return iconProvider()->icon(QFileIconProvider::Folder);
    }

    return ContentTreeItem::data(column, role);
}

FolderContentTreeItem *FolderContentTreeItem::appendFolder(const QString &name)
{
    QString path = m_Folder + "\\" + name;

    return (FolderContentTreeItem*)appendChild(std::make_unique<FolderContentTreeItem>(QDir::toNativeSeparators(path)));
}

FileContentTreeItem *FolderContentTreeItem::appendFile(const QString &name)
{
    QString path = m_Folder + "\\" + name;
    return (FileContentTreeItem*)appendChild(std::make_unique<FileContentTreeItem>(QDir::toNativeSeparators(path)));
}

FmtContentTreeItem *FolderContentTreeItem::appendFmtItem(const QString &name)
{
    QString path = m_Folder + "\\" + name;
    return (FmtContentTreeItem*)appendChild(std::make_unique<FmtContentTreeItem>(QDir::toNativeSeparators(path)));
}

DatFileContentTreeItem *FolderContentTreeItem::appendDatItem(const QString &name)
{
    QString path = m_Folder + "\\" + name;
    return (DatFileContentTreeItem*)appendChild(std::make_unique<DatFileContentTreeItem>(QDir::toNativeSeparators(path)));
}

MakeResult FolderContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action != ActionMake)
        return ResultSuccess;

    QDir d(m_Folder);
    bool hr = d.mkpath(m_Folder);

    if (hr)
        msg = QString("Создан каталог <b>%1</b>").arg(m_Folder);
    else
        msg = QString("Каталог <b>%1</b> не создан").arg(m_Folder);

    return hr ? ResultSuccess : ResultFail;
}

// ----------------------------------------------------------------------

OraSqlFolderContentTreeItem::OraSqlFolderContentTreeItem(const QString &folder, ContentTreeItem *parentItem) :
    FolderContentTreeItem(folder, parentItem)
{

}

OraSqlFolderContentTreeItem::~OraSqlFolderContentTreeItem()
{

}

OraSqlFolderContentTreeItem::PathChunks OraSqlFolderContentTreeItem::deserializeGroupsFromJson(const QByteArray& jsonString) const
{
    QList<QStringList> chunks;

    QTextCodec *cp866 = QTextCodec::codecForName("IBM 866");
    QJsonDocument doc = QJsonDocument::fromJson(cp866->toUnicode(jsonString).toLocal8Bit());

    if (doc.isArray())
    {
        QJsonArray jsonArray = doc.array();
        for (const QJsonValue& chunkValue : qAsConst(jsonArray))
        {
            QStringList chunk;
            if (chunkValue.isArray())
            {
                QJsonArray innerArray = chunkValue.toArray();
                for (const QJsonValue& strValue : qAsConst(innerArray))
                    chunk.append(strValue.toString());
            }

            chunks.append(chunk);
        }
    }

    return chunks;
}

QString OraSqlFolderContentTreeItem::getFileNameWithoutExtension(const QString& fullPath) const
{
    QFileInfo fileInfo(fullPath);
    return fileInfo.baseName(); // Возвращает имя файла без расширения
}

OraSqlFolderContentTreeItem::PathChunks OraSqlFolderContentTreeItem::deserializeGroupsFromXml(const QByteArray& xmlString) const
{
    QList<QStringList> chunks;

    QDomDocument doc;
    if (doc.setContent(xmlString))
    {
        QDomElement root = doc.documentElement();
        QDomNodeList chunkNodes = root.elementsByTagName("chunk");
        for (int i = 0; i < chunkNodes.size(); ++i)
        {
            QDomElement chunkElement = chunkNodes.at(i).toElement();
            QStringList chunk;
            QDomNodeList itemNodes = chunkElement.elementsByTagName("item");

            for (int j = 0; j < itemNodes.size(); ++j)
            {
                QDomElement itemElement = itemNodes.at(j).toElement();
                chunk.append(itemElement.text());
            }
            chunks.append(chunk);
        }
    }
    return chunks;
}

OraSqlFolderContentTreeItem::PathChunks OraSqlFolderContentTreeItem::GetDiffGroups(const QStringList &paths) const
{
    PathChunks chunks;

    QScopedPointer<QProcess> process(new QProcess());
    toolStartProcess(process.data(), "DiffToScript.exe", {"--normal"}, false, true);

    QTextStream stream(process.data());
    stream << paths.join(";");
    stream.flush();

    process->closeWriteChannel();
    process->waitForReadyRead();

    QByteArray output = process->readAllStandardOutput();
    chunks = deserializeGroupsFromJson(output);

    process->waitForFinished();

    return chunks;
}

QStringList OraSqlFolderContentTreeItem::findFileInPathChunks(const OraSqlFolderContentTreeItem::PathChunks& chunks, const QString& file) const
{
    for (const QStringList& pathList : chunks)
    {
        if (pathList.isEmpty())
            continue; // Пропускаем пустые списки

        // Проверяем, начинается ли первый элемент с '!'
        if (pathList.first().startsWith('!'))
        {
            // Сравниваем только с первым элементом
            QString firstElement = pathList.first().mid(1); // Убираем '!' для сравнения
            if (getFileNameWithoutExtension(firstElement) == file)
                return pathList; // Возвращаем весь QStringList, если файл найден
        }
        else
        {
            // Сравниваем file с каждым элементом в QStringList
            for (const QString& path : pathList)
            {
                if (getFileNameWithoutExtension(path) == file)
                {
                    // Возвращаем QStringList, содержащий только найденный файл
                    return QStringList{path};
                }
            }
        }
    }

    return QStringList(); // Возвращаем пустой QStringList, если файл не найден
}

MakeResult OraSqlFolderContentTreeItem::make(const MakeAction &action, QString &msg, const MakeParams &params) const
{
    if (action == ActionMake)
        return FolderContentTreeItem::make(action, msg, params);
    else if (action == ActionPrepare)
    {
        QTextStream stream(&msg);
        stream << QString::fromUtf8("Выполнена подготовка групп таблиц для запусска DiffToScript:");
        QStringList paths;
        for (int i = 0; i < childCount(); i++)
        {
            DatFileContentTreeItem *item = child<DatFileContentTreeItem>(i);

            if (item)
                paths.append(item->fullFileName());
        }

        stream << "<ul>";
        PathChunks chunks = GetDiffGroups(paths);
        for (int i = 0; i < childCount(); i++)
        {
            DatFileContentTreeItem *item = child<DatFileContentTreeItem>(i);

            if (item)
            {
                QString fname = getFileNameWithoutExtension(item->fullFileName());
                QStringList lst = findFileInPathChunks(chunks, fname);
                item->setChunks(lst);

                if (!lst.isEmpty())
                    stream << "<li><b>" << fname.toUpper() << "</b> (" << lst.join("; ").remove("!") << ")</li>";
                else
                    stream << "<li><b>" << fname.toUpper() << QString::fromUtf8("</b> (игнорируется)</li>");
            }
        }
        stream << "</ul>";
    }
    return ResultSuccess;
}
