#include "export/pgexporter.h"
#include <QSqlError>
#include <QSqlRecord>
#include <connectioninfo.h>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QByteArray>
#include <QRegularExpression>
#include <recordparser.h>
#include <difftableinfo.h>
#include <difffield.h>

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

QString PostgresExporter::unquoteString(const QString &value) const
{
    if (value.length() >= 2 && value.startsWith("'") && value.endsWith("'"))
        return value.mid(1, value.length() - 2);
    return value;
}

bool PostgresExporter::parseRecFile(const QString &recFilePath, QStringList &clobValues)
{
    QFile recFile(recFilePath);
    if (!recFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit error(QString("Cannot open CLOB rec file: %1").arg(recFilePath));
        return false;
    }

    QTextStream stream(&recFile);
    stream.setCodec("IBM 866");

    QString content = stream.readAll();
    recFile.close();

    int pos = 0;
    while (pos < content.length())
    {
        int start = content.indexOf("<startlob>", pos);
        if (start == -1)
            break;

        start += QString("<startlob>").length();
        int end = content.indexOf("<endlob>", start);
        if (end == -1)
        {
            emit error(QString("Unclosed <startlob> in rec file: %1").arg(recFilePath));
            return false;
        }

        clobValues.append(content.mid(start, end - start));
        pos = end + QString("<endlob>").length();
    }

    return true;
}

bool PostgresExporter::prepareTargetTable(const QString &table, const QList<ColumnInfo> &columns)
{
    Q_UNUSED(columns)

    QSqlQuery query(m_connection->db());
    QString sql = QString("/*@ DisConv */TRUNCATE TABLE %1 RESTART IDENTITY CASCADE").arg(table.toLower());
    query.prepare(sql);

    return executeQuery(&query, QString("Truncate table: %1").arg(table));
}

bool PostgresExporter::importDataFile(const QString &datFilePath, const QString &table, const QList<ColumnInfo> &columns)
{
    Q_UNUSED(table)

    QFileInfo datInfo(datFilePath);
    QString workDir = datInfo.absoluteDir().path();
    QString pgTable = datInfo.baseName().toLower();

    bool splitFileMode = (m_clobMode == ClobMode_SplitFile);
    QList<int> clobIndexes;
    for (int i = 0; i < columns.size(); ++i)
    {
        if (columns[i].type == "CLOB")
            clobIndexes.append(i);
    }

    if (splitFileMode && !clobIndexes.isEmpty())
    {
        QString recFilePath = QDir(workDir).absoluteFilePath(datInfo.baseName().toUpper() + ".rec");
        return importSplitFile(datFilePath, pgTable, columns, clobIndexes, recFilePath);
    }

    return importInlineFile(datFilePath, pgTable, columns);
}

bool PostgresExporter::importInlineFile(const QString &datFilePath, const QString &pgTable, const QList<ColumnInfo> &columns)
{
    QFile datFile(datFilePath);
    if (!datFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit error(QString("Cannot open dat file: %1").arg(datFilePath));
        return false;
    }

    QTextStream stream(&datFile);
    stream.setCodec("IBM 866");

    // Построить DiffFields для RecordParser
    DiffFields diffFields;
    for (const ColumnInfo &col : columns)
    {
        DiffField *df = new DiffField();
        df->name = col.name;
        df->type = 0;
        // NUMBER и DATE в DAT пишутся без кавычек, остальные типы - в кавычках
        df->isString = !(col.type == "NUMBER" || col.type == "FLOAT" || col.type == "DATE");
        diffFields.append(df);
    }

    QStringList columnNames;
    for (const ColumnInfo &col : columns)
        columnNames << col.name;

    RecordParser parser(&diffFields, columnNames);

    // Подготовить INSERT
    QStringList colNamesLower;
    QStringList placeholders;
    for (const ColumnInfo &col : columns)
    {
        QString lower = col.name.toLower();
        colNamesLower << lower;
        placeholders << QString(":%1").arg(lower);
    }

    QString insertSql = QString("/*@ DisConv */INSERT INTO %1(%2) VALUES(%3)")
                        .arg(pgTable)
                        .arg(colNamesLower.join(", "))
                        .arg(placeholders.join(", "));

    QSqlDatabase db = m_connection->db();
    if (!db.transaction())
    {
        emit error(QString("Failed to start transaction: %1").arg(db.lastError().text()));
        datFile.close();
        return false;
    }

    bool isDataSection = false;
    int rowCount = 0;
    int errorCount = 0;
    const int maxErrors = 10;

    QTextStream stdOutput(stdout);
    stdOutput.setCodec("IBM 866");

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        if (!isDataSection)
        {
            if (line.contains("BEGINDATA", Qt::CaseInsensitive))
                isDataSection = true;
            continue;
        }

        if (line.trimmed().isEmpty())
            continue;

        if (!parser.parseRecord(line))
        {
            emit error(QString("Parse error in file %1 at row %2: %3")
                       .arg(datFilePath)
                       .arg(rowCount + 1)
                       .arg(parser.getErrors().join("; ")));
            errorCount++;
            if (errorCount >= maxErrors)
            {
                db.rollback();
                datFile.close();
                return false;
            }
            continue;
        }

        QStringList values = parser.getValues();
        if (values.size() != columns.size())
        {
            emit error(QString("Value count mismatch in file %1 at row %2: expected %3, got %4")
                       .arg(datFilePath)
                       .arg(rowCount + 1)
                       .arg(columns.size())
                       .arg(values.size()));
            errorCount++;
            if (errorCount >= maxErrors)
            {
                db.rollback();
                datFile.close();
                return false;
            }
            continue;
        }

        QSqlQuery insertQuery(db);
        insertQuery.prepare(insertSql);

        for (int i = 0; i < columns.size(); ++i)
        {
            QString raw = unquoteString(values[i]);
            QVariant val = formatValueForInsert(raw, columns[i]);
            insertQuery.bindValue(QString(":%1").arg(columns[i].name.toLower()), val);
        }

        if (!executeQuery(&insertQuery, QString("Insert row %1 into %2").arg(rowCount + 1).arg(pgTable)))
        {
            errorCount++;
            if (errorCount >= maxErrors)
            {
                db.rollback();
                datFile.close();
                return false;
            }
        }

        rowCount++;
        if (rowCount % 1000 == 0)
        {
            WriteLog(stdOutput, QString("Processing... %1 rows").arg(rowCount));
            emit importProgress(rowCount, -1);
        }
    }

    datFile.close();

    if (!db.commit())
    {
        emit error(QString("Failed to commit transaction: %1").arg(db.lastError().text()));
        db.rollback();
        return false;
    }

    WriteLog(stdOutput, QString("Total rows imported: %1").arg(rowCount));
    emit importProgress(rowCount, rowCount);
    return true;
}

bool PostgresExporter::importSplitFile(const QString &datFilePath, const QString &pgTable,
                                        const QList<ColumnInfo> &columns,
                                        const QList<int> &clobIndexes,
                                        const QString &recFilePath)
{
    Q_UNUSED(datFilePath)

    QStringList clobValues;
    if (!parseRecFile(recFilePath, clobValues))
        return false;

    // В SplitFile режиме каждая строка .rec содержит по одному блоку <startlob>...<endlob> на каждое CLOB-поле
    int clobsPerRow = clobIndexes.size();
    if (clobValues.size() % clobsPerRow != 0)
    {
        emit error(QString("CLOB count mismatch in rec file %1: total %2, expected multiple of %3")
                   .arg(recFilePath)
                   .arg(clobValues.size())
                   .arg(clobsPerRow));
        return false;
    }

    QStringList colNamesLower;
    QStringList placeholders;
    for (const ColumnInfo &col : columns)
    {
        QString lower = col.name.toLower();
        colNamesLower << lower;
        placeholders << QString(":%1").arg(lower);
    }

    QString insertSql = QString("/*@ DisConv */INSERT INTO %1(%2) VALUES(%3)")
                        .arg(pgTable)
                        .arg(colNamesLower.join(", "))
                        .arg(placeholders.join(", "));

    QSqlDatabase db = m_connection->db();
    if (!db.transaction())
    {
        emit error(QString("Failed to start transaction: %1").arg(db.lastError().text()));
        return false;
    }

    int rowCount = 0;
    QTextStream stdOutput(stdout);
    stdOutput.setCodec("IBM 866");

    for (int row = 0; row < clobValues.size() / clobsPerRow; ++row)
    {
        QSqlQuery insertQuery(db);
        insertQuery.prepare(insertSql);

        int clobOffset = row * clobsPerRow;
        int clobNum = 0;

        for (int i = 0; i < columns.size(); ++i)
        {
            QVariant val;
            if (clobIndexes.contains(i))
            {
                QString raw = clobValues[clobOffset + clobNum];
                // Убрать окружающие кавычки, если они есть (clob в rec может быть закавычен)
                if (raw.startsWith("'") && raw.endsWith("'"))
                    raw = raw.mid(1, raw.length() - 2);
                val = formatValueForInsert(raw, columns[i]);
                clobNum++;
            }
            else
            {
                val = QVariant();
            }
            insertQuery.bindValue(QString(":%1").arg(columns[i].name.toLower()), val);
        }

        if (!executeQuery(&insertQuery, QString("Insert CLOB row %1 into %2").arg(row + 1).arg(pgTable)))
        {
            db.rollback();
            return false;
        }

        rowCount++;
        if (rowCount % 1000 == 0)
        {
            WriteLog(stdOutput, QString("Processing... %1 CLOB rows").arg(rowCount));
            emit importProgress(rowCount, -1);
        }
    }

    if (!db.commit())
    {
        emit error(QString("Failed to commit transaction: %1").arg(db.lastError().text()));
        db.rollback();
        return false;
    }

    WriteLog(stdOutput, QString("Total CLOB rows imported: %1").arg(rowCount));
    emit importProgress(rowCount, rowCount);
    return true;
}

QVariant PostgresExporter::formatValueForInsert(const QString &rawValue, const ColumnInfo &col)
{
    // NULL маркеры
    if (rawValue.isEmpty() || rawValue == QString(QChar(1)) || rawValue == QString(QChar(2)))
        return QVariant();

    if (col.type == "NUMBER" || col.type == "FLOAT")
    {
        bool ok;
        if (col.pgtype == "integer" || col.pgtype == "smallint" ||
            col.pgtype == "serial" || col.pgtype == "bigserial")
        {
            int val = rawValue.toInt(&ok);
            if (ok)
                return QVariant(val);
        }
        else if (col.pgtype == "bigint")
        {
            qlonglong val = rawValue.toLongLong(&ok);
            if (ok)
                return QVariant(val);
        }

        double val = rawValue.toDouble(&ok);
        if (ok)
            return QVariant(val);

        return QVariant();
    }

    if (col.type == "DATE")
    {
        QDateTime dt = QDateTime::fromString(rawValue, "dd-MM-yyyy:HH:mm:ss");
        if (dt.isValid())
            return QVariant(dt);
        return QVariant();
    }

    if (col.type == "CHAR")
    {
        QString value = rawValue;
        value.replace(QLatin1Char('\x02'), QLatin1Char('\0'));
        return QVariant(value);
    }

    if (col.type == "VARCHAR2")
    {
        QString value = rawValue;
        value.replace("chr(10)", "\n");
        value.replace("chr(13)", "\r");
        return QVariant(value);
    }

    if (col.type == "CLOB" || col.pgtype == "text" || col.pgtype == "json" ||
        col.pgtype == "jsonb" || col.pgtype == "xml")
    {
        return QVariant(rawValue);
    }

    if (col.type == "BLOB" || col.pgtype == "bytea")
    {
        QByteArray bytes = QByteArray::fromHex(rawValue.toLatin1());
        return QVariant(bytes);
    }

    if (col.pgtype == "boolean")
    {
        QString v = rawValue.toUpper();
        if (v == "1" || v == "Y" || v == "T" || v == "TRUE")
            return QVariant(true);
        if (v == "0" || v == "N" || v == "F" || v == "FALSE")
            return QVariant(false);
        return QVariant();
    }

    return QVariant(rawValue);
}
