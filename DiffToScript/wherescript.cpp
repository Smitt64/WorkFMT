#include "wherescript.h"

WhereScriptJoin::WhereScriptJoin(QString tableName)
    : _tableName(tableName)
{

}

QString WhereScriptJoin::buildWhere(const IndexFields& fields, const QStringList &values)
{
    return "";
}

QString WhereScriptJoin::buildExists()
{
//    using Direction = TablesTree::Iterator::IteratorDirection;

//    QStringList ons;
//    QStringList fields;

//    TablesTree::Iterator it(&_join, Direction::idParent);
//    TablesTree::Iterator itParent = ++it;

//    for (const Link& link: _join.getLinks())
//    {
//        QString on = QString("%1.%2 = %3.%4").arg(_join.getCurrentTable(), link.field, );
//        ons.append(on);
//    }
//    QString sql = QString("select 1 from %1 inner join %2 on %3")

    return "";
}


QString WhereScriptIndex::buildWhere(const IndexFields& fields, const QStringList& values)
{
    QStringList conditions;
    for (int i = 0; i < fields.count(); ++i)
    {
        if (_index.indexByName(fields[i].name) == -1)
            continue;
        conditions.append(fields[i].name + " = " + values[i]);
    }

    return conditions.join(" AND ");
}
