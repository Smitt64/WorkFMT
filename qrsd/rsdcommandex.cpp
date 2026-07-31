#include "rsdcommandex.h"

RsdCommandEx::BindParam::BindParam(const QVariant &value, QSql::ParamType paramType, const QString &pname, RsdCommandEx *parent) :
    m_pParent(parent)
{
    this->value = nullptr;
    name = pname;
    nameBytes = pname.toLocal8Bit();

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
    else if (value.type() == QVariant::ByteArray)
    {
        byteArrayValue = value.toByteArray();
        valType = RSDPT_BLOB;
        valueSize = sizeof(RSDBLOB);
        this->value = malloc(static_cast<size_t>(valueSize));
        memset(this->value, 0, static_cast<size_t>(valueSize));

        RSDBLOB *blob = static_cast<RSDBLOB*>(this->value);
        blob->data = byteArrayValue.isEmpty() ? nullptr : static_cast<void*>(byteArrayValue.data());
        blob->size = static_cast<size_t>(byteArrayValue.size());
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
    clearBatch();
}

void RsdCommandEx::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, placeholder, this);

    // Для BLOB параметр передаётся через структуру RSDBLOB: длина и индикатор не нужны,
    // драйвер сам забирает размер из RSDBLOB.size.
    if (prm->valType == RSDPT_BLOB)
        addParam(prm->nameBytes.data(), prm->valType, prm->value, nullptr, 0, prm->dir);
    else
        addParam(prm->nameBytes.data(), prm->valType, prm->value, (long*)&prm->valueSize, prm->valueSize, prm->dir);

    m_Params.append(prm);
}

void RsdCommandEx::bindValue(int index, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, "", this);

    if (prm->valType == RSDPT_BLOB)
        insertParam(index, prm->nameBytes.data(), prm->valType, prm->value, nullptr, 0, prm->dir);
    else
        insertParam(index, prm->nameBytes.data(), prm->valType, prm->value, (long*)&prm->valueSize, prm->valueSize, prm->dir);

    m_Params.append(prm);
}

void RsdCommandEx::bindBatch(const QString &placeholder, const QVariantList &values, QSql::ParamType paramType)
{
    const int count = values.size();

    // Поэлементно конвертируем значения через BindParam, чтобы переиспользовать
    // существующую логику преобразования QVariant -> RSD. Для каждого элемента
    // определяем размер; NULL/невалидные значения не конвертируются.
    QVector<BindParam*> elems;
    elems.reserve(count);

    RSDValType_t valType = RSDPT_LPSTR; // тип по умолчанию для полностью NULL-колонки
    long stride = 1;                    // шаг буфера = максимальный размер элемента
    bool typeFound = false;

    for (int i = 0; i < count; ++i)
    {
        const QVariant &v = values.at(i);

        if (!v.isValid() || v.isNull())
        {
            elems.append(nullptr);
            continue;
        }

        BindParam *p = new BindParam(v, paramType, placeholder, this);

        // BindParam оставляет value == nullptr для неподдерживаемых типов QVariant —
        // трактуем такое значение как NULL, чтобы не падать на memcpy.
        if (!p->value)
        {
            delete p;
            elems.append(nullptr);
            continue;
        }

        elems.append(p);

        if (!typeFound)
        {
            valType = p->valType;
            typeFound = true;
        }

        if (p->valueSize > stride)
            stride = p->valueSize;
    }

    BatchParam *batch = new BatchParam();
    const size_t bufSize = static_cast<size_t>(stride) * static_cast<size_t>(count);
    batch->value = malloc(bufSize ? bufSize : 1);
    memset(batch->value, 0, bufSize ? bufSize : 1);
    batch->indLen = static_cast<long*>(malloc(sizeof(long) * static_cast<size_t>(count ? count : 1)));

    for (int i = 0; i < count; ++i)
    {
        BindParam *p = elems.at(i);

        if (!p)
        {
            batch->indLen[i] = RSDBS_NULL;
            continue;
        }

        memcpy(static_cast<char*>(batch->value) + static_cast<size_t>(i) * stride, p->value, p->valueSize);
        batch->indLen[i] = p->valueSize;

        delete p;
    }

    m_BatchParams.append(batch);

    RSDBindParamIO_t dir = RSDBP_IN;
    if (paramType == QSql::Out)
        dir = RSDBP_OUT;
    else if (paramType == QSql::InOut)
        dir = RSDBP_IN_OUT;

    // Сохраняем QByteArray с именем placeholder'а, чтобы addParam получил валидный char*.
    // toLocal8Bit() возвращает временный объект, без сохранения получается висячий указатель,
    // что в release-сборке приводит к повреждению имени параметра.
    m_BatchParamNames.append(placeholder.toLocal8Bit());
    addParam(m_BatchParamNames.last().data(), valType, batch->value, batch->indLen, stride, dir);
}

void RsdCommandEx::clearBatch()
{
    qDeleteAll(m_BatchParams);
    m_BatchParams.clear();
    m_BatchParamNames.clear();
}

RsdDriver *RsdCommandEx::driver()
{
    return m_pDriver;
}
