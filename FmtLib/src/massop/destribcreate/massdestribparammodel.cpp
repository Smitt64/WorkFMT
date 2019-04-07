#include "massdestribparammodel.h"
#include "massdestribcreate.h"

MassDestribParamModel::MassDestribParamModel(MassDestribCreate *info, QObject *parent) :
    QAbstractItemModel (parent),
    pInfo(info)
{

}

MassDestribParamModel::~MassDestribParamModel()
{

}

int MassDestribParamModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return fld_Count;
}

QModelIndex MassDestribParamModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
        return createIndex(row, column, Q_NULLPTR);

    int prow = parent.row();
    return createIndex(row, column, m_Tables.at(prow));
}

QModelIndex MassDestribParamModel::parent(const QModelIndex &index) const
{
    void *pointer = index.internalPointer();

    if (pointer == Q_NULLPTR)
        return QModelIndex();

    MassDestribParamModelElement *element = static_cast<MassDestribParamModelElement*>(pointer);

    if (element == Q_NULLPTR)
        return QModelIndex();

    return createIndex(m_Tables.indexOf(element), 0, Q_NULLPTR);
}

int MassDestribParamModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return m_Tables.size();

    void *pointer = parent.internalPointer();
    if (pointer != Q_NULLPTR)
        return 0;
    /*MassDestribParamModelElement *element = static_cast<MassDestribParamModelElement*>(pointer);

    if (element == Q_NULLPTR || element->table.isNull())
        return 0;*/

    return m_Tables[parent.row()]->table->fieldsCount();
}

QVariant MassDestribParamModel::data(const QModelIndex &index, int role) const
{
    void *pointer = index.internalPointer();

    if (role == Qt::DisplayRole)
    {
        if (index.column() == fld_Name)
        {
            if (pointer)
            {
                MassDestribParamModelElement *element = static_cast<MassDestribParamModelElement*>(pointer);
                return element->table->field(index.row())->name();
            }
            else
            {
                if (index.column() == fld_Name)
                    return m_Tables[index.row()]->table->name();
            }
        }
    }
    return QVariant();
}

QVariant MassDestribParamModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
        {
            if (section == fld_Name)
                return tr("Наименование");
            else if (section == fld_Action)
                return tr("Действие");
            else if (section == fld_UnloadFmt)
                return tr("Выгрузить Fmt в xml");
            else if (section == fld_UnloadDat)
                return tr("Выгрузить *.dat");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

void MassDestribParamModel::setTables(const QStringList &list)
{
    beginResetModel();
    qDeleteAll(m_Tables);
    m_Tables.clear();

    foreach(const QString &str, list) {
        MassDestribParamModelElement *element = new MassDestribParamModelElement();
        element->table.reset(new FmtTable(pInfo->connectionInfo()));
        if (element->table->load(str))
            m_Tables.append(element);
        else
            delete element;
    }

    qSort(m_Tables.begin(), m_Tables.end(), [=](MassDestribParamModelElement *s1, MassDestribParamModelElement *s2)
    {
        return s1->table->name().toLower() < s2->table->name().toLower();
    });
    endResetModel();
}
