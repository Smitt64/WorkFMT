#include "massdestribparammodel.h"
#include "massdestribcreate.h"
#include "fmtcore.h"

QStringList MassDestribParamModel::m_TableActions = QStringList()
        << QObject::tr("Создание таблицы")
        << QObject::tr("Изменение полей");

QStringList MassDestribParamModel::m_FldsActions = QStringList()
        << QObject::tr("Без действия")
        << QObject::tr("Добавлени поля")
        << QObject::tr("Изменение поля")
        << QObject::tr("Удаление поля");

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

    if (m_Tables[parent.row()]->action == ActionModifyFld)
        return m_Tables[parent.row()]->table->fieldsCount();

    return 0;
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
                    return m_Tables[index.row()]->table->name();
        }
        else if (index.column() == fld_Action)
        {
            if (pointer)
            {
                QModelIndex prnt = index.parent();
                FmtField *fld = m_Tables[prnt.row()]->table->field(index.row());
                if (m_Tables[prnt.row()]->flds.contains(fld))
                    return m_FldsActions[m_Tables[prnt.row()]->flds[fld].action];
                else
                    return m_FldsActions[0];
            }
            else
                return m_TableActions[m_Tables[index.row()]->action];
        }
        else if (index.column() == fld_UnloadFmt)
        {
            if (!pointer)
                return CheckSymbolFromVariant(m_Tables[index.row()]->UnloadFmt);
        }
        else if (index.column() == fld_UnloadCreateTables)
        {
            if (!pointer)
                return CheckSymbolFromVariant(m_Tables[index.row()]->UnloadCreateTables);
        }
        else if (index.column() == fld_UnloadDat)
        {
            if (!pointer)
                return CheckSymbolFromVariant(m_Tables[index.row()]->UnloadDat);
        }
    }
    else if (role == Qt::BackgroundRole)
    {
        if (!parent(index).isValid())
            return QColor(Qt::yellow).lighter();
        else
        {
            if (index.column() > fld_Action)
                return QColor(Qt::lightGray);
        }
    }
    else if (role == Qt::EditRole)
    {
        if (index.column() == fld_Action)
        {
            if (!pointer)
                return m_Tables[index.row()]->action;
            else {
                MassDestribParamModelElement *element = static_cast<MassDestribParamModelElement*>(pointer);
                FmtField *fld = element->table->field(index.row());

                if (element->flds.contains(fld))
                    return element->flds[fld].action;
                else
                    return 0;
            }
        }
        else if (index.column() == fld_UnloadFmt)
        {
            if (!pointer)
                return m_Tables[index.row()]->UnloadFmt;
        }
        else if (index.column() == fld_UnloadCreateTables)
        {
            if (!pointer)
                return m_Tables[index.row()]->UnloadCreateTables;
        }
        else if (index.column() == fld_UnloadDat)
        {
            if (!pointer)
                return m_Tables[index.row()]->UnloadDat;
        }
    }
    return QVariant();
}

bool MassDestribParamModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    void *pointer = index.internalPointer();
    if (role == Qt::EditRole)
    {
        if (index.column() == fld_Action)
        {
            if (!pointer)
            {
                qint16 action = value.value<qint16>();
                if (action == ActionModifyFld)
                {
                    m_Tables[index.row()]->action = action;
                    beginInsertRows(index, 0, m_Tables[index.row()]->table->fieldsCount());
                    endInsertRows();
                }
                else
                {
                    beginRemoveRows(index, 0, m_Tables[index.row()]->table->fieldsCount() - 1);
                    m_Tables[index.row()]->action = action;
                    endRemoveRows();
                }
                return true;
            }
            else {
                MassDestribParamModelElement *element = static_cast<MassDestribParamModelElement*>(pointer);
                FmtField *fld = element->table->field(index.row());
                element->flds[fld].action = static_cast<qint16>(value.toInt());
                return true;
            }
        }
        else if (index.column() == fld_UnloadFmt)
        {
            m_Tables[index.row()]->UnloadFmt = value.toBool();
            return true;
        }
        else if (index.column() == fld_UnloadCreateTables)
        {
            m_Tables[index.row()]->UnloadCreateTables = value.toBool();
            return true;
        }
        else if (index.column() == fld_UnloadDat)
        {
            m_Tables[index.row()]->UnloadDat = value.toBool();
            return true;
        }
    }
    return false;
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
            else if (section == fld_UnloadCreateTables)
                return tr("Выгрузить CreateTables");
            else if (section == fld_UnloadDat)
                return tr("Выгрузить *.dat");
        }
    }
    return QAbstractItemModel::headerData(section, orientation, role);
}

Qt::ItemFlags MassDestribParamModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags fl = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    if (!index.parent().isValid())
    {
        if (index.column() != fld_Name)
            fl |= Qt::ItemIsEditable;
    }
    else {
        if (index.column() == fld_Action)
            fl |= Qt::ItemIsEditable;
    }
    return fl;
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

const QStringList &MassDestribParamModel::getTablesActions() const
{
    return m_TableActions;
}

const QStringList &MassDestribParamModel::getFieldsActions() const
{
    return m_FldsActions;
}
