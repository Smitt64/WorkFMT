#include "connectionlistmodel.h"
#include <connectioninfo.h>

ConnectionListModel::ConnectionListModel(QList<ConnectionInfo *> list, QObject *parent) :
    QAbstractItemModel(parent)
{
    m_Items = list;
}

int ConnectionListModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant ConnectionListModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    ConnectionInfo *info = m_Items[index.row()];
    if (role == Qt::DisplayRole)
    {
        if (info->type() == ConnectionInfo::CON_ORA)
            return QString("%1@%2").arg(info->schemeName(), info->service());
        else
            return QString("%1").arg(info->schemeName());
    }
    else if (role == Qt::DecorationRole)
        return info->colorIcon();
    else if (role == Qt::CheckStateRole)
        value = m_CheckedItems.contains(info) ? Qt::Checked : Qt::Unchecked;

    return value;
}

bool ConnectionListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    ConnectionInfo *info = m_Items[index.row()];
    if (role == Qt::CheckStateRole)
    {
        if (value.toInt() == Qt::Checked)
            m_CheckedItems.append(info);
        else
            m_CheckedItems.takeAt(m_CheckedItems.indexOf(info));
        return true;
    }
    return QAbstractItemModel::setData(index, value, role);
}

QModelIndex ConnectionListModel::index(int row, int column, const QModelIndex &parent) const
{
    return createIndex(row, column);
}

QModelIndex ConnectionListModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

int ConnectionListModel::rowCount(const QModelIndex &parent) const
{
    return m_Items.size();
}

Qt::ItemFlags ConnectionListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable;
}
