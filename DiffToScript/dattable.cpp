#include "dattable.h"
#include "recordparser.h"
#include "difflogging.h"

DatTable::DatTable()
{
    _errorCount = 0;
}

void DatTable::loadData(const ParsedLines& lines)
{
    qCInfo(logDatTable) << "Start load records. Parsed lines = " << lines.count();
    RecordParser recParser(&fields);

    for (const ParsedLine& line: lines)
    {
        if (line.lineType == ltTable)
        {
            name = line.value;
            qCInfo(logDatTable) << "Table name loaded " << name;
            continue;
        }

        if (recParser.parseRecord(line.value))
        {
            records.append({recParser.getValues(), line.lineType, line.lineUpdateType});
            qCInfo(logDatTable) << "Record added: " << recParser.getValues().join("m");
        }
        else
        {
            qCWarning(logDatTable) << "Error in line: " << line.value;
            _errors.append("Error in line: " + line.value);
            _errors.append(recParser.getErrors());
            _errorCount++;
        }
    }    
    qCInfo(logDatTable) << "End load records. Record added " << records.count();
}

bool DatTable::hasInserts() const
{
    for (const DatRecord &rec : records)
    {
        if (rec.lineType == ltInsert)
            return true;
    }

    return false;
}
