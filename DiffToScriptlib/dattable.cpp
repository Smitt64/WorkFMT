#include "dattable.h"
#include "difflogging.h"
#include "linespareser.h"
#include "recordparser.h"

#include <QTextStream>


DatTable::DatTable()
{

}

bool DatTable::load(const QString& dir, const QString& tableName, DiffConnection* conn)
{
    QString fileName = dir + "/" + tableName.toUpper() + ".DAT";
    if (!QFile::exists(fileName))
    {
        qCWarning(logDatTable) << "File " << fileName << "not found";
        return false;
    }

    if (conn->getConnection() == nullptr)
    {
        qCWarning(logDatTable) << "There aren't connection for " << tableName;
        return false;
    }

    QFile inputFile(fileName);
    inputFile.open(QFile::WriteOnly);
    QTextStream os(&inputFile);

    //RecordParser recParser{};


}

bool DatTable::hasInserts() const
{
    for (const DatRecord *rec : records)
    {
        if (rec->lineType == ltInsert)
            return true;
    }

    return false;
}
