#ifndef DIFFCORE_H
#define DIFFCORE_H

#include <QObject>
#include <QString>
#include <QPair>
#include <QMap>
#include <QStringList>

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

struct DatRecord
{
    QStringList values;
    LineType lineType;
    QString lineTypeStr() const;
    QString lineUpdateTypeStr() const;
    LineUpdateType lineUpdateType = lutNone;
};

inline QString DatRecord::lineTypeStr() const
{
    switch (lineType) {
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

inline QString DatRecord::lineUpdateTypeStr() const
{
    switch (lineUpdateType) {
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

struct DatRecords : public QVector<DatRecord>
{
    QVector<QStringList> getRecords(std::initializer_list<LineType> types);
};

inline QVector<QStringList> DatRecords::getRecords(std::initializer_list<LineType> types)
{
    QVector<QStringList> v;
    for (const DatRecord& rec: (*this))
        for (const LineType& type: types)
            if (type == rec.lineType)
            {
                v.append(rec.values);
            }
    return v;
}

#endif // DIFFCORE_H
