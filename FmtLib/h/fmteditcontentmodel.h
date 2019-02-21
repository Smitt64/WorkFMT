#ifndef FMTEDITCONTENTMODEL_H
#define FMTEDITCONTENTMODEL_H

#include <QObject>
#include <QSqlQueryModel>
#include "fmttable.h"

bool QModelIndexLessThanRow(const QModelIndex &v1, const QModelIndex &v2);
bool QModelIndexGrThanRow(const QModelIndex &v1, const QModelIndex &v2);
class FmtEditContentModel : public QSqlTableModel
{
public:
    FmtEditContentModel(FmtSharedTablePtr table, QObject *parent = Q_NULLPTR);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual bool submit();

    QString recordRowID(const QModelIndex &index) const;
    bool isNullCell(const QModelIndex &index) const;
    void setOrder(const QString &order);
    QString order() const;

public slots:
    virtual bool selectRow(int row);
    virtual bool select();

protected:
    virtual bool insertRowIntoTable(const QSqlRecord &values);
    virtual bool deleteRowFromTable(int row);
    virtual bool updateRowInTable(int row, const QSqlRecord &values);

    virtual QString selectStatement() const;

private:
    QString MakeTableSqlQuery() const;
    FmtSharedTablePtr pTable;
    QModelIndexList m_ChangedIndexes;
    QSqlQuery m_Query;
    QString m_Order;

    //QMap<QString, QVariant> m_NewRow;
};

#endif // FMTEDITCONTENTMODEL_H
