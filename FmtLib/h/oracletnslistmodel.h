#ifndef ORACLETNSLISTMODEL_H
#define ORACLETNSLISTMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT OracleTnsListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    enum
    {
        mtns_Name = 0,
        mtns_ServiceName,
        mtns_Protocol,
        mtns_Host,
        mtns_Port,
        mtns_TnsNamesFilePath,
    };
    OracleTnsListModel(QObject *parent = NULL);

    static QStringList getOracleHomes();
    static QString getOracleHomePath(QString OracleHomeRegistryKey);
    static QString getTNSNAMESORAFilePath(QString OracleHomeRegistryKey);

    static QString clearString(QString str);

    quint32 getIndexByDsn(const QString &str);

private:
    void loadTNSNames(const QString &strTNSNAMESORAFilePath);
    int findClosePose(const QString &str, int pos);
};

#endif // ORACLETNSLISTMODEL_H
