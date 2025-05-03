#include "dbspellingoracle.h"
#include "fmtcore.h"
#include "sqlscriptmain.h"

QString DbSpellingOracle::toDate(const QString& value)
{
    QString tmp = value;

    if (tmp.isEmpty())
        tmp = "01-01-0001";

    return "TO_DATE('" + tmp + "', 'DD-MM-YYYY HH24:MI:SS')";
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

QString DbSpellingOracle::blobTypeName(const int &type)
{
    return BlobFieldTypeOraString(type);
}

QString DbSpellingOracle::getExceptionName(const int &type)
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

QString DbSpellingOracle::functionParamType(const qint16 &type)
{
    QString result = fmtOracleDecl(type);
    result = funcDeclType(result);
    return result;
}

void DbSpellingOracle::functionChunks(QStringList &BeginCreateReplace,
                                      QStringList &EndCreateReplace,
                                      const QString &name,
                                      const QString &params,
                                      const QString &returnType)
{
    QString declaration;

    if (returnType.isEmpty())
        declaration = "PROCEDURE ";
    else
        declaration = "FUNCTION ";

    declaration += name + " (";
    declaration += params + ")";

    if (!returnType.isEmpty())
    {
        declaration += QString(" RETURN %1")
                .arg(returnType);
    }

    BeginCreateReplace.append(declaration);
    BeginCreateReplace.append("AS");
    BeginCreateReplace.append("-- #conv-Oracle");
    BeginCreateReplace.append(QString("%1 ErrorText VARCHAR2(2000) := CHR(1);").arg(Padding()));
    BeginCreateReplace.append("-- #conv-PG");
    BeginCreateReplace.append("-- DECLARE");
    BeginCreateReplace.append("-- ErrorText varchar(2000);");
    BeginCreateReplace.append("-- #conv-end ");

    EndCreateReplace.append("END;");
}

QString DbSpellingOracle::getDefaultValueForType(const qint16 &Type, const int &size)
{
    QString val = fmtGetOraDefaultVal(Type, size);

    return val.replace("''", "'");
}

QStringList DbSpellingOracle::getExceptionInfo(const QString &varname)
{
    QStringList lst;
    lst.append(Padding() + "-- #conv-Oracle");
    lst.append(Padding() + QString("%1 := Substr(sqlerrm, 11);").arg(varname));
    lst.append(Padding() + "-- #conv-PG");
    lst.append(Padding() + QString("-- %1 := sqlerrm;").arg(varname));
    lst.append(Padding() + "-- #conv-end ");
    return lst;
}
