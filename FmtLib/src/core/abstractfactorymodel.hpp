#ifndef ABSTRACTFACTORYMODEL_HPP
#define ABSTRACTFACTORYMODEL_HPP

#include "fmtlibfactory.h"

template <class Base, class IdType>
class AbstractFactoryModel : public QAbstractTableModel
{
public:
    enum
    {
        FieldKey = 0,
        FieldAlias,
    };

    AbstractFactoryModel(FmtLibFactory<Base, IdType> *pFactory, QObject *parent = Q_NULLPTR) :
        QAbstractTableModel(parent)
    {
        m_pFactory = pFactory;
    }

    virtual ~AbstractFactoryModel()
    {

    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(parent)
        return m_pFactory->count();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE
    {
        Q_UNUSED(parent)
        return 2;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE
    {
        if (!index.isValid())
            return QVariant();

        if (role != Qt::DisplayRole && role != Qt::EditRole)
            return QVariant();

        QVariant value;
        QStringList lst = m_pFactory->getIds();
        QString id = lst[index.row()];

        if (index.column() == AbstractFactoryModel::FieldKey)
            value = id;
        else
        {
            QString alias = m_pFactory->getAlias(id);

            if (alias.isEmpty())
                alias = id;
            value = alias;
        }
        return value;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE
    {
        Q_UNUSED(index);
        Q_UNUSED(value);
        Q_UNUSED(role);
        return false;
    }

private:
    FmtLibFactory<Base, IdType> *m_pFactory;
};

#endif // ABSTRACTFACTORYMODEL_HPP
