#ifndef DIFFTABLEINFO_H
#define DIFFTABLEINFO_H

#include <difffield.h>
#include <fmttable.h>
#include <diffcore.h>
#include "toolsruntime.h"

class DiffTableInfo
{
public:
    DiffTableInfo();
    QString name;
    DiffFields fields;
    DatIndexes indexes;    
    QStringList realFields;
    void loadFromFmt(FmtTable* fmtTable); //TODO replace FmtTable to QString
    bool firstUniq(DatIndex &idx, bool skipAutoInc) const;
    DiffField field(const QString &name) const;
    DiffFields missingFldInDat() const;
};

class DiffTable: public DiffTableInfo
{
public:
    DatRecords records;
};

#endif // DIFFTABLEINFO_H
