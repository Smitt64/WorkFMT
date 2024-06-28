#ifndef DATTABLEINFO_H
#define DATTABLEINFO_H

#include <difffield.h>
#include <fmttable.h>

class DatTableInfo
{
public:
    DatTableInfo();
    QString name;

    QStringList realFields;
    DiffFields fields;
    DatIndexes indexes;

    bool firstUniq(DatIndex &idx, bool skipAutoInc) const;

    void loadFromFmt(FmtTable* fmtTable);
    DiffField field(const QString &name) const;

    DiffFields missingFldInDat() const;
};

#endif // DATTABLEINFO_H
