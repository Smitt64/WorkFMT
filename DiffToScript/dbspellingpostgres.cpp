#include "dbspellingpostgres.h"
#include "fmtcore.h"

DbSpellingPostgres::DbSpellingPostgres()
{

}

QStringList DbSpellingPostgres::getDeclare()
{
    return QStringList()
            << "do $do$"
            << "DECLARE";
}

QStringList DbSpellingPostgres::getBegin()
{
    return QStringList()
            << "BEGIN";
}

QStringList DbSpellingPostgres::getEnd()
{
    return QStringList()
            << "end;"
            << "$do$ language plpgsql;"
            << "/";
}

QString DbSpellingPostgres::chr()
{
    return "glob_func.chr";
}

QString DbSpellingPostgres::nvl()
{
    return "COALESCE";
}

QString DbSpellingPostgres::toBlob(const QString& value)
{
    return QString("decode(%1, 'hex')").arg(value);
}

QString DbSpellingPostgres::blobTypeName(const int &type)
{
    return BlobFieldTypePgString(type);
}

QString DbSpellingPostgres::toDate(const QString& value)
{
    return "glob_func.to_timestamp_immutable('" + value + "', 'DD-MM-YYY')";
}

QString DbSpellingPostgres::getExceptionName(const ExcceptionType &type)
{
    QString result;

    switch(type)
    {
    case ExceptDupValOnIndex:
        result = "UNIQUE_VIOLATION";
        break;
    }

    return result;
}

QString DbSpellingPostgres::callProcedure(const QString &proc)
{
    QString result = QString("PERFORM %1").arg(proc.simplified());

    if (result.back() != ';')
        result += ';';

    return result;
}

QString DbSpellingPostgres::functionParamType(const qint16 &type)
{
    QString result = fmtPostgresDecl(type);
    result = funcDeclType(result);
    return result;
}

DbSpelling::FunctionDeclarePos DbSpellingPostgres::functionDeclare() const
{
    return FunctionBeforeBlocks;
}

void DbSpellingPostgres::functionChunks(QStringList &BeginCreateReplace,
                                        QStringList &EndCreateReplace,
                                        const QString &name,
                                        const QString &params,
                                        const QString &returnType)
{
    QString declaration = "CREATE OR REPLACE FUNCTION ";

    declaration += name + " (";
    declaration += params + ")";

    if (!returnType.isEmpty())
    {
        declaration += QString(" RETURNS %1")
                .arg(returnType);
    }
    else
        declaration += QString(" RETURNS VOID");

    BeginCreateReplace.append(declaration);
    BeginCreateReplace.append(QString("AS $%1$").arg(name));
    BeginCreateReplace.append("DECLARE");

    EndCreateReplace.append("END;");
    EndCreateReplace.append(QString("$%1$ LANGUAGE PLPGSQL;").arg(name));
    EndCreateReplace.append("/");
}

QString DbSpellingPostgres::getDefaultValueForType(const qint16 &Type, const int &size)
{
    QString val = fmtGetPgDefaultVal(Type, size);

    return val.replace("''", "'");
}

bool DbSpellingPostgres::needDropFunctions() const
{
    return true;
}

QString DbSpellingPostgres::dropFunction(const QString &proc, const QString &returnType) const
{
    return QString("DROP FUNCTION IF EXISTS %1;").arg(proc);
}
