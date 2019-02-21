#ifndef SELECTFIELDSMODEL_H
#define SELECTFIELDSMODEL_H

#include <QSortFilterProxyModel>

class FmtTable;
class FmtField;
class SelectFieldsModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    SelectFieldsModel(FmtTable *table, QObject *parent = Q_NULLPTR);
    virtual ~SelectFieldsModel();

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual QModelIndex	parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QList<FmtField*> checkedFields();

public slots:
    void setFilterFieldName(const QString &name);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_FilterName;
    FmtTable *pTable;
    QList<FmtField*> m_pChecked;
};

#endif // SELECTFIELDSMODEL_H
