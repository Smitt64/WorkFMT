#ifndef CREATETABLESQLPARSER_H
#define CREATETABLESQLPARSER_H

#include <QString>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QSharedPointer>

class FmtTable;
class ConnectionInfo;

struct CreateTableSqlField
{
    QString name;
    QString comment;
    qint16 type;
    qint32 size;
    qint16 decpoint;

    CreateTableSqlField() :
        type(0),
        size(0),
        decpoint(0)
    {
    }
};

struct CreateTableSqlIndex
{
    QString name;
    bool unique;
    QStringList columns;

    CreateTableSqlIndex() :
        unique(false)
    {
    }
};

struct CreateTableSqlResult
{
    QString tableName;
    QString comment;
    bool isTemporary; // GLOBAL TEMPORARY TABLE -> флаг fmtnf_Temp у FmtTable
    QList<CreateTableSqlField> fields;

    // Индексы из CREATE [UNIQUE] INDEX ... ON <таблица> (...)
    QList<CreateTableSqlIndex> indeces;

    // BLOB/CLOB в FMT является свойством таблицы, а не отдельным полем.
    // Колонка BLOB/CLOB (t_fmtblobdata_xxx) в fields не попадает.
    bool hasBlob;
    qint16 blobType;

    QString error;

    CreateTableSqlResult() :
        isTemporary(false),
        hasBlob(false),
        blobType(0)
    {
    }
};

namespace CreateTableSqlParser
{
    bool parse(const QString &sqlText, CreateTableSqlResult &result);

    // Создаёт FmtTable по результату разбора: имя, комментарий, флаг
    // GLOBAL TEMPORARY, blob-параметры таблицы и поля с типами/размерами
    QSharedPointer<FmtTable> createTable(ConnectionInfo *current,
                                         const CreateTableSqlResult &result,
                                         const QString &tableName,
                                         const QString &tableComment);
}

#endif // CREATETABLESQLPARSER_H
