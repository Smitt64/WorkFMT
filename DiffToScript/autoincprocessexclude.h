#ifndef AUTOINCPROCESSEXCLUDE_H
#define AUTOINCPROCESSEXCLUDE_H

#include "autoincprocess.h"

class AutoIncProcessExclude : public AutoIncProcess
{
public:
    AutoIncProcessExclude(DatTable* datTable);

    // AutoIncProcess interface
public:
    bool excludeAutoinc(QStringList &lst) override;
};

#endif // AUTOINCPROCESSEXCLUDE_H
