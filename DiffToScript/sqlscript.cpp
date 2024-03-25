#include "sqlscript.h"
#include "difflogging.h"


SqlScript::SqlScript(DatTable *datTable, DbSpelling* dbSpelling, WhereScript* whereScript, QObject *parent) : QObject(parent)
  , _datTable(datTable)
  , _dbSpelling(dbSpelling)
  , _autoIncProcess(new AutoIncProcess(datTable))
  , _whereScript(whereScript)
{

}

DatRecords::iterator SqlScript::buildInsert(QTextStream &os, const IndexFields& fields, DatRecords::iterator first, DatRecords::iterator last)
{
    QString fieldsJoin;
    if (fields.count())
        fieldsJoin = fields[0].name;
    for (int i = 1; i < fields.count(); ++i)
        fieldsJoin += ", " + fields[i].name;

    for (;first != last; ++first)
    {
        if (first->lineType != ltInsert)
            break;        
        os << QString("INSERT INTO %1 (%2) VALUES(%3);").arg(_datTable->name, fieldsJoin, first->values.join(", ")) << Qt::endl;
    }
    return first;
    //TODO DATE SPELL
}

DatRecords::iterator SqlScript::buildDelete(QTextStream &os, const IndexFields& fields, DatRecords::iterator first, DatRecords::iterator last)
{
    for (;first != last; ++first)
    {
        if (first->lineType != ltDelete)
            break;
        os << QString("DELETE FROM %1 WHERE %2;").arg(_datTable->name, _whereScript->buildWhere(fields, first->values)) << Qt::endl;
    }
    return first;
}

DatRecords::iterator SqlScript::buildUpdate(QTextStream &os, const IndexFields &fields, DatRecords::iterator first, DatRecords::iterator last)
{
    DatRecords::iterator lastUpdate = first;
    while (lastUpdate != last && lastUpdate->lineType == ltUpdate)
        ++lastUpdate;

    DatRecords::iterator mid = first + (lastUpdate - first) / 2;
    for (;first != mid; ++first)
    {
        DatRecords::iterator itOldValues = first;
        DatRecords::iterator itNewValues = first + (mid - first);

        QStringList setList;
        for (int i = 0; i < fields.count(); ++i)
            if (itNewValues->values[i] != itOldValues->values[i])
                setList.append(fields[i].name + " = " + itNewValues->values[i]);

        if (first->lineType != ltUpdate)
            break;
        os << QString("UPDATE %1 SET %2 WHERE %3;").arg(_datTable->name, setList.join(", "), _whereScript->buildWhere(fields, itOldValues->values)) << Qt::endl;
    }

    return lastUpdate;
}

void SqlScript::buildScript(QTextStream &os)
{
    if (_datTable->records.count() == 0)
        return;

    IndexFields colunms = getIndexFieldColumns(_datTable->fields);
//    _autoIncProcess->excludeAutoinc(colunms);

    for(DatRecord& rec: _datTable->records)
    {
        _dbSpelling->setSpelling(_datTable->fields, rec.values);
        _autoIncProcess->excludeAutoinc(rec.values);
    }

    DatRecords::iterator first = _datTable->records.begin();
    DatRecords::iterator last = _datTable->records.end();

    while (first != last)
    {
        switch (first->lineType)
        {
        case ltUpdate:
            first = buildUpdate(os, colunms, first, last);
            break;
        case ltInsert:
            first = buildInsert(os, colunms, first, last);
            break;
        case ltDelete:
            first = buildDelete(os, colunms, first, last);
            break;
        }
    }
}

//Устанавливает класс, который будет отвечать за обработку автоинкрементных полей
void SqlScript::setAutoIncProcess(AutoIncProcess* autoIncProcess)
{
    //Для старого вызывает деструктор и устонавлявает новый
    _autoIncProcess.reset(autoIncProcess);
}

IndexFields SqlScript::getIndexFieldColumns(const DiffFields& fields) const
{
    IndexFields colunms;
    for (DiffField f: fields)
    {        
        colunms.append({f.name, f.isAutoinc});
    }

    return colunms;
}

bool SqlScript::checkRecord(const DiffFields& fields, const DatRecord &record, QTextStream& os) const
{
    if (fields.count() != record.values.count())
    {
        os << QString("Error. fields count = %1, values count = %2. Values: %3").arg(
                  QString::number(_datTable->fields.count()),
                  QString::number(record.values.count()),
                  _datTable->records[0].values.join(", ")
              ) << Qt::endl;
        return false;
    }
    return true;
}
