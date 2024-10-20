#ifndef RECENTCONNECTIONLIST_H
#define RECENTCONNECTIONLIST_H

#include <QAbstractTableModel>
#include <QObject>
#include <QDateTime>
#include <QIcon>
#include "fmtcore.h"

#define PswdRole (Qt::UserRole + 1)

typedef struct tagRecentListOld
{
    QString dsn;
    QString user;
    QString pass;
} RecentListOld;

typedef struct tagRecentList : RecentListOld
{
    QDateTime lastConnect;
    quint16 connectionType;
}RecentList;

typedef struct tagRecentList2 : tagRecentList
{
    QMap<QString, QVariant> Options;
}RecentList2;

typedef QMap<QString, QVariant> OptionsMap;
class FMTLIBSHARED_EXPORT RecentConnectionList : public QAbstractTableModel
{
    Q_OBJECT
    typedef QList<RecentList2> RecentListType;
public:
    enum ConnectionType
    {
        Oracle = DataBaseType::Oracle,
        PostgreSQL = DataBaseType::PostgreSQL
    };
    Q_ENUM(ConnectionType)
    RecentConnectionList(QObject *parent = nullptr);
    virtual ~RecentConnectionList();

    bool load();
    bool save();
    bool isEmpty() const;
    void append(const QString &user, const QString &pass, const QString &dsn, const quint16 &type, const OptionsMap &Options);
    void setConnectionType(const QString &user, const QString &dsn, const quint16 &type);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_FINAL;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_FINAL;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) Q_DECL_FINAL;

    bool contains(const QString &user, const QString &dsn) const;
    void sortlist();

    static QString connectionTypeName(const quint16 &type);
    static QString connectionName(const RecentList &rec);
    static QString connectionToolTip(const RecentList &rec);

    RecentList2 record(const QModelIndex &item) const;
    RecentList2 record(const int &item) const;

private:
    void sortPrivate();
    RecentListType::const_iterator find(const QString &user, const QString &dsn) const;
    RecentListType::iterator find(const QString &user, const QString &dsn);
    QString sConfigOraFilePath;

    QIcon m_Oracle, m_PostgreSQL;

    RecentListType m_List;
};

class FilterRecentConnectionList : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterRecentConnectionList(QObject *parent = nullptr);
    virtual ~FilterRecentConnectionList() = default;

    void setAcceptOracle(const bool &value);
    void setAcceptPostgre(const bool &value);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const Q_DECL_OVERRIDE;

private:
    bool m_AcceptOracle, m_AcceptPostgre;
};

Q_DECLARE_METATYPE(RecentListOld)
Q_DECLARE_METATYPE(RecentList)
Q_DECLARE_METATYPE(RecentList2)

#endif // RECENTCONNECTIONLIST_H
