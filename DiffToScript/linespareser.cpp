#include "linespareser.h"
#include <algorithm>
#include <difflogging.h>
#include "recordparser.h"


LinesParser::LinesParser(QString token)
    :_token(token)
{

}

void LinesParser::parseLines(QTextStream& is, ParsedLines& lines, ScriptTable *dt)
{
    while (parseLine(is, lines, dt));
}


bool LinesInsertParser::parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt)
{
    QString s = is.read(_token.count());

    while (s == "\\" && !is.atEnd())
    {
        is.readLine();
        s = is.read(_token.count());
    }

    if (s == _token)
    {
        RecordParser recParser(&dt->fields, dt->realFields);
        auto parsedLine = ParsedLine{is.readLine(), ltInsert};
        if(recParser.parseRecord(parsedLine.value))
        {
            QStringList key;
            foreach(const DiffField &uf, dt->uniqFields)
            {
                key.append(recParser.getValues()[dt->fields.indexByFieldName(uf.name)]);
            }
            key.append("-"); //сначала находим удаление для определения операции обновления
            if(lines.contains(key))
            {
                lines[key].lineType = ltUpdate;
                lines[key].lineUpdateType = lutOld;

                parsedLine.lineType = ltUpdate;
                parsedLine.lineUpdateType = lutNew;
            }
            key.last() = "+";
            lines.insert(key, parsedLine);

            qCInfo(logLinesParser) << "Added line for insert: " << parsedLine.value;
        }
        else
        {
            qCWarning(logLinesParser) << "Error parseRecord for line: " << parsedLine.value;

        }
        return true;
    }
    qCInfo(logLinesParser) << "Wrong token: " << s;
    is.seek(is.pos() - _token.count());
    return false;
}

bool LinesDeleteParser::parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt)
{
    QString s = is.read(_token.count());
    if (s == _token)
    {
        RecordParser recParser(&dt->fields, dt->realFields);
        auto parsedLine = ParsedLine{is.readLine(), ltDelete};
        if(recParser.parseRecord(parsedLine.value))
        {
            QStringList key;
            foreach(const DiffField &uf, dt->uniqFields)
            {
                key.append(recParser.getValues()[dt->fields.indexByFieldName(uf.name)]);
            }
            key.append("+"); //сначала находим вставку для определения операции обновления
            if(lines.contains(key))
            {
                lines[key].lineType = ltUpdate;
                lines[key].lineUpdateType = lutNew;

                parsedLine.lineType = ltUpdate;
                parsedLine.lineUpdateType = lutOld;
            }
            key.last() = "-";
            lines.insert(key, parsedLine);

            qCInfo(logLinesParser) << "Added line for insert: " << parsedLine.value;
        }
        else
        {
            qCWarning(logLinesParser) << "Error parseRecord for line: " << parsedLine.value;
        }

        return true;
    }

    qCInfo(logLinesParser) << "Wrong token: " << s;
    is.seek(is.pos() - _token.count());
    return false;
}

void LinesUpdateParser::parseLines(QTextStream &is, ParsedLines &lines, ScriptTable *dt)
{
    /*int delCnt = lines.count();
    QScopedPointer<LinesParser> delParser(new LinesDeleteParser("-"));
    delParser->parseLines(is, lines);
    delCnt = lines.count() - delCnt;

    if (!lines.count())
        return;

    int midIndex = lines.count() - 1;

    int insCnt = lines.count();
    QScopedPointer<LinesParser> insParser(new LinesInsertParser("+"));
    insParser->parseLines(is, lines);
    insCnt = lines.count() - insCnt;

    int minCnt = std::min(delCnt, insCnt);
    for (int i = 0; i < minCnt; ++i)
    {
        lines[midIndex - i].lineType= ltUpdate;
        lines[midIndex - i].lineUpdateType = lutOld;
        qInfo(logLinesParser) << "Line changed for update: " << lines[midIndex - i].value;
        lines[midIndex + i + 1].lineType = ltUpdate;
        lines[midIndex + i + 1].lineUpdateType = lutNew;
        qInfo(logLinesParser) << "Line changed for update: " << lines[midIndex + i + 1].value;
    }*/
}


bool LinesTablePareser::parseLine(QTextStream &is, ParsedLines &lines, ScriptTable *dt)
{
    QString token = is.read(_token.count());
    if (token != _token)
    {
        is.seek(is.pos() - token.count());
        qInfo(logLinesParser) << "Wrong token = " << token;
        return false;
    }

    QString line = is.readLine();
    line = line.split("/").last().toLower();

    int pos = line.indexOf(".dat");
    if (pos == -1)
    {
        qInfo(logLinesParser) << "Wrong line for table: " << line;
        return false;
    }

    line = line.mid(0, pos);
    QStringList key;
    key.append("t");
    auto parsedLine = ParsedLine{line, ltTable};
    lines.insert(key, parsedLine);
    if(dt)
    {
        dt->name = parsedLine.value;
    }
    qCInfo(logLinesParser) << "Added line for table name: " << lines[key].value;
    return true;
}
