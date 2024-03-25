#include "oraclespelling.h"
#include "fmtcore.h"

QString OracleSpelling::toDate(QString value)
{
    return "TO_DATE('" + value + "', 'DD-MM-YYY')";
}

void OracleSpelling::setSpelling(const DiffFields &fields, QStringList &values)
{
    for (int i = 0; i < fields.count(); ++i)
    {
        if (fields[i].type == fmtt_DATE)
            values[i] = toDate(values[i]);
    }
}
