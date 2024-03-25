#include "dattableinfo.h"
#include "fmtcore.h"
#include "fmtfield.h"
#include "difflogging.h"
#include "fmtindex.h"
#include "fmtsegment.h"

DatTableInfo::DatTableInfo()
{

}

QString BlobTypeToString(int type)
{
    switch (type)
    {
    case 0:
        return "BT_BLOB_NO";
    case 1:
        return "BT_BLOB_VAR";
    case 2:
        return "BT_BLOB_STREAM";
    case 3:
        return "BT_CLOB";
    }

    throw std::runtime_error(QObject::tr("Неизвестное значение типа блоба: %1")
                             .arg(type).toLocal8Bit().data());
}


void DatTableInfo::loadFromFmt(FmtTable *fmtTable)
{   
    FmtInit();
    fields.clear();
    name = fmtTable->name();
    qCInfo(logDatTable) << "Start load from fmt. Table name = " << name;
    for (int i = 0; i < fmtTable->fieldsCount(); ++i)
    {
        FmtField* fld = fmtTable->field(i);
        bool isString = (fld->isString() || fld->type() == fmtt_CHR);

        DiffField df;
        df.name = fld->name();
        df.type = fld->type();
        df.typeName = fmtTypeNameForType(fld->type());
        df.isAutoinc = fld->isAutoInc();
        df.isString = isString;

        fields.append(df);
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
}
