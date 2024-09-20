#include "tablesgroupprovider.h"
#include "fmtcore.h"
#include "loggingcategories.h"
#include "toolsruntime.h"
#include <QDebug>
#include <QtXml>
#include <QDebug>
#include <QCoreApplication>
#include <QFile>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

TablesGroupProvider *TablesGroupProvider::m_pInstance = NULL;
TablesGroupProvider::TablesGroupProvider(QObject *parent) : QObject(parent)
{

}

bool TablesGroupProvider::Init()
{
    bool result = true;
    _db = QSqlDatabase::addDatabase("QSQLITE", "TablesGroupProvider");
    _db.setDatabaseName(QString("%1/tables.dat").arg(QDir::currentPath()));

    result = _db.open();

    if (result)
    {
        QStringList tables = _db.tables();
        if (!tables.contains("table_groups"))
        {
            CreateTableGroupsTable();
            _db.commit();
        }

        if (!tables.contains("table_in_group"))
        {
            CreateTablesInGroupTable();
            _db.commit();
        }

        m_pGroupsModel = new QSqlTableModel(this, _db);
        m_pGroupsModel->setTable("table_groups");

        if (!m_pGroupsModel->select())
            qCWarning(logCore()) << "Can't select groups:" << _db.lastError().text();

        createDefaultGroups();
    }
    else
        qCWarning(logCore()) << "Can't load tables groups:" << _db.lastError().text();

    return result;
}

bool TablesGroupProvider::isGroupExists(const QString &name)
{
    QSqlQuery q(_db);
    q.prepare("SELECT count(1) FROM table_groups WHERE name = :name");
    q.bindValue(":name", name);

    if (ExecuteQuery(&q))
        qCWarning(logCore()) << "Can't select groups:" << _db.lastError().text();

    return q.next() && q.value(0).toInt() != 0;
}

void TablesGroupProvider::createDefaultGroups()
{
    QByteArray data = toolReadTextFileContent(":/TablesGroupProvider", "UTF-8").toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    for (QJsonValue v : obj.value("groups").toArray()) 
    {
        QJsonObject group = v.toObject();
        QString name = group.value("name").toString();

        QString groupUID;
        if (!isGroupExists(name))
            addGroup(name, &groupUID);
        else
            groupUID = selectGroupUid(name);

        QJsonArray tables = group.value("tables").toArray();
        for (QJsonValue v : tables)
        {
            QString table = v.toString();
            addTable(groupUID, table);
        }
    }
}

QString TablesGroupProvider::ReadFileContent(const QString &filename)
{
    QString str;
    QFile f(filename, this);
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        str = f.readAll();
        f.close();
    }
    return str;
}

void TablesGroupProvider::CreateTableGroupsTable()
{
    QSqlQuery q(_db);
    q.prepare(this->ReadFileContent(":/create_table_groups"));
    ExecuteQuery(&q);
}

void TablesGroupProvider::CreateTablesInGroupTable()
{
    QSqlQuery q(_db);
    q.prepare(this->ReadFileContent(":/create_table_in_group"));
    ExecuteQuery(&q);
}

TablesGroupProvider *TablesGroupProvider::instance()
{
    if (!TablesGroupProvider::m_pInstance)
    {
        TablesGroupProvider *tmp = new TablesGroupProvider(NULL);
        if (!tmp->Init())
            delete tmp;
        else
            TablesGroupProvider::m_pInstance = tmp;
    }
    return TablesGroupProvider::m_pInstance;
}

bool TablesGroupProvider::addTable(const QString &groupUID, const QString &table)
{
    QSqlQuery q(_db);
    q.prepare("insert into table_in_group(group_uid,table_name)values(?,?)");
    q.bindValue(0, groupUID);
    q.bindValue(1, table);
    bool result = !ExecuteQuery(&q);

    if (!result)
        _db.commit();

    return result;
}

bool TablesGroupProvider::addTable(const int &groupModelIndex, const QString &table)
{
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    return addTable(groupUID, table);
}

bool TablesGroupProvider::removeTable(const int &groupModelIndex, const QString &table)
{
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    QSqlQuery q(_db);
    q.prepare("delete from table_in_group where group_uid = ? AND table_name = ?");
    q.bindValue(0, groupUID);
    q.bindValue(1, table);

    bool result = !ExecuteQuery(&q);

    if (!result)
        _db.commit();

    return result;
}

QSharedPointer<QSqlQuery> TablesGroupProvider::getQueryForSeletGroup(const int &groupModelIndex)
{
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    QSharedPointer<QSqlQuery> query(new QSqlQuery(_db));
    query->prepare("select * from table_in_group where group_uid = ?");
    query->bindValue(0, groupUID);

    return query;
}

int TablesGroupProvider::getTablesCountInGroup(const int &groupModelIndex)
{
    int count = -1;
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    QSqlQuery query(_db);
    query.prepare("select count(*) from table_in_group where group_uid = ?");
    query.bindValue(0, groupUID);

    if (!ExecuteQuery(&query) && query.next())
        count = query.value(0).toInt();

    return count;
}

bool TablesGroupProvider::addGroup(const QString &name, QString *uid)
{
    QSqlQuery query(_db);
    query.prepare("insert into table_groups(id,name) values(?,?)");
    query.bindValue(0, QUuid::createUuid().toString());
    query.bindValue(1, name);

    bool result = !ExecuteQuery(&query);

    if (result)
    {
        if (uid)
            *uid = selectGroupUid(name);

        m_pGroupsModel->select();
    }

    return result;
}

QStringList TablesGroupProvider::getGroupsList()
{
    QStringList lst;

    for (int i = 0; i < m_pGroupsModel->rowCount(); i++)
        lst.append(m_pGroupsModel->record(i).value(1).toString());

    return lst;
}

bool TablesGroupProvider::removeGroup(const int &groupModelIndex)
{
    return !trn(_db, [this, groupModelIndex]() -> int
    {
        int stat = 0;
        QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
        QSqlQuery q(_db);
        q.prepare("delete from table_groups where id = ?");
        q.bindValue(0, groupUID);
        stat = ExecuteQuery(&q);

        if (!stat)
        {
            QSqlQuery q2(_db);
            q2.prepare("delete from table_in_group where group_uid = ?");
            q2.bindValue(0, groupUID);
            stat = ExecuteQuery(&q2);
        }

        if (!stat)
            m_pGroupsModel->select();

        return stat;
    });
}

bool TablesGroupProvider::renameGroup(const int &groupModelIndex, const QString &name)
{
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    QSqlQuery query(_db);
    query.prepare("update table_groups set name = ? where id = ");
    query.bindValue(0, name);
    query.bindValue(1, groupUID);

    bool result = !ExecuteQuery(&query);

    if (result)
    {
        _db.commit();
        m_pGroupsModel->select();
    }

    return result;
}

QString TablesGroupProvider::selectGroupUid(const QString &name)
{
    QString uid;
    QSqlQuery query(_db);
    query.prepare("select id from  table_groups where name = ?");
    query.bindValue(0, name);

    if (!ExecuteQuery(&query) && query.next())
        uid = query.value(0).toString();

    return uid;
}

QSqlDatabase &TablesGroupProvider::db()
{
    return _db;
}

QString TablesGroupProvider::groupUid(const int &groupModelIndex)
{
    QString groupUID = m_pGroupsModel->data(m_pGroupsModel->index(groupModelIndex, 0)).toString();
    return groupUID;
}
