#include "selectfieldsmodel.h"
#include "fmttable.h"
#include "fmtfield.h"
#include "fmtfildsmodel.h"

SelectFieldsModel::SelectFieldsModel(FmtTable *table, QObject *parent) :
    QSortFilterProxyModel(parent)
{
    pTable = table;
    setSourceModel(pTable->fieldsModel());
}

SelectFieldsModel::~SelectFieldsModel()
{

}

void SelectFieldsModel::setFilterFieldName(const QString &name)
{
    m_FilterName = name;
    invalidate();
}

bool SelectFieldsModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent);
    FmtFildsModel *fieldModel = (FmtFildsModel*)sourceModel();

    if (fieldModel->isInsertRow(fieldModel->index(sourceRow, 0)))
        return false;

    bool hr = false;
    FmtField *fld = pTable->field(sourceRow);

    if (fld->name().contains(m_FilterName, Qt::CaseInsensitive))
        hr = true;

    return hr;
}

Qt::ItemFlags SelectFieldsModel::flags(const QModelIndex &index) const
{
    if (index.column() == 0)
        return QSortFilterProxyModel::flags(index) | Qt::ItemIsUserCheckable;
    return QSortFilterProxyModel::flags(index);
}

QVariant SelectFieldsModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        QModelIndex mappedIndex = mapToSource(index);
        if (m_pChecked.contains(pTable->field(mappedIndex.row())))
            return Qt::Checked;
        else
            return Qt::Unchecked;
    }
    return QSortFilterProxyModel::data(index, role);
}

bool SelectFieldsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::CheckStateRole)
    {
        QModelIndex mappedIndex = mapToSource(index);
        FmtField *fld = pTable->field(mappedIndex.row());
        if (value.toInt() == Qt::Checked)
            m_pChecked.append(fld);
        else
            m_pChecked.removeOne(fld);
        return true;
    }
    return QSortFilterProxyModel::setData(index, value, role);
}

QModelIndex	SelectFieldsModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)
    return QModelIndex();
}

int SelectFieldsModel::rowCount(const QModelIndex &parent) const
{
    return QSortFilterProxyModel::rowCount(parent);
}

QList<FmtField*> SelectFieldsModel::checkedFields()
{
    return m_pChecked;
}
