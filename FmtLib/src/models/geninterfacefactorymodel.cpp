#include "geninterfacefactorymodel.h"
#include "fmtgeninterface.h"
// FmtGenInterface::m_pGenInterfaceFactory

GenInterfaceFactoryModel::GenInterfaceFactoryModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}

GenInterfaceFactoryModel::~GenInterfaceFactoryModel()
{

}

int GenInterfaceFactoryModel::rowCount(const QModelIndex &parent) const
{
    return FmtGenInterface::m_pGenInterfaceFactory.count();
}

int GenInterfaceFactoryModel::columnCount(const QModelIndex &parent) const
{
    return 2;
}

bool GenInterfaceFactoryModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;
}

QVariant GenInterfaceFactoryModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    QVariant value;
    QStringList lst = FmtGenInterface::m_pGenInterfaceFactory.getIds();
    QString id = lst[index.row()];

    if (index.column() == GenInterfaceFactoryModel::FieldKey)
        value = id;
    else {
        QString alias = FmtGenInterface::m_pGenInterfaceFactory.getAlias(id);

        if (alias.isEmpty())
            alias = id;
        value = alias;
    }
    return value;
}
