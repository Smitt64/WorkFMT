#include "rsddriver.h"
#include "rsdsqlresult.h"
#include <QSqlError>
#include <QVariant>

RsdDriver::RsdDriver(QObject *parent) :
    QSqlDriver(parent),
    BaseErrorSetter<RsdDriver>(this)
{
    codec866 = QTextCodec::codecForName("IBM 866");
    codec1251 = QTextCodec::codecForName("Windows-1251");
    m_Env.reset(new CRsdEnvironment("RDDrvO", "RDDrvO.dll"));
}

RsdDriver::~RsdDriver()
{

}

void RsdDriver::close()
{
    try
    {
        m_Connection->close();
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
    }

    return result;
}

bool RsdDriver::open(const QString &db, const QString &user, const QString &password, const QString &host, int port, const QString &options)
{
    bool hr = true;

    try
    {
        QByteArray db866 = codec866->fromUnicode(db);
        QByteArray user866 = codec866->fromUnicode(user);
        QByteArray password866 = codec866->fromUnicode(password);
        m_Connection.reset(new CRsdConnection(*m_Env.get(), db866.data(), user866.data(), password866.data()));
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

QString RsdDriver::fromOem866(const char *str) const
{
    return codec866->toUnicode(str);
}

QByteArray RsdDriver::toOem866(const QString &str) const
{
    return codec866->fromUnicode(str);
}

CRsdConnection *RsdDriver::connection()
{
    return m_Connection.data();
}