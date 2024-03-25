#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QString>
#include "diffcore.h"
#include "difffield.h"

class DbSpelling
{
public:
    virtual QString toDate(const QString& value);
    virtual void setSpelling(const DiffFields& fields, QStringList& values);
    virtual QStringList getDeclare() = 0;
    virtual QStringList getBegin() = 0;
    virtual QStringList getEnd() = 0;
};

#endif // DBBUILDER_H
