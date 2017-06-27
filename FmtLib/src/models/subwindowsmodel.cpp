#include "subwindowsmodel.h"
#include "connectioninfo.h"
#include <QMdiSubWindow>

SubWindowsTreeItem::SubWindowsTreeItem(const QList<QVariant> &data, SubWindowsTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
    wnd = NULL;
    info = NULL;
}

SubWindowsTreeItem::~SubWindowsTreeItem()
{
    qDeleteAll(m_childItems);
}

void SubWindowsTreeItem::appendChild(SubWindowsTreeItem *item)
{
    m_childItems.append(item);
}

SubWindowsTreeItem *SubWindowsTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int SubWindowsTreeItem::childCount() const
{
    return m_childItems.count();
}

int SubWindowsTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant SubWindowsTreeItem::data(int column, int role) const
{
    if (!m_parentItem || !m_parentItem->parentItem())
    {
        if (role == Qt::DisplayRole && info)
            return info->schemeName();

        if (role == Qt::DecorationRole && info)
            return info->colorIcon();
    }
    else if (wnd)
    {
        if (role == Qt::DisplayRole)
        {
            QString str = wnd->windowTitle();
            str = str.mid(str.indexOf("@") + 1);
            return str;
        }

        if (role == Qt::DecorationRole)
        {
            return wnd->windowIcon();
        }
    }

    return m_itemData.value(column);
}

SubWindowsTreeItem *SubWindowsTreeItem::parentItem()
{
    return m_parentItem;
}

int SubWindowsTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<SubWindowsTreeItem*>(this));

    return 0;
}

// ==================================================================================================

SubWindowsModel::SubWindowsModel(QObject *parent) : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "Title" << "Summary";
    rootItem = new SubWindowsTreeItem(rootData);
}

SubWindowsModel::~SubWindowsModel()
{
    delete rootItem;
}

int SubWindowsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant SubWindowsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if(role == Qt::FontRole && !index.parent().isValid())
    {
        QFont boldFont;
        boldFont.setBold(true);
        return boldFont;
    }

    if (role != Qt::DisplayRole && role != Qt::DecorationRole)
        return QVariant();

    SubWindowsTreeItem *item = static_cast<SubWindowsTreeItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

Qt::ItemFlags SubWindowsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    if (!index.parent().isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index);
}

QVariant SubWindowsModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex SubWindowsModel::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    SubWindowsTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SubWindowsTreeItem*>(parent.internalPointer());

    SubWindowsTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex SubWindowsModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SubWindowsTreeItem *childItem = static_cast<SubWindowsTreeItem*>(index.internalPointer());
    SubWindowsTreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SubWindowsModel::rowCount(const QModelIndex &parent) const
{
    SubWindowsTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SubWindowsTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void SubWindowsModel::addConnection(ConnectionInfo *info)
{
    SubWindowsTreeItem *con = new SubWindowsTreeItem(QList<QVariant>(), rootItem);
    con->info = info;
    con->wnd = NULL;
    rootItem->appendChild(con);
}

QModelIndex SubWindowsModel::addWindow(ConnectionInfo *info, QMdiSubWindow *wnd)
{
    SubWindowsTreeItem *con = new SubWindowsTreeItem(QList<QVariant>(), rootItem);
    con->wnd = wnd;

    QModelIndex nindex;
    for(int i = 0; i < rootItem->m_childItems.size(); i++)
    {
        SubWindowsTreeItem *item = rootItem->m_childItems[i];
        if (item->info == info)
        {
            con->wnd = wnd;
            con->m_parentItem = item;

            beginInsertRows(createIndex(i, 0, item), item->childCount(), item->childCount());
            nindex = createIndex(item->childCount(), 0, item);
            item->appendChild(con);
            endInsertRows();

            connect(wnd, SIGNAL(destroyed(QObject*)), SLOT(windowClosed(QObject*)));

            emit windowsUpdated();
        }
    }

    return nindex;
}

void SubWindowsModel::windowClosed(QObject *wnd)
{
    bool stop = false;
    for(int i = 0; i < rootItem->m_childItems.size(); i++)
    {
        SubWindowsTreeItem *item = rootItem->m_childItems[i];

        for (int j = 0; j < item->childCount(); j++)
        {
            SubWindowsTreeItem *item2 = item->m_childItems[j];

            if (item2->wnd == wnd)
            {
                beginRemoveRows(parent(createIndex(j, 0, item2)), j, j);
                stop = true;
                item->m_childItems.removeOne(item2);
                endRemoveRows();
                delete item2;

                break;
            }
        }
        if (stop)
            break;
    }
}

QModelIndex SubWindowsModel::findWindow(ConnectionInfo *info, QMdiSubWindow *wnd)
{
    QModelIndex indx;

    bool stop = false;
    for(int i = 0; i < rootItem->m_childItems.size(); i++)
    {
        SubWindowsTreeItem *item = rootItem->m_childItems[i];

        if (item->info == info)
        {
            for (int j = 0; j < item->childCount(); j++)
            {
                SubWindowsTreeItem *item2 = item->m_childItems[j];

                if (item2->wnd == wnd)
                {
                    indx = createIndex(j, 0, item2);
                    stop = true;
                    break;
                }
            }

        }
        if (stop)
            break;
    }
    return indx;
}

QMdiSubWindow *SubWindowsModel::window(const QModelIndex &index)
{
    QMdiSubWindow *wnd = NULL;

    SubWindowsTreeItem *item = static_cast<SubWindowsTreeItem*>(index.internalPointer());
    if (index.isValid() && item)
    {
        wnd = item->wnd;
    }

    return wnd;
}

void SubWindowsModel::removeConnection(ConnectionInfo *info)
{
    for(int i = 0; i < rootItem->m_childItems.size(); i++)
    {
        SubWindowsTreeItem *item = rootItem->m_childItems[i];
        if (item->info == info)
        {
            beginRemoveRows(parent(createIndex(i, 0, item)), i, i);
            rootItem->m_childItems.removeOne(item);
            delete item;
            endRemoveRows();
        }
    }
}
