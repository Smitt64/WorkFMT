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
    DatRecord() : QObject() { }

    DatRecord(const DatRecord& other)
        : QObject(other.parent()),
          values(other.values),
          lineType(other.lineType),
          lineUpdateType(other.lineUpdateType)
    {
    }

    DatRecord(const QStringList& vals,
              LineType lType,
              LineUpdateType luType = lutNone,
              QObject* parent = nullptr) :
        QObject()
    {
        values = vals;
        lineType = lType;
        lineUpdateType = luType;
    }

    DatRecord& operator=(const DatRecord& other)
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

    int size() const
    {
        return values.size();
    }

    const QStringList &getValues() const
    {
        return values;
    }

    int getLineType() const
    {
        return lineType;
    }

    int getLineUpdateType() const
    {
        return lineUpdateType;
    }

    QString lineTypeStr() const
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

    QString lineUpdateTypeStr() const
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

    QStringList values;
    LineType lineType;
    LineUpdateType lineUpdateType = lutNone;
};

class DatRecords : public QObject, public QVector<DatRecord>
{
    Q_OBJECT
    Q_PROPERTY(int size READ size CONSTANT)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex)
public:
    DatRecords() :
        QObject(),
        QVector<DatRecord>()
    {

    }

    DatRecords(const DatRecords& other)
        : QObject(other.parent()),
          QVector<DatRecord>(other)
    {
    }

    DatRecords& operator=(const DatRecords& other)
    {
        if (this != &other) // Проверка на самоприсваивание
        {
            QObject::setParent(other.parent());
            QVector<DatRecord>::operator=(other);
        }

        return *this;
    }

    // Переместить итератор в начало
    Q_INVOKABLE void toFront()
    {
        m_currentIndex = -1;
    }

    // Переместить итератор в конец
    Q_INVOKABLE void toBack()
    {
        m_currentIndex = size();
    }

    // Есть ли следующий элемент
    Q_INVOKABLE bool hasNext() const
    {
        return m_currentIndex + 1 < size();
    }

    // Получить следующий элемент и переместить итератор
    Q_INVOKABLE const DatRecord *next()
    {
        if (!hasNext())
            return nullptr;

        ++m_currentIndex;
        const DatRecord &rec = QVector<DatRecord>::at(m_currentIndex);
        return &rec;
    }

    // Есть ли предыдущий элемент
    Q_INVOKABLE bool hasPrevious() const
    {
        return m_currentIndex > 0;
    }

    // Получить предыдущий элемент и переместить итератор
    Q_INVOKABLE const DatRecord *previous()
    {
        if (!hasPrevious())
            return nullptr;

        --m_currentIndex;
        const DatRecord &rec = QVector<DatRecord>::at(m_currentIndex);
        return &rec;
    }

    // Получить текущую позицию итератора
    Q_INVOKABLE int currentIndex() const
    {
        return m_currentIndex;
    }

    // Установить текущую позицию итератора
    Q_INVOKABLE void setCurrentIndex(int index)
    {
        if (index >= -1 && index < size() && index != m_currentIndex)
            m_currentIndex = index;
    }

    // Получить запись по индексу
    Q_INVOKABLE const DatRecord *record(int index) const
    {
        if (index < 0 || index >= size())
            return nullptr;

        const DatRecord &rec = QVector<DatRecord>::at(m_currentIndex);
        return &rec;
    }

    void append(const QStringList& values, LineType lineType, LineUpdateType updateType = lutNone)
    {
        QVector<DatRecord>::append(DatRecord(values, lineType, updateType));
    }

    inline QVector<QStringList> getRecords(std::initializer_list<LineType> types)
    {
        QVector<QStringList> v;
        for (const DatRecord& rec: (*this))
        {
            for (const LineType& type: types)
            {
                if (type == rec.lineType)
                    v.append(rec.values);
            }
        }
        return v;
    }

private:
    int m_currentIndex; // Текущая позиция итератора
};

#endif // DIFFCORE_H
