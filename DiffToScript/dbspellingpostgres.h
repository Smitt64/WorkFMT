#ifndef DBSPELLINGPOSTGRES_H
#define DBSPELLINGPOSTGRES_H

#include "dbspelling.h"

class DbSpellingPostgres : public DbSpelling
{
public:
    DbSpellingPostgres();


    // DbSpelling interface
public:
    QStringList getDeclare() override;
    QStringList getBegin() override;
    QStringList getEnd() override;
};

#endif // DBSPELLINGPOSTGRES_H
