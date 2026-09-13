#include "settingstablemodel.h"

SettingsTableModel::SettingsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int SettingsTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_objects.count();
}

int SettingsTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant SettingsTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_objects.count() || index.row() < 0)
        return QVariant();

    QSharedPointer<RegInfoObj> obj = m_objects.at(index.row());
    if (!obj)
        return QVariant();

    Column column = static_cast<Column>(index.column());

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (column) {
        case FullNameColumn: return obj->fullName();
        case TypeNameColumn: return obj->typeName();
        case ValueColumn: return role == Qt::EditRole ? obj->defaultValue() : obj->getDefaultValueAsString();
        default: return QVariant();
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        if (column == TypeNameColumn)
            return Qt::AlignCenter;
        return QVariant(static_cast<int>(Qt::AlignLeft | Qt::AlignVCenter));
    }

    if (role == Qt::ToolTipRole)
    {
        if (column == FullNameColumn)
            return obj->fullName();
        if (column == ValueColumn)
            return tr("Значение: %1\nТип: %2").arg(obj->getDefaultValueAsString()).arg(obj->typeName());
    }

    return QVariant();
}

QVariant SettingsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (static_cast<Column>(section)) {
    case FullNameColumn: return tr("Полное имя");
    case TypeNameColumn: return tr("Тип");
    case ValueColumn: return tr("Значение");
    default: return QVariant();
    }
}

bool SettingsTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= m_objects.count() || index.row() < 0 || role != Qt::EditRole)
        return false;

    QSharedPointer<RegInfoObj> obj = m_objects.at(index.row());
    if (!obj)
        return false;

    Column column = static_cast<Column>(index.column());
    bool changed = false;

    switch (column) {
    case TypeNameColumn:
        obj->setTypeName(value.toString());
        changed = true;
        break;
    case ValueColumn:
        if (obj->validateDefaultValue(value)) {
            obj->setDefaultValue(value);
            changed = true;
        }
        break;
    default:
        break;
    }

    if (changed) {
        if (column == TypeNameColumn) {
            // При смене типа значение может быть переконвертировано
            emit dataChanged(index, index.sibling(index.row(), ValueColumn), {role});
        } else {
            emit dataChanged(index, index, {role});
        }
        return true;
    }

    return false;
}

Qt::ItemFlags SettingsTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // Таблица только для просмотра: значения берутся из подключенной БД
    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void SettingsTableModel::addSetting(const QString &fullName, qint16 type, const QVariant &value)
{
    QSharedPointer<RegInfoObj> obj(new RegInfoObj());
    obj->setFullName(fullName);
    obj->setType(type);
    obj->setDefaultValue(value);

    beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
    m_objects.append(obj);
    endInsertRows();
}

void SettingsTableModel::addSetting(QSharedPointer<RegInfoObj> obj)
{
    if (!obj)
        return;

    beginInsertRows(QModelIndex(), m_objects.count(), m_objects.count());
    m_objects.append(obj);
    endInsertRows();
}

void SettingsTableModel::removeSetting(int row)
{
    if (row < 0 || row >= m_objects.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_objects.removeAt(row);
    endRemoveRows();
}

void SettingsTableModel::clear()
{
    beginResetModel();
    m_objects.clear();
    endResetModel();
}

QSharedPointer<RegInfoObj> SettingsTableModel::setting(int row) const
{
    if (row < 0 || row >= m_objects.count())
        return nullptr;
    return m_objects.at(row);
}

RegInfoObjList SettingsTableModel::settings() const
{
    return m_objects;
}

bool SettingsTableModel::containsFullName(const QString &fullName) const
{
    for (const auto &obj : m_objects) {
        if (obj && obj->fullName().compare(fullName, Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}
