#ifndef AUTOINCPROCESS_H
#define AUTOINCPROCESS_H

#include <QStringList>

#include "dattable.h"

class AutoIncProcess
{
public:
    AutoIncProcess(DatTable* datTable);
    virtual bool excludeAutoinc(QStringList &lst);
    const QStringList& getErrors() const {return _errors; }
protected:
    QStringList _errors;
    DatTable* _datTable;
};

#endif // AUTOINCPROCESS_H
