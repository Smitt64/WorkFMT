#include "oraexporter.h"
#include "connectioninfo.h"
#include <QSqlRecord>
#include <QSqlError>
#include "toolsruntime.h"
#include "fmtcore.h"

OracleExporter::OracleExporter(QObject *parent) : ExporterBase(parent)
{
}

QString OracleExporter::getTableExistsQuery(const QString &table)
{
    return QString("SELECT * FROM %1 WHERE ROWNUM < 2").arg(table.toUpper());
}

QString OracleExporter::getCountQuery(const QString &table)
{
    return QString("SELECT COUNT(*) FROM %1").arg(table.toUpper());
}

QString OracleExporter::getLimitClause(int limit)
{
    return QString(" AND ROWNUM <= %1").arg(limit);
}

QStringList OracleExporter::getTableColumns(const QString &table)
{
    QStringList columns;

    QString query = QString("SELECT column_name, data_type "
        "FROM cols "
        "WHERE table_name = '%1' "
        "ORDER BY column_id"
    ).arg(table.toUpper());

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);

    if (!executeQuery(&colQuery, QString("Get columns for: %1").arg(table)))
        return columns;

    while (colQuery.next())
        columns << colQuery.value(0).toString();

    return columns;
}

QString OracleExporter::getColumnType(const QString &table, const QString &column)
{
    QString query = QString("SELECT data_type "
        "FROM cols "
        "WHERE table_name = '%1' AND column_name = '%2'"
    ).arg(table.toUpper(), column.toUpper());

    QSqlQuery typeQuery(m_connection->db());
    typeQuery.prepare(query);

    if (executeQuery(&typeQuery) && typeQuery.next())
        return typeQuery.value(0).toString();

    return "VARCHAR2";
}

QStringList OracleExporter::getPrimaryKeyColumns(const QString &table)
{
    QStringList pkColumns;

    QString query = QString(R"(SELECT cols.column_name
        FROM user_constraints cons
        JOIN user_cons_columns cols ON cons.constraint_name = cols.constraint_name
        WHERE cons.table_name = '%1'
        AND cons.constraint_type = 'P'
        ORDER BY cols.position
    )").arg(table.toUpper());

    QSqlQuery pkQuery(m_connection->db());
    pkQuery.prepare(query);

    if (executeQuery(&pkQuery))
    {
        while (pkQuery.next())
            pkColumns << pkQuery.value(0).toString();
    }

    return pkColumns;
}

QString OracleExporter::getOrderByClause(const QString &table)
{
    QStringList pkColumns = getPrimaryKeyColumns(table);

    if (pkColumns.isEmpty())
    {
        // Пробуем найти UNIQUE индекс
        QString query = QString(R"(SELECT cols.column_name
            FROM user_ind_columns cols
            WHERE cols.index_name IN (
                SELECT index_name
                FROM (
                    SELECT index_name
                    FROM user_indexes
                    WHERE table_name = '%1'
                    AND uniqueness = 'UNIQUE'
                    ORDER BY index_name
                )
                WHERE ROWNUM = 1
            )
            ORDER BY cols.column_position
        )").arg(table.toUpper());

        QSqlQuery uniqueQuery(m_connection->db());
        uniqueQuery.prepare(query);

        if (executeQuery(&uniqueQuery))
        {
            while (uniqueQuery.next())
            {
                QString col = uniqueQuery.value(0).toString();
                if (!pkColumns.contains(col))
                    pkColumns << col;
            }
        }
    }

    return pkColumns.isEmpty() ? "" : " ORDER BY " + pkColumns.join(", ");
}

bool OracleExporter::hasLargeObjectFields(const QString &table)
{
    // Если метаданные уже загружены, используем кеш
    if (m_metadataLoaded && m_currentTable == table)
    {
        for (const ColumnInfo &col : qAsConst(m_columnsCache))
        {
            if (col.type == "CLOB" || col.type == "BLOB")
                return true;
        }

        return false;
    }

    // Иначе делаем запрос (хотя лучше всегда загружать метаданные сначала)
    QString query = QString("SELECT COUNT(1) FROM cols WHERE table_name = '%1' AND data_type = 'CLOB'"
    ).arg(table.toUpper());

    QSqlQuery loQuery(m_connection->db());
    loQuery.prepare(query);

    if (executeQuery(&loQuery) && loQuery.next())
        return loQuery.value(0).toInt() > 0;

    return false;
}

QString OracleExporter::formatValueForSqlLoader(const QVariant &value, const ColumnInfo &col, const bool &isNull)
{
    if (col.type == "NUMBER" || col.type == "FLOAT")
        return variantNumberToString(value);
    else if (col.type == "CHAR")
    {
        if (value.isNull() || isNull)
            return "'" + QLatin1Char('\x02') + "'";

        QString escaped = value.toString();
        escaped.replace(QLatin1Char('\0'), QLatin1Char('\x02'));
        return "'" + escaped + "'";
    }
    else if (col.type == "DATE")
        return value.toString();
    else if (col.type == "CLOB")
    {
        QString clobValue = value.toString();

        if (m_clobMode == ClobMode_Simplified)
            clobValue = clobValue.simplified();
        else if (m_clobMode == ClobMode_SimplifiedTrimmed)
            clobValue = clobValue.simplified().trimmed();

        return clobValue;
    }
    else if (col.type == "BLOB")
    {
        if ((value).isNull() || isNull)
            return "''";

        QByteArray raw = value.toByteArray();
        return "'" + raw.toHex().toUpper() + "'";
    }
    else
    {
        if ((value).isNull() || isNull)
            return "''";

        QString escaped = value.toString();
        escaped.remove(QChar(0));
        escaped.replace("'", "''");
        escaped.replace("\n", "chr(10)");
        escaped.replace("\r", "chr(13)");

        if (escaped == QLatin1Char('\1'))
            return escaped;

        if (escaped == QLatin1Char('\0') || value.isNull() || isNull)
            return "''";

        if (escaped.isEmpty())
            return "''";

        return "'" + escaped + "'";
    }
}

QString OracleExporter::getSelectQuery(const QString &table, const QStringList &columns)
{
    QStringList formattedColumns;

    for (const QString &col : columns)
    {
        QString colType = getCachedColumnType(col);

        if (colType == "CLOB" || colType == "BLOB")
            formattedColumns << col << QString("DBMS_LOB.GETLENGTH(%1) as %1_LOBLEN").arg(col);
        else if (colType == "DATE")
            formattedColumns << QString("TO_CHAR(%1, 'DD-MM-YYYY:HH24:MI:SS') AS %1").arg(col);
        else
            formattedColumns << col;
    }

    return QString("SELECT %1 FROM %2%3")
           .arg(formattedColumns.join(", "))
           .arg(table.toUpper())
           .arg(getOrderByClause(table));
}

bool OracleExporter::loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns)
{
    QString query = QString("SELECT column_name, data_type "
        "FROM cols "
        "WHERE table_name = '%1' "
        "ORDER BY column_id"
    ).arg(table.toUpper());

    QSqlQuery colQuery(m_connection->db());
    colQuery.prepare(query);

    if (!executeQuery(&colQuery, QString("Load metadata for: %1").arg(table)))
        return false;

    while (colQuery.next())
    {
        QString colName = colQuery.value(0).toString();
        QString colType = colQuery.value(1).toString();
        columns.append(ColumnInfo(colName, colType));
    }

    return !columns.isEmpty();
}
