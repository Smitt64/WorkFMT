#include "mapfildsmodel.h"
#include "fmtfield.h"

MapFildsModel::MapFildsModel(FmtSharedTablePtr table, QObject *parent) :
    QAbstractTableModel(parent)
{
    pTable = table;
}

MapFildsModel::~MapFildsModel()
{

}

int MapFildsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return pTable->fieldsCount();
}

int MapFildsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant MapFildsModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    FmtField *pFld = pTable->field(static_cast<qint16>(index.row()));
    if (index.column() == 0)
        return pFld->undecorateName();

    return m_Data[index.row()];
}

bool MapFildsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole)
        m_Data[index.row()] = value;
    return true;
}

Qt::ItemFlags MapFildsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags oldFlags = QAbstractTableModel::flags(index);
    if (index.column() == 0)
        oldFlags &= ~(Qt::ItemIsEditable);
    else
        oldFlags |= Qt::ItemIsEditable;

    return oldFlags;
}
