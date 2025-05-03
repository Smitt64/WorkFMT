#ifndef DIFFFIELD_H
#define DIFFFIELD_H

#include <QObject>
#include <QString>
#include <QList>
#include <QException>
#include <limits>
#include "rslobj/iterableobjectbase.h"

class ExceptionOutOfRange: public QException
{
public:
    void raise() const override { throw *this; }
    ExceptionOutOfRange *clone() const override { return new ExceptionOutOfRange(*this); }
};

class IndexField : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(bool isAutoinc READ getIsAutoinc CONSTANT)
    Q_PROPERTY(qint16 type READ getType CONSTANT)
    Q_PROPERTY(bool isString READ getIsString CONSTANT)
public:
    IndexField();
    IndexField(const QString& _name, qint16 _type, bool _isAutoinc, bool _isString);
    IndexField(const IndexField& other);
    virtual ~IndexField() = default;

    IndexField& operator=(const IndexField& other);

    QString name;
    bool isAutoinc;
    qint16 type;

    bool isString;

    QString getName() const;
    bool getIsAutoinc() const;
    qint16 getType() const;
    bool getIsString() const;
};

Q_DECLARE_OPAQUE_POINTER(IndexField);

bool operator==(const IndexField& a, const IndexField& b);
bool operator!=(const IndexField& a, const IndexField& b);

class IndexFields : public ListIterableObject<IndexField*>
{
    Q_OBJECT
public:
    Q_INVOKABLE int indexByName(const QString& name) const;
};

Q_DECLARE_OPAQUE_POINTER(IndexFields);

bool operator==(const IndexFields& a, const IndexFields& b);

class DiffField: public IndexField
{
    Q_OBJECT
    // Свойства для собственных полей DiffField
    Q_PROPERTY(QString typeName READ getTypeName CONSTANT)
    Q_PROPERTY(int size READ getSize CONSTANT)
public:
    DiffField();
    DiffField(const QString& name, qint16 type, const QString& typeName, bool isAutoinc = false, bool isString = false);
    DiffField(const DiffField& other);

    DiffField& operator=(const DiffField& other);

    QString typeName;
    int size;
    //bool isString;
    bool isDate() const;
    bool isBlob() const;
    bool isValid() const;

    QString getTypeName() const;
    int getSize() const;
};

Q_DECLARE_OPAQUE_POINTER(DiffField);

class DiffFields: public ListIterableObject<DiffField*>
{
    Q_OBJECT
public:
    virtual ~DiffFields();
    //using QList<DiffField*>::QList;
    Q_INVOKABLE int indexByFieldName(QString name) const;
    Q_INVOKABLE DiffField *fieldByName(const QString& name);
};

Q_DECLARE_OPAQUE_POINTER(DiffFields);

class DatIndex : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName CONSTANT)
    Q_PROPERTY(bool hasAutoinc READ hasAutoinc CONSTANT)
    Q_PROPERTY(bool isUnique READ IsUnique CONSTANT)
    Q_PROPERTY(IndexFields *fields READ getFields CONSTANT)
public:
    DatIndex();
    DatIndex(const QString &_name, const bool &_isUnique);
    DatIndex(const DatIndex& other);

    DatIndex& operator=(const DatIndex& other);

    QString name;
    IndexFields fields;

    bool isUnique = false;
    bool hasAutoinc() const;

    const QString &getName() const;
    const bool &IsUnique() const;
    IndexFields *getFields();
//    DatIndex& operator=(const DatIndex& a);
//    DatIndex():name(""),isUnique(false){}
};

Q_DECLARE_OPAQUE_POINTER(DatIndex);
bool operator==(const DatIndex& a, const DatIndex& b);

//using DatIndexes = QList<DatIndex>;
class DatIndexes : public ListIterableObject<DatIndex*>
{
    Q_OBJECT
public:
    DatIndexes();
    virtual ~DatIndexes();
};

Q_DECLARE_OPAQUE_POINTER(DatIndexes);

#endif // DIFFFIELD_H
