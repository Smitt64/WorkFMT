#ifndef RECENTCONNECTIONLIST_H
#define RECENTCONNECTIONLIST_H

#include <QAbstractTableModel>
#include <QObject>
#include <QDateTime>
#include <QIcon>

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

class RecentConnectionList : public QAbstractTableModel
{
    Q_OBJECT
    typedef QList<RecentList> RecentListType;
public:
    enum ConnectionType
    {
        Oracle = 0,
        PostgreSQL
    };
    Q_ENUM(ConnectionType)
    RecentConnectionList(QObject *parent = nullptr);
    virtual ~RecentConnectionList();

    bool load();
    bool save();
    bool isEmpty() const;
    void append(const QString &user, const QString &pass, const QString &dsn);
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

    RecentList record(const QModelIndex &item) const;
    RecentList record(const int &item) const;

private:
    void sortPrivate();
    RecentListType::const_iterator find(const QString &user, const QString &dsn) const;
    RecentListType::iterator find(const QString &user, const QString &dsn);
    QString sConfigOraFilePath;

    QIcon m_Oracle, m_PostgreSQL;

    RecentListType m_List;
};

Q_DECLARE_METATYPE(RecentListOld)
Q_DECLARE_METATYPE(RecentList)

#endif // RECENTCONNECTIONLIST_H
