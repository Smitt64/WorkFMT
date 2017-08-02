#ifndef CONNECTIONINFO_H
#define CONNECTIONINFO_H

#include <QtCore>
#include <QSqlDatabase>
#include <QColor>
#include <QIcon>
#include "fmtlib_global.h"

class FmtTablesModel;
class FMTLIBSHARED_EXPORT ConnectionInfo
{
    /// @private
    friend class FmtTablesModel;
    /// @private
    friend class OracleAuthDlg;
public:
    enum ConnectionType
    {
        CON_NON = 0,
        CON_ORA,
        CON_SQLITE
    };
    /**
     * @brief ConnectionInfo
     * @param dbalias
     */
    ConnectionInfo(const QString &dbalias = QString());
    virtual ~ConnectionInfo();

    FmtTablesModel *tablesModel();
    QSqlDatabase db();

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

    QIcon colorIcon();
    QColor color();

    void updateFmtList();

    bool isOpen() const;
    int type() const;

    bool openSqlite(const QString &filename);

protected:
    QString m_Alias, m_SchemeName, m_Host, m_Service, m_User, m_Password, m_DSN;
    qint32 m_Port;
    QSqlDatabase _db;
    FmtTablesModel *pModel;
    QColor m_Color;
    QIcon m_Icon;
    ConnectionType m_Type;
};

#endif // CONNECTIONINFO_H
