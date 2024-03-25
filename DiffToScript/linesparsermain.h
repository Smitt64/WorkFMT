#ifndef LINESPARSERMAIN_H
#define LINESPARSERMAIN_H

#include "linespareser.h"
#include "diffcore.h"

#include <QScopedPointer>
#include <QTextStream>

class LinesParserMain
{
public:
    LinesParserMain();
    bool parseDoc(QTextStream &is);
    void setTableParser(LinesParser* linesParser) {_linesTableParser.reset(linesParser);};
    void setInsertParser(LinesParser* linesParser) {_linesInsertParser.reset(linesParser);};
    void setDeleteParser(LinesParser* linesParser) {_linesDeleteParser.reset(linesParser);};
    void setUpdateParser(LinesParser* linesParser) {_linesUpdateParser.reset(linesParser);};
    QStringList getLines(std::initializer_list<LineType> types);
    const ParsedLines &getParsedLines() const {return _lines;};
    int linesCount(std::initializer_list<LineType> types) const;
    ParsedLines getParsedLines(std::initializer_list<LineType> types) const;
private:
    ParsedLines _lines;
    QString _tableName;
    QScopedPointer<LinesParser> _linesTableParser;
    QScopedPointer<LinesParser> _linesUpdateParser;
    QScopedPointer<LinesParser> _linesDeleteParser;
    QScopedPointer<LinesParser> _linesInsertParser;
    bool lookToken(QTextStream& is, const QString& token, qint64 pos);
    QString lookToken(QTextStream& is);
};

#endif // LINESPARSERMAIN_H
