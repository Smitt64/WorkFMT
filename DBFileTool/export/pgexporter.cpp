#include "export/pgexporter.h"
#include <QSqlError>
#include <QSqlRecord>
#include <connectioninfo.h>
#include <QThread>

PostgresExporter::PostgresExporter(QObject *parent) : ExporterBase(parent)
{
}

QString PostgresExporter::getTableExistsQuery(const QString &table)
{
    return QString("/*@ DisConv */SELECT 1 FROM %1 LIMIT 1").arg(table);
}

QString PostgresExporter::getCountQuery(const QString &table)
{
    return QString("/*@ DisConv */SELECT COUNT(*) FROM %1").arg(table);
}

QString PostgresExporter::getLimitClause(int limit)
{
    return QString(" LIMIT %1").arg(limit);
}

QStringList PostgresExporter::getTableColumns(const QString &table)
{
    QStringList columns;

    QString query = R"(
        /*@ DisConv */SELECT column_name, data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND table_schema = :table_schema
        ORDER BY ordinal_position
    )";

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);
    colQuery.bindValue(":table", table.toLower());
    colQuery.bindValue(":table_schema", m_connection->dsn());

    if (!executeQuery(&colQuery, QString("Get columns for: %1").arg(table))) {
        return columns;
    }

    while (colQuery.next()) {
        columns << colQuery.value(0).toString();
    }

    return columns;
}

QString PostgresExporter::getColumnType(const QString &table, const QString &column)
{
    QString query = R"(
        /*@ DisConv */SELECT data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND column_name = :column
        AND table_schema = :table_schema
    )";

    QSqlQuery typeQuery(m_connection->db());
    typeQuery.prepare(query);
    typeQuery.bindValue(":table", table.toLower());
    typeQuery.bindValue(":column", column.toLower());
    typeQuery.bindValue(":table_schema", m_connection->dsn());

    if (executeQuery(&typeQuery) && typeQuery.next()) {
        QString pgType = typeQuery.value(0).toString();
        return mapPostgresTypeToOracleType(pgType);
    }

    return "VARCHAR2";
}

QStringList PostgresExporter::getPrimaryKeyColumns(const QString &table)
{
    QStringList pkColumns;

    QString query = R"(
        /*@ DisConv */SELECT kcu.column_name
        FROM information_schema.table_constraints tc
        JOIN information_schema.key_column_usage kcu
            ON tc.constraint_name = kcu.constraint_name
        WHERE tc.table_name = :table
        AND tc.constraint_type = 'PRIMARY KEY'
        AND tc.table_schema = :table_schema
        ORDER BY kcu.ordinal_position
    )";

    QSqlQuery pkQuery(m_connection->db());
    pkQuery.prepare(query);
    pkQuery.bindValue(":table", table.toLower());
    pkQuery.bindValue(":table_schema", m_connection->dsn());

    if (executeQuery(&pkQuery)) {
        while (pkQuery.next()) {
            pkColumns << pkQuery.value(0).toString();
        }
    }

    return pkColumns;
}

QString PostgresExporter::getOrderByClause(const QString &table)
{
    QStringList orderColumns = getPrimaryKeyColumns(table);

    if (orderColumns.isEmpty())
    {
        // Пробуем найти UNIQUE индекс
        QString query = R"(/*@ DisConv */SELECT
            a.attname AS column_name
        FROM
            pg_class t
            JOIN pg_index ix ON t.oid = ix.indrelid
            JOIN pg_class i ON i.oid = ix.indexrelid
            JOIN pg_namespace n ON n.oid = t.relnamespace
            JOIN pg_attribute a ON a.attrelid = t.oid AND a.attnum = ANY(ix.indkey)
        WHERE
            t.relname = :table
            AND n.nspname = :table_schema
            AND ix.indisunique = true
            AND NOT ix.indisprimary
            AND i.relname = (
                SELECT MIN(i2.relname)
                FROM pg_class t2
                JOIN pg_index ix2 ON t2.oid = ix2.indrelid
                JOIN pg_class i2 ON i2.oid = ix2.indexrelid
                JOIN pg_namespace n2 ON n2.oid = t2.relnamespace
                WHERE t2.relname = :table
                    AND n2.nspname = :table_schema
                    AND ix2.indisunique = true
                    AND NOT ix2.indisprimary
            )
        ORDER BY
            array_position(ix.indkey, a.attnum)
        )";

        QSqlQuery uniqueQuery(m_connection->db());
        uniqueQuery.prepare(query);
        uniqueQuery.bindValue(":table", table.toLower());
        uniqueQuery.bindValue(":table_schema", m_connection->dsn());

        if (executeQuery(&uniqueQuery))
        {
            while (uniqueQuery.next())
                orderColumns << uniqueQuery.value(0).toString();
        }
    }

    return orderColumns.isEmpty() ? "" : " ORDER BY " + orderColumns.join(", ");
}

bool PostgresExporter::hasLargeObjectFields(const QString &table)
{
    QString query = R"(
        /*@ DisConv */SELECT COUNT(*)
        FROM information_schema.columns
        WHERE table_name = :table
        AND data_type IN ('text', 'json', 'jsonb', 'xml')
    )";

    QSqlQuery loQuery(m_connection->db());
    loQuery.prepare(query);
    loQuery.bindValue(":table", table.toLower());

    if (executeQuery(&loQuery) && loQuery.next()) {
        return loQuery.value(0).toInt() > 0;
    }

    return false;
}

#define RETURN_IF_NULL_OR_EMPTY(value) if ((value).isNull() || (value).toString().isEmpty()) return ""
QString PostgresExporter::formatValueForSqlLoader(const QVariant &value, const ColumnInfo &col, const bool &isNull)
{
    if (col.type == "NUMBER")
    {
        RETURN_IF_NULL_OR_EMPTY(value);

        if (col.pgtype == "integer" || col.pgtype == "bigint" || col.pgtype == "smallint" ||
                col.pgtype == "serial" || col.pgtype == "bigserial")
        {
            return variantNumberToString(value.toLongLong());
        }

        return variantNumberToString(value);
    }
    else if (col.type == "DATE")
        return value.toString();
    else if (col.type == "CLOB")
    {
        bool isBlob = false;
        QString textValue = value.toString();
        isBlob = textValue.startsWith("\\x", Qt::CaseInsensitive) || col.pgtype == "bytea";

        if (!isBlob)
        {
            if (m_clobMode == ClobMode_Simplified)
                textValue = textValue.simplified();
            else if (m_clobMode == ClobMode_SimplifiedTrimmed)
                textValue = textValue.simplified().trimmed();
        }
        else
        {
            if ((value).isNull() || isNull)
                return "''";

            textValue = textValue.remove("\\x", Qt::CaseInsensitive).toUpper();
        }

        return "'" + textValue + "'";;
    }
    else if (col.type == "BLOB")
    {
        if ((value).isNull() || isNull)
            return "''";

        QByteArray raw = value.toByteArray();
        return "'" + raw.toHex().toUpper() + "'";
    }
    else if (col.type == "CHAR")
    {
        if (value.isNull() || isNull)
            return "'" + QLatin1Char('\x02') + "'";

        QString escaped = value.toString();
        escaped.replace(QLatin1Char('\0'), QLatin1Char('\x02'));
        return "'" + escaped + "'";
    }
    else
    {
        if ((value).isNull() || isNull)
            return "''";

        // VARCHAR2, TEXT
        QString escaped = value.toString();
        escaped = escaped.remove(QChar(0));
        //escaped.replace("'", "''");
        escaped = escaped.replace("\n", "chr(10)");
        escaped = escaped.replace("\r", "chr(13)");

        if (escaped == QLatin1Char('\1'))
            return escaped;

        if (escaped == QLatin1Char('\0') || value.isNull() || isNull)
            return "''";

        if (escaped.isEmpty())
            return "''";

        return "'" + escaped + "'";
    }
}

QString PostgresExporter::mapPostgresTypeToOracleType(const QString &pgType)
{
    // Числовые типы
    if (pgType == "integer" || pgType == "bigint" || pgType == "smallint" ||
        pgType == "numeric" || pgType == "decimal" || pgType == "real" ||
        pgType == "double precision" || pgType == "serial" || pgType == "bigserial")
    {
        return "NUMBER";
    }

    // Символьные типы фиксированной длины
    if (pgType == "char" || pgType == "character" || pgType == "bpchar")
        return "CHAR";

    // Дата/время
    if (pgType == "date" || pgType == "timestamp" || pgType == "timestamptz" ||
        pgType == "time" || pgType == "timetz" || pgType == "timestamp without time zone")
    {
        return "DATE";
    }

    // Большие объекты
    if (pgType == "text" || pgType == "json" || pgType == "jsonb" ||
        pgType == "xml" || pgType == "bytea")
    {
        /*if (pgType == "bytea")
            return "BLOB";*/

        return "CLOB";
    }

    return "VARCHAR2";
}

QString PostgresExporter::getSelectQuery(const QString &table,
                                         const QStringList &columns)
{
    QStringList formattedColumns;

    for (const QString &col : columns)
    {
        QString colType = getCachedColumnType(col); // Используем кеш

        if (colType == "CLOB")
            formattedColumns << QString("CAST(%1 AS TEXT) %1").arg(col) << QString("COALESCE(LENGTH(%1), 0) %1_LOBLEN").arg(col);
        else if (colType == "BLOB")
            formattedColumns << QString("COALESCE(CAST(%1, '\\x00'::BYTEA) AS TEXT), '00') %1").arg(col) << QString("COALESCE(LENGTH(%1), 0) %1_LOBLEN").arg(col);
        else if (colType == "DATE")
            formattedColumns << QString("TO_CHAR(%1, 'DD-MM-YYYY:HH24:MI:SS') %1").arg(col);
        else
            formattedColumns << col;
    }

    return QString("/*@ DisConv */SELECT %1 FROM %2 %3")
           .arg(formattedColumns.join(", "))
           .arg(table)
           .arg(getOrderByClause(table));
}

bool PostgresExporter::loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns)
{
    QString query = R"(
        /*@ DisConv */SELECT column_name, data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND table_schema = :table_schema
        ORDER BY ordinal_position
    )";

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);
    colQuery.bindValue(":table", table.toLower());
    colQuery.bindValue(":table_schema", m_connection->dsn());

    if (!executeQuery(&colQuery, QString("Load metadata for: %1").arg(table)))
        return false;

    while (colQuery.next())
    {
        QString colName = colQuery.value(0).toString().toUpper();
        QString pgType = colQuery.value(1).toString();
        QString oracleType = mapPostgresTypeToOracleType(pgType);
        columns.append(ColumnInfo(colName, oracleType, pgType));
    }

    return !columns.isEmpty();
}
