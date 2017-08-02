#ifndef CONNECTIONLISTMODEL_H
#define CONNECTIONLISTMODEL_H

#include <QAbstractItemModel>

class ConnectionInfo;
class ConnectionListModel : public QAbstractItemModel
{
public:
    ConnectionListModel(QList<ConnectionInfo*> list, QObject *parent = Q_NULLPTR);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data (const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex &index) const;
    virtual int rowCount (const QModelIndex &parent = QModelIndex()) const;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    QList<ConnectionInfo*> сheckedItems() const { return m_CheckedItems; }

private:
    QList<ConnectionInfo*> m_Items;
    QList<ConnectionInfo*> m_CheckedItems;
};

#endif // CONNECTIONLISTMODEL_H
