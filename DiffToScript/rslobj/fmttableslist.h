#ifndef FMTTABLESLIST_H
#define FMTTABLESLIST_H

#include "fmttable.h"
#include "rslmodule/iterableobjectbase.h"

class FmtTablesList : public ListIterableObject<FmtTable*>
{
    Q_OBJECT
public:
    FmtTablesList() = default;
    virtual ~FmtTablesList() = default;

    Q_INVOKABLE bool hasTable(const QString &name);
    Q_INVOKABLE FmtTable *fmt(const QString &name);
};

#endif // FMTTABLESLIST_H
