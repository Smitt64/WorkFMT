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
};

#endif // DBBUILDERORACLE_H
