#include "fmtsourcemodel.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtcore.h"

FmtSourceModel::FmtSourceModel(QObject *parent)
    : QAbstractItemModel{parent},
    pTable(nullptr)
{

}

FmtSourceModel::~FmtSourceModel()
{

}

QModelIndex FmtSourceModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (row < 0 || row >= rowCount() || column < 0 || column >= col_Count)
        return QModelIndex();

    return createIndex(row, column);
}

QModelIndex FmtSourceModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int FmtSourceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (!pTable)
        return 0;

    return pTable ? pTable->fieldsCount() : 0;
}

int FmtSourceModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return col_Count;
}

QVariant FmtSourceModel::data(const QModelIndex &index, int role) const
{
    if (!pTable)
        return QVariant();

    if (!index.isValid() || index.row() >= pTable->fieldsCount())
        return QVariant();

    FmtField *fld = pTable->field(index.row());

    if (!pTable)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case col_Name:
            return fld->undecorateName();
        case col_Type:
            return fmtTypeForId(fld->type());
        case col_Source:
        {
            // Здесь можно хранить источник данных в поле (например, в userData)
            QVariant source = fld->property("dataSource");
            if (source.isValid() && role == Qt::DisplayRole)
                return source;
            else
            {
                FmtField *fld = field(index.row());
                return fmtGetOraDefaultVal(fld->type(), fld->size(), false);
            }
        }
        }
    }
    else if (role == Qt::SizeHintRole)
        return QSize(20, 25);
    else if (role == Qt::BackgroundRole)
    {
        if (index.row() % 2 == 0)
            return QBrush(QColor(248, 248, 248));
    }

    return QVariant();
}

bool FmtSourceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!pTable)
        return false;

    if (!index.isValid() || index.row() >= pTable->fieldsCount() || role != Qt::EditRole)
        return false;

    if (index.column() == col_Source)
    {
        FmtField *fld = pTable->field(index.row());
        fld->setProperty("dataSource", value);
        emit dataChanged(index, index);
        return true;
    }

    return false;
}

QVariant FmtSourceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case col_Name: return "Имя поля";
        case col_Type: return "Тип поля";
        case col_Source: return "Источник данных";
        }
    }
    return QVariant();
}

Qt::ItemFlags FmtSourceModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (index.isValid() && index.column() == col_Source)
    {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

FmtTable *FmtSourceModel::fmtTable() const
{
    return pTable;
}

void FmtSourceModel::setTable(FmtTable *table)
{
    beginResetModel();
    pTable = table;
    endResetModel();
}

FmtField *FmtSourceModel::field(int row) const
{
    return pTable->field(row);
}
