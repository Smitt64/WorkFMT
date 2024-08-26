#ifndef FMTNAMESPACE_H
#define FMTNAMESPACE_H

#include <QObject>
#include "fmtcore.h"

#define BegiFmtEnumNamespace(Name) class Name##Namespace : public QObject \
{\
    Q_OBJECT \
public:\
    Name##Namespace() : QObject(){}   \
    enum Name\
    { \

#define EndFmtEnumNamespace(Name) }; Q_ENUM(Name) };

BegiFmtEnumNamespace(FmtKeyFlags)
Duplicates = fmtkf_Duplicates,
Modifiable = fmtkf_Modifiable,
Binary     = fmtkf_Binary,
NullVal    = fmtkf_NullVal,
Segment    = fmtkf_Segment,
Alternate  = fmtkf_Alternate,
Descending = fmtkf_Descending,
Supplement = fmtkf_Supplement,
Extended   = fmtkf_Extended,
Manual     = fmtkf_Manual,
Local      = fmtkf_Local,
EndFmtEnumNamespace(FmtKeyFlags)

BegiFmtEnumNamespace(FmtTypes)
INT = fmtt_INT,
LONG = fmtt_LONG,
BIGINT = fmtt_BIGINT,
FLOAT = fmtt_FLOAT,
DOUBLE = fmtt_DOUBLE,
MONEY = fmtt_MONEY,
STRING = fmtt_STRING,
SNR = fmtt_SNR,
DATE = fmtt_DATE,
TIME = fmtt_TIME,
CHR = fmtt_CHR,
UCHR = fmtt_UCHR,
NUMERIC = fmtt_NUMERIC,
EndFmtEnumNamespace(FmtTypes)

BegiFmtEnumNamespace(FmtKeyNullVal)
Nullval_None = keynullval_None,
Nullval_All = keynullval_All,
Nullval_Any = keynullval_Any,
EndFmtEnumNamespace(FmtKeyNullVal)

class FmtNamespace : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString fmtVersion READ fmtVersion)
    Q_PROPERTY(QStringList fmtTypes READ fmtTypes)
public:
    enum FmtBlobType
    {
        BLOBT_BLOB_NO = 0,
        BLOBT_BLOB_VAR,
        BLOBT_BLOB_STREAM,
        BLOBT_CLOB
    };

    Q_ENUM(FmtBlobType)

    FmtNamespace(QObject *parent = nullptr);
    virtual ~FmtNamespace();

    QString fmtVersion() const;
    Q_INVOKABLE QString fmtTypeNameForType(const qint16 &type) const;
    Q_INVOKABLE qint16 fmtTypeFromIndex(const qint16 &id) const;
    Q_INVOKABLE QString fmtOracleDecl(const qint16 &Type) const;
    Q_INVOKABLE QString fmtPostgresDecl(const qint16 &Type) const;
    Q_INVOKABLE QString fmtCppStructTypeName(const qint16 &Type) const;
    Q_INVOKABLE QString fmtCppStructDbTypeName(const qint16 &Type) const;
    Q_INVOKABLE QString fmtCppStructDbBaseTypeName(const qint16 &Type) const;
    Q_INVOKABLE QString fmtRslTypeName(const qint16 &Type) const;
    Q_INVOKABLE QString fmtRslValueName(const qint16 &Type) const;
    Q_INVOKABLE bool fmtTypeCanHaveCustomSize(const qint16 &Type) const;
    Q_INVOKABLE QString fmtGetOraDefaultVal(const qint16 &Type, const int &size) const;
    Q_INVOKABLE QString fmtGetPgDefaultVal(const qint16 &Type, const int &size) const;

    Q_INVOKABLE quint16 fmtTypeSize(const qint16 &Type) const;
    Q_INVOKABLE quint16 fmtTypeIndexSize(const qint16 &Type) const;
    Q_INVOKABLE QString fmtRsdType(const qint16 &Type) const;
    Q_INVOKABLE QString fmtRsdConstant(const qint16 &Type) const;
    Q_INVOKABLE QString fmtZeroConstant(const qint16 &Type) const;

    Q_INVOKABLE QString fmtTableStructName(const QString &table);
    Q_INVOKABLE QStringList fmtTableStringList(const QString &table);
    Q_INVOKABLE QString fmtGetTableExtension(const QString &table);
    Q_INVOKABLE QString fmtGetTableFileName(const QString &table);

    Q_INVOKABLE QStringList fmtGenGetTriggers(ConnectionInfo *connection, const QString &table);

    Q_INVOKABLE QString BlobTypeToString(const qint16 &Type) const;
    Q_INVOKABLE QString BlobFieldString(const qint16 &Type) const;
    Q_INVOKABLE QString BlobFieldTypeOraString(const qint16 &Type) const;
    Q_INVOKABLE QString BlobFieldTypePgString(const qint16 &Type) const;

    Q_INVOKABLE QString fmtTableSqlText(FmtTable *pTable) const;

    Q_INVOKABLE QString NullString(const int &index) const;

    QStringList fmtTypes() const;
signals:

};

#endif // FMTNAMESPACE_H
