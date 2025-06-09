#include "linesparsermain.h"
#include "difflogging.h"
#include <qloggingcategory.h>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QXmlStreamWriter>

QString lineTypeToString(LineType type)
{
    switch (type) {
        case ltInsert: return "ltInsert";
        case ltDelete: return "ltDelete";
        case ltUpdate: return "ltUpdate";
        case ltTable:  return "ltTable";
        case ltLast:   return "ltLast";
        default:       return "Unknown";
    }

    return QString();
}

QString lineUpdateTypeToString(LineUpdateType type)
{
    switch (type)
    {
        case lutNone: return "lutNone";
        case lutOld:  return "lutOld";
        case lutNew:  return "lutNew";
        case lutLast: return "lutLast";
        default:      return "Unknown";
    }

    return QString();
}

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
    //QString s;
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

bool LinesParserMain::parseTableName(QTextStream &is)
{
    _lines.clear();

    if (!_linesTableParser.isNull() && lookToken(is, _linesTableParser->getToken()))
        _linesTableParser->parseLines(is, _lines);


    return true;
}

bool LinesParserMain::parseDoc(QTextStream &is, ScriptTable *dt)
{
    int cnt = 0;
    QString line;
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
        else if (!_linesInsertParser.isNull() && token == _linesInsertParser->getToken())
        {
            qCInfo(logLinesParserMain) << "Start parse lines for insert";
            _linesInsertParser->parseLines(is, _lines, dt);
        }
        else if (!_linesDeleteParser.isNull() && token == _linesDeleteParser->getToken())
        {
            qCInfo(logLinesParserMain) << "Start parse lines for delete";
            _linesDeleteParser->parseLines(is, _lines, dt);
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
    for (const ParsedLine& line: qAsConst(_lines))
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
    for (const ParsedLine& line: qAsConst(_lines))
    {
        for (const LineType& t: types)
        {
            if (line.lineType == t)
                lines.append(line.value);
        }
    }
    return lines;
}

int LinesParserMain::linesCount(std::initializer_list<LineType> types) const
{
    int cnt = 0;
    for (const ParsedLine& line: _lines)
    {
        for (const LineType& t: types)
        {
            if (line.lineType == t)
                cnt++;
        }
    }
    return cnt;
}

ParsedLines LinesParserMain::getParsedLines(std::initializer_list<LineType> types) const
{
    ParsedLines lines;
    for (auto it = _lines.cbegin(), end = _lines.cend(); it != end; ++it)
        for (const LineType& t: types)
            if (it.value().lineType == t)
                lines.insert(it.key(), it.value());
    return lines;
}

void LinesParserMain::serializeLines(QTextStream& stream, const QString& format) const
{
    if (format.toLower() == "json")
        serializeToJson(stream);
    else if (format.toLower() == "xml")
        serializeToXml(stream);
    else
        stream << "Unsupported format\n";
}

void LinesParserMain::serializeToJson(QTextStream& stream) const
{
    QJsonArray jsonArray;

    for (const ParsedLine& line : _lines)
    {
        QJsonObject jsonObject;
        jsonObject["value"] = line.value;
        jsonObject["lineType"] = lineTypeToString(line.lineType); // Преобразуем enum в строку
        jsonObject["lineUpdateType"] = lineUpdateTypeToString(line.lineUpdateType); // Преобразуем enum в строку
        jsonArray.append(jsonObject);
    }

    QJsonDocument jsonDocument(jsonArray);
    stream << jsonDocument.toJson(QJsonDocument::Compact); // Записываем JSON в поток
}

void LinesParserMain::serializeToXml(QTextStream& stream) const
{
    QXmlStreamWriter xmlWriter(stream.device()); // Используем QTextStream для записи XML
    xmlWriter.setCodec(stream.codec());

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("lines");

    for (const ParsedLine& line : _lines)
    {
        xmlWriter.writeStartElement("line");
        xmlWriter.writeAttribute("value", line.value);
        xmlWriter.writeAttribute("lineType", lineTypeToString(line.lineType)); // Преобразуем enum в строку
        xmlWriter.writeAttribute("lineUpdateType", lineUpdateTypeToString(line.lineUpdateType)); // Преобразуем enum в строку
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
}
