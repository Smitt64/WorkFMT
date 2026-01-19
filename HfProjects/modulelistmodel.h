#ifndef MODULELISTMODEL_H
#define MODULELISTMODEL_H

#include <QAbstractTableModel>
#include <QSet>
#include <tuple>

typedef QMap<QString, std::tuple<QString, QString, QString>> ProjectsTuple;

class ModuleListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum
    {
        ColumnProject = 0,
        ColumnTarget,
        ColumnVcxProj,

        ColumnCount
    };
    explicit ModuleListModel(QObject* parent = nullptr);

    // Переопределение методов QAbstractTableModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Метод для добавления модулей в модель
    void setModules(const ProjectsTuple &projects);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    // Метод для получения выбранных модулей
    QSet<QString> getSelectedModules() const;

private:
    enum
    {
        ItemTargetName = 0,
        ItemTargetExt,
        ItemVcxProj
    };
    ProjectsTuple m_projects; // Данные о проектах
    QSet<QString> m_selectedModules; // Список выбранных модулей
};

#endif // MODULELISTMODEL_H
