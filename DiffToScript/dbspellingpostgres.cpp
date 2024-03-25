#include "dbspellingpostgres.h"

DbSpellingPostgres::DbSpellingPostgres()
{

}

QStringList DbSpellingPostgres::getDeclare()
{
    return QStringList()
            << "DO"
            << "$$"
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
            << "$$ "
            << "language plpgsql;";

}
