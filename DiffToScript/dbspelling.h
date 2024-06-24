#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QString>
#include "diffcore.h"
#include "difffield.h"

class DbSpelling
{
public:
    enum ExcceptionType
    {
        ExceptDupValOnIndex = 1,
    };
    virtual QString toDate(const QString& value);
    virtual QString toBlob(const QString& value);
    virtual QString chr();
    virtual void setSpelling(const DiffFields& fields, QStringList& values);
    virtual QStringList getDeclare() = 0;
    virtual QStringList getBegin() = 0;
    virtual QStringList getEnd() = 0;

    virtual QString getExceptionName(const ExcceptionType &type) = 0;
};

#endif // DBBUILDER_H
