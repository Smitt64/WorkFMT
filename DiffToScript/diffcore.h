#ifndef DIFFCORE_H
#define DIFFCORE_H

#include <QObject>
#include <QString>
#include <QPair>
#include <QMap>
#include <QStringList>
#include <QVariant>
#include "rslobj/iterableobjectbase.h"

enum LineType
{
    ltInsert,
    ltDelete,
    ltUpdate,
    ltTable,
    ltLast
};

enum LineUpdateType
{
    lutNone,
    lutOld,
    lutNew,
    lutLast
};

struct ParsedLine
{
    QString value;
    LineType lineType;
    LineUpdateType lineUpdateType = lutNone;
};

using ParsedLines = QMap<QStringList, ParsedLine>;

class DatRecord : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int size READ size CONSTANT)
    Q_PROPERTY(int lineType READ getLineType CONSTANT)
    Q_PROPERTY(int lineUpdateType READ getLineUpdateType CONSTANT)
    Q_PROPERTY(QString lineTypeStr READ lineTypeStr CONSTANT)
    Q_PROPERTY(QString lineUpdateTypeStr READ lineUpdateTypeStr CONSTANT)
    Q_PROPERTY(QStringList values READ getValues CONSTANT)
public:
    DatRecord();
    DatRecord(const DatRecord& other);
    DatRecord(const QStringList& vals,
              LineType lType,
              LineUpdateType luType = lutNone,
              QObject* parent = nullptr);

    DatRecord& operator=(const DatRecord& other);

    int size() const;
    const QStringList &getValues() const;
    int getLineType() const;
    int getLineUpdateType() const;
    QString lineTypeStr() const;
    QString lineUpdateTypeStr() const;

    QStringList values;
    LineType lineType;
    LineUpdateType lineUpdateType = lutNone;
};

class DatRecords : public VectorIterableObject<DatRecord*>
{
    Q_OBJECT
public:
    DatRecords();
    DatRecords(const DatRecords& other);
    DatRecords& operator=(const DatRecords& other);

    // Получить запись по индексу
    Q_INVOKABLE const DatRecord *record(int index) const;

    void append(const QStringList& values, LineType lineType, LineUpdateType updateType = lutNone);

    QVector<QStringList> getRecords(std::initializer_list<LineType> types);
    Q_INVOKABLE QVariantList getRecords(const QVariantList &types);
};

Q_DECLARE_OPAQUE_POINTER(DatRecord);
Q_DECLARE_OPAQUE_POINTER(DatRecords);

#endif // DIFFCORE_H
