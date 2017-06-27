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
    /**
     * @brief ConnectionInfo
     * @param dbalias
     */
    ConnectionInfo(const QString &dbalias);

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

protected:
    QString m_Alias, m_SchemeName, m_Host, m_Service, m_User, m_Password, m_DSN;
    qint32 m_Port;
    QSqlDatabase _db;
    FmtTablesModel *pModel;
    QColor m_Color;
    QIcon m_Icon;
};

#endif // CONNECTIONINFO_H
