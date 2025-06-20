#include "dbspellingpostgres.h"
#include "fmtcore.h"
#include "sqlscriptmain.h"

DbSpellingPostgres::DbSpellingPostgres() :
    DbSpelling()
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
    QString tmp = value;

    if (tmp.isEmpty())
        tmp = "01-01-0001";

    return "glob_func.to_timestamp_immutable('" + tmp + "', 'DD-MM-YYY')";
}

QString DbSpellingPostgres::getExceptionName(const int &type)
{
    QString result;

    switch(type)
    {
    case ExceptDupValOnIndex:
        result = "UNIQUE_VIOLATION";
        break;
    case ExceptNoDataFound:
        result = "NO_DATA_FOUND";
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
          // numeric
    if (type == fmtt_INT || type == fmtt_LONG || type == fmtt_BIGINT || type == fmtt_MONEY  || type == fmtt_NUMERIC)
        result = "numeric";
    else if (type == fmtt_FLOAT || type == fmtt_DOUBLE)
        result = "float";

    result = funcDeclType(result);
    return result;
}

int DbSpellingPostgres::functionDeclare() const
{
    return FunctionBeforeBlocks;
}

QString DbSpellingPostgres::getProcKeyWord(const bool &rettype)
{
    return "FUNCTION ";
}

QString DbSpellingPostgres::getProcReturnKeyWord(const QString &returntype)
{
    if (!returntype.isEmpty())
        return QString(" RETURNS %1").arg(returntype);

    return QString(" RETURNS VOID");
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
    BeginCreateReplace.append(QString("%1 ErrorText varchar(2000);").arg(Padding()));

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

QString DbSpellingPostgres::dropFunction(const QString &fullproc) const
{
    QString normal = fullproc;
    normal = normal.remove("FUNCTION ");

    int pos = normal.indexOf("RETURNS");
    normal = normal.mid(0, pos).simplified();
    return QString("DROP FUNCTION IF EXISTS %1;").arg(normal);
}

QStringList DbSpellingPostgres::getExceptionInfo(const QString &varname)
{
    QStringList lst;
    lst.append(Padding() + QString("%1 := sqlerrm;").arg(varname));
    return lst;
}
