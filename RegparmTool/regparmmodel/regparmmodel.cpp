#include "RegParmModel.h"
#include "RegParmItem.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RegParmModel::RegParmModel(const QSqlDatabase &database, QObject *parent)
    : QAbstractItemModel(parent), m_database(database), m_rootItem(nullptr)
{
    m_branchIcon = QIcon::fromTheme("folder");
    m_leafIcon = QIcon::fromTheme("text-x-generic");

    setupModelData();
}

RegParmModel::~RegParmModel()
{
    delete m_rootItem;
}

void RegParmModel::setupModelData()
{
    beginResetModel();

    QVector<QVariant> rootData(RegParmItem::FIELD_COUNT);
    m_rootItem = new RegParmItem(rootData);

    // Загружаем только корневые элементы
    loadChildItems(m_rootItem, 0);
    m_rootItem->setChildrenLoaded(true);

    endResetModel();
}

void RegParmModel::loadChildItems(RegParmItem *parentItem, qint64 parentId)
{
    QSqlQuery query(m_database);
    query.prepare("SELECT T_KEYID, T_PARENTID, T_NAME, T_TYPE, T_GLOBAL, "
                  "T_DESCRIPTION, T_SECURITY, T_ISBRANCH, T_TEMPLATE "
                  "FROM DREGPARM_DBT WHERE T_PARENTID = :parentId ORDER BY T_NAME");
    query.bindValue(":parentId", parentId);

    if (!query.exec()) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QVector<QVariant> columnData;
        for (int i = 0; i < RegParmItem::FIELD_COUNT; ++i) {
            columnData << query.value(i);
        }

        RegParmItem *childItem = new RegParmItem(columnData, parentItem);
        parentItem->appendChild(childItem);
        childItem->setChildrenLoaded(false); // Дети еще не загружены
    }
}

QModelIndex RegParmModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    RegParmItem *parentItem = getItem(parent);
    RegParmItem *childItem = parentItem->child(row);

    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex RegParmModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RegParmItem *childItem = getItem(index);
    RegParmItem *parentItem = childItem->parentItem();

    return (parentItem == m_rootItem || !parentItem)
           ? QModelIndex()
           : createIndex(parentItem->row(), 0, parentItem);
}

int RegParmModel::rowCount(const QModelIndex &parent) const
{
    RegParmItem *parentItem = getItem(parent);
    return parentItem ? parentItem->childCount() : 0;
}

int RegParmModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return RegParmItem::FIELD_COUNT;
}

QVariant RegParmModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RegParmItem *item = getItem(index);

    if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    else if (role == Qt::DecorationRole && index.column() == 0) {
        return item->isBranch() ? m_branchIcon : m_leafIcon;
    }

    return QVariant();
}

QVariant RegParmModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        static const QStringList headers = {
            "ID", "Parent ID", "Name", "Type", "Global",
            "Description", "Security", "Is Branch", "Template"
        };
        return section < headers.size() ? headers[section] : QVariant();
    }
    return QVariant();
}

bool RegParmModel::hasChildren(const QModelIndex &parent) const
{
    RegParmItem *item = getItem(parent);

    if (!item)
        return false;

    // Если дети уже загружены, возвращаем фактическое количество
    if (item->childrenLoaded())
        return item->childCount() > 0;

    // Если дети не загружены, проверяем в базе данных, есть ли дети
    QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM DREGPARM_DBT WHERE T_PARENTID = :parentId");
    query.bindValue(":parentId", item->keyId());

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return item->isBranch(); // Если запрос не удался, полагаемся на T_ISBRANCH
}

bool RegParmModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    RegParmItem *item = getItem(parent);
    return item && !item->childrenLoaded() && hasChildren(parent);
}

void RegParmModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid())
        return;

    RegParmItem *parentItem = getItem(parent);
    if (!parentItem || parentItem->childrenLoaded())
        return;

    // Проверяем, есть ли вообще дети
    QSqlQuery countQuery(m_database);
    countQuery.prepare("SELECT COUNT(*) FROM DREGPARM_DBT WHERE T_PARENTID = :parentId");
    countQuery.bindValue(":parentId", parentItem->keyId());

    int childCount = 0;
    if (countQuery.exec() && countQuery.next()) {
        childCount = countQuery.value(0).toInt();
    }

    if (childCount == 0) {
        parentItem->setChildrenLoaded(true);
        return;
    }

    // Загружаем детей
    beginInsertRows(parent, 0, childCount - 1);
    loadChildItems(parentItem, parentItem->keyId());
    parentItem->setChildrenLoaded(true);
    endInsertRows();
}

void RegParmModel::loadChildren(const QModelIndex &parent)
{
    fetchMore(parent);
}

RegParmItem *RegParmModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        RegParmItem *item = static_cast<RegParmItem *>(index.internalPointer());
        if (item)
            return item;
    }
    return m_rootItem;
}

QVariant RegParmModel::getFieldData(const QModelIndex &index, RegParmItem::Field field) const
{
    RegParmItem *item = getItem(index);
    return item ? item->field(field) : QVariant();
}

QVariant RegParmModel::getFieldDataByName(const QModelIndex &index, const QString &fieldName) const
{
    RegParmItem *item = getItem(index);
    return item ? item->fieldByName(fieldName) : QVariant();
}

void RegParmModel::loadChildrenForFilter(const QModelIndex &parent)
{
    RegParmItem *parentItem = getItem(parent);
    if (!parentItem || parentItem->childrenLoaded())
        return;

    // Загружаем детей без отправки сигналов (для использования в фильтре)
    loadChildItems(parentItem, parentItem->keyId());
    parentItem->setChildrenLoaded(true);
}
