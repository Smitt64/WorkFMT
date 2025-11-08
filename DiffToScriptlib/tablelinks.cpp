#include "tablelinks.h"
#include "difflogging.h"

const char TAG_TABLE[] = "table";
const char TAG_NAME[] = "name";
const char TAG_TABLE_NAME[] = "tbl_name";
const char TAG_FIELD[] = "field";
const char TAG_INDEX[] = "index";
const char TAG_LINKS[] = "child_tbls";
const char TAG_LINK_FIELDS[] = "linkFields";
const char TAG_LINK_TABLE_NAME[] = "tbl_name";
const char TAG_LINK_FIELD_NAMES[] = "fld_names";
const char TAG_LINK_PARENT_FIELD_NAMES[] = "parent_fld_names";

const char TAG_LINK_CHILD_FIELD[] = "field";
const char TAG_LINK_PARENT_FIELD[] = "parentField";

void TableLinks::setChildTable(TableLinks *tableLinks)
{
    _childTables.append(tableLinks);
}

TableLinksList *TableLinks::getChildTable()
{
    return &_childTables;
}

QJsonObject TableLinks::toJson()
{
    QJsonObject jsonObj;
    jsonObj.insert(TAG_TABLE_NAME, tableName);
    jsonObj.insert(TAG_LINKS, links.toJson());
    jsonObj.insert(TAG_INDEX, indexToJson(index));
    return jsonObj;
}

QJsonArray TableLinks::indexToJson(const FieldList &fields)
{
    QJsonArray jsonArray;
    for (const QString& f: fields)
        jsonArray.append(f);
    return jsonArray;
}

void TableLinks::fromJson(QJsonObject jsonTable)
{
    links.fromJson(jsonTable[TAG_LINKS].toArray());
    tableName = jsonTable[TAG_TABLE_NAME].toString();
    index = getIndexFromJson(jsonTable[TAG_INDEX].toArray());
}

FieldList TableLinks::getIndexFromJson(QJsonArray jsonArray)
{
    FieldList fields;
    for (int i = 0; i < jsonArray.count(); ++i)
        fields.append({jsonArray[i].toString()});
    return fields;
}

void TableLinks::loadLinks(QString fileName)
{
    qCInfo(logTableLinks) << "Start load" << fileName;
    QFile f(fileName);
    if (!f.open(QFile::ReadOnly | QIODevice::Text))
    {
        qCWarning(logTableLinks) << "File not opened: " << fileName;
        return;
    }
    qCInfo(logTableLinks) << "File opened: " << fileName;
    QByteArray arr = f.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    f.close();

    QJsonObject jsonDoc = doc.object();
    QJsonObject jsonTable = jsonDoc[TAG_TABLE].toObject();

    if (jsonTable.isEmpty())
        qCWarning(logTableLinks, "JsonObject \"%s\" is empty. File %s", TAG_TABLE, fileName.toStdString().c_str());
    else
        qCInfo(logTableLinks, "JsonObject \"%s\" is parsed. File %s", TAG_TABLE, fileName.toStdString().c_str());

    fromJson(jsonTable);
}

void TableLinks::saveLinks(QString fileName)
{
    QJsonObject tableLinks;
    tableLinks.insert(TAG_TABLE, toJson());

    QJsonDocument doc(tableLinks);
    QFile f(fileName);
    f.open(QFile::WriteOnly);
    f.write(doc.toJson());
}

const QString &TableLinks::getTableName() const
{
    return tableName;
}

const QStringList &TableLinks::getIndex() const
{
    return index;
}

Links *TableLinks::getLinks()
{
    return &links;
}

// ----------------------------------------------------------------------------------------

const QString &Link::getTableName() const
{
    return tableName;
}

LinkFieldList *Link::getFields()
{
    return &fields;
}

QJsonObject Link::toJson()
{
    QJsonObject json;

    json.insert(TAG_LINK_TABLE_NAME, tableName);

    QJsonArray jsonLinkFields;
    QJsonArray jsonFields;
    QJsonArray jsonParentFields;

    for (LinkField *field: fields)
    {
        jsonFields.append(field->field);
        jsonParentFields.append(field->parentField);
    }

    json.insert(TAG_LINK_FIELD_NAMES, jsonFields);
    json.insert(TAG_LINK_PARENT_FIELD_NAMES, jsonParentFields);

    return json;
}

bool Link::fromJson(QJsonObject jsonObj)
{
    if (jsonObj[TAG_LINK_TABLE_NAME].isNull())
    {
        qCWarning(logTableLinks) << TAG_LINK_TABLE_NAME << "not loaded.";
        return false;
    }

    if (jsonObj[TAG_LINK_FIELD_NAMES].isNull())
    {
        qCWarning(logTableLinks) << TAG_LINK_FIELD_NAMES << "not loaded.";
        return false;
    }

    if (jsonObj[TAG_LINK_PARENT_FIELD_NAMES].isNull())
    {
        qCWarning(logTableLinks) << TAG_LINK_PARENT_FIELD_NAMES << "not loaded.";
        return false;
    }

    tableName = jsonObj[TAG_LINK_TABLE_NAME].toString();
    QJsonArray jsonFields = jsonObj[TAG_LINK_FIELD_NAMES].toArray();
    QJsonArray jsonParentFields = jsonObj[TAG_LINK_PARENT_FIELD_NAMES].toArray();

    for (int i = 0; i < jsonFields.count(); ++i)
    {
        LinkField *linkField = new LinkField();
        linkField->field = jsonFields[i].toString();
        linkField->parentField = jsonParentFields[i].toString();
        fields.append(linkField);
    }

    qCInfo(logTableLinks) << TAG_LINK_PARENT_FIELD_NAMES << "loaded.";
    return true;
}

bool operator==(const Link &a, const Link& b)
{
    return (b.fields == a.fields && b.tableName == a.tableName);
}

// ----------------------------------------------------------------------------------------

QJsonArray Links::toJson()
{
    QJsonArray jsonArr;
    for (auto it = begin(); it != end(); ++it)
        jsonArr.append((*it)->toJson());

    return jsonArr;
}

void Links::fromJson(QJsonArray jsonArray)
{
    for (const QJsonValue &value: jsonArray)
    {
        Link *link = new Link();
        link->fromJson(value.toObject());
        append(link);
    }
}

const Link *Links::getByTableParentName(QString name) const
{
    for (const Link *link: *this)
    {
        if (link->tableName.toLower() == name.toLower())
            return link;
    }

    Q_ASSERT(false);
}

int Links::getIndexByChildTableName(QString name) const
{
    for (int i = 0; i < count(); ++i)
    {
        if ((*this)[i]->tableName.toLower() == name.toLower())
            return i;
    }

    return -1;
}

/*bool operator==(const Links &a, const Links &b)
{
    for (const Link& link: b)
    {
        if (std::find(a.begin(), a.end(), link) == a.end())
            return false;
    }
    return b.count() == a.count();
}*/

bool operator==(const TableLinks& a, const TableLinks& b)
{
    return (a.index == b.index && a.links == b.links && a.tableName == b.tableName);
}

// ----------------------------------------------------------------------------------------

LinkField::LinkField(QObject* parent)
    : QObject(parent)
{

}

LinkField::LinkField(const QString& field, const QString& parentField, QObject* parent)
    : QObject(parent),
      field(field),
      parentField(parentField)
{

}

LinkField::LinkField(const LinkField& other)
    : QObject(other.parent()),  // Копируем только parent
      field(other.field),
      parentField(other.parentField)
{

}

QJsonObject LinkField::toJson()
{
    QJsonObject json;
    json.insert(TAG_LINK_CHILD_FIELD, field);
    json.insert(TAG_LINK_PARENT_FIELD, parentField);
    return json;
}

void LinkField::fromJson(QJsonObject jsonObj)
{
    field = jsonObj[TAG_LINK_CHILD_FIELD].toString();
    parentField = jsonObj[TAG_LINK_PARENT_FIELD].toString();
}

/*bool operator==(const LinkField& a, const LinkField& b)
{
    return (a.field == b.field && a.parentField == b.parentField);
}*/

const QString &LinkField::getField() const
{
    return field;
}

const QString &LinkField::getParentField() const
{
    return parentField;
}
