#include "rsdcommandex.h"

RsdCommandEx::BindParam::BindParam(const QVariant &value, QSql::ParamType paramType, const QString &pname, RsdCommandEx *parent) :
    m_pParent(parent)
{
    this->value = nullptr;
    name = pname;

    switch(paramType)
    {
    case QSql::In:
        dir = RSDBP_IN;
        break;
    case QSql::Out:
        dir = RSDBP_OUT;
        break;
    case QSql::InOut:
        dir = RSDBP_IN_OUT;
        break;
    }

    if (value.type() == QVariant::Int)
    {
        setBuffer<RSDLONG>(value);
        valType = RSDPT_LONG;
    }
    else if (value.type() == QVariant::UInt)
    {
        setBuffer<RSDULONG>(value);
        valType = RSDPT_ULONG;
    }
    else if (value.type() == QVariant::LongLong)
    {
        setBuffer<RSDBIGINT>(value);
        valType = RSDPT_BIGINT;
    }
    else if (value.type() == QVariant::ULongLong)
    {
        setBuffer<RSDUBIGINT>(value);
        valType = RSDPT_UBIGINT;
    }
    else if (value.type() == QVariant::Double)
    {
        setBuffer<RSDDOUBLE>(value);
        valType = RSDPT_DOUBLE;
    }
    else if (value.type() == QVariant::Char)
    {
        setBuffer(value.toChar());
        valType = RSDPT_CHAR;
    }
    else if (value.type() == QVariant::String)
    {
        setBuffer(value.toString());
        valType = RSDPT_LPSTR;
    }
    else if (value.type() == QVariant::Date)
    {
        setBuffer(value.toDate());
        valType = RSDPT_DATE;
    }
    else if (value.type() == QVariant::Time)
    {
        setBuffer(value.toTime());
        valType = RSDPT_TIME;
    }
    else if (value.type() == QVariant::DateTime)
    {
        setBuffer(value.toDateTime());
        valType = RSDPT_TIMESTAMP;
    }
}

void RsdCommandEx::BindParam::setBuffer(const QString &str)
{
    QByteArray oemStr = m_pParent->driver()->toOem866(str);
    valueSize = oemStr.size() + 1;

    size_t size = static_cast<size_t>(valueSize);
    this->value = malloc(size);
    memset(this->value, 0, size);

    char *cstr = static_cast<char*>(this->value);
    strcpy_s(cstr, size, oemStr.data());
}

void RsdCommandEx::BindParam::setBuffer(const QChar &qchar)
{
    QByteArray str = m_pParent->driver()->toOem866(QString(qchar));
    setBuffer<RSDCHAR>(QVariant(str.at(0)));
}

void RsdCommandEx::BindParam::setBuffer(const QDate &qdate)
{
    bdate _rsDate = qDateToRsDate(qdate);
    memset(&_rsDate, 0, sizeof(bdate));
    copyBuffer(_rsDate);
}

void RsdCommandEx::BindParam::setBuffer(const QTime &qtime)
{
    btime _rsTime = qTimeToRsTime(qtime);
    copyBuffer(_rsTime);
}

void RsdCommandEx::BindParam::setBuffer(const QDateTime &qdatetime)
{
    btimestamp _rsDateTime = qDateTimeToRsTimeStamp(qdatetime);
    copyBuffer(_rsDateTime);
}

RsdCommandEx::BindParam::~BindParam()
{
    if (value)
        free(value);
}

int RsdCommandEx::BindParam::typeSize(const QVariant::Type &type)
{
    int size = 0;

    switch(type)
    {
    case QVariant::Int:
        size = sizeof(RSDLONG);
        break;
    case QVariant::LongLong:
        size = sizeof(qint64);
        break;
    case QVariant::Double:
        size = sizeof(qreal);
        break;
    }

    return size;
}

RsdCommandEx::RsdCommandEx(CRsdConnection *con, RsdDriver *driver) :
    CRsdCommand(*con),
    m_pDriver(driver)
{

}

RsdCommandEx::~RsdCommandEx()
{

}

void RsdCommandEx::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, placeholder, this);
    addParam(prm->name.toLocal8Bit().data(), prm->valType, prm->value, (long*)&prm->valueSize, prm->valueSize, prm->dir);

    m_Params.append(prm);
}

void RsdCommandEx::bindValue(int index, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, "", this);
    insertParam(index, prm->name.toLocal8Bit().data(), prm->valType, prm->value, (long*)&prm->valueSize, prm->valueSize, prm->dir);

    m_Params.append(prm);
}

RsdDriver *RsdCommandEx::driver()
{
    return m_pDriver;
}
