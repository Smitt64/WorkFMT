#ifndef DIFFFIELD_H
#define DIFFFIELD_H

#include <QObject>
#include <QString>
#include <QList>
#include <QException>
#include <limits>

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
};
bool operator==(const IndexField& a, const IndexField& b);
bool operator!=(const IndexField& a, const IndexField& b);

struct IndexFields : QList<IndexField>{
    int indexByName(const QString& name) const;
};

bool operator==(const IndexFields& a, const IndexFields& b);


class DiffField: public QObject, public IndexField
{
    Q_OBJECT
    // Свойства для полей IndexField
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(bool isAutoinc READ getIsAutoinc CONSTANT)
    Q_PROPERTY(qint16 type READ getType CONSTANT)
    Q_PROPERTY(bool isString READ getIsString CONSTANT)

    // Свойства для собственных полей DiffField
    Q_PROPERTY(QString typeName READ getTypeName CONSTANT)
    Q_PROPERTY(int size READ getSize CONSTANT)
public:
    DiffField();
    DiffField(const QString& name, qint16 type, const QString& typeName, bool isAutoinc = false, bool isString = false);
    DiffField(const DiffField& other);

    DiffField& operator=(const DiffField& other);

    qint16 type;
    QString typeName;
    int size;
    //bool isString;
    bool isDate() const;
    bool isBlob() const;
    bool isValid() const;

    QString getName() const;
    bool getIsAutoinc() const;
    qint16 getType() const;
    bool getIsString() const;
    QString getTypeName() const;
    int getSize() const;
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

Q_DECLARE_OPAQUE_POINTER(DiffField);

#endif // DIFFFIELD_H
