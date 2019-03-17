#include "massinittablesparammodel.h"
#include "fmtcore.h"

MassInitTablesParamModel::MassInitTablesParamModel(QStringList tables, QObject *parent) :
    QAbstractTableModel(parent)
{
    setTables(tables);
}

MassInitTablesParamModel::~MassInitTablesParamModel()
{

}

int MassInitTablesParamModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_Tables.count();
}

int MassInitTablesParamModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return MassInitTablesParamModel::FieldCount;
}

QVariant MassInitTablesParamModel::data(const QModelIndex &index, int role) const
{
    const MassInitTablesParamData &param = *std::next(m_Tables.begin(), index.row());
    if (role == Qt::DisplayRole)
    {
        if (index.column() == FieldTableName)
            return *std::next(m_Tables.keyBegin(), index.row());
        if (index.column() == FieldInitTable)
            return param.initTable ? tr("Да") : tr("Нет");
        if (index.column() == FieldInitIndex)
            return param.initTable ? tr("Да") : tr("Нет");
    }
    else if (role == Qt::CheckStateRole)
    {
        if (index.column() == FieldInitTable)
            return param.initTable ? Qt::Checked : Qt::Unchecked;
        if (index.column() == FieldInitIndex)
            return param.initTable ? Qt::Checked : Qt::Unchecked;
    }
    else if (role == Qt::TextAlignmentRole)
    {
        if (index.column() != FieldTableName)
            return Qt::AlignCenter;
    }

    return QVariant();
}

bool MassInitTablesParamModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    MassInitTablesParamData &param = *std::next(m_Tables.begin(), index.row());
    if (role == Qt::CheckStateRole)
    {
        if (index.column() == FieldInitTable)
        {
            param.initTable = value.toBool();
            return true;
        }
        if (index.column() == FieldInitIndex)
        {
            param.initTable = value.toBool();
            return true;
        }
    }
    return false;
}

void MassInitTablesParamModel::setTables(const QStringList &lst)
{
    beginResetModel();
    m_Tables.clear();
    foreach(const QString &table, lst) {
        m_Tables[table] = MassInitTablesParamData();
    }
    endResetModel();
}

QVariant MassInitTablesParamModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    if (role == Qt::DisplayRole)
    {
        if (section == FieldTableName)
            return tr("Таблица");
        else if (section == FieldInitTable)
            return tr("Создать/пернсоздать таблицу");
        else if (section == FieldInitIndex)
            return tr("Создать индексы и триггеры");
    }
    /*else if (role == Qt::CheckStateRole)
    {
        if (section != FieldTableName)
            return Qt::Unchecked;
    }*/
    return QAbstractTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags MassInitTablesParamModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fl = QAbstractTableModel::flags(index);
    if (index.column() == FieldInitTable || index.column() == FieldInitIndex)
        fl |= Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    return fl;
}
