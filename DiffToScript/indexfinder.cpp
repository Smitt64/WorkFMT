#include "indexfinder.h"


FieldList IndexFinder::findIndex(const TableLinks& tableLinks)
{
    return tableLinks.index;
}

bool isContainAutoinc(const IndexFields& fields)
{
    for (const IndexField& field: fields)
        if (field.isAutoinc)
            return true;
    return false;
}

IndexFields IndexFinder::findIndex(const DatTableInfo &datTableInfo)
{
    std::vector<DatIndex> v{DatIndex()}; // По умочанию вернёт пустой индекс
    for (const DatIndex& index: datTableInfo.indexes)
        if (index.isUnique && !isContainAutoinc(index.fields))
            v.push_back(index);

    if(v.size())
    {
        return v[0].fields;
    }

    return IndexFields();
}

