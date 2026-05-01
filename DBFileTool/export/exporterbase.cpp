#include "exporterbase.h"
#include "connectioninfo.h"
#include <fmtcore.h>
#include <toolsruntime.h>
#include <QSqlError>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QThread>

ExporterBase::ExporterBase(QObject *parent) : QObject(parent)
{
}

ExporterBase::~ExporterBase()
{
}

void ExporterBase::setConnectionInfo(ConnectionInfo *conn)
{
    m_connection = conn;
}

void ExporterBase::setOutputDirectory(const QDir &outdir)
{
    m_outputDir = outdir;
    if (!m_outputDir.exists()) {
        m_outputDir.mkpath(".");
    }
}

void ExporterBase::setClobMode(ClobMode mode)
{
    m_clobMode = mode;
}

bool ExporterBase::executeQuery(QSqlQuery *query, const QString &errorContext)
{
    QString error;
    int result = toolExecuteQuery(query, &error);

    if (result != 0) {
        QString errorMsg = errorContext.isEmpty() ? error :
                          QString("%1: %2").arg(errorContext, error);
        //emit error(errorMsg);
        return false;
    }
    return true;
}

bool ExporterBase::checkTableExists(const QString &table)
{
    QSqlQuery query(m_connection->db());
    QString checkQuery = getTableExistsQuery(table);
    query.prepare(checkQuery);

    if (!executeQuery(&query, QString("Check table exists: %1").arg(table))) {
        return false;
    }

    return query.next();
}

int ExporterBase::getTableRowCount(const QString &table)
{
    QSqlQuery query(m_connection->db());
    QString countQuery = getCountQuery(table);
    query.prepare(countQuery);

    if (!executeQuery(&query, QString("Get row count for: %1").arg(table))) {
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

void ExporterBase::writeControlFile(const QString &table,
                                    const QStringList &columns,
                                    const QStringList &largeObjectColumns)
{
    QString datFilePath = m_outputDir.absoluteFilePath(
        QString("%1.dat").arg(table.toUpper())
    );

    QFile datFile(datFilePath);
    if (!datFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        emit error(QString("Cannot create control file: %1").arg(datFilePath));
        return;
    }

    QTextStream stream(&datFile);
    stream.setCodec("IBM 866");

    bool fHasClobFields = !largeObjectColumns.isEmpty();

    // Заголовок SQL*Loader
    stream << "LOAD DATA" << Qt::endl;

    if (!fHasClobFields || m_clobMode != ClobMode_SplitFile) {
        stream << "INFILE *" << Qt::endl;
    } else {
        stream << QString("INFILE '%1.rec' \"str '<endrec>\\r\\n'\"")
                  .arg(table) << Qt::endl;
    }

    stream << "INTO TABLE " << table.toUpper() << Qt::endl;
    stream << "FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY X'27'" << Qt::endl;
    stream << "TRAILING NULLCOLS" << Qt::endl;
    stream << "(" << Qt::endl;

    // Описание колонок
    for (int i = 0; i < m_columnsCache.size(); ++i) {
        const ColumnInfo &col = m_columnsCache[i];

        if (i > 0) {
            stream << "," << Qt::endl;
        }

        stream << "  " << col.name;

        if (col.type == "CLOB") {
            if (m_clobMode == ClobMode_SplitFile) {
                stream << QString(" CHAR(%1) ENCLOSED BY '<startlob>' AND '<endlob>'")
                          .arg(RSD_BLOB_MAX_LEN);
            }
            // Для других режимов CLOB пишется как есть
        }
        else if (col.type == "CHAR") {
            stream << " \"REPLACE(:" << col.name << ", chr(2), chr(0))\"";
        }
        else if (col.type == "VARCHAR2") {
            stream << " CHAR(4000) \"REPLACE(:" << col.name << ",'chr(10)',chr(10))\"";
        }
        else if (col.type == "DATE") {
            stream << " DATE 'DD-MM-YYYY:HH24:MI:SS'";
        }
        else if (col.type == "NUMBER" || col.type == "FLOAT") {
            // Никаких дополнительных масок для чисел
        }
        else if (col.type == "BLOB") {
            stream << " CHAR(32000)";
        }
        else {
            // VARCHAR2 и другие типы
            stream << " CHAR(4000)";
        }
    }

    stream << Qt::endl << ")" << Qt::endl;

    // Если не режим SplitFile, добавляем секцию BEGINDATA
    if (!fHasClobFields || m_clobMode != ClobMode_SplitFile) {
        stream << Qt::endl << "BEGINDATA" << Qt::endl;
    }

    datFile.close();

    // Если есть BLOB поля, генерируем дополнительный SQL файл
    bool hasBlob = false;
    for (const ColumnInfo &col : m_columnsCache) {
        if (col.type == "BLOB") {
            hasBlob = true;
            break;
        }
    }

    if (hasBlob) {
        QString blobFileName = m_outputDir.absoluteFilePath(
            QString("%1_BLOB.sql").arg(table.toUpper())
        );
        QFile blobFile(blobFileName);

        if (blobFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QString blobTemplate = toolReadTextFileContent(":/res/BlobTemplate.sql");
            blobTemplate = blobTemplate.arg(table.toUpper());

            QTextStream blobStream(&blobFile);
            blobStream.setCodec("IBM 866");
            blobStream << blobTemplate;
            blobFile.close();
        }
    }
}

void ExporterBase::writeDataFile(const QString &table,
                                 const QStringList &columns,
                                 const QStringList &largeObjectColumns)
{
    Q_UNUSED(columns)

    QString datFilePath = m_outputDir.absoluteFilePath(
        QString("%1.dat").arg(table.toUpper())
    );

    QFile datFile(datFilePath);
    if (!datFile.open(QIODevice::Append | QIODevice::Text)) {
        emit error(QString("Cannot open data file: %1").arg(datFilePath));
        return;
    }

    QTextStream stream(&datFile);
    stream.setCodec("IBM 866");

    // Для SplitFile режима открываем отдельный файл для CLOB
    QFile linesFile;
    QTextStream linesStream;
    bool splitFileMode = !largeObjectColumns.isEmpty() && m_clobMode == ClobMode_SplitFile;

    if (splitFileMode) {
        QString linesFilePath = m_outputDir.absoluteFilePath(
            QString("%1.rec").arg(table)
        );
        linesFile.setFileName(linesFilePath);
        if (!linesFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            emit error(QString("Cannot create CLOB file: %1").arg(linesFilePath));
            return;
        }
        linesStream.setDevice(&linesFile);
        linesStream.setCodec("IBM 866");
    }

    // Формируем SELECT запрос
    QStringList columnNames;
    for (const ColumnInfo &col : m_columnsCache) {
        columnNames << col.name;
    }

    QString queryStr = getSelectQuery(table, columnNames);
    QSqlQuery query(m_connection->db());
    query.setForwardOnly(true);
    query.prepare(queryStr);

    if (!executeQuery(&query, QString("Select data from: %1").arg(table))) {
        return;
    }

    int totalRows = getTableRowCount(table);
    if (totalRows < 0) {
        totalRows = 0;
    }

    int rowCount = 0;

    QTextStream stdOutput(stdout);
    stdOutput.setCodec("IBM 866");

    // Для отслеживания прогресса
    int lastProgressPercent = -1;

    while (query.next()) {
        QStringList rowValues;

        // Формируем значения для всех колонок
        for (int i = 0; i < m_columnsCache.size(); ++i) {
            const ColumnInfo &col = m_columnsCache[i];
            QVariant value = query.value(i);

            QString formatted;

            // NULL значения
            if (value.isNull()) {
                formatted = "";
            }
            // Обработка в зависимости от типа
            else if (col.type == "CLOB") {
                formatted = formatValueForSqlLoader(value, col.type);
                if (splitFileMode) {
                    // В основном файле будет номер строки, а CLOB в отдельный файл
                    continue;
                }
            }
            else if (col.type == "BLOB") {
                formatted = formatValueForSqlLoader(value, col.type);
            }
            else {
                formatted = formatValueForSqlLoader(value, col.type);
            }

            rowValues << formatted;
        }

        // Запись данных
        if (splitFileMode) {
            // В основном файле пишем номер записи
            stream << rowCount + 1 << Qt::endl;

            // В отдельный файл пишем CLOB значения
            for (int i = 0; i < m_columnsCache.size(); ++i) {
                const ColumnInfo &col = m_columnsCache[i];
                if (col.type == "CLOB") {
                    QVariant value = query.value(i);
                    QString clobValue = formatValueForSqlLoader(value, col.type);
                    linesStream << "<startlob>" << clobValue << "<endlob>";
                }
            }
            linesStream << "<endrec>" << Qt::endl;
        } else {
            // Обычный режим - пишем все значения в одну строку
            stream << rowValues.join(",") << Qt::endl;
        }

        rowCount++;

        // Отображение прогресса каждые 1000 строк или каждые 5%
        int currentPercent = (totalRows > 0) ? (rowCount * 100 / totalRows) : 0;
        if (rowCount % 1000 == 0 || currentPercent > lastProgressPercent + 5) {
            stdOutput << QString("Processing... %1 rows (%2%)")
                         .arg(rowCount)
                         .arg(currentPercent) << Qt::endl;
            stdOutput.flush();
            emit progress(rowCount, totalRows);
            lastProgressPercent = currentPercent;
        }
    }

    datFile.close();
    if (splitFileMode) {
        linesFile.close();
    }

    stdOutput << QString("Total rows processed: %1").arg(rowCount) << Qt::endl;
    stdOutput.flush();
}

QString ExporterBase::escapeForSqlLoader(const QString &value)
{
    QString escaped = value;
    escaped.replace("\\", "\\\\");
    escaped.replace("\"", "\\\"");
    return escaped;
}

bool ExporterBase::exportTable(const QString &table)
{
    if (!m_connection || !m_connection->isOpen()) {
        emit error("Database not connected");
        return false;
    }

    emit tableStarted(table);

    QTextStream stdOutput(stdout);
    stdOutput.setCodec("IBM 866");

    // Загружаем метаданные таблицы ОДИН РАЗ
    stdOutput << QString("Loading table metadata...") << Qt::endl;
    if (!loadTableMetadata(table)) {
        emit error(QString("Failed to load metadata for table: %1").arg(table));
        emit tableFinished(table, false);
        return false;
    }

    // Получаем колонки из кеша
    QStringList columns;
    QStringList largeObjectColumns;
    for (const ColumnInfo &col : m_columnsCache) {
        columns << col.name;
        if (col.isLargeObject) {
            largeObjectColumns << col.name;
        }
    }

    if (columns.isEmpty()) {
        emit error(QString("No columns found for table: %1").arg(table));
        emit tableFinished(table, false);
        return false;
    }

    // Проверка существования таблицы
    stdOutput << QString("Checking exists table...") << Qt::endl;
    if (!checkTableExists(table)) {
        stdOutput << QString("Table does not exist or cannot be accessed: %1").arg(table) << Qt::endl;
        emit error(QString("Table does not exist: %1").arg(table));
        emit tableFinished(table, false);
        return false;
    }

    // Получаем общее количество строк
    int totalRows = getTableRowCount(table);
    if (totalRows < 0) {
        emit error(QString("Cannot get row count for table: %1").arg(table));
        emit tableFinished(table, false);
        return false;
    }

    stdOutput << QString("Total rows to export: %1").arg(totalRows) << Qt::endl;

    // Пишем управляющий файл (использует кеш)
    stdOutput << QString("Writing control file...") << Qt::endl;
    writeControlFile(table, columns, largeObjectColumns);

    // Пишем файл данных (использует кеш)
    stdOutput << QString("Writing data file...") << Qt::endl;
    writeDataFile(table, columns, largeObjectColumns);

    stdOutput << QString("Unloading finished for table: %1").arg(table) << Qt::endl;
    emit tableFinished(table, true);

    // Очищаем кеш для следующей таблицы
    m_metadataLoaded = false;
    m_columnsCache.clear();

    return true;
}

bool ExporterBase::exportTables(const QStringList &tables)
{
    bool allSuccess = true;
    for (const QString &table : tables) {
        if (!exportTable(table)) {
            allSuccess = false;
        }
    }
    return allSuccess;
}

bool ExporterBase::loadTableMetadata(const QString &table)
{
    if (m_metadataLoaded && m_currentTable == table) {
        return true; // Уже загружено
    }

    m_columnsCache.clear();
    m_currentTable = table;
    m_metadataLoaded = false;

    if (!loadTableMetadataImpl(table, m_columnsCache)) {
        emit error(QString("Failed to load metadata for table: %1").arg(table));
        return false;
    }

    m_metadataLoaded = true;
    return true;
}

QString ExporterBase::getCachedColumnType(const QString &column) const
{
    for (const ColumnInfo &info : m_columnsCache) {
        if (info.name.compare(column, Qt::CaseInsensitive) == 0) {
            return info.type;
        }
    }
    return "VARCHAR2";
}
