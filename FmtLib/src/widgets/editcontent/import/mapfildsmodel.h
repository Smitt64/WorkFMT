#ifndef MAPFILDSMODEL_H
#define MAPFILDSMODEL_H

#include <QAbstractItemModel>
#include <QAbstractTableModel>
#include "fmttable.h"

class MapFildsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MapFildsModel(FmtSharedTablePtr table, QObject *parent);
    virtual ~MapFildsModel();

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    FmtSharedTablePtr pTable;
    QMap<qint16, QVariant> m_Data;
};

#endif // MAPFILDSMODEL_H
