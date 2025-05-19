#include "diffcore.h"

DatRecord::DatRecord() :
    QObject()
{
}

DatRecord::DatRecord(const DatRecord& other)
    : QObject(other.parent()),
      values(other.values),
      lineType(other.lineType),
      lineUpdateType(other.lineUpdateType)
{
}

DatRecord::DatRecord(const QStringList& vals,
                     LineType lType,
                     LineUpdateType luType,
                     QObject* parent) :
    QObject()
{
    values = vals;
    lineType = lType;
    lineUpdateType = luType;
}

DatRecord& DatRecord::operator=(const DatRecord& other)
{
    if (this != &other)
    {
        setParent(other.parent());
        values = other.values;
        lineType = other.lineType;
        lineUpdateType = other.lineUpdateType;
    }

    return *this;
}

int DatRecord::size() const
{
    return values.size();
}

const QStringList &DatRecord::getValues() const
{
    return values;
}

int DatRecord::getLineType() const
{
    return lineType;
}

int DatRecord::getLineUpdateType() const
{
    return lineUpdateType;
}

QString DatRecord::lineTypeStr() const
{
    switch (lineType)
    {
    case ltDelete:
        return "Delete";
    case ltInsert:
        return "Insert";
    case ltUpdate:
        return "Update";
    default:
        return "";
    }
}

QString DatRecord::lineUpdateTypeStr() const
{
    switch (lineUpdateType)
    {
    case lutNew:
        return "New";
    case lutOld:
        return "Old";
    case lutNone:
        return "Nan";
    default:
        return "";
    }
}

void DatRecord::setVal(const int &index, const QString &value)
{
    values[index] = value;
}

// ------------------------------------------------------------------------

DatRecords::DatRecords() :
    VectorIterableObject<DatRecord*>()
{

}

DatRecords::DatRecords(const DatRecords& other) :
    VectorIterableObject<DatRecord*>()
{
}

DatRecords& DatRecords::operator=(const DatRecords& other)
{
    if (this != &other) // Проверка на самоприсваивание
    {
        QObject::setParent(other.parent());
         VectorIterableObject<DatRecord*>::operator=(other);
    }

    return *this;
}

const DatRecord *DatRecords::record(int index) const
{
    if (index < 0 || index >= size())
        return nullptr;

    const DatRecord *rec = QVector<DatRecord*>::at(m_currentIndex);
    return rec;
}

void DatRecords::append(const QStringList& values, LineType lineType, LineUpdateType updateType)
{
    QVector<DatRecord*>::append(new DatRecord(values, lineType, updateType));
}

QVector<QStringList> DatRecords::getRecords(std::initializer_list<LineType> types)
{
    QVector<QStringList> v;
    for (const DatRecord* rec: (*this))
    {
        for (const LineType& type: types)
        {
            if (type == rec->lineType)
                v.append(rec->values);
        }
    }
    return v;
}

QVariantList DatRecords::getRecords(const QVariantList &types)
{
    QVariantList v;
    for (const DatRecord *rec: (*this))
    {
        for (const QVariant& type: types)
        {
            if (type.toInt() == rec->lineType)
                v.append(QVariant::fromValue(rec->values));
        }
    }
    return v;
}
