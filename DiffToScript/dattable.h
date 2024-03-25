#ifndef DATTABLE_H
#define DATTABLE_H

#include "diffcore.h"
#include "dattableinfo.h"

class DatTable: public DatTableInfo
{
public:
    DatRecords records;
    DatTable();
    void loadData(const ParsedLines& lines);
    const QStringList& getErrors() const { return _errors; }
    int errorCount() const { return _errorCount; }
private:
    QStringList _errors; //Ошибки после выполнения функции loadData.
    int _errorCount; // Ошибки за всё время существования объекта.
};

#endif // DATTABLE_H
