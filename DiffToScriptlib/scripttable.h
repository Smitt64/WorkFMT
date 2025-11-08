#ifndef SCRIPTTABLE_H
#define SCRIPTTABLE_H

#include "diffcore.h"
#include "difftableinfo.h"
#include "DiffToScriptl_global.h"

class DIFFTOSCRIPTL_EXPORT ScriptTable: public DiffTable
{
    Q_OBJECT
    Q_PROPERTY(QStringList errors READ getErrors CONSTANT)
    Q_PROPERTY(int errorCount READ errorCount CONSTANT)
    Q_PROPERTY(bool hasInserts READ hasInserts CONSTANT)
    Q_PROPERTY(bool hasUpdates READ hasUpdates CONSTANT)
    Q_PROPERTY(bool hasDeleteions READ hasDeleteions CONSTANT)
public:
    ScriptTable();
    void loadData(const ParsedLines& lines);

    const QStringList& getErrors() const { return _errors; }

    int errorCount() const { return _errorCount; }
    void parseUpdateRecords(ScriptTable& datTable);

    bool hasInserts() const;
    bool hasUpdates() const;
    bool hasDeleteions() const;

    Q_INVOKABLE bool containsAny(const QString& str, const QStringList& stringList);
    Q_INVOKABLE QString getPrimaryKey(DatIndexes *indexes);
    Q_INVOKABLE QString getPrimaryKey();

private:
    QStringList _errors; // Ошибки после выполнения функции loadData.
    int _errorCount; // Ошибки за всё время существования объекта.
    DatRecords::iterator parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last);
};

Q_DECLARE_OPAQUE_POINTER(ScriptTable);

#endif // SCRIPTTABLE_H
