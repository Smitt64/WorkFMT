#ifndef DIFFFIELD_H
#define DIFFFIELD_H

#include <QString>
#include <QList>
#include <QException>


class ExceptionOutOfRange: public QException
{
public:
    void raise() const override { throw *this; }
    ExceptionOutOfRange *clone() const override { return new ExceptionOutOfRange(*this); }
};

struct IndexField
{
    QString name;
    bool isAutoinc;
    qint16 type;

    bool isString;
//    IndexField(const QString& name, bool isAutoinc):name(name),isAutoinc(isAutoinc){}
};
bool operator==(const IndexField& a, const IndexField& b);
bool operator!=(const IndexField& a, const IndexField& b);

struct IndexFields : QList<IndexField>{
    int indexByName(const QString& name) const;
};

bool operator==(const IndexFields& a, const IndexFields& b);


struct DiffField: IndexField
{
    qint16 type;
    QString typeName;
    int size;
    bool isString;
    bool isDate() const;
    bool isBlob() const;
    DiffField(){}
    DiffField(const QString& name, qint16 type, const QString& typeName, bool isAutoinc = false, bool isString = false)
        : IndexField{name, isAutoinc}, type(type), typeName(typeName), isString(isString){}
};

struct DiffFields: public QList<DiffField>
{
    using QList<DiffField>::QList;
    int indexByFieldName(QString name) const;
    DiffField& fieldByName(const QString& name);
};


struct DatIndex
{
    QString name = "";
    IndexFields fields;

    bool isUnique = false;
    bool hasAutoinc() const;
//    DatIndex& operator=(const DatIndex& a);
//    DatIndex():name(""),isUnique(false){}
};

bool operator==(const DatIndex& a, const DatIndex& b);

using DatIndexes = QList<DatIndex>;

#endif // DIFFFIELD_H
