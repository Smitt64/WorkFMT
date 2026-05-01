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
                                           const QString &columnType) override;
    virtual QString getSelectQuery(const QString &table,
                                   const QStringList &columns) override;
    virtual QString getTableExistsQuery(const QString &table) override;
    virtual QString getCountQuery(const QString &table) override;
    virtual QString getLimitClause(int limit = 1) override;

    virtual bool loadTableMetadataImpl(const QString &table, QList<ColumnInfo> &columns);

private:
    QString mapPostgresTypeToOracleType(const QString &pgType);
    QStringList getPrimaryKeyColumns(const QString &table);
};

#endif // PGXPORTER_H
