#include "scripttable.h"
#include "recordparser.h"
#include "difflogging.h"

ScriptTable::ScriptTable()
{
    _errorCount = 0;
}

void ScriptTable::loadData(const ParsedLines& lines)
{
    qCInfo(logScriptTable) << "Start load records. Parsed lines = " << lines.count();
    RecordParser recParser(&fields, realFields);

    for (const ParsedLine& line: lines)
    {
        if (line.lineType == ltTable)
        {
            name = line.value;
            qCInfo(logScriptTable) << "Table name loaded " << name;
            continue;
        }

        if (!containsAny(line.value, realFields))
        {
            if (recParser.parseRecord(line.value))
            {
                records.append(recParser.getValues(), line.lineType, line.lineUpdateType);
                qCInfo(logScriptTable) << "Record added: " << recParser.getValues().join("m");
            }
            else
            {
                qCWarning(logScriptTable) << "Error in line: " << line.value;
                _errors.append("Error in line: " + line.value);
                _errors.append(recParser.getErrors());
                _errorCount++;
            }
        }
    }    
    qCInfo(logScriptTable) << "End load records. Record added " << records.count();
}

bool ScriptTable::containsAny(const QString& str, const QStringList& stringList)
{
    for (const QString& item : stringList)
    {
        if (str.contains(item))
            return true;
    }

    return false;
}

QString ScriptTable::getPrimaryKey(DatIndexes *indexes)
{
    for (const DatIndex *index: *indexes)
    {
        if (index->isUnique && index->fields.count() == 1 && index->fields[0]->isAutoinc)
            return index->fields[0]->name;
    }

    return "";
}

QString ScriptTable::getPrimaryKey()
{
    return getPrimaryKey(&indexes);
}

DatRecords::iterator ScriptTable::parseUpdateBlock(int indexPrimaryKey, DatRecords::iterator first, DatRecords::iterator last)
{
    DatRecords::iterator it = first;

    for(;it != last && (*it)->lineType == ltUpdate && (*it)->lineUpdateType == lutOld; ++it);
    for(;it != last && (*it)->lineType == ltUpdate && (*it)->lineUpdateType == lutNew; ++it);
    last = it;

    DatRecords::iterator mid = first + (last - first) / 2;

    for (;mid != last; ++first, ++mid)
    {
        if ((*first)->values[indexPrimaryKey] != (*mid)->values[indexPrimaryKey])
        {
            (*first)->lineType = ltDelete;
            (*first)->lineUpdateType = lutOld;

            (*mid)->lineType = ltInsert;
            (*mid)->lineUpdateType = lutNew;

            qCInfo(logScriptTable) << "Line type changed for primary key " << (*first)->values[indexPrimaryKey];
        }
    }

    return last;
}

void ScriptTable::parseUpdateRecords(ScriptTable& datTable)
{
    QString primaryKeyField = datTable.getPrimaryKey();
    if (primaryKeyField == "")
        return;

    int indexPrimaryKey = datTable.fields.indexByFieldName(primaryKeyField);
    if (indexPrimaryKey == -1)
        return;

    DatRecords::iterator it = datTable.records.begin();
    while (it != datTable.records.end())
    {
        if ((*it)->lineType != ltUpdate)
            ++it;
        else
            it = parseUpdateBlock(indexPrimaryKey, it, datTable.records.end());
    }

    qCInfo(logScriptTable) << "Parsing update records is done.";
}

bool ScriptTable::hasInserts() const
{
    for (const DatRecord *rec : records)
    {
        if (rec->lineType == ltInsert)
            return true;
    }

    return false;
}

bool ScriptTable::hasUpdates() const
{
    for (const DatRecord *rec : records)
    {
        if (rec->lineType == ltUpdate)
            return true;
    }

    return false;
}

bool ScriptTable::hasDeleteions() const
{
    for (const DatRecord *rec : records)
    {
        if (rec->lineType == ltDelete)
            return true;
    }

    return false;
}
