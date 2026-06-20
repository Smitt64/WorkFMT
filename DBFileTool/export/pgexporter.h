#ifndef PGXPORTER_H
#define PGXPORTER_H

#include "exporterbase.h"

class PostgresExporter : public ExporterBase
{
    Q_OBJECT

public:
    explicit PostgresExporter(QObject *parent = nullptr);

protected:
    virtual QStringList getTableColumns(const QString &table) override;
    virtual QString getColumnType(const QString &table, const QString &column) override;
    virtual QString getOrderByClause(const QString &table) override;
    virtual bool hasLargeObjectFields(const QString &table) override;
    virtual QString formatValueForSqlLoader(const QVariant &value,
                                            const ColumnInfo &col,
                                            const bool &isNull) override;
    virtual QString getSelectQuery(const QString &table,
                                   const QStringList &columns) override;
    virtual QString getTableExistsQuery(const QString &table) override;
    virtual QString getCountQuery(const QString &table) override;
    virtual QString getLimitClause(int limit = 1) override;

    virtual bool loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns);

    // Импорт в PostgreSQL
    virtual bool prepareTargetTable(const QString &table, const QList<ColumnInfo> &columns) override;
    virtual bool importDataFile(const QString &datFilePath, const QString &table, const QList<ColumnInfo> &columns) override;
    virtual QVariant formatValueForInsert(const QString &rawValue, const ColumnInfo &col) override;

private:
    QString mapPostgresTypeToOracleType(const QString &pgType);
    QStringList getPrimaryKeyColumns(const QString &table);

    bool parseRecFile(const QString &recFilePath, QStringList &clobValues);
    QString unquoteString(const QString &value) const;
    bool importInlineFile(const QString &datFilePath, const QString &pgTable, const QList<ColumnInfo> &columns);
    bool importSplitFile(const QString &datFilePath, const QString &pgTable,
                         const QList<ColumnInfo> &columns,
                         const QList<int> &clobIndexes,
                         const QString &recFilePath);
};

#endif // PGXPORTER_H
