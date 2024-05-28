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
