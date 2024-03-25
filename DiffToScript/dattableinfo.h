#ifndef DATTABLEINFO_H
#define DATTABLEINFO_H

#include <difffield.h>
#include <fmttable.h>

class DatTableInfo
{
public:
    DatTableInfo();
    QString name;
    DiffFields fields;
    DatIndexes indexes;    
    void loadFromFmt(FmtTable* fmtTable);
};

#endif // DATTABLEINFO_H
