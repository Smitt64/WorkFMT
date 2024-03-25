#include "difffield.h"
#include "fmtcore.h"

bool operator==(const IndexField& a, const IndexField& b)
{
    return (a.name == b.name && a.isAutoinc == b.isAutoinc);
}

bool operator!=(const IndexField& a, const IndexField& b)
{
    return !(a == b);
}


bool operator==(const IndexFields& a, const IndexFields& b)
{
    if (a.count() != b.count())
        return false;

    for (int i = 0; i < a.count(); ++i)
        if (a[i] != b[i])
            return false;

    return true;
}


bool operator==(const DatIndex& a, const DatIndex& b)
{
    bool bb[3];
    bb[0] = a.fields == b.fields;
    bb[1] = a.isUnique == b.isUnique;
    bb[2] = a.name == b.name;

    return (bb[0] && bb[1] && bb[2]);
}

//DatIndex &DatIndex::operator=(const DatIndex &a)
//{
//    isUnique = a.isUnique;
//    name = a.name;
//    for(QString s: a.fields)
//        fields.append(s);
//    return *this;
//}

//DatIndex &DatIndex::operator=(const DatIndex &a)
//{
//    isUnique = a.isUnique;
//    name = a.name;
//    fields = a.fields;
//}

int IndexFields::indexByName(const QString& name) const
{
    for (int i = 0; i < count(); ++i)
        if ((*this)[i].name.toLower() == name.toLower())
            return i;
    return -1;
}

int DiffFields::indexByFieldName(QString name) const
{
    for (int i = 0; i < count(); ++i)
        if ((*this)[i].name.toLower() == name.toLower())
            return i;
    return -1;
}

DiffField& DiffFields::fieldByName(const QString &name)
{
    for (int i = 0; i < count(); ++i)
        if ((*this)[i].name.toLower() == name.toLower())
            return (*this)[i];
    Q_ASSERT_X(false, "DiffFields::fieldByName", "Name not found");
}

bool DatIndex::hasAutoinc() const
{
    for (const IndexField& field: fields)
    {
        if (field.isAutoinc)
            return true;
    }
    return false;
}

bool DiffField::isDate() const
{
    return (type == fmtt_DATE || type == fmtt_DATETIME);
}






