#ifndef LINESPARESER_H
#define LINESPARESER_H

#include <QTextStream>

#include "diffcore.h"
#include "scripttable.h"
#include "DiffToScriptl_global.h"

class DIFFTOSCRIPTL_EXPORT LinesParser
{
public:
    LinesParser(QString token);
    QString getToken() const {return _token;}
    virtual void parseLines(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL);
protected:
    QString _token;
    virtual bool parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) = 0;
};

class DIFFTOSCRIPTL_EXPORT LinesInsertParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) override;
};

class DIFFTOSCRIPTL_EXPORT LinesUpdateParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    const QString _tokenPlus = "+";
    bool parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) override {return false;};
    void parseLines(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) override;
};

class DIFFTOSCRIPTL_EXPORT LinesDeleteParser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) override;
};

class DIFFTOSCRIPTL_EXPORT LinesTablePareser: public LinesParser
{
public:
    using LinesParser::LinesParser;
protected:
    bool parseLine(QTextStream& is, ParsedLines& lines, ScriptTable *dt = NULL) override;
};


#endif // LINESPARESER_H
