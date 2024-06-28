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

QString DbSpelling::nvl()
{
    return "NVL";
}

void DbSpelling::setSpelling(const DiffFields &fields, QStringList &values)
{
    for (int i = 0; i < fields.count(); ++i)
    {
        if (fields[i].type == fmtt_DATE)
            values[i] = toDate(values[i]);
    }
}

QString DbSpelling::toBlob(const QString& value)
{
    return value;
}

QString DbSpelling::funcDeclType(const QString& decl)
{
    QString result = decl;

    int pos = result.indexOf("(");
    if (pos != -1)
        result = result.mid(0, pos);

    return result;
}

QString DbSpelling::callProcedure(const QString &proc)
{
    QString result = proc.simplified();

    if (result.back() != ';')
        result += ';';

    return result;
}

DbSpelling::FunctionDeclarePos DbSpelling::functionDeclare() const
{
    return FunctionInDeclareBlock;
}

bool DbSpelling::needDropFunctions() const
{
    return false;
}

QString DbSpelling::dropFunction(const QString &proc, const QString &fullproc, const QString &returnType) const
{
    return QString();
}
