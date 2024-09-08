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
    QStringList _errors; //РћС€РёР±РєРё РїРѕСЃР»Рµ РІС‹РїРѕР»РЅРµРЅРёСЏ С„СѓРЅРєС†РёРё loadData.
    int _errorCount; // РћС€РёР±РєРё Р·Р° РІСЃС‘ РІСЂРµРјСЏ СЃСѓС‰РµСЃС‚РІРѕРІР°РЅРёСЏ РѕР±СЉРµРєС‚Р°.
    QString getPrimaryKey(const DatIndexes& indexes);
    DatRecords::iterator parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last);
};

#endif // SCRIPTTABLE_H
