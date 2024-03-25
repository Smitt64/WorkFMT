#include "autoincprocess.h"

AutoIncProcess::AutoIncProcess(DatTable* datTable)
    :_datTable(datTable)
{

}

bool AutoIncProcess::excludeAutoinc(QStringList &lst)
{
    if (lst.count() != _datTable->fields.count())
        return false;
    return true;
}

