#include "odbctablemodel.h"
#include "fmtcore.h"
#include <QSettings>
#include <QIcon>

#define DRIVER_SQORA "SQORA"
#define DRIVER_PSQL "psqlodbc"

OdbcTableModel::OdbcTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    /*m_Oracle = QIcon(":/dblogo/Oracle.png");
    m_PostgreSQL = QIcon(":/dblogo/PostgreSQL.png");*/

#ifdef Q_OS_WIN
    loadDataSources64Node();
    loadOdbcElements64Node();

    loadDataSourcesNode();
    loadOdbcElementsNode();
#endif
}

OdbcTableModel::~OdbcTableModel()
{

}

#ifdef Q_OS_WIN
void OdbcTableModel::loadDataSources(ODBCDataSources &dataSources, QSettings *src)
{
    QStringList sources = src->childKeys();
    for (const QString &key : qAsConst(sources))
    {
        dataSources[key] = src->value(key).toString();
    }
}

void OdbcTableModel::loadDataSources64Node()
{
    QSettings src("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\ODBC\\ODBC.INI\\ODBC Data Sources", QSettings::Registry64Format);
    loadDataSources(m_DataSources64, &src);
}

void OdbcTableModel::loadDataSourcesNode()
{
    QSettings src("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", QSettings::Registry64Format);
    loadDataSources(m_DataSources, &src);
}

void OdbcTableModel::FillHomePath(OdbcElement &element, const quint16 &node)
{
    QSettings::Format format = QSettings::Registry32Format;
    QString pathMask = "HKEY_LOCAL_MACHINE\\SOFTWARE\\ORACLE\\KEY_%1";

    if (node == Node64)
    {
        format = QSettings::Registry64Format;
        pathMask = "HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\ORACLE\\KEY_%1";
    }

    QString name = element._home.remove("Oracle in ");
    QSettings src(pathMask.arg(name), format);
    element._homeDir = src.value("ORACLE_HOME").toString();
}

void OdbcTableModel::loadOdbcElements(ODBCDataSources &dataSources, QSettings *src, const quint16 &node)
{
    QStringList childGroups = src->childGroups();
    for (const QString &key : qAsConst(childGroups))
    {
        if (!key.compare("ODBC Data Sources", Qt::CaseInsensitive))
            continue;

        OdbcElement element;
        element._node = node;

        bool hr = readElement(src, key, element, dataSources);
        if (hr)
        {
            FillHomePath(element, node);
            m_List.append(element);
        }
    }
}

void OdbcTableModel::loadOdbcElements64Node()
{
    QSettings src("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\ODBC\\ODBC.INI", QSettings::Registry64Format);
    loadOdbcElements(m_DataSources64, &src, Node64);
}

void OdbcTableModel::loadOdbcElementsNode()
{
    QSettings src("HKEY_LOCAL_MACHINE\\SOFTWARE\\ODBC\\ODBC.INI", QSettings::Registry64Format);
    loadOdbcElements(m_DataSources, &src, Node32);
}

bool OdbcTableModel::readElement(QSettings *node, const QString &key, OdbcElement &element, const ODBCDataSources &dataSources)
{
    QString filename = node->fileName();
    QSettings src(QString("%2\\%1").arg(key, filename), QSettings::Registry64Format);
    QString driver = src.value("Driver").toString();

    if (!driver.contains(DRIVER_SQORA, Qt::CaseInsensitive) && !driver.contains(DRIVER_PSQL, Qt::CaseInsensitive))
        return false;

    if (driver.contains(DRIVER_SQORA, Qt::CaseInsensitive))
        element._db = DataBaseType::Oracle;
    else
        element._db = DataBaseType::PostgreSQL;

    element._name = key;
    element._Driver = driver;
    element._Description = src.value("Description").toString();
    element._home = dataSources[key];

    return true;
}
#endif

int OdbcTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return fld_Max;
}

int OdbcTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_List.size();
}

const OdbcElement &OdbcTableModel::element(const QModelIndex &index) const
{
    return m_List[index.row()];
}

QModelIndex OdbcTableModel::indexOfService(const QString &key) const
{
    for (int i = 0; i < m_List.size(); i++)
    {
        if (m_List[i]._name == key)
            return createIndex(i, 0);
    }
    return QModelIndex();
}

QVariant OdbcTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    OdbcTableModel *pThis = const_cast<OdbcTableModel*>(this);

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        const OdbcElement &element = m_List[index.row()];
        if (index.column() == fld_Name)
            return element._name;
        else if (index.column() == fld_Home)
            return element._home;
        else if (index.column() == fld_OraHome)
            return element._homeDir;
    }
    else if (role == Qt::DecorationRole)
    {
        const OdbcElement &element = m_List[index.row()];
/*
 * m_Oracle = QIcon(":/dblogo/Oracle.png");
    m_PostgreSQL = QIcon(":/dblogo/PostgreSQL.png");
 * */
        if (element._db == DataBaseType::Oracle)
        {
            if (!pThis->m_Oracle)
                pThis->m_Oracle.reset(new QIcon(":/dblogo/Oracle.png"));

            return *pThis->m_Oracle.data();
        }
        else
        {
            if (!m_PostgreSQL)
                pThis->m_PostgreSQL.reset(new QIcon(":/dblogo/PostgreSQL.png"));

            return *pThis->m_PostgreSQL.data();
        }
    }
    return QVariant();
}
