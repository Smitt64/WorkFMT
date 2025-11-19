#include "RegParmItem.h"
#include <QHash>

// Инициализация статической карты полей
const QHash<QString, int> RegParmItem::FIELD_MAP = RegParmItem::initFieldMap();

QHash<QString, int> RegParmItem::initFieldMap()
{
    QHash<QString, int> map;
    map["T_KEYID"] = T_KEYID;
    map["T_PARENTID"] = T_PARENTID;
    map["T_NAME"] = T_NAME;
    map["T_TYPE"] = T_TYPE;
    map["T_GLOBAL"] = T_GLOBAL;
    map["T_DESCRIPTION"] = T_DESCRIPTION;
    map["T_SECURITY"] = T_SECURITY;
    map["T_ISBRANCH"] = T_ISBRANCH;
    map["T_TEMPLATE"] = T_TEMPLATE;
    return map;
}

RegParmItem::RegParmItem(const QVector<QVariant> &data, RegParmItem *parent)
    : m_itemData(data), m_parentItem(parent)
{
    // Гарантируем, что всегда есть нужное количество полей
    if (m_itemData.size() < FIELD_COUNT) {
        m_itemData.resize(FIELD_COUNT);
    }
}

RegParmItem::~RegParmItem()
{
    qDeleteAll(m_childItems);
}

void RegParmItem::appendChild(RegParmItem *child)
{
    m_childItems.append(child);
}

RegParmItem *RegParmItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int RegParmItem::childCount() const
{
    return m_childItems.count();
}

int RegParmItem::columnCount() const
{
    return m_itemData.count();
}

QVariant RegParmItem::data(int column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

int RegParmItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<RegParmItem *>(this));
    return 0;
}

RegParmItem *RegParmItem::parentItem()
{
    return m_parentItem;
}

qint64 RegParmItem::keyId() const
{
    return m_itemData[T_KEYID].toLongLong();
}

qint64 RegParmItem::parentId() const
{
    return m_itemData[T_PARENTID].toLongLong();
}

QString RegParmItem::name() const
{
    return m_itemData[T_NAME].toString();
}

int RegParmItem::type() const
{
    return m_itemData[T_TYPE].toInt();
}

QString RegParmItem::global() const
{
    return m_itemData[T_GLOBAL].toString();
}

QString RegParmItem::description() const
{
    return m_itemData[T_DESCRIPTION].toString();
}

QString RegParmItem::security() const
{
    return m_itemData[T_SECURITY].toString();
}

bool RegParmItem::isBranch() const
{
    return m_itemData[T_ISBRANCH].toString() == "1";
}

QString RegParmItem::templateStr() const
{
    return m_itemData[T_TEMPLATE].toString();
}

QVariant RegParmItem::field(Field field) const
{
    if (field < 0 || field >= m_itemData.size())
        return QVariant();
    return m_itemData[field];
}

QVariant RegParmItem::fieldByName(const QString &fieldName) const
{
    int fieldIndex = fieldIndexByName(fieldName);
    if (fieldIndex == -1)
        return QVariant();
    return m_itemData[fieldIndex];
}

int RegParmItem::fieldIndexByName(const QString &fieldName)
{
    return FIELD_MAP.value(fieldName.toUpper(), -1);
}

bool RegParmItem::hasChildren() const
{
    return isBranch() || !m_childItems.isEmpty();
}

void RegParmItem::setChildrenLoaded(bool loaded)
{
    m_childrenLoaded = loaded;
}

bool RegParmItem::childrenLoaded() const
{
    return m_childrenLoaded;
}
