#include "fmtnamespace.h"
#include "fmtcore.h"

FmtNamespace::FmtNamespace(QObject *parent) :
    QObject(parent)
{

}

FmtNamespace::~FmtNamespace()
{

}

QString FmtNamespace::fmtVersion() const
{
    return GetVersionNumberString();
}

QStringList FmtNamespace::fmtTypes() const
{
    return ::fmtTypes();
}

QString FmtNamespace::fmtTypeNameForType(const qint16 &type) const
{
    return ::fmtTypeNameForType(type);
}

qint16 FmtNamespace::fmtTypeFromIndex(const qint16 &id) const
{
    return ::fmtTypeFromIndex(id);
}

QString FmtNamespace::fmtOracleDecl(const qint16 &Type) const
{
    return ::fmtOracleDecl(Type);
}

QString FmtNamespace::fmtPostgresDecl(const qint16 &Type) const
{
    return ::fmtPostgresDecl(Type);
}

QString FmtNamespace::fmtCppStructTypeName(const qint16 &Type) const
{
    return ::fmtCppStructTypeName(Type);
}

QString FmtNamespace::fmtCppStructDbTypeName(const qint16 &Type) const
{
    return ::fmtCppStructDbTypeName(Type);
}

QString FmtNamespace::fmtCppStructDbBaseTypeName(const qint16 &Type) const
{
    return ::fmtCppStructDbBaseTypeName(Type);
}

QString FmtNamespace::fmtRslTypeName(const qint16 &Type) const
{
    return ::fmtRslTypeName(Type);
}

QString FmtNamespace::fmtRslValueName(const qint16 &Type) const
{
    return ::fmtRslValueName(Type);
}

bool FmtNamespace::fmtTypeCanHaveCustomSize(const qint16 &Type) const
{
    return ::fmtTypeCanHaveCustomSize(Type);
}

QString FmtNamespace::fmtGetOraDefaultVal(const qint16 &Type, const int &size) const
{
    return ::fmtGetOraDefaultVal(Type, size);
}

QString FmtNamespace::fmtGetPgDefaultVal(const qint16 &Type, const int &size) const
{
    return ::fmtGetPgDefaultVal(Type, size);
}

quint16 FmtNamespace::fmtTypeSize(const qint16 &Type) const
{
    return ::fmtTypeSize(Type);
}

quint16 FmtNamespace::fmtTypeIndexSize(const qint16 &Type) const
{
    return ::fmtTypeIndexSize(Type);
}

QString FmtNamespace::fmtRsdType(const qint16 &Type) const
{
    return ::fmtRsdType(Type);
}

QString FmtNamespace::fmtRsdConstant(const qint16 &Type) const
{
    return ::fmtRsdConstant(Type);
}

QString FmtNamespace::fmtZeroConstant(const qint16 &Type) const
{
    return ::fmtZeroConstant(Type);
}

QString FmtNamespace::BlobTypeToString(const qint16 &Type) const
{
    return ::BlobTypeToString(Type);
}

QString FmtNamespace::BlobFieldString(const qint16 &Type) const
{
    return ::BlobFieldString(Type);
}

QString FmtNamespace::BlobFieldTypeOraString(const qint16 &Type) const
{
    return ::BlobFieldTypeOraString(Type);
}

QString FmtNamespace::BlobFieldTypePgString(const qint16 &Type) const
{
    return ::BlobFieldTypePgString(Type);
}

QString FmtNamespace::NullString(const int &index) const
{
    return ::NullString(index);
}

QString FmtNamespace::fmtTableStructName(const QString &table)
{
    return ::FmtTableStructName(table);
}

QStringList FmtNamespace::fmtTableStringList(const QString &table)
{
    return ::FmtTableStringList(table);
}

QString FmtNamespace::fmtGetTableExtension(const QString &table)
{
    return ::FmtGetTableExtension(table);
}

QString FmtNamespace::fmtGetTableFileName(const QString &table)
{
    return ::FmtGetTableFileName(table);
}

QStringList FmtNamespace::fmtGenGetTriggers(ConnectionInfo *connection, const QString &table)
{
    return ::FmtGenGetTriggers(connection, table);
}

QString FmtNamespace::fmtTableSqlText(FmtTable *pTable) const
{
    return ::FmtTableSqlText(pTable);
}