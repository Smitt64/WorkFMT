#include "diffparser.h"

DiffParser::DiffParser(QObject *parent) : QObject(parent)
{

}

QString DiffParser::getToken(QTextStream& is)
{
    qint64 pos = is.pos();
    QString s;

    s = is.read(3);
    is.seek(pos);
    if (s == "+++")
        return "+++";

    s = is.read(1);
    is.seek(pos);
    if (s == "+")
        return "+";

    s = is.read(3);
    is.seek(pos);
    if (s == "---")
        return "---";

    s = is.read(1);
    is.seek(pos);
    if (s == "-")
        return "-";

    return "";
}

bool DiffParser::parseNewLine(QTextStream& is)
{
    QString s = is.readLine(1);
    if (s == "+")
    {
        insList.append(is.readLine());
        return true;
    }
    is.seek(is.pos() - 1);
    return false;
}

void DiffParser::parseNewLines(QTextStream& is)
{
    while (parseNewLine(is));
}

bool DiffParser::parseOldLine(QTextStream& is)
{
    QString s = is.readLine(1);
    if (s == "-")
    {
        delList.append(is.readLine());
        return true;
    }
    is.seek(is.pos() - 1);
    return false;
}

void DiffParser::parseOldLines(QTextStream& is)
{
    while (parseOldLine(is));
}

void DiffParser::parseDoc(QTextStream& is)
{
    QTextStream os(stdout);
    int cnt = 0;

    QString line;
    QString token;
    while (!is.atEnd())
    {
        token = getToken(is);
        if (token == "+++")
            newTable = is.readLine();
        else if (token == "+")
            parseNewLines(is);
        else if (token == "---")
            oldTable = is.readLine();
        else if (token == "-")
            parseOldLines(is);
        else
            is.readLineInto(&line);

        cnt++;
    }

    os << "cnt = " << cnt << Qt::endl;

    for (QString &s: delList)
        os << s << Qt::endl;
    for (QString &s: insList)
        os << s << Qt::endl;
}
