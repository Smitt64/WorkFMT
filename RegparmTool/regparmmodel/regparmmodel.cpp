#include "RegParmModel.h"
#include "RegParmItem.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug> // для qWarning()
#include <QTextCodec>

static bool sqliteTableHasColumn(QSqlDatabase &db, const QString &tableName, const QString &columnName);
static QString formatRegValue(int type, const QSqlQuery &query, bool hasBlobColumn);

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

    m_hasValBlob = sqliteTableHasColumn(m_database, QStringLiteral("DREGVAL_DBT"), QStringLiteral("T_FMTBLOBDATA_XXXX"));

    QVector<QVariant> rootData(RegParmItem::FIELD_COUNT);
    m_rootItem = new RegParmItem(rootData);

    // Загружаем только корневые элементы
    loadChildItems(m_rootItem, 0);
    m_rootItem->setChildrenLoaded(true);

    endResetModel();
}

bool RegParmModel::hasBlobColumn() const
{
    return m_hasValBlob;
}

void RegParmModel::loadChildItems(RegParmItem *parentItem, qint64 parentId)
{
    QString sql = QStringLiteral(
        "SELECT p.T_KEYID, p.T_PARENTID, p.T_NAME, p.T_TYPE, p.T_GLOBAL, "
        "p.T_DESCRIPTION, p.T_SECURITY, p.T_ISBRANCH, p.T_TEMPLATE, "
        "v.T_LINTVALUE, v.T_LDOUBLEVALUE");

    if (m_hasValBlob)
        sql += QStringLiteral(", v.T_FMTBLOBDATA_XXXX");

    sql += QStringLiteral(
        " FROM DREGPARM_DBT p "
        "LEFT JOIN DREGVAL_DBT v ON p.T_KEYID = v.T_KEYID "
        "AND v.T_REGKIND = 0 AND v.T_OBJECTID = 0 "
        "WHERE p.T_PARENTID = :parentId ORDER BY p.T_NAME");

    QSqlQuery query(m_database);
    query.prepare(sql);
    query.bindValue(":parentId", parentId);

    if (!query.exec()) {
        qWarning() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        QVector<QVariant> columnData;
        for (int i = 0; i < RegParmItem::FIELD_COUNT - 1; ++i) {
            columnData << query.value(i);
        }

        int type = query.value(3).toInt();
        columnData << formatRegValue(type, query, m_hasValBlob);

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

static QString typeNameFromCode(int type)
{
    switch (type)
    {
    case 0: return QStringLiteral("INTEGER");
    case 1: return QStringLiteral("DOUBLE");
    case 2: return QStringLiteral("STRING");
    case 3: return QStringLiteral("BINARY");
    case 4: return QStringLiteral("FLAG");
    default: return QStringLiteral("UNKNOWN");
    }
}

static bool sqliteTableHasColumn(QSqlDatabase &db, const QString &tableName, const QString &columnName)
{
    QSqlQuery query(db);
    query.prepare(QStringLiteral("PRAGMA table_info(%1)").arg(tableName));

    if (!query.exec())
        return false;

    while (query.next())
    {
        if (query.value(1).toString().compare(columnName, Qt::CaseInsensitive) == 0)
            return true;
    }

    return false;
}

static QString formatRegValue(int type, const QSqlQuery &query, bool hasBlobColumn)
{
    switch (type)
    {
    case 0: // INTEGER
    case 4: // FLAG
        return query.value(QStringLiteral("T_LINTVALUE")).toString();

    case 1: // DOUBLE
        return query.value(QStringLiteral("T_LDOUBLEVALUE")).toString();

    case 2: // STRING
    case 3: // BINARY
    {
        if (!hasBlobColumn)
            return QString();

        QString blobHex = query.value(QStringLiteral("T_FMTBLOBDATA_XXXX")).toString().trimmed();
        if (blobHex.isEmpty())
            return QString();

        QByteArray bytes = QByteArray::fromHex(blobHex.toLatin1());
        while (!bytes.isEmpty() && bytes.endsWith('\0'))
            bytes.chop(1);

        if (type == 3) // BINARY
            return bytes.toHex(' ').toUpper();

        QTextCodec *codec = QTextCodec::codecForName("IBM 866");
        return codec ? codec->toUnicode(bytes) : QString::fromLocal8Bit(bytes);
    }

    default:
        return QString();
    }
}

QVariant RegParmModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    RegParmItem *item = getItem(index);

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case RegParmItem::T_TYPE:
            return typeNameFromCode(item->type());

        case RegParmItem::T_GLOBAL:
        case RegParmItem::T_SECURITY:
        case RegParmItem::T_ISBRANCH: {
            QString value = item->data(index.column()).toString().trimmed();
            if (value == QLatin1String("X") ||
                (index.column() == RegParmItem::T_ISBRANCH && value == QLatin1String("1")))
                return QString(QChar(0x2713));
            return QString();
        }

        default:
            return item->data(index.column());
        }
    }
    else if (role == Qt::TextAlignmentRole) {
        if (index.column() == RegParmItem::T_GLOBAL ||
            index.column() == RegParmItem::T_SECURITY ||
            index.column() == RegParmItem::T_ISBRANCH)
            return Qt::AlignCenter;
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
            "Description", "Security", "Is Branch", "Template", "Value"
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

    // Кэшируем результат COUNT-запроса по ключу узла
    qint64 keyId = item->keyId();
    auto it = m_childrenCache.find(keyId);
    if (it != m_childrenCache.end())
        return it.value();

    QSqlQuery query(m_database);
    query.prepare("SELECT COUNT(*) FROM DREGPARM_DBT WHERE T_PARENTID = :parentId");
    query.bindValue(":parentId", keyId);

    bool has = false;
    if (query.exec() && query.next()) {
        has = query.value(0).toInt() > 0;
    }

    m_childrenCache.insert(keyId, has);
    return has;
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
