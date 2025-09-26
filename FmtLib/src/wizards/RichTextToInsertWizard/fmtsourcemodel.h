#ifndef FMTSOURCEMODEL_H
#define FMTSOURCEMODEL_H

#include <QAbstractItemModel>

class FmtTable;
class FmtField;
class FmtSourceModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum Column
    {
        col_Name = 0,
        col_Type,
        col_Source,
        col_Count
    };

    explicit FmtSourceModel(QObject *parent = nullptr);
    virtual ~FmtSourceModel();

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    FmtTable *fmtTable() const;
    void setTable(FmtTable *table);

    FmtField *field(int row) const;

private:
    FmtTable *pTable;
};

#endif // FMTSOURCEMODEL_H
