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
    QString chr() override;
};

#endif // DBSPELLINGPOSTGRES_H
