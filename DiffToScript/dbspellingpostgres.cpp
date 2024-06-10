#include "dbspellingpostgres.h"

DbSpellingPostgres::DbSpellingPostgres()
{

}

QStringList DbSpellingPostgres::getDeclare()
{
    return QStringList()
            << "do $do$"
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
            << "$do$ language plpgsql;";
}

QString DbSpellingPostgres::chr()
{
    return "glob_func.chr";
}

QString DbSpellingPostgres::toBlob(const QString& value)
{
    return QString("decode(%1, 'hex')").arg(value);
}
