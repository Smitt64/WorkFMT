#ifndef GENINTERFACEFACTORY_H
#define GENINTERFACEFACTORY_H

#include <QObject>
#include <QAbstractTableModel>

class GenInterfaceFactoryModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        FieldKey = 0,
        FieldAlias,
    };
    GenInterfaceFactoryModel(QObject *parent = Q_NULLPTR);
    virtual ~GenInterfaceFactoryModel();

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
};

#endif // GENINTERFACEFACTORY_H
