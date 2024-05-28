#include "dbspelling.h"
#include "fmtcore.h"

QString DbSpelling::toDate(const QString& value)
{
    return "TO_DATE('" + value + "', 'DD-MM-YYYY:HH24:MI:SS')";
}

QString DbSpelling::chr()
{
    return "CHR";
}

void DbSpelling::setSpelling(const DiffFields &fields, QStringList &values)
{
    for (int i = 0; i < fields.count(); ++i)
    {
        if (fields[i].type == fmtt_DATE)
            values[i] = toDate(values[i]);
    }
}
