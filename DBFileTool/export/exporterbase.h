#ifndef EXPORTERBASE_H
#define EXPORTERBASE_H

#include <QDir>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTextStream>
#include <QScopedPointer>
#include <QDateTime>

typedef struct ColumnInfo
{
    QString name;
    QString type;
    bool isLargeObject;

    ColumnInfo() : isLargeObject(false) {}
    ColumnInfo(const QString &n, const QString &t)
        : name(n), type(t), isLargeObject(t == "CLOB" || t == "TEXT") {}
} ColumnInfo;

class ConnectionInfo;
class ExporterBase : public QObject
{
    Q_OBJECT

public:
    enum ClobMode {
        ClobMode_Simplified,        // Упрощенный режим
        ClobMode_SplitFile,         // Вынос в отдельный файл
        ClobMode_SimplifiedTrimmed  // Упрощенный с обрезкой пробелов
    };

    explicit ExporterBase(QObject *parent = nullptr);
    virtual ~ExporterBase();

    // Настройка экспорта
    void setConnectionInfo(ConnectionInfo *conn);
    void setOutputDirectory(const QDir &outdir);
    void setClobMode(ClobMode mode);
    ClobMode clobMode() const { return m_clobMode; }

    // Основные методы экспорта
    bool exportTable(const QString &table);
    bool exportTables(const QStringList &tables);

signals:
    void progress(int currentRow, int totalRows);
    void tableStarted(const QString &table);
    void tableFinished(const QString &table, bool success);
    void error(const QString &message);

protected:
    // Виртуальные методы - специфичные для каждой БД
    virtual QStringList getTableColumns(const QString &table) = 0;
    virtual QString getColumnType(const QString &table, const QString &column) = 0;
    virtual QString getOrderByClause(const QString &table) = 0;
    virtual bool hasLargeObjectFields(const QString &table) = 0;
    virtual QString formatValueForSqlLoader(const QVariant &value,
                                           const QString &columnType) = 0;
    virtual QString getSelectQuery(const QString &table,
                                   const QStringList &columns) = 0;
    virtual QString getTableExistsQuery(const QString &table) = 0;
    virtual QString getCountQuery(const QString &table) = 0;
    virtual QString getLimitClause(int limit = 1) = 0;

    // Общие вспомогательные методы
    bool executeQuery(QSqlQuery *query, const QString &errorContext = QString());
    void writeControlFile(const QString &table,
                         const QStringList &columns,
                         const QStringList &largeObjectColumns);
    void writeDataFile(const QString &table,
                       const QStringList &columns,
                       const QStringList &largeObjectColumns);
    bool checkTableExists(const QString &table);
    int getTableRowCount(const QString &table);
    QString escapeForSqlLoader(const QString &value);

    virtual bool loadTableMetadata(const QString &table);

        // Получение типов из кеша (быстро)
        QString getCachedColumnType(const QString &column) const;
        const QList<ColumnInfo>& getCachedColumns() const { return m_columnsCache; }

        // Убираем старый метод getColumnType из виртуальных
        // Вместо него добавляем метод загрузки метаданных
        virtual bool loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns) = 0;

    // Данные
    ConnectionInfo *m_connection = nullptr;
    QDir m_outputDir;
    ClobMode m_clobMode = ClobMode_Simplified;

    QList<ColumnInfo> m_columnsCache;
    QString m_currentTable;
    bool m_metadataLoaded = false;
};

#endif // EXPORTERBASE_H
