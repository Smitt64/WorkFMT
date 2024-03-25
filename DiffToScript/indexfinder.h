#ifndef INDEXFINDER_H
#define INDEXFINDER_H

#include "diffcore.h"
#include "dattable.h"
#include "tablelinks.h"
#include "dattableinfo.h"

class IndexFinder
{
public:
    FieldList findIndex(const TableLinks& tableLinks);
    IndexFields findIndex(const DatTableInfo& datTableInfo);
private:
    QList<DatTable*> _joinedTable;
};

#endif // INDEXFINDER_H
