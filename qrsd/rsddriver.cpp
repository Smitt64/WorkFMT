#include "rsddriver.h"
#include "rsdsqlresult.h"
#include <QSqlError>
#include <QVariant>
#include <Windows.h>
#include <rsdcore.h>
#include <QTextStream>

RsdDriver::RsdDriver(QObject *parent) :
    QSqlDriver(parent),
    BaseErrorSetter<RsdDriver>(this),
    m_Connection(nullptr)
{
    codec866 = QTextCodec::codecForName("IBM 866");
    codec1251 = QTextCodec::codecForName("Windows-1251");
}

RsdDriver::~RsdDriver()
{
    m_ResultsList.clear();
}

void RsdDriver::close()
{
    if (m_Connection)
    {
        for (auto item : qAsConst(m_ResultsList))
            item->onBeforeCloseConnection();

        coreConnClose(m_Connection);
    }
}

bool RsdDriver::isOpen() const
{
    if (!m_Connection)
        return false;

    return coreConnIsOpen(m_Connection);
}

QSqlResult *RsdDriver::createResult() const
{
    return new RsdSqlResult(this);
}

QVariant RsdDriver::handle() const
{
    return QVariant::fromValue(this);
}

bool RsdDriver::hasFeature(QSqlDriver::DriverFeature feature) const
{
    bool result = false;
    switch(feature)
    {
    case QSqlDriver::Transactions:
    case QSqlDriver::BLOB:
    case QSqlDriver::PreparedQueries:
    case QSqlDriver::NamedPlaceholders:
    case QSqlDriver::LowPrecisionNumbers:
        result = true;
        break;
    default:
        result = false;
    }

    return result;
}

bool RsdDriver::isUnicode() const
{
    if (!m_Connection)
        return false;

    return coreConnIsUnicode(m_Connection);
}

bool RsdDriver::isPostgres() const
{
    if (!m_Connection)
        return false;

    return coreConnIsPostgres(m_Connection);
}

bool RsdDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &options)
{
    bool hr = coreConnOpen(db.toLocal8Bit().constData(),
                           user.toLocal8Bit().constData(),
                           password.toLocal8Bit().constData(),
                           options.toLocal8Bit().constData(),
                           (void**)&m_Connection);

    if (!hr)
    {
        setLastRsdError(m_Connection);
        coreFreeHandle(&m_Connection);
    }

    return hr;
}

bool RsdDriver::beginTransaction()
{
    if (!m_Connection)
        return false;

    return coreConnBeginTrn(m_Connection);
}

bool RsdDriver::commitTransaction()
{
    if (!m_Connection)
        return false;

    return coreConnCommitTrn(m_Connection);
}

bool RsdDriver::rollbackTransaction()
{
    if (!m_Connection)
        return false;

    return coreConnRollbackTrn(m_Connection);
}

QString RsdDriver::fromOem866(const QLatin1String &str) const
{
    return codec866->toUnicode(str.latin1());
}

QString RsdDriver::fromOem866(const char *str) const
{
    return codec866->toUnicode(str);
}

QByteArray RsdDriver::toOem866(const QString &str) const
{
    return codec866->fromUnicode(str);
}

QString RsdDriver::from1251(const QLatin1String &str) const
{
    return codec1251->toUnicode(str.latin1());
}

QString RsdDriver::from1251(const char *str) const
{
    return codec1251->toUnicode(str);
}

QByteArray RsdDriver::to1251(const QString &str) const
{
    return codec1251->fromUnicode(str);
}

Qt::HANDLE RsdDriver::connection()
{
    return m_Connection;
}

QTextCodec *RsdDriver::getCodec866()
{
    return codec866;
}

void RsdDriver::addResult(RsdSqlResult *result)
{
    m_ResultsList.append(result);
}

void RsdDriver::removeResult(RsdSqlResult *result)
{
    m_ResultsList.removeOne(result);
}

bdate qDateToRsDate(const QDate &qdate)
{
    bdate _rsDate;
    memset(&_rsDate, 0, sizeof(bdate));

    if (!qdate.isNull())
    {
        _rsDate.day  = static_cast<unsigned char>(qdate.day());
        _rsDate.mon  = static_cast<unsigned char>(qdate.month());
        _rsDate.year = static_cast<unsigned short>(qdate.year());
    }

    return _rsDate;
}

QDate rsDateToQDate(const bdate &_rsdate)
{
    QDate qdate;

    if (_rsdate.day != 0 && _rsdate.mon != 0 && _rsdate.year != 0)
        qdate = QDate(_rsdate.year, _rsdate.mon, _rsdate.day);

    return qdate;
}

btime qTimeToRsTime(const QTime &qtime)
{
    btime _rsTime;
    memset(&_rsTime, 0, sizeof(btime));

    _rsTime.hour = static_cast<unsigned char>(qtime.hour());
    _rsTime.min = static_cast<unsigned char>(qtime.minute());
    _rsTime.sec = static_cast<unsigned char>(qtime.second());

    return _rsTime;
}

QTime rsTimeToQTime(const btime &_rstime)
{
    return QTime(_rstime.hour, _rstime.min, _rstime.sec);
}

btimestamp qDateTimeToRsTimeStamp(const QDateTime &qdatetime)
{
    btimestamp _rsDateTime;
    memset(&_rsDateTime, 0, sizeof(btimestamp));

    _rsDateTime.date = qDateToRsDate(qdatetime.date());
    _rsDateTime.time = qTimeToRsTime(qdatetime.time());

    return _rsDateTime;
}

QDateTime rsTimeStampToQDateTime(const btimestamp &_rstimestamp)
{
    QDateTime qdatetime;
    qdatetime.setDate(rsDateToQDate(_rstimestamp.date));
    qdatetime.setTime(rsTimeToQTime(_rstimestamp.time));

    return qdatetime;
}
