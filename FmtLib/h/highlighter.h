#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "fmtcore.h"
#include <QHash>
#include <QTextCharFormat>

class QTextDocument;

class FMTLIBSHARED_EXPORT Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    enum HighlighterCode
    {
        HC_CPP = 1,
        HC_JS,
    };
    Highlighter(QTextDocument *parent = Q_NULLPTR);
    Highlighter(const HighlighterCode &type, QTextDocument *parent = Q_NULLPTR);

protected:
    void highlightBlock(const QString &text);

private:
    void init(const HighlighterCode &type);
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
        bool isNotCaseInsensitive;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QTextCharFormat numbersFormat;

    QTextCharFormat keywordFormat2;
};

// ===========================================
class SqlHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SqlHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat typeFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    QTextCharFormat keywordFormat2;
};

#endif // HIGHLIGHTER_H
