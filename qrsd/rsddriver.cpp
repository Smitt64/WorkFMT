#include "rsddriver.h"
#include "rsdsqlresult.h"
#include <QSqlError>
#include <QVariant>

RsdDriver::RsdDriver(QObject *parent) :
    QSqlDriver(parent),
    BaseErrorSetter<RsdDriver>(this),
    m_RDDrvO("RDDrvO"),
    m_RDDrvODll("RDDrvO.dll")
{
    qputenv("NLS_LANG", "AMERICAN_CIS.RU8PC866");
    codec866 = QTextCodec::codecForName("IBM 866");
    codec1251 = QTextCodec::codecForName("Windows-1251");
}

RsdDriver::~RsdDriver()
{
    m_ResultsList.clear();
}

void RsdDriver::close()
{
    try
    {
        if (m_Connection)
        {
            for (auto item : qAsConst(m_ResultsList))
                item->onBeforeCloseConnection();

            m_Connection->close();
        }
    }
    catch (XRsdError& e)
    {
        setLastRsdError(e);
    }
    catch(...)
    {
        QString err = "Непредусмотренное исключение";
        setLastError(QSqlError(err, QString(), QSqlError::TransactionError));
    }
}

bool RsdDriver::isOpen() const
{
    return !m_Connection.isNull();
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

bool RsdDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &options)
{
    bool hr = true;

    try
    {
        if (options.contains("RSD_UNICODE"))
        {
            qstrcpy(m_RDDrvO, "RDDrvOu");
            qstrcpy(m_RDDrvODll, (QString("%1.dll").arg(m_RDDrvO)).toLocal8Bit().data());
        }

        m_Env.reset(new CRsdEnvironment(m_RDDrvO, m_RDDrvODll));
        m_Env->SetOdbcInterface(NULL);
        m_Env->setClientEncoding(RSDENC_OEM);

        qstrcpy(db866, codec866->fromUnicode(db).data());
        qstrcpy(user866, codec866->fromUnicode(user).data());
        qstrcpy(password866, codec866->fromUnicode(password).data());

        m_Connection.reset(new CRsdConnection(*m_Env.get(), db866, user866, password866));
        m_Connection->setClientEncoding(RSDENC_OEM);
        m_Connection->setServerEncoding(RSDENC_OEM);
        m_Connection->open();
    }
    catch (XRsdError& e)
    {
        m_Connection.reset();
        setLastRsdError(e, QSqlError::ConnectionError);
        hr = false;
    }
    catch(...)
    {
        m_Connection.reset();
        setLastUnforeseenError(QSqlError::ConnectionError);
        hr = false;
    }

    return hr;
}

bool RsdDriver::beginTransaction()
{
    bool result = true;

    try
    {
        m_Connection->beginTrans();
    }
    catch (XRsdError& e)
    {
        setLastRsdError(e, QSqlError::TransactionError);
        result = false;
    }
    catch(...)
    {
        setLastUnforeseenError(QSqlError::TransactionError);
        result = false;
    }

    return result;
}

bool RsdDriver::commitTransaction()
{
    bool result = true;

    try
    {
        m_Connection->commitTrans();
    }
    catch (XRsdError& e)
    {
        setLastRsdError(e, QSqlError::TransactionError);
        result = false;
    }
    catch(...)
    {
        setLastUnforeseenError(QSqlError::TransactionError);
        result = false;
    }

    return result;
}

bool RsdDriver::rollbackTransaction()
{
    bool result = true;

    try
    {
        m_Connection->rollbackTrans();
    }
    catch (XRsdError& e)
    {
        setLastRsdError(e, QSqlError::TransactionError);
        result = false;
    }
    catch(...)
    {
        setLastUnforeseenError(QSqlError::TransactionError);
        result = false;
    }

    return result;
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

CRsdConnection *RsdDriver::connection()
{
    return m_Connection.data();
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
