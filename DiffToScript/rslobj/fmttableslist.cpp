#include "fmttableslist.h"

bool FmtTablesList::hasTable(const QString &name)
{
    for (int i = 0; i < GetSize(); i++)
    {
        auto table = at(i);
        if (!table->name().compare(name, Qt::CaseInsensitive))
            return true;
    }

    return false;
}

FmtTable *FmtTablesList::fmt(const QString &name)
{
    for (int i = 0; i < GetSize(); i++)
    {
        auto table = at(i);
        if (!table->name().compare(name, Qt::CaseInsensitive))
            return table;
    }

    return nullptr;
}
