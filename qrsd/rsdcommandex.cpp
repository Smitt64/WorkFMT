#include "rsdcommandex.h"

RsdCommandEx::BindParam::BindParam(const QVariant &value, QSql::ParamType paramType, const QString &pname)
{
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

    int namesize = pname.length() + 1;
    name = (char*)malloc(namesize);
    memset(name, 0, namesize);
    strcpy(name, pname.toLocal8Bit().data());

    if (value.type() == QVariant::Int)
    {
        valueSize = typeSize(value.type());
        this->value = malloc(valueSize);
        valType = RSDPT_LONG;
    }
    else if (value.type() == QVariant::LongLong)
    {
        valueSize = typeSize(value.type());
        this->value = malloc(valueSize);
        valType = RSDPT_BIGINT;
    }
    else if (value.type() == QVariant::Double)
    {
        this->value = new qreal();
        valueSize = typeSize(value.type());
        valType = RSDPT_DOUBLE;
    }
    else if (value.type() == QVariant::String)
    {
        QString str = value.toString();
        valueSize = str.length() + 1;
        this->value = malloc(valueSize);
        memset(this->value, 0, valueSize);
        valType = RSDPT_LPSTR;

        char *cstr = (char*)this->value;
        strcpy(cstr, str.toLocal8Bit().data());
    }

}

RsdCommandEx::BindParam::~BindParam()
{

}

int RsdCommandEx::BindParam::typeSize(const QVariant::Type &type)
{
    int size = 0;

    switch(type)
    {
    case QVariant::Int:
        size = sizeof(qint32);
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

RsdCommandEx::RsdCommandEx(CRsdConnection *con) :
    CRsdCommand(*con)
{

}

RsdCommandEx::~RsdCommandEx()
{

}

void RsdCommandEx::bindValue(const QString &placeholder, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, placeholder);
    addParam(prm->name, prm->valType, prm->value, (long*)&prm->valueSize, prm->valueSize, prm->dir);

    m_Params.append(prm);
}

void RsdCommandEx::bindValue(int index, const QVariant &val, QSql::ParamType paramType)
{
    RsdCommandEx::BindParam *prm = new RsdCommandEx::BindParam(val, paramType, "");
    insertParam(index, prm->name, prm->valType, prm->value, (long*)&prm->valueSize);

    m_Params.append(prm);
}
