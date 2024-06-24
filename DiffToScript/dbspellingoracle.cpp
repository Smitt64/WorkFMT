#include "dbspellingoracle.h"

QString DbSpellingOracle::toDate(const QString& value)
{
    return "TO_DATE('" + value + "', 'DD-MM-YYY')";
}

QStringList DbSpellingOracle::getDeclare()
{
    return QStringList() << "DECLARE";
}

QStringList DbSpellingOracle::getBegin()
{
    return QStringList() << "BEGIN";
}

QStringList DbSpellingOracle::getEnd()
{
    return QStringList() << "END;"
                         << "/";
}

QString DbSpellingOracle::toBlob(const QString& value)
{
    return QString("HEXTORAW(%1)").arg(value);
}

QString DbSpellingOracle::getExceptionName(const ExcceptionType &type)
{
    QString result;

    switch(type)
    {
    case ExceptDupValOnIndex:
        result = "DUP_VAL_ON_INDEX";
        break;
    }

    return result;
}
