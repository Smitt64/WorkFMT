#include "recordparser.h"
#include "fmtfield.h"
#include "fmtcore.h"
#include <QRegExp>

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

        if (fld->isValid())
        {
            QString value;
            bool success = true;
            if (fld->isString)
            {
                if ((success = parseString(is, value)))
                    _values.push_back(value);
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
