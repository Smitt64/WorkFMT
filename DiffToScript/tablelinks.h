#ifndef TABLELINKS_H
#define TABLELINKS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include "rslmodule/iterableobjectbase.h"

class LinkField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString field READ getField CONSTANT)
    Q_PROPERTY(QString parentField READ getParentField CONSTANT)
public:
    LinkField(QObject* parent = nullptr);
    LinkField(const QString& field, const QString& parentField, QObject* parent = nullptr);
    LinkField(const LinkField& other);

    QString field;
    QString parentField;

    QJsonObject toJson();
    void fromJson(QJsonObject jsonObj);

    const QString &getField() const;
    const QString &getParentField() const;
};

Q_DECLARE_OPAQUE_POINTER(LinkField);

//bool operator==(const LinkField& a, const LinkField& b);

class LinkFieldList : public ListIterableObject<LinkField*, CachedObjectDeleter>
{
public:
    LinkFieldList() = default;
    virtual ~LinkFieldList() = default;
};

Q_DECLARE_OPAQUE_POINTER(LinkFieldList);

class Link : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tableName READ getTableName CONSTANT)
    Q_PROPERTY(LinkFieldList *fields READ getFields CONSTANT)
public:
    Link() = default;
    virtual ~Link() = default;

    QString tableName;
    LinkFieldList fields;

    QJsonObject toJson();
    bool fromJson(QJsonObject jsonObj);

    const QString &getTableName() const;
    LinkFieldList *getFields();
};

Q_DECLARE_OPAQUE_POINTER(Link);

bool operator==(const Link& a, const Link& b);

class Links : public ListIterableObject<Link*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    Links() = default;
    virtual ~Links() = default;

    QJsonArray toJson();
    void fromJson(QJsonArray jsonArray);

    Q_INVOKABLE const Link *getByTableParentName(QString name) const;
    Q_INVOKABLE int getIndexByChildTableName(QString name) const;
    //bool operator==(const Links& a);
};

Q_DECLARE_OPAQUE_POINTER(Links);
//bool operator==(const Links &a, const Links &b);

using Field = QString;
using FieldList = QStringList;

class TableLinks;
class TableLinksList : public ListIterableObject<TableLinks*, CachedObjectDeleter>
{
    Q_OBJECT
public:
    TableLinksList() = default;
    virtual ~TableLinksList() = default;
};

Q_DECLARE_OPAQUE_POINTER(TableLinksList);

class TableLinks : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tableName READ getTableName CONSTANT)
    Q_PROPERTY(QStringList index READ getIndex CONSTANT)
    Q_PROPERTY(Links *links READ getLinks CONSTANT)
public:
    TableLinks() = default;
    //virtual ~TableLinks() = default;
    virtual ~TableLinks()
    {
        //qDebug() << "~TableLinks()";
    }

    QString tableName;
    Links links;
    FieldList index;

    void setChildTable(TableLinks *tableLinks);
    Q_INVOKABLE TableLinksList *getChildTable();

    QJsonObject toJson();
    void fromJson(QJsonObject jsonTable);

    QJsonArray indexToJson(const FieldList &fields);
    FieldList getIndexFromJson(QJsonArray jsonArray);

    void loadLinks(QString fileName);
    void saveLinks(QString fileName);

    const QString &getTableName() const;
    const QStringList &getIndex() const;
    Links *getLinks();

private:
    TableLinksList _childTables;
};

Q_DECLARE_OPAQUE_POINTER(TableLinks);

bool operator==(const TableLinks& a, const TableLinks& b);


#endif // TABLELINKS_H
