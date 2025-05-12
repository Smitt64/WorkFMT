#ifndef DBSPELLINGPOSTGRES_H
#define DBSPELLINGPOSTGRES_H

#include "dbspelling.h"

class DbSpellingPostgres : public DbSpelling
{
    Q_OBJECT
public:
    Q_INVOKABLE DbSpellingPostgres();

    QStringList getDeclare() override;
    QStringList getBegin() override;
    QStringList getEnd() override;
    QString toBlob(const QString& value) override;
    QString chr() override;
    QString nvl() override;
    QString toDate(const QString& value) override;
    QString blobTypeName(const int &type) override;
    QString callProcedure(const QString &proc) override;

    QString getExceptionName(const int &type) override;
    QString functionParamType(const qint16 &type) override;

    bool needDropFunctions() const override;
    QString dropFunction(const QString &proc, const QString &fullproc, const QString &returnType) const override;
    int functionDeclare() const override;
    void functionChunks(QStringList &BeginCreateReplace,
                        QStringList &EndCreateReplace,
                        const QString &name,
                        const QString &params,
                        const QString &returnType) override;

    QString getDefaultValueForType(const qint16 &Type, const int &size) override;
    QStringList getExceptionInfo(const QString &varname) override;
};

#endif // DBSPELLINGPOSTGRES_H
