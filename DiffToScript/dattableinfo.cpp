#include "dattableinfo.h"
#include "fmtcore.h"
#include "toolsruntime.h"
#include "fmtfield.h"
#include "difflogging.h"
#include "fmtindex.h"
#include "fmtsegment.h"
#include <QtSql>

DatTableInfo::DatTableInfo()
{

}

DiffField DatTableInfo::field(const QString &name) const
{
    DiffFields::const_iterator iter = std::find_if(fields.cbegin(), fields.cend(), [=](const DiffField &fld) -> bool
    {
        return fld.name.toUpper() == name.toUpper();
    });

    if (iter != fields.cend())
        return *iter;

    return DiffField();
}

void DatTableInfo::loadFromFmt(FmtTable *fmtTable)
{   
    FmtInit();
    fields.clear();
    name = fmtTable->name().toUpper();
    qCInfo(logDatTable) << "Start load from fmt. Table name = " << name;
    for (int i = 0; i < fmtTable->fieldsCount(); ++i)
    {
        FmtField* fld = fmtTable->field(i);
        bool isString = (fld->isString() || fld->type() == fmtt_CHR);

        DiffField df;
        df.name = fld->name().toUpper();
        df.type = fld->type();
        df.typeName = fmtTypeNameForType(fld->type());
        df.isAutoinc = fld->isAutoInc();
        df.isString = isString;

        fields.append(df);
        realFields.append(df.name.toUpper());
        qCInfo(logDatTable) << "Loaded FMT field: " << df.name << " " << df.typeName << " " << (df.isAutoinc?"autoinc":"");
    }
    if (fmtTable->blobLen() > 0)
    {
        DiffField df = { "T_FMTBLOBDATA_XXXX", fmtTable->blobType(), BlobTypeToString(fmtTable->blobType()), false, true};
        fields.append(df);
        qCInfo(logDatTable) << "Loaded FMT field: " << df.name << " " << df.typeName << " " << (df.isAutoinc?"autoinc":"");
    }

    for (int i = 0; i < fmtTable->indecesCount(); ++i)
    {
        FmtIndex* indx = fmtTable->tableIndex(i);
        DatIndexes::iterator it = indexes.insert(indexes.end(), DatIndex{});
        it->name = indx->name();
        it->isUnique = indx->isUnique();

        qCInfo(logDatTable) << "Loaded FMT index: " << it->name << " isUnique " << it->isUnique;

        for (int j = 0; j < indx->segmentsCount(); ++j)
        {
            FmtSegment* sgmt = indx->segment(j);
            it->fields.append({sgmt->field()->name(), sgmt->field()->isAutoInc()});
            qCInfo(logDatTable) << "Added field for index: name = " << sgmt->field()->name() << " isAutoInc = " << sgmt->field()->isAutoInc();
        }
    }
    qCInfo(logDatTable) << "End load from fmt. Table name = " << name;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QUuid::createUuid().toString());
    QString infoFile = toolFullFileNameFromDir("datstruct.info");
    db.setDatabaseName(infoFile);

    if (db.open())
    {
        QSqlQuery q(db);
        if (q.prepare("SELECT column FROM DAT_FIELDS where upper(NAME) = :val order by id"))
        {
            q.bindValue(":val", name);

            if (q.exec())
            {
                bool isFirst = true;
                while(q.next())
                {
                    if (isFirst)
                    {
                        realFields.clear();
                        isFirst = false;
                    }

                    QString fld = q.value(0).toString();
                    qCInfo(logDatTable) << "Loaded dat struc field" << fld;

                    realFields.append(fld);
                }
            }
            else
            {
                QString err = q.lastError().text();
                qCWarning(logDatTable) << "Can't find dat struct info for table" << name << q.lastError().text();
            }
        }
        else
        {
            QString err = q.lastError().text();
            qCWarning(logDatTable) << "Can't find dat struct info for table" << name << q.lastError().text();
        }

        db.close();
    }
    else
        qCWarning(logDatTable) << "Can't open dat struct info file: datstruct.info";
}
