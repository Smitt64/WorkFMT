#ifndef TABLESGROUPPROVIDER_H
#define TABLESGROUPPROVIDER_H

#include <QObject>
#include <QtSql>
#include "fmtlib_global.h"

class FMTLIBSHARED_EXPORT TablesGroupProvider : public QObject
{
    Q_OBJECT
public:
    static TablesGroupProvider *instance();
    QAbstractItemModel *groupsModel() { return m_pGroupsModel; }
    bool addTable(const QString &groupUID, const QString &table);
    bool addTable(const int &groupModelIndex, const QString &table);
    bool removeTable(const int &groupModelIndex, const QString &table);
    bool removeGroup(const int &groupModelIndex);
    bool renameGroup(const int &groupModelIndex, const QString &name);
    QString selectGroupUid(const QString &name);
    QString groupUid(const int &groupModelIndex);

    QSharedPointer<QSqlQuery> getQueryForSeletGroup(const int &groupModelIndex);
    int getTablesCountInGroup(const int &groupModelIndex);

    bool addGroup(const QString &name, QString *uid = NULL);

    QStringList getGroupsList();
    QSqlDatabase &db();

    bool isGroupExists(const QString &name);

private:
    explicit TablesGroupProvider(QObject *parent = 0);
    bool Init();
    
    void createDefaultGroups();
    QString ReadFileContent(const QString &filename);
    void CreateTableGroupsTable();
    void CreateTablesInGroupTable();

    static TablesGroupProvider *m_pInstance;
    QSqlDatabase _db;

    QSqlTableModel *m_pGroupsModel;
};

#endif // TABLESGROUPPROVIDER_H
