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
    return QString("SELECT 1 FROM %1 LIMIT 1").arg(table);
}

QString PostgresExporter::getCountQuery(const QString &table)
{
    return QString("SELECT COUNT(*) FROM %1").arg(table);
}

QString PostgresExporter::getLimitClause(int limit)
{
    return QString(" LIMIT %1").arg(limit);
}

QStringList PostgresExporter::getTableColumns(const QString &table)
{
    QStringList columns;

    QString query = R"(
        SELECT column_name, data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND table_schema = 'public'
        ORDER BY ordinal_position
    )";

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);
    colQuery.bindValue(":table", table.toLower());

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
        SELECT data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND column_name = :column
        AND table_schema = 'public'
    )";

    QSqlQuery typeQuery(m_connection->db());
    typeQuery.prepare(query);
    typeQuery.bindValue(":table", table.toLower());
    typeQuery.bindValue(":column", column.toLower());

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
        SELECT kcu.column_name
        FROM information_schema.table_constraints tc
        JOIN information_schema.key_column_usage kcu
            ON tc.constraint_name = kcu.constraint_name
        WHERE tc.table_name = :table
        AND tc.constraint_type = 'PRIMARY KEY'
        AND tc.table_schema = 'public'
        ORDER BY kcu.ordinal_position
    )";

    QSqlQuery pkQuery(m_connection->db());
    pkQuery.prepare(query);
    pkQuery.bindValue(":table", table.toLower());

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

    if (orderColumns.isEmpty()) {
        // Пробуем найти UNIQUE индекс
        QString query = R"(
            SELECT kcu.column_name
            FROM information_schema.table_constraints tc
            JOIN information_schema.key_column_usage kcu
                ON tc.constraint_name = kcu.constraint_name
            WHERE tc.table_name = :table
            AND tc.constraint_type = 'UNIQUE'
            AND tc.table_schema = 'public'
            ORDER BY kcu.ordinal_position
            LIMIT 1
        )";

        QSqlQuery uniqueQuery(m_connection->db());
        uniqueQuery.prepare(query);
        uniqueQuery.bindValue(":table", table.toLower());

        if (executeQuery(&uniqueQuery)) {
            while (uniqueQuery.next()) {
                orderColumns << uniqueQuery.value(0).toString();
            }
        }
    }

    return orderColumns.isEmpty() ? "" : " ORDER BY " + orderColumns.join(", ");
}

bool PostgresExporter::hasLargeObjectFields(const QString &table)
{
    QString query = R"(
        SELECT COUNT(*)
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

QString PostgresExporter::formatValueForSqlLoader(const QVariant &value,
                                                  const QString &columnType)
{
    if (value.isNull() || value.toString().isEmpty()) {
        return "";
    }

    if (columnType == "NUMBER") {
        return value.toString();
    }
    else if (columnType == "DATE") {
        QDateTime dt = value.toDateTime();
        if (!dt.isValid()) {
            dt = QDateTime::fromString(value.toString(), "yyyy-MM-dd HH:mm:ss");
        }
        return dt.toString("DD-MM-YYYY:HH24:MI:SS");
    }
    else if (columnType == "CLOB") {
        QString textValue = value.toString();

        if (m_clobMode == ClobMode_Simplified) {
            textValue = textValue.simplified();
        } else if (m_clobMode == ClobMode_SimplifiedTrimmed) {
            textValue = textValue.simplified().trimmed();
        }

        return textValue;
    }
    else if (columnType == "CHAR") {
        QString escaped = value.toString();
        escaped.replace(QLatin1Char('\0'), QLatin1Char('\x02'));
        return "'" + escaped + "'";
    }
    else { // VARCHAR2, TEXT
        QString escaped = value.toString();
        escaped.replace("'", "''");
        escaped.replace("\n", "chr(10)");
        escaped.replace("\r", "chr(13)");
        escaped.replace(QString::fromUtf8("\0"), "chr(0)");
        return "'" + escaped + "'";
    }
}

QString PostgresExporter::mapPostgresTypeToOracleType(const QString &pgType)
{
    // Числовые типы
    if (pgType == "integer" || pgType == "bigint" || pgType == "smallint" ||
        pgType == "numeric" || pgType == "decimal" || pgType == "real" ||
        pgType == "double precision" || pgType == "serial" || pgType == "bigserial") {
        return "NUMBER";
    }

    // Символьные типы фиксированной длины
    if (pgType == "char" || pgType == "character" || pgType == "bpchar") {
        return "CHAR";
    }

    // Дата/время
    if (pgType == "date" || pgType == "timestamp" || pgType == "timestamptz" ||
        pgType == "time" || pgType == "timetz") {
        return "DATE";
    }

    // Большие объекты
    if (pgType == "text" || pgType == "json" || pgType == "jsonb" ||
        pgType == "xml" || pgType == "bytea") {
        return "CLOB";
    }

    // По умолчанию - переменная строка
    return "VARCHAR2";
}

QString PostgresExporter::getSelectQuery(const QString &table,
                                         const QStringList &columns)
{
    QStringList formattedColumns;

    for (const QString &col : columns) {
        QString colType = getCachedColumnType(col); // Используем кеш

        if (colType == "CLOB") {
            formattedColumns << QString("CAST(%1 AS TEXT) AS %1").arg(col);
        } else if (colType == "DATE") {
            formattedColumns << QString("TO_CHAR(%1, 'DD-MM-YYYY:HH24:MI:SS') AS %1").arg(col);
        } else {
            formattedColumns << col;
        }
    }

    return QString("SELECT %1 FROM %2%3")
           .arg(formattedColumns.join(", "))
           .arg(table)
           .arg(getOrderByClause(table));
}

bool PostgresExporter::loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns)
{
    QString query = R"(
        SELECT column_name, data_type
        FROM information_schema.columns
        WHERE table_name = :table
        AND table_schema = 'public'
        ORDER BY ordinal_position
    )";

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);
    colQuery.bindValue(":table", table.toLower());

    if (!executeQuery(&colQuery, QString("Load metadata for: %1").arg(table))) {
        return false;
    }

    while (colQuery.next()) {
        QString colName = colQuery.value(0).toString();
        QString pgType = colQuery.value(1).toString();
        QString oracleType = mapPostgresTypeToOracleType(pgType);
        columns.append(ColumnInfo(colName, oracleType));
    }

    return !columns.isEmpty();
}
