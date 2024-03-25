#ifndef WHERESCRIPT_H
#define WHERESCRIPT_H

#include "join.h"

class WhereScript
{
public:
    virtual QString buildWhere(const IndexFields& fields, const QStringList& values) = 0;
};

class WhereScriptIndex: public WhereScript
{
public:
    WhereScriptIndex(const IndexFields& indexFields):_index(indexFields){}
    QString buildWhere(const IndexFields& fields, const QStringList& values) override;
private:
    IndexFields _index;
};

class WhereScriptJoin: public WhereScript
{
public:
    WhereScriptJoin(QString tableName);
    void setTableName();
    QString buildWhere(const IndexFields& fields, const QStringList& values) override;
    QString buildExists();
    QString _tableName;
};

#endif // WHERESCRIPT_H
