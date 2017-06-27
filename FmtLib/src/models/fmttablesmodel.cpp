#include "fmttablesmodel.h"
#include "fmtcore.h"
#include <QtSql>
#include <QDebug>

FmtTablesModel::FmtTablesModel(ConnectionInfo *ConInfo, QObject *parent) :
    QSqlQueryModel(parent)
{
    pInfo = ConInfo;
    pQuery = new QSqlQuery(pInfo->_db);

    tableIcon = QIcon(":/table");
    tmpTableIcon = QIcon(":/tablet");
}

void FmtTablesModel::updateFmtList()
{
    if (!m_FilterString.contains(","))
    {
        QString str = "%" + m_FilterString + "%";
        pQuery->prepare("select * from FMT_NAMES WHERE lower(T_NAME) LIKE ? ORDER BY T_NAME ASC");
        pQuery->bindValue(0, str.toLower());
        ExecuteQuery(pQuery);
    }
    else
    {
        QStringList list = m_FilterString.split(',');
        QString query = "select * from FMT_NAMES WHERE ";

        bool NeedOr = false;
        int pos = 0;
        foreach (const QString table, list) {
            Q_UNUSED(table);
            if (NeedOr)
                query += " OR ";
            else
                NeedOr = true;
            query += " lower(T_NAME) LIKE ? ";
        }
        query += " ORDER BY T_NAME ASC";

        pQuery->prepare(query);

        foreach (const QString table, list) {
            QString str = "%" + table.trimmed().simplified() + "%";
            pQuery->bindValue(pos ++, str.toLower());
        }
        ExecuteQuery(pQuery);
    }
    setQuery(*pQuery);
}

void FmtTablesModel::setFilterString(const QString &str)
{
    m_FilterString = str;
}

Qt::ItemFlags FmtTablesModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QSqlQueryModel::flags(index);
    return defaultFlags;
}

ConnectionInfo *FmtTablesModel::connection()
{
    return pInfo;
}

QVariant FmtTablesModel::data(const QModelIndex &item, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return QString("%1 (%2)")
                .arg(record(item.row()).value(fnc_Name).toString())
                .arg(record(item.row()).value(fnc_Comment).toString());
    }

    if (role == Qt::UserRole)
    {
        return record(item.row()).value(fnc_Name).toString();
    }

    if (role == Qt::StatusTipRole || role == Qt::WhatsThisRole || role == Qt::ToolTipRole)
    {
        return record(item.row()).value(fnc_Comment).toString();
    }

    if (role == Qt::DecorationRole)
    {
        quint16 Flags = record(item.row()).value(fnc_Flags).toInt();

        if (hasTemporaryFlag(Flags))
        {
            return (QVariant)tmpTableIcon;
        }
        else
        {
            return (QVariant)tableIcon;
        }
    }

    return QSqlQueryModel::data(item, role);
}
