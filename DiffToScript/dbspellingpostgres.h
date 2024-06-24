#ifndef DBSPELLINGPOSTGRES_H
#define DBSPELLINGPOSTGRES_H

#include "dbspelling.h"

class DbSpellingPostgres : public DbSpelling
{
public:
    DbSpellingPostgres();

    QStringList getDeclare() override;
    QStringList getBegin() override;
    QStringList getEnd() override;
    QString toBlob(const QString& value) override;
    QString chr() override;
    QString toDate(const QString& value) override;

    QString getExceptionName(const ExcceptionType &type) override;
};

#endif // DBSPELLINGPOSTGRES_H
