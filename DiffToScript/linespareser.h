#ifndef LINESPARESER_H
#define LINESPARESER_H

#include <QTextStream>

#include "diffcore.h"

class LinesParser
{
public:
    LinesParser(QString token);
    QString getToken() const {return _token;}
    virtual void parseLines(QTextStream& is, ParsedLines& lines);
protected:
    QString _token;
    virtual bool parseLine(QTextStream& is, ParsedLines& lines) = 0;
};


class LinesInsertParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines) override;
};


class LinesUpdateParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    const QString _tokenPlus = "+";
    bool parseLine(QTextStream& is, ParsedLines& lines) override {return false;};
    void parseLines(QTextStream& is, ParsedLines& lines) override;
};


class LinesDeleteParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines) override;
};



class LinesTablePareser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines) override;
};


#endif // LINESPARESER_H
