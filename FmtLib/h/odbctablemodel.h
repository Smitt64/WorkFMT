#ifndef ODBCTABLEMODEL_H
#define ODBCTABLEMODEL_H

#include <QIcon>
#include <QAbstractTableModel>

typedef struct tagOdbcElement
{
    QString _name, _Description, _home, _Driver;
    quint8 _node, _db;
}OdbcElement;

class QSettings;
typedef QVector<OdbcElement> OdbcElementVector;
class OdbcTableModel : public QAbstractTableModel
{
    Q_OBJECT

#ifdef Q_OS_WIN
    typedef QMap<QString,QString> ODBCDataSources;
#endif
public:

    enum NodeType
    {
        Node32 = 0,
        Node64,
    };

    enum Columns
    {
        fld_Name,
        fld_Home,

        fld_Max
    };

    OdbcTableModel(QObject *parent = nullptr);
    virtual ~OdbcTableModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_FINAL;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_FINAL;

    const OdbcElement &element(const QModelIndex &index) const;

private:
#ifdef Q_OS_WIN
    void loadDataSources(ODBCDataSources &dataSources, QSettings *src);
    void loadOdbcElements(ODBCDataSources &dataSources, QSettings *src, const quint16 &node);
    bool readElement(QSettings *node, const QString &key, OdbcElement &element, const ODBCDataSources &dataSources);

    void loadDataSources64Node();
    void loadOdbcElements64Node();

    void loadDataSourcesNode();
    void loadOdbcElementsNode();

    ODBCDataSources m_DataSources64, m_DataSources;
#endif

    QIcon m_Oracle, m_PostgreSQL;
    OdbcElementVector m_List;
};

#endif // ODBCTABLEMODEL_H
