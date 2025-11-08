#ifndef DATTABLE_H
#define DATTABLE_H

#include "difftableinfo.h"
#include "diffconnection.h"
#include "DiffToScriptl_global.h"

class DIFFTOSCRIPTL_EXPORT DatTable : public DiffTable
{
public:
    DatTable();
    bool load(const QString& dir, const QString& tableName, DiffConnection* conn);

    bool hasInserts() const;
};

#endif // DATTABLE_H
