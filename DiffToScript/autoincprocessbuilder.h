#ifndef AUTOINCPROCESSBUILDER_H
#define AUTOINCPROCESSBUILDER_H

#include "taskoptions.h"
#include "autoincprocess.h"

class AutoIncProcessBuilder
{
public:
    AutoIncProcessBuilder(DatTable* datTable);
    AutoIncProcess* buildAutoIncProcess(const TaskOptions& optns);
private:
    DatTable* _datTable;
};

#endif // AUTOINCPROCESSBUILDER_H
