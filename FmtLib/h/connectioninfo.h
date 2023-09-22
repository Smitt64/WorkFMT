#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QtCore>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QColor>
#include <QIcon>
#include "fmtlib_global.h"

#define QRSD_DRIVER "qrsd"

class FmtTablesModel;
class FMTLIBSHARED_EXPORT ConnectionInfo : public QObject
{
    Q_OBJECT
    /// @private
    friend class FmtTablesModel;
    /// @private
    friend class OracleAuthDlg;
public:
    enum ConnectionType
    {
        CON_NON = 0,
        CON_ORA,
        CON_POSTGRESQL,
        CON_SQLITE
    };
    /**
     * @brief ConnectionInfo
     * @param dbalias
     */
    ConnectionInfo(const QString &dbalias = QString());
    virtual ~ConnectionInfo();

    FmtTablesModel *tablesModel();
    QSqlDatabase &db();
    QSqlDriver *driver();

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
    QString dbAlias() const { return m_Alias; }

    QString host() const { return m_Host; }
    QString service() const { return m_Service; }
    QString user() const { return m_User; }
    QString password() const { return m_Password; }
    QString dsn() const { return m_DSN; }

    QIcon colorIcon(const QSize &size = QSize(32, 32));
    QColor color();

    void updateFmtList();

    bool isOpen() const;
    int type() const;
    QString typeName() const;

    bool open(const QString &drv, const QString &user, const QString &password, const QString &dsn, QString *error = nullptr);
    bool openSqlite(const QString &filename);
    bool openSqlteMemory();

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
};

#endif // CONNECTIONINFO_H
