#ifndef REGPARMMODEL_H
#define REGPARMMODEL_H

#include <QAbstractItemModel>
#include <QSqlDatabase>
#include <QIcon>
#include "regparmitem.h"

class RegParmModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit RegParmModel(const QSqlDatabase &database, QObject *parent = nullptr);
    ~RegParmModel();

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    // Custom methods
    void loadChildren(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

    // Методы для доступа к данным
    RegParmItem *getItem(const QModelIndex &index) const;
    QVariant getFieldData(const QModelIndex &index, RegParmItem::Field field) const;
    QVariant getFieldDataByName(const QModelIndex &index, const QString &fieldName) const;

    void loadChildrenForFilter(const QModelIndex &parent);

private:
    void setupModelData();
    void loadChildItems(RegParmItem *parentItem, qint64 parentId = 0);

    QSqlDatabase m_database;
    RegParmItem *m_rootItem;
    QIcon m_branchIcon;
    QIcon m_leafIcon;
};

Q_DECLARE_OPAQUE_POINTER(RegParmModel)

#endif // REGPARMMODEL_H
