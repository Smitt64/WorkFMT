#include "fmttablesmodel.h"
#include "fmtcore.h"
#include <QtSql>
#include <QDebug>

FmtTablesModel::FmtTablesModel(ConnectionInfo *ConInfo, QObject *parent) :
    QSqlQueryModel(parent),
    pQuery(Q_NULLPTR)
{
    pInfo = ConInfo;
    tableIcon = QIcon::fromTheme("Table");
    tmpTableIcon = QIcon::fromTheme("TemporalTable");
    recTableIcon = QIcon::fromTheme("FmtRecord");
}

FmtTablesModel::~FmtTablesModel()
{

}

void FmtTablesModel::updateFmtList()
{
    if (pQuery == Q_NULLPTR)
        pQuery = new QSqlQuery(pInfo->_db);

    /*pQuery->prepare("select * from FMT_NAMES WHERE lower(T_NAME) LIKE 'dad%' ORDER BY T_NAME ASC");
    ExecuteQuery(pQuery);*/
    if (!m_FilterString.contains(","))
    {
        QString str = "%" + m_FilterString + "%";
        pQuery->prepare("select /*+ FIRST_ROWS */ * from FMT_NAMES WHERE lower(T_NAME) LIKE ? ORDER BY T_NAME ASC");
        pQuery->bindValue(0, str.toLower());
        //pQuery->prepare("select /*+ FIRST_ROWS */ * from FMT_NAMES ORDER BY T_NAME ASC");
        ExecuteQuery(pQuery);
    }
    else
    {
        QStringList list = m_FilterString.split(',');
        QString query = "select /*+ FIRST_ROWS */ * from FMT_NAMES WHERE ";

        bool NeedOr = false;
        int pos = 0;
        foreach (const QString table, list) {
            Q_UNUSED(table)
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

QString FmtTablesModel::filterString()
{
    return m_FilterString;
}

QVariant FmtTablesModel::fieldValue(const QModelIndex &item, const int &id) const
{
    QModelIndex idx = index(item.row(), id);
    return QSqlQueryModel::data(idx, Qt::EditRole);
}

QVariant FmtTablesModel::data(const QModelIndex &item, int role) const
{
    if (role == Qt::DisplayRole)
    {
        QVariant name = fieldValue(item, fnc_Name);
        QVariant comment = fieldValue(item, fnc_Comment);

        return QString("%1 (%2)")
                .arg(name.toString())
                .arg(comment.toString());
    }

    if (role == Qt::UserRole)
    {
        QSqlRecord rec = record(item.row());
        QVariant name = rec.value(fnc_Name);

        return name.toString();
    }

    if (role == Qt::StatusTipRole || role == Qt::WhatsThisRole || role == Qt::ToolTipRole)
    {
        QSqlRecord rec = record(item.row());
        QVariant comment = rec.value(fnc_Comment);

        return comment.toString();
    }

    if (role == Qt::DecorationRole)
    {
        QSqlRecord rec = record(item.row());
        quint16 Flags = rec.value(fnc_Flags).value<quint16>();

        if (hasTemporaryFlag(Flags))
            return QVariant::fromValue(tmpTableIcon);
        else if (hasRecordFlag(Flags))
            return QVariant::fromValue(recTableIcon);
        else
            return QVariant::fromValue(tableIcon);
    }

    return QSqlQueryModel::data(item, role);
}
