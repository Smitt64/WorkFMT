#include "autoincprocessexclude.h"

#include <fmttable.h>
#include <fmtfield.h>

AutoIncProcessExclude::AutoIncProcessExclude(DatTable* datTable)
    :AutoIncProcess(datTable)
{

}

bool AutoIncProcessExclude::excludeAutoinc(QStringList &lst)
{
    int cnt = _datTable->fields.count();
    if (lst.count() != cnt)
        return false;

    QStringList newNames;
    for (int i = 0; i < lst.count(); ++i)
    {
        if (_datTable->fields[i].isAutoinc)
            continue;
        newNames.append(lst[i]);
    }

    lst = newNames;

    return true;
}
