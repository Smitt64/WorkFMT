#ifndef RSDDRIVER_H
#define RSDDRIVER_H

#include <QVariant>
#include <QSqlDriver>
#include <QScopedPointer>
#include <QTextCodec>
#include <RsdC.h>
#include <BaseErrorSetter.hpp>

class RsdSqlResult;
class RsdDriver Q_DECL_FINAL: public QSqlDriver, public BaseErrorSetter<RsdDriver>
{
    Q_OBJECT
    friend class BaseErrorSetter<RsdDriver>;
public:
    RsdDriver(QObject *parent = nullptr);
    virtual ~RsdDriver();

    void close() Q_DECL_OVERRIDE;
    QSqlResult *createResult() const Q_DECL_OVERRIDE;
    bool hasFeature(QSqlDriver::DriverFeature feature) const Q_DECL_OVERRIDE;
    bool open(const QString &db, const QString &user = QString(), const QString &password = QString(), const QString &host = QString(), int port = -1, const QString &options = QString()) Q_DECL_OVERRIDE;
    QVariant handle() const Q_DECL_OVERRIDE;
    bool isOpen() const Q_DECL_OVERRIDE;

    bool beginTransaction() Q_DECL_OVERRIDE;
    bool commitTransaction() Q_DECL_OVERRIDE;
    bool rollbackTransaction() Q_DECL_OVERRIDE;

    QString fromOem866(const QLatin1String &str) const;
    QByteArray toOem866(const QString &str) const;

    QString from1251(const QLatin1String &str) const;
    QByteArray to1251(const QString &str) const;

    CRsdConnection *connection();

    QTextCodec *getCodec866();

    void addResult(RsdSqlResult *result);
    void removeResult(RsdSqlResult *result);

private:
    QTextCodec *codec866, *codec1251;
    QScopedPointer<CRsdEnvironment> m_Env;
    QScopedPointer<CRsdConnection> m_Connection;

    char m_RDDrvO[256], m_RDDrvODll[256];

    char db866[256], user866[256], password866[256];
    QList<RsdSqlResult*> m_ResultsList;
};

bdate qDateToRsDate(const QDate &qdate);
btime qTimeToRsTime(const QTime &qtime);
btimestamp qDateTimeToRsTimeStamp(const QDateTime &qdatetime);

QDate rsDateToQDate(const bdate &_rsdate);
QTime rsTimeToQTime(const btime &_rstime);
QDateTime rsTimeStampToQDateTime(const btimestamp &_rstimestamp);

Q_DECLARE_METATYPE(const RsdDriver*)

#endif // RSDDRIVER_H
