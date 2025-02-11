#ifndef TABLELINKS_H
#define TABLELINKS_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

struct LinkField
{
    QString field;
    QString parentField;
    QJsonObject toJson();
    void fromJson(QJsonObject jsonObj);
};

bool operator==(const LinkField& a, const LinkField& b);

struct Link
{
    QString tableName;
    QList<LinkField> fields;
    QJsonObject toJson();
    bool fromJson(QJsonObject jsonObj);
};

bool operator==(const Link& a, const Link& b);

struct Links : public QList<Link>
{
    QJsonArray toJson();
    void fromJson(QJsonArray jsonArray);
    const Link &getByTableParentName(QString name) const;
    int getIndexByChildTableName(QString name) const;
    bool operator==(const Links& a);
};
bool operator==(const Links &a, const Links &b);

using Field = QString;
using FieldList = QStringList;

struct TableLinks;
using TableLinksList = QList<TableLinks>;

struct TableLinks
{
    QString tableName;
    Links links;
    FieldList index;
    void setChildTable(const TableLinks& tableLinks);
    const QList<TableLinks>& getChildTable() const;
    QJsonObject toJson();
    QJsonArray indexToJson(const FieldList &fields);
    void fromJson(QJsonObject jsonTable);
    FieldList getIndexFromJson(QJsonArray jsonArray);
    void loadLinks(QString fileName);
    void saveLinks(QString fileName);
private:
    QList<TableLinks> _childTables;
};

bool operator==(const TableLinks& a, const TableLinks& b);


#endif // TABLELINKS_H
