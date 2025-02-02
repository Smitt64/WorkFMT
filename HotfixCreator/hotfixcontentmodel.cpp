#include "hotfixcontentmodel.h"
#include "model/contenttreeitem.h"
#include "model/foldercontenttreeitem.h"
#include "model/filecontenttreeitem.h"
#include "model/fmtcontenttreeitem.h"
#include <svn/svnstatusmodel.h>
#include <QDebug>
#include <QFileInfo>

HotfixContentModel::HotfixContentModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem.reset(new FolderContentTreeItem(""));
    rootItem->setModel(this);
}

HotfixContentModel::~HotfixContentModel()
{

}

int HotfixContentModel::columnCount(const QModelIndex &parent) const
{
    /*if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();*/
    return 2;
}

QVariant HotfixContentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};

    const auto *item = static_cast<const ContentTreeItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

bool HotfixContentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    ContentTreeItem *item = static_cast<ContentTreeItem*>(index.internalPointer());
    return item->setData(value, index.column(), role);
}

Qt::ItemFlags HotfixContentModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags f = index.isValid() ? QAbstractItemModel::flags(index) : Qt::ItemFlags(Qt::NoItemFlags);
    f |= Qt::ItemIsAutoTristate;

    if (index.isValid())
    {
        ContentTreeItem *Item = static_cast<ContentTreeItem*>(index.internalPointer());

        if (Item->checkable())
            f |= Qt::ItemIsUserCheckable;
    }

    return f;
}

QVariant HotfixContentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant{};

    static QStringList headers =
    {
        tr("Item name"),
        tr("Action")
    };

    if (role == Qt::DisplayRole)
        return headers[section];

    return QAbstractItemModel::headerData(section, orientation, role);
}

QModelIndex HotfixContentModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};

    ContentTreeItem *parentItem = parent.isValid()
        ? static_cast<ContentTreeItem*>(parent.internalPointer())
        : rootItem.get();

    if (auto *childItem = parentItem->child(row))
        return createIndex(row, column, childItem);

    return {};
}

QModelIndex HotfixContentModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return {};

    auto *childItem = static_cast<ContentTreeItem*>(index.internalPointer());
    ContentTreeItem *parentItem = childItem->parentItem();

    return parentItem != rootItem.get()
        ? createIndex(parentItem->row(), 0, parentItem) : QModelIndex{};
}

int HotfixContentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    const ContentTreeItem *parentItem = parent.isValid()
        ? static_cast<const ContentTreeItem*>(parent.internalPointer())
        : rootItem.get();

    return parentItem->childCount();
}

void HotfixContentModel::contentItemChanged(const int &column, const QVector<int> &roles)
{
    ContentTreeItem *item = dynamic_cast<ContentTreeItem*>(sender());

    if (!item)
        return;

    //if (!item->parentItem())
    //    return;

    QModelIndex index = createIndex(item->row(), column, item);
    emit dataChanged(index, index, roles);
}

bool HotfixContentModel::isFile(const QString &name)
{
    static QStringList fileTypes =
    {
        ".mac", ".cpp", ".c", ".vcxproj", ".s362", ".hpp",
        ".hss", ".h", ".def", ".xml", ".doc", ".docx", ".dot",
        ".xls", ".xlsx"
    };

    int pos = 0;
    if ((pos = name.lastIndexOf(".")) == -1)
        return false;

    QString ext = name.mid(pos);

    return fileTypes.contains(ext);
}

bool HotfixContentModel::isExcludeElement(const QString &name)
{
    static QStringList fileTypes =
    {
        "ExternLib",
        "tools"
    };

    return fileTypes.contains(name, Qt::CaseInsensitive);
}

ContentTreeItem *HotfixContentModel::makePathEx(FolderParents &Parents,
                                                const QString &path,
                                                Qt::HANDLE elem,
                                                FolderContentTreeItem *parent,
                                                PathMaker maker)
{
    ContentTreeItem *lastCreateItem = nullptr;
    const SvnStatusElement &element = *((SvnStatusElement*)elem);

    QStringList separated = path.split("\\");
    for (int i = 0; i < separated.size(); i++)
    {
        if (!isFile(separated[i]) && !isExcludeElement(separated[i]))
        {
            if (!Parents.contains(separated[i]))
            {
                FolderContentTreeItem *realParent = !i ? parent : Parents[separated[i - 1]];
                FolderContentTreeItem *item = dynamic_cast<FolderContentTreeItem*>(maker(realParent, separated[i]));

                        //realParent->appendFolder(separated[i]);

                lastCreateItem = item;
                Parents.insert(separated[i], item);
            }
        }
        else if (!isExcludeElement(separated[i]))
        {
            FolderContentTreeItem *realParent = !i ? parent : Parents[separated[i - 1]];
            FileContentTreeItem *item = dynamic_cast<FileContentTreeItem*>(maker(realParent, separated[i]));
                    //realParent->appendFile(separated[i]);
            item->setSvnAction(element.action);
            item->setFullFileName(element.fullpath);
            lastCreateItem = item;
        }
    }

    return lastCreateItem;
}

ContentTreeItem *HotfixContentModel::makePath(FolderParents &Parents, const QString &path, Qt::HANDLE elem, FolderContentTreeItem *parent)
{
    PathMaker func = [=](FolderContentTreeItem *parent, const QString &name) -> ContentTreeItem*
    {
        if (!isFile(name) && !isExcludeElement(name))
            return parent->appendFolder(name);
        else if (!isExcludeElement(name))
            return parent->appendFile(name);

        return nullptr;
    };

    return makePathEx(Parents, path, elem, parent, func);
    /*ContentTreeItem *lastCreateItem = nullptr;

    const SvnStatusElement &element = *((SvnStatusElement*)elem);

    QStringList separated = path.split("\\");
    for (int i = 0; i < separated.size(); i++)
    {
        if (!isFile(separated[i]) && !isExcludeElement(separated[i]))
        {
            if (!Parents.contains(separated[i]))
            {
                FolderContentTreeItem *realParent = !i ? parent : Parents[separated[i - 1]];
                FolderContentTreeItem *item = realParent->appendFolder(separated[i]);

                lastCreateItem = item;
                Parents.insert(separated[i], item);
            }
        }
        else if (!isExcludeElement(separated[i]))
        {
            FolderContentTreeItem *realParent = !i ? parent : Parents[separated[i - 1]];
            FileContentTreeItem *item = realParent->appendFile(separated[i]);
            item->setSvnAction(element.action);
            item->setFullFileName(element.fullpath);
            lastCreateItem = item;
        }
    }

    return lastCreateItem;*/
}

void HotfixContentModel::makeModel(const QString &source, const QString &dst, const QString &hfname, const bool &NewFormat)
{
    beginResetModel();
    rootItem.reset(new FolderContentTreeItem(source));
    rootItem->setModel(this);

    QString path = dst + "\\" + hfname;
    FolderContentTreeItem *hfroot = (FolderContentTreeItem*)rootItem->appendChild(std::make_unique<FolderContentTreeItem>(QDir::toNativeSeparators(path)));
    FolderContentTreeItem *appserver = nullptr;
    FolderContentTreeItem *Templs = nullptr;
    FolderContentTreeItem *AddFiles = nullptr;

    SvnStatusModel svn;
    svn.setPath(source, QString());

    FolderParents Parents, AddFilesParents;
    auto getAppServerItem = [&appserver, &hfroot]()
    {
        if (!appserver)
            appserver = hfroot->appendFolder("СП");

        return appserver;
    };

    auto getTemplItem = [&Templs, &getAppServerItem]()
    {
        if (!Templs)
            Templs = getAppServerItem()->appendFolder("Templs");

        return Templs;
    };

    for (int i = 0; i < svn.rowCount(); i++)
    {
        const SvnStatusElement &element = svn.element(i);

        QStringList separated = element.path.split("\\");
        if (element.path.contains("Source\\", Qt::CaseInsensitive))
        {
            if (!element.path.contains("\\mac", Qt::CaseInsensitive))
                makePath(Parents, element.path, (Qt::HANDLE)&element, hfroot);
            else
            {
                QString macpath = element.path;
                macpath = macpath.remove("Source\\", Qt::CaseInsensitive);
                makePath(Parents, macpath, (Qt::HANDLE)&element, getAppServerItem());
            }
        }
        else if (element.path.contains("LBR\\", Qt::CaseInsensitive))
        {
            QString lbrpath = element.path;
            lbrpath = lbrpath.remove("Distrib\\", Qt::CaseInsensitive);

            QStringList lst = lbrpath.split("\\");
            lst.erase(std::remove_if(lst.begin(), lst.end(), [=](const QString &item)
            {
                if (!item.contains(".src", Qt::CaseInsensitive) &&
                        !item.contains(".xml", Qt::CaseInsensitive) &&
                        !item.contains("LBR", Qt::CaseInsensitive))
                {
                    return true;
                }

                return false;
            }), lst.end());
            makePath(Parents, lst.join("\\"), (Qt::HANDLE)&element, hfroot);
        }
        else if (element.path.contains("Templs\\", Qt::CaseInsensitive))
        {
            QString templpath = element.path;

            int pos = templpath.indexOf("Templs\\", 0, Qt::CaseInsensitive);
            templpath = templpath.mid(pos + 7);

            makePath(Parents, templpath, (Qt::HANDLE)&element, getTemplItem());
        }
        else if (element.path.contains("DBFile\\", Qt::CaseInsensitive))
        {
            if (!AddFiles)
                AddFiles = hfroot->appendFolder("AddFiles");

            makeAddFiles(AddFilesParents, element.path, (Qt::HANDLE)&element, AddFiles);
        }
        qDebug() << element.action << element.filename << element.path;
    }
    endResetModel();
}

void HotfixContentModel::makeAddFiles(FolderParents &Parents, const QString &path, Qt::HANDLE elem, FolderContentTreeItem *AddFiles)
{
    const SvnStatusElement &element = *((SvnStatusElement*)elem);

    PathMaker IdxMaker = [=](FolderContentTreeItem *parent, const QString &name) -> ContentTreeItem*
    {
        if (!isFile(name) && !isExcludeElement(name))
            return parent->appendFolder(name);
        else if (!isExcludeElement(name))
        {
            QFileInfo fi(name);
            FmtContentTreeItem *item = parent->appendFmtItem(fi.baseName());
            item->setCheckable(true);

            item->parentItem()->setCheckable(true);
            item->parentItem()->setTristate(true);

            return item;
        }

        return nullptr;
    };

    if (element.path.contains("FMT\\", Qt::CaseInsensitive))
    {
        QString fmtpath = element.path;

        int pos = fmtpath.indexOf("FMT\\", 0, Qt::CaseInsensitive);
        QString xmlpath = QString("01_FMT\\XML\\") + fmtpath.mid(pos + 4);
        QString idxpath = QString("03_INDX\\") + fmtpath.mid(pos + 4);

        makePath(Parents, xmlpath, (Qt::HANDLE)&element, AddFiles);
        makePathEx(Parents, idxpath, (Qt::HANDLE)&element, AddFiles, IdxMaker);
    }
}
