#include "sqlscriptinsert.h"

#include <QStringBuilder>

SqlScriptInsert::SqlScriptInsert()
{

}

int getAutoincIndex(const DatTable* datTable)
{
    for (int i = 0; i < datTable->fields.count(); ++i)
        if (datTable->fields[i].isAutoinc)
            return i;
    return -1;
}



QString buildVariableName(const DatTable* datTable)
{
    int autoIncIndex = getAutoincIndex(datTable);
    if (autoIncIndex == -1)
        return "";

    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variableName = "v_" + autoIncField;
    return variableName;
}

QString buildVariable(const DatTable* datTable)
{
    int autoIncIndex = getAutoincIndex(datTable);
    if (autoIncIndex == -1)
        return "";

    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variableName = buildVariableName(datTable);
    return QString("\t%1 %2.%3\%TYPE;").arg(variableName, datTable->name, autoIncField);
}

DatRecord& findByField(DatTable* datTable, QString fieldName, QString value)
{
    int index = datTable->fields.indexByFieldName(fieldName);

    for (int i = 0; i < datTable->records.count(); ++i)
    {
        if (datTable->records[i].values[index] == value)
            return datTable->records[i];
    }
}

QString buildStatement(const DatTable* datTable, const QVector<int>& indexForeingKey, QString parentVariable, const std::vector<int>& recordIndexes)
{
    QStringList list;
    for (int recIndex: recordIndexes)
    {
        DatRecord rec = datTable->records[recIndex];
        for (int i = 0; i < indexForeingKey.count(); ++i)
            rec.values[indexForeingKey[i]] = parentVariable;
        QString values = rec.values.join(", ");
        list.append(QString("\tINSERT INTO %1 VALUES (%2);").arg(datTable->name,  values));
    }
    return list.join("\n");
}

QString buildParentStatement(const DatTable* datTable, int recIndex)
{
    int autoIncIndex = getAutoincIndex(datTable);
    DatRecord rec = datTable->records[recIndex];
    rec.values[autoIncIndex] = "0";
    QString values = rec.values.join(", ");
    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variable = "v_" + autoIncField;
    return QString("\tINSERT INTO %1 VALUES (%2) RETURNING %3 INTO %4;").arg(datTable->name,  values, autoIncField, variable);
}

void buildWhere()
{
    IndexFields fields; // = getUniqueFields();
}

QString buildParentStatementDelete(const DatTable* datTable, int recIndex)
{
    int autoIncIndex = getAutoincIndex(datTable);
    DatRecord rec = datTable->records[recIndex];
    rec.values[autoIncIndex] = "0";
    QString values = rec.values.join(", ");
    QString autoIncField = datTable->fields[autoIncIndex].name;
    QString variable = "v_" + autoIncField;
    QString where;
    return QString("\tdelete from %1 where %2;").arg(datTable->name,  where);
}

void SqlScriptInsert::build(QTextStream& os)
{
    QStringList variables;
    variables << buildVariable(_join->parent->datTable);
    if (variables.back() == "")
        variables.pop_back();


    QStringList sql;
    sql << "DECLARE"
        << variables
        << "BEGIN";
    for (int i = 0; i < _join->parent->datTable->records.count(); ++i)
    {
        sql << buildParentStatement(_join->parent->datTable, i);
        //Indexes indexForeing = getForeingFieldIndex(tabOrder[0].first->name, tabOrder[1].first, tabOrder[1].second);
        //sql << buildStatement(_join->child, _join->fieldIndexChild, buildVariableName(_join->parent), _join->indexUpToDown[i]);
    }
    sql << "END;\n";
    os << sql.join("\n");
}
