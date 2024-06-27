#ifndef DBBUILDERORACLE_H
#define DBBUILDERORACLE_H

#include "dbspelling.h"

class DbSpellingOracle : public DbSpelling
{
public:
    //explicit OracleSpelling(QObject *parent = nullptr);

    // DbSpelling interface
public:
    QString toDate(const QString& value) override;
    QStringList getDeclare() override;
    QStringList getBegin() override;
    QStringList getEnd() override;
    QString toBlob(const QString& value) override;
    QString blobTypeName(const int &type) override;

    QString getExceptionName(const ExcceptionType &type) override;
    QString functionParamType(const qint16 &type) override;

    void functionChunks(QStringList &BeginCreateReplace,
                        QStringList &EndCreateReplace,
                        const QString &name,
                        const QString &params,
                        const QString &returnType) override;

    QString getDefaultValueForType(const qint16 &Type, const int &size) override;
};

#endif // DBBUILDERORACLE_H
