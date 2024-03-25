#include "autoincprocessbuilder.h"
#include "autoincprocessexclude.h"

AutoIncProcessBuilder::AutoIncProcessBuilder(DatTable* datTable)
    :_datTable(datTable)
{

}

//Возвращает класс отвечающий за автоинкрементные поля.
AutoIncProcess* AutoIncProcessBuilder::buildAutoIncProcess(const TaskOptions& optns)
{
    if (optns[ctoAutoInc].isSet && optns[ctoAutoInc].value == "0")
    {
        return new AutoIncProcessExclude(_datTable);
    }
    return new AutoIncProcess(_datTable);
}
