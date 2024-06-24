#include "linespareser.h"
#include <algorithm>
#include <difflogging.h>


LinesParser::LinesParser(QString token)
    :_token(token)
{

}

void LinesParser::parseLines(QTextStream& is, ParsedLines& lines)
{
    while (parseLine(is, lines));
}


bool LinesInsertParser::parseLine(QTextStream& is, ParsedLines& lines)
{
    QString s = is.read(_token.count());

    while (s == "\\" && !is.atEnd())
    {
        is.readLine();
        s = is.read(_token.count());
    }

    if (s == _token)
    {        
        lines.append(ParsedLine{is.readLine(), ltInsert});
        qCInfo(logLinesParser) << "Added line for insert: " << lines.back().value;
        return true;
    }
    qCInfo(logLinesParser) << "Wrong token: " << s;
    is.seek(is.pos() - _token.count());
    return false;
}

bool LinesDeleteParser::parseLine(QTextStream& is, ParsedLines& lines)
{
    QString s = is.read(_token.count());
    if (s == _token)
    {
        lines.append({is.readLine(), ltDelete});
        qCInfo(logLinesParser) << "Added line for delete: " << lines.back().value;
        return true;
    }
    qCInfo(logLinesParser) << "Wrong token: " << s;
    is.seek(is.pos() - _token.count());
    return false;
}

void LinesUpdateParser::parseLines(QTextStream &is, ParsedLines &lines)
{
    int delCnt = lines.count();
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
    }
}


bool LinesTablePareser::parseLine(QTextStream &is, ParsedLines &lines)
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
    lines.append({line, ltTable});
    qCInfo(logLinesParser) << "Added line for table name: " << lines.back().value;
    return true;
}
