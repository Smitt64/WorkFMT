#include "linesparsermain.h"
#include "difflogging.h"

#include <qloggingcategory.h>


LinesParserMain::LinesParserMain()
{

}


bool LinesParserMain::lookToken(QTextStream& is, const QString& token, qint64 pos = -1)
{
    if (pos == -1)
        pos = is.pos();

    QString s;
    s = is.read(token.count());
    is.seek(pos);
    if (s == token)
        return true;

    return false;
}

QString LinesParserMain::lookToken(QTextStream& is)
{
    qint64 pos = is.pos();
    QString s;
    QString token;

    token = "+++";
    if(lookToken(is, token, pos))
        return token;

    token = "---";
    if(lookToken(is, token, pos))
        return token;

    token = "+";
    if(lookToken(is, token, pos))
        return token;

    token = "-";
    if(lookToken(is, token, pos))
        return token;

    token = "Index: ";
    if(lookToken(is, token, pos))
        return token;

    return "";
}


bool LinesParserMain::parseDoc(QTextStream &is)
{
    int cnt = 0;
    QString line;

    _lines.clear();

    if (!_linesTableParser.isNull() && lookToken(is, _linesTableParser->getToken()))
        _linesTableParser->parseLines(is, _lines);

    QString token;
    while (!is.atEnd())
    {
        token = lookToken(is);
        qCInfo(logLinesParserMain) << logLinesParserMain().categoryName() <<  "Token = " << token;
        if (token == "+++" || token == "---" || token == "" || token == "\\")
        {
            is.readLineInto(&line); // skip line
            qCInfo(logLinesParserMain()) << "Parse line skipped";
        }
        else if (!_linesUpdateParser.isNull() && token == _linesUpdateParser->getToken())
        {
            qCInfo(logLinesParserMain) << "Start parse lines for update";
            _linesUpdateParser->parseLines(is, _lines);
        }
        else if (!_linesInsertParser.isNull() && token == _linesInsertParser->getToken())
        {
            qCInfo(logLinesParserMain) << "Start parse lines for insert";
            _linesInsertParser->parseLines(is, _lines);
        }
        else if (!_linesDeleteParser.isNull() && token == _linesDeleteParser->getToken())
        {
            qCInfo(logLinesParserMain) << "Start parse lines for delete";
            _linesDeleteParser->parseLines(is, _lines);
        }
        else if (token == "Index: ") // Началась следующая таблица
        {
            qCInfo(logLinesParserMain) << "End parse table";
            break;
        }
        else
        {
            is.readLineInto(&line); // skip line
            qCInfo(logLinesParserMain) << "Parse line skipped";
        }

        cnt++;
    }

    QVector<int> typesCnt(ltLast);
    for (const ParsedLine& line: _lines)
        typesCnt[line.lineType]++;

    qCInfo(logLinesParserMain) << "Parsed lines: "
                               << "insert =" << typesCnt[ltInsert]
                               << "delete =" << typesCnt[ltDelete]
                               << "update =" << typesCnt[ltUpdate]
                                  ;

    return true;
}

QStringList LinesParserMain::getLines(std::initializer_list<LineType> types)
{
    QStringList lines;
    for (const ParsedLine& line: _lines)
        for (const LineType& t: types)
            if (line.lineType == t)
                lines.append(line.value);
    return lines;
}

int LinesParserMain::linesCount(std::initializer_list<LineType> types) const
{
    int cnt = 0;
    for (const ParsedLine& line: _lines)
        for (const LineType& t: types)
            if (line.lineType == t)
                cnt++;
    return cnt;
}

ParsedLines LinesParserMain::getParsedLines(std::initializer_list<LineType> types) const
{
    ParsedLines lines;
    for (const ParsedLine& line: _lines)
        for (const LineType& t: types)
            if (line.lineType == t)
                lines.append(line);
    return lines;
}
