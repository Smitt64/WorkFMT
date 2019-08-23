#include "fmtindecesmodel.h"
#include "fmttable.h"
#include "fmtindex.h"
#include "fmtfield.h"
#include "fmtsegment.h"

FmtIndecesModel::FmtIndecesModel(FmtTable *parent) :
    QAbstractItemModel((QObject*)parent)
{
    pTable = parent;
    rootItem = new FmtSegment(Q_NULLPTR);
}

int FmtIndecesModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 11;
}

FmtIndecesModelItem *FmtIndecesModel::getItem(const QModelIndex &index) const
{
    if (index.isValid())
    {
        FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return Q_NULLPTR;
}

QVariant FmtIndecesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::SizeHintRole && index.column() == FmtIndecesModelItem::fld_Panel)
        return QSize(50, 25);

    if (role != Qt::DisplayRole && role != Qt::DecorationRole)
        return QVariant();

    FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(index.internalPointer());
    return item->data(index.column(), role);
}

Qt::ItemFlags FmtIndecesModel::flags(const QModelIndex &index) const
{
    qint32 Flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (!index.parent().isValid())
    {
        if (index.column() == FmtIndecesModelItem::fld_AutoInc)
        {
            if (rowCount(index) == 1)
                Flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        }
        if (index.column() == FmtIndecesModelItem::fld_Local)
            Flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        if (index.column() == FmtIndecesModelItem::fld_Dup)
            Flags |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        if (index.column() == FmtIndecesModelItem::fld_Null)
            Flags |= Qt::ItemIsEditable;
    }
    else
    {
        if (index.column() == FmtIndecesModelItem::fld_Name || index.column() == FmtIndecesModelItem::fld_Desc
                || index.column() == FmtIndecesModelItem::fld_ExcIndx || index.column() == FmtIndecesModelItem::fld_Comment)
            Flags |= Qt::ItemIsEditable;

        if (index.column() == FmtIndecesModelItem::fld_NotNull)
        {
            QModelIndex p = index.parent();
            FmtIndecesModelItem *item = static_cast<FmtIndecesModelItem*>(this->index(p.row(), 0).internalPointer());

            int value = item->data(FmtIndecesModelItem::fld_Null, Qt::EditRole).toInt();
            if (value != 0)
                Flags |= Qt::ItemIsEditable;
        }
    }
    return static_cast<Qt::ItemFlags>(Flags);
}

QVariant FmtIndecesModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return FmtIndexPropertyTextByFieldId(section);
    }

    return QVariant();
}

QModelIndex FmtIndecesModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    FmtIndecesModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FmtIndecesModelItem*>(parent.internalPointer());

    FmtIndecesModelItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FmtIndecesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    FmtIndecesModelItem *childItem = static_cast<FmtIndecesModelItem*>(index.internalPointer());
    FmtIndecesModelItem *parentItem = childItem->parent();

    if (parentItem == rootItem || !parentItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int FmtIndecesModel::rowCount(const QModelIndex &parent) const
{
    FmtIndecesModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FmtIndecesModelItem*>(parent.internalPointer());
    return parentItem->childCount();
}

QModelIndex FmtIndecesModel::indexForItem(FmtIndecesModelItem *item) const
{
    FmtIndecesModelItem *parentItem = item->parent();
    if (rootItem == item || !parentItem)
        return QModelIndex();

    return createIndex(item->childNumber(), 0, item);
}

void FmtIndecesModel::setPropertyChanged(FmtIndecesModelItem *item, const quint16 &column)
{
    QModelIndex i = indexForItem(item);
    emit dataChanged(this->index(i.row(), column, i.parent()), this->index(i.row(), column, i.parent()));
}

bool FmtIndecesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    FmtIndecesModelItem *parentItem = Q_NULLPTR;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FmtIndecesModelItem*>(parent.internalPointer());

    if (row >= parentItem->childCount())
        return false;

    if (parentItem == rootItem)
        pTable->removeIndex(row);
    else
    {
        FmtIndex *pIndex = dynamic_cast<FmtIndex*>(parentItem);

        if (pIndex)
            pIndex->removeSegment(row);
    }

    return true;
}

void FmtIndecesModel::RemoveFmtIndex(FmtIndex *pIndex)
{
    int i = rootItem->childItems.indexOf(pIndex);

    if (i == -1)
        return;

    rootItem->childItems.takeAt(i);
}

bool FmtIndecesModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(count);
    FmtIndecesModelItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<FmtIndecesModelItem*>(parent.internalPointer());

    if (parentItem == rootItem)
    {
        FmtIndex *pIndex = pTable->addIndex();
        Q_UNUSED(pIndex)
        parentItem->insertItem(row);
    }
    else
    {
        if (dynamic_cast<FmtIndex*>(parentItem))
            parentItem->insertItem(0);
        else
        {
            FmtIndex *pIndex = dynamic_cast<FmtIndex*>(parentItem->parent());
            pIndex->insertItem(row + 1);
        }
    }

    return true;
}

void FmtIndecesModel::addIndex()
{
    beginInsertRows(QModelIndex(), pTable->m_pIndeces.size(), pTable->m_pIndeces.size());
    pTable->addIndex();
    endInsertRows();

    emit indexAdded();
}
