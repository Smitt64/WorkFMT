#include "modulelistmodel.h"

ModuleListModel::ModuleListModel(QObject* parent) :
    QAbstractTableModel(parent)
{

}

int ModuleListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_projects.size();
}

int ModuleListModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return ColumnCount; // Два столбца: модуль и checkbox
}

QVariant ModuleListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_projects.size())
        return QVariant();

    QString projectName = m_projects.keys().at(index.row());

    if (role == Qt::DisplayRole)
    {
        // Отображаем имя проекта и атрибуты в скобках
        if (index.column() == ColumnProject)
            return projectName;
        else if (index.column() == ColumnTarget)
        {
            QString targetName = std::get<ItemTargetName>(m_projects[projectName]);
            QString targetExt = std::get<ItemTargetExt>(m_projects[projectName]);

            return QString("%1%2").arg(targetName).arg(targetExt);;
        }
        else if (index.column() == ColumnVcxProj)
        {
            QString VcxProj = std::get<ItemVcxProj>(m_projects[projectName]);
            return VcxProj;
        }
    }
    else if (index.column() == 0 && role == Qt::CheckStateRole)
        return m_selectedModules.contains(projectName) ? Qt::Checked : Qt::Unchecked; // Checkbox

    return QVariant();
}

bool ModuleListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid() && index.column() == 0 && role == Qt::CheckStateRole)
    {
        QString projectName = m_projects.keys().at(index.row());
        if (value.toInt() == Qt::Checked)
            m_selectedModules.insert(projectName);
        else
            m_selectedModules.remove(projectName);

        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    }
    return false;
}

Qt::ItemFlags ModuleListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags flags = QAbstractTableModel::flags(index);
    flags |= Qt::ItemIsUserCheckable; // Разрешаем checkbox

    return flags;
}

void ModuleListModel::setModules(const ProjectsTuple &projects)
{
    beginResetModel();
    m_projects = projects;
    m_selectedModules.clear();
    endResetModel();
}

QSet<QString> ModuleListModel::getSelectedModules() const
{
    return m_selectedModules;
}

QVariant ModuleListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal)
        return QVariant{};

    static QStringList headers =
    {
        tr("Проект"),
        tr("Файл"),
        tr("Файл проекта")
    };

    if (role == Qt::DisplayRole)
        return headers[section];

    return QAbstractItemModel::headerData(section, orientation, role);
}
