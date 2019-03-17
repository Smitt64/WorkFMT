#ifndef MASSINITTABLESPARAMMODEL_H
#define MASSINITTABLESPARAMMODEL_H

#include <QAbstractTableModel>
#include <QMap>

typedef struct
{
    bool initTable, initIndex;
}MassInitTablesParamData;

class MassInitTablesParamModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        FieldTableName = 0,
        FieldInitTable,
        FieldInitIndex,

        FieldCount
    };
    MassInitTablesParamModel(QStringList tables, QObject *parent = Q_NULLPTR);
    virtual ~MassInitTablesParamModel();

    void setTables(const QStringList &lst);

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

private:
    QMap<QString, MassInitTablesParamData> m_Tables;
};

#endif // MASSINITTABLESPARAMMODEL_H
