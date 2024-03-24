#include "sqlbuilder.h"

SqlBuilder::SqlBuilder(QObject *parent) : QObject(parent)
{

}

void SqlBuilder::buildInsert(QTextStream& os, QStringList insList, QString tableName)
{
    for (QString &s: insList)
        os << "INSERT INTO " << tableName << tableName << Qt::endl;
}
