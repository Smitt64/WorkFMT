#ifndef FMTTABLESMODEL_H
#define FMTTABLESMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include <QIcon>
#include "connectioninfo.h"
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT FmtTablesModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    FmtTablesModel(ConnectionInfo *ConInfo, QObject *parent = 0);

    void updateFmtList();
    void setFilterString(const QString &str);
    ConnectionInfo *connection();
    QString filterString();

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

private:
    ConnectionInfo *pInfo;
    QString m_FilterString;
    QIcon tableIcon, tmpTableIcon;

    QSqlQuery *pQuery;
};

#endif // FMTTABLESMODEL_H
