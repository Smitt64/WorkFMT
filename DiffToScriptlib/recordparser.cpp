#include "recordparser.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include "difftableinfo.h"
#include <QTextStream>
#include <QRegExp>
#include <QSql>
#include <rslmodule/sql/sqldatabase.h>

RecordParser::RecordParser(DiffFields* diffFields, const QStringList &realFields, QObject *parent) :
    QObject(parent),
    _diffFields(diffFields),
    _realFields(realFields)
{

}

DiffField *RecordParser::field(const QString &name)
{
    auto iter = std::find_if(_diffFields->cbegin(), _diffFields->cend(),
                                                   [name](const DiffField *fld) -> bool
    {
        if (!fld->name.compare(name, Qt::CaseInsensitive))
            return true;

        return false;
    });

    if (iter != _diffFields->cend())
        return *iter;

    return nullptr;
}

bool RecordParser::parseRecord(QString line)
{
    _errors.clear();
    QTextStream is(&line);
    QString c;
    _values.clear();
    int readCnt = 0;

    for (int i = 0; i < _realFields.count(); ++i)
    {
        DiffField *fld = field(_realFields[i]);

        if (!fld)
        {
            // Поле есть в DAT, но отсутствует в описании таблицы — пропускаем
            // значение, чтобы не было падения и смещения колонок.
            QString value;
            parseValue(is, value);
            _values.push_back(value);

            if (!is.atEnd() && getToken(is) == ",")
                is.read(1);

            readCnt++;
            continue;
        }

        if (fld->isValid())
        {
            QString value;
            bool success = true;
            if (fld->isString)
            {
                if ((success = parseString(is, value)))
                    _values.push_back(value);
                else
                    _values.push_back(QChar(1));
            }
            else
            {
                if ((success = parseValue(is, value)))
                    _values.push_back(value);
            }

            c = getToken(is);
            if (!is.atEnd() && getToken(is) != ",")
                success = false;

            if (success == false)
                _errors.append("\tError. Field number = " + QString::number(i + 1) + ", name = " + fld->name);

            is.read(1);
            readCnt++;
        }
        else
        {
            if (fld->isDate())
                _values.push_back(QString("TO_DATE('01.01.0001', 'dd.mm.yyyy')"));
            else if (fld->isString)
                _values.push_back(QString("CHR(1)"));
            else
                _values.push_back(QString("0"));
        }
    }

    if (!is.atEnd())
    {
        is.readLineInto(&line);        
        _errors.append(QString("\tFields read count = %1. Error before symbols: %2").arg(QString::number(readCnt), line));
        return false;
    }

    return true;
}

QString RecordParser::getToken(QTextStream &is)
{
    qint64 pos = is.pos();
    QString s;

    s = is.read(1);
    is.seek(pos);
    return s;
}

void readChars(QTextStream& is, QString& value)
{
    QString c;
    qint64 pos = is.pos(); // pos медленная функция, поэтому лучше позицию отслеживать самостоятельно.

    while (!is.atEnd())
    {
        c = is.read(1);

        if (c != "'")
        {
            value += c;
            ++pos;
        }
        else
        {
            QString c2 = is.read(1);
            if (c2 == "'")
            {
                value += c + c2;
                pos += 2;
            }
            else
            {
                is.seek(pos);
                return;
            }
        }
    }
}

bool RecordParser::parseString(QTextStream& is, QString& value)
{
    QString c = getToken(is);
    if (c == QChar(0) || c == QChar(1) || c == QChar(2)) //CHR(1) и CHR(2) - тоже строка
    {
        value = is.read(1);
        return true;
    }

    if (getToken(is) != "'")
        return false;

    is.read(1);
    value += "'";

    readChars(is, value);

    if (getToken(is) != "'")
        return false;

    is.read(1);
    value += "'";

    return true;
}

bool RecordParser::parseValue(QTextStream &is, QString &value)
{
    QString c;
    qint64 pos = is.pos();

    while (!is.atEnd())
    {
        c = is.read(1);
        if (c == ",")
        {
            is.seek(pos);
            return true;
        }
        value += c;
        ++pos;
    }
    return true;
}

// ---------------------------------------------------------------------------

QString diffGetTypeStringForSqlite(const quint16 &type)
{
    QString fldtype;

    switch(type)
    {
    case fmtt_INT:
    case fmtt_LONG:
    case fmtt_BIGINT:
        fldtype = "INTEGER";
        break;
    case fmtt_FLOAT:
    case fmtt_DOUBLE:
        fldtype = "REAL";
        break;
    case fmtt_MONEY:
    case fmtt_NUMERIC:
        fldtype = "NUMERIC";
        break;
    default:
        fldtype = "TEXT";
        break;
    }

    return fldtype;
}

// ---------------------------------------------------------------------------

QString diffCreateTableForSqlite(DiffTable *table)
{
    QString sql;
    QTextStream ss(&sql);
    ss << "CREATE TABLE " << table->name.toUpper() << " (" << Qt::endl;

    bool fFirst = true;
    for (const QString &fld : qAsConst(table->realFields))
    {
        DiffField *dfld = table->field(fld);

        if (!fFirst)
            ss << "," << Qt::endl;

        ss << fld.toUpper() << " ";

        if (dfld)
            ss << diffGetTypeStringForSqlite(dfld->type);
        else
            ss << "TEXT";

        fFirst = false;
    }

    ss << ")";

    return sql;
}

QString diffCreateChangesTableForSqlite(DiffTable *table)
{
    QString sql;
    QTextStream ss(&sql);
    ss << "CREATE TABLE " << table->name.toUpper() << "_CHANGE (" << Qt::endl;

    ss << "t_id__ INTEGER PRIMARY KEY AUTOINCREMENT";

    for (const QString &fld : qAsConst(table->realFields))
    {
        DiffField *dfld = table->field(fld);
        ss << "," << Qt::endl << fld.toUpper() << " ";

        if (dfld)
            ss << diffGetTypeStringForSqlite(dfld->type);
        else
            ss << "TEXT";
    }

    ss << "," << Qt::endl << "t_change__ TEXT";
    ss << ")";

    return sql;
}

namespace {

static QString prepareValueForSqlite(const QString &rawValue, DiffField *field)
{
    if (!field || !field->isString)
        return rawValue.isEmpty() ? QString("0") : rawValue;

    QString value = rawValue;

    if (value == QChar(1) || value == QChar(2))
        return QString();

    if (value.endsWith("'"))
        value.chop(1);

    if (value.startsWith("'"))
        value = value.mid(1);

    if (value == QChar(1) || value == QChar(2))
        return QString();

    // DAT использует '' для экранирования апострофа, как и SQL:
    // раскрываем парные апострофы в одиночный символ.
    value.replace("''", "'");
    return value;
}

static int calculateBatchSize(int columnCount)
{
    const int maxSqliteVariables = 900; // SQLITE_MAX_VARIABLE_NUMBER обычно 999
    if (columnCount <= 0)
        return 1;
    return qBound(1, maxSqliteVariables / columnCount, 500);
}

static bool execBatchInsert(QSqlDatabase &Connection, const QString &tableName,
                            const QStringList &fields, const QVector<QVariantList> &columnValues)
{
    if (columnValues.isEmpty() || columnValues.first().isEmpty())
        return true;

    QStringList placeholders;
    for (int i = 0; i < fields.size(); ++i)
        placeholders.append("?");

    const QString oneRow = "(" + placeholders.join(",") + ")";
    QString sql = QString("INSERT INTO %1(%2) VALUES %3")
            .arg(tableName)
            .arg(fields.join(","))
            .arg(oneRow);

    QSqlQuery query(Connection);
    query.prepare(sql);

    for (int i = 0; i < fields.size(); ++i)
        query.bindValue(i, QVariant(columnValues[i]));

    return query.execBatch();
}

} // namespace

// ---------------------------------------------------------------------------

bool diffLoadChangesToSqlite(QSqlDatabase &Connection, DiffTable *table)
{
    QSqlQuery query(Connection);
    query.prepare(QString("DROP TABLE IF EXISTS %1_CHANGE").arg(table->name));
    ExecuteQuery(&query);

    QString createTableSql = diffCreateChangesTableForSqlite(table);
    query.prepare(createTableSql);

    if (ExecuteQuery(&query) != 0)
        return false;

    const QStringList &fields = table->realFields;
    const int colCount = fields.size();
    const int batchSize = calculateBatchSize(colCount + 1); // + t_change__

    QStringList allFields = fields;
    allFields.append("t_change__");

    QVector<QVariantList> columnValues(allFields.size());

    for (int recno = 0; recno < table->records.count(); ++recno)
    {
        DatRecord *rec = table->records[recno];
        QStringList values = rec->values;

        switch (rec->lineType)
        {
        case ltInsert:
            values.append("I");
            break;
        case ltDelete:
            values.append("D");
            break;
        case ltUpdate:
            values.append(rec->lineUpdateType == lutOld ? "O" : "N");
            break;
        }

        for (int i = 0; i < colCount; ++i)
        {
            DiffField *field = table->field(fields[i]);
            columnValues[i].append(prepareValueForSqlite(values[i], field));
        }

        columnValues.last().append(prepareValueForSqlite(values.last(), nullptr));

        if (columnValues.first().size() >= batchSize)
        {
            if (!execBatchInsert(Connection, table->name + "_CHANGE", allFields, columnValues))
                return false;

            for (QVariantList &list : columnValues)
                list.clear();
        }
    }

    return execBatchInsert(Connection, table->name + "_CHANGE", allFields, columnValues);
}

bool diffLoadDatToSqlite(const QString &filename, QSqlDatabase &Connection, DiffTable *table, bool changes)
{
    QSqlQuery query(Connection);
    query.prepare(QString("DROP TABLE IF EXISTS %1").arg(table->name));
    ExecuteQuery(&query);

    QString createTableSql = diffCreateTableForSqlite(table);
    query.prepare(createTableSql);

    if (ExecuteQuery(&query) != 0)
        return false;

    for (DatIndex *index : table->indexes)
    {
        QString indexsql = "CREATE ";

        if (index->isUnique)
            indexsql += "UNIQUE ";

        indexsql += "INDEX " + index->name + " ON " + table->name + "(" + index->fields.fields().join(",") + ")";

        QSqlQuery indexquery(Connection);
        indexquery.prepare(indexsql);
        ExecuteQuery(&indexquery);
    }

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    if (!Connection.transaction())
        return false;

    const QStringList &fields = table->realFields;
    const int colCount = fields.size();
    const int batchSize = calculateBatchSize(colCount);

    QVector<QVariantList> columnValues(colCount);

    QScopedPointer<RecordParser> parser(new RecordParser(&table->fields, table->realFields));

    bool isDataSection = false;
    QTextStream stream(&f);
    stream.setCodec("IBM 866");

    while (!stream.atEnd())
    {
        QString line = stream.readLine();

        if (!isDataSection)
        {
            if (line.contains("BEGINDATA", Qt::CaseInsensitive))
                isDataSection = true;

            continue;
        }

        RecordParser *ptrParser = parser.data();
        if (!ptrParser->parseRecord(line))
            continue;

        const QStringList values = parser->getValues();
        for (int i = 0; i < colCount; ++i)
        {
            DiffField *field = table->field(fields[i]);
            columnValues[i].append(prepareValueForSqlite(values[i], field));
        }

        if (columnValues.first().size() >= batchSize)
        {
            if (!execBatchInsert(Connection, table->name, fields, columnValues))
            {
                Connection.rollback();
                return false;
            }

            for (QVariantList &list : columnValues)
                list.clear();
        }
    }

    if (!execBatchInsert(Connection, table->name, fields, columnValues))
    {
        Connection.rollback();
        return false;
    }

    if (changes && !diffLoadChangesToSqlite(Connection, table))
    {
        Connection.rollback();
        return false;
    }

    if (!Connection.commit())
    {
        Connection.rollback();
        return false;
    }

    return true;
}

bool diffLoadDatToSqlite(const QString &filename, SqlDatabase *Connection, DiffTable *table, bool changes)
{
    if (!Connection || !Connection->isOpen())
        return false;

    return diffLoadDatToSqlite(filename, Connection->database(), table, changes);
}
