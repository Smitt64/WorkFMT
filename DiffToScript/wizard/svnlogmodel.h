#ifndef SVNLOGMODEL_H
#define SVNLOGMODEL_H

#include <QAbstractTableModel>
#include <QDateTime>

typedef struct
{
    qint32 revision;
    QString author, scr, message;
    QString action;
    QDateTime date;
}SvnLogElement;

typedef struct
{
    QString path, kind;
    QString action;
}SvnLogInfoElement;

typedef QMap<QString, QString> SvnInfoMap;
SvnInfoMap SvnGetRepoInfo(const QString &path);

class SvnLogModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        fld_revision = 0,
        fld_author,
        fld_date,
        fld_scr,
        fld_message,

        fld_Count
    };
    explicit SvnLogModel(QObject *parent = nullptr);
    virtual ~SvnLogModel();

    void setPath(const QString &path, const QString &url);
    void setDateRange(const QDate &from, const QDate &to);

    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    const SvnLogElement &element(const int &row) const;

public slots:
    void refresh();

private:
    QList<SvnLogElement> m_Elements;
    QString m_Path, m_Url;

    QVariant m_Limit;
    QDate m_From, m_To;
};

// ----------------------------------------------------------------------

class SvnLogItemsModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum
    {
        fld_action = 0,
        fld_name,

        fld_Count
    };
    explicit SvnLogItemsModel(QObject *parent = nullptr);
    virtual ~SvnLogItemsModel();

    void setPath(const QString &path, const QString &url);

    virtual QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    const SvnLogInfoElement &element(const int &row) const;

public slots:
    void refresh(const QString &revision);

private:
    QList<SvnLogInfoElement> m_Elements;
    QString m_Path, m_Url;
};

#endif // SVNLOGMODEL_H
