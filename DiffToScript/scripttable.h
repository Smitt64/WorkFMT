#ifndef SCRIPTTABLE_H
#define SCRIPTTABLE_H

#include "diffcore.h"
#include "difftableinfo.h"

class ScriptTable: public DiffTable
{
public:
    ScriptTable();
    void loadData(const ParsedLines& lines);
    const QStringList& getErrors() const { return _errors; }
    int errorCount() const { return _errorCount; }
    void parseUpdateRecords(ScriptTable& datTable);
    bool hasInserts() const;

private:
    QStringList _errors; // Ошибки после выполнения функции loadData.
    int _errorCount; // Ошибки за всё время существования объекта.
    QString getPrimaryKey(const DatIndexes& indexes);
    bool containsAny(const QString& str, const QStringList& stringList);
    DatRecords::iterator parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last);
};

#endif // SCRIPTTABLE_H
