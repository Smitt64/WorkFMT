#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QColor>
#include <QIcon>
#include "fmtlib_global.h"

class FmtTablesModel;
class FMTLIBSHARED_EXPORT ConnectionInfo : public QObject
{
    Q_OBJECT
    /// @private
    friend class FmtTablesModel;
    /// @private
    friend class OracleAuthDlg;
    Q_PROPERTY(bool isOpen READ isOpen CONSTANT)
    Q_PROPERTY(bool isUnicode READ isUnicode CONSTANT)
    Q_PROPERTY(QString typeName READ typeName CONSTANT)
    Q_PROPERTY(int type READ type CONSTANT)
    Q_PROPERTY(QString user READ user CONSTANT)
    Q_PROPERTY(QString password READ password CONSTANT)
    Q_PROPERTY(QString dsn READ dsn CONSTANT)
    Q_PROPERTY(QObject *db READ sqlDatabaseObj CONSTANT)

    Q_PROPERTY(QString connectionName READ connectionName CONSTANT)
public:
    enum ConnectionType
    {
        CON_NON = 0,
        CON_ORA,
        CON_POSTGRESQL,
        CON_SQLITE
    };

    enum ConnectionFeature
    {
        NoFeature = 0,
        CanCreateTable = 1,
        CanSaveToXml,
        CanLoadUnloadDbf
    };

    Q_ENUM(ConnectionType)
    Q_ENUM(ConnectionFeature)
    /**
     * @brief ConnectionInfo
     * @param dbalias
     */
    Q_INVOKABLE ConnectionInfo(const QString &dbalias = QString());
    virtual ~ConnectionInfo();

    FmtTablesModel *tablesModel();
    QSqlDatabase &db();
    QSqlDriver *driver();

    Q_INVOKABLE bool hasFeature(ConnectionInfo::ConnectionFeature feature) const;

    /**
     * @brief Устанавливает имя схемы
     * @param str Имя схемы
     * @se
     */
    void setSchemeName(const QString &str) { m_SchemeName = str; }

    /**
     * @brief Возвращает имя схемы
     * @return Имя схемы
     */
    QString schemeName() const { return m_SchemeName; }
    QString connectionName() const { return m_Alias; }

    QString host() const { return m_Host; }
    QString service() const { return m_Service; }
    QString user() const { return m_User; }
    QString password() const { return m_Password; }
    QString dsn() const { return m_DSN; }

    QObject *sqlDatabaseObj() { return m_SqlDatabaseObj; }

    QIcon colorIcon(const QSize &size = QSize(32, 32));
    QString color();

    void updateFmtList();

    bool isOpen() const;
    bool isUnicode() const;
    int type() const;
    QString typeName() const;

    Q_INVOKABLE bool open(const QString &drv, const QString &user, const QString &password, const QString &dsn,
              const QString &options = QString(), QString *error = nullptr);

    Q_INVOKABLE bool openSqlite(const QString &filename);
    Q_INVOKABLE bool openSqlteMemory();
    Q_INVOKABLE void close();

    FmtTablesModel *addModel();
    FmtTablesModel *getModel(const int &index);
    void deleteModel(const int &index);
    int getFilterIndex(const int &index);
    int lastIndex() const;
    int modelCount() const;

    explicit operator int() const;

protected:
    bool isOracle();
    bool isSqlite();
    bool isPostgre();
    QString m_Alias, m_SchemeName, m_Host, m_Service, m_User, m_Password, m_DSN;
    qint32 m_Port;
    QSqlDatabase _db;
    FmtTablesModel *pModel;
    QColor m_Color;
    QIcon m_Icon;
    ConnectionType m_Type;
    QList<FmtTablesModel*> pModels;
    QList<int> m_Index;
    int Index;
    int m_GlobalId;

    QObject *m_SqlDatabaseObj;
};

#endif // CONNECTIONINFO_H
