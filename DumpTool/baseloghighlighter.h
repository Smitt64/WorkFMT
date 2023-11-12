#ifndef BASELOGHIGHLIGHTER_H
#define BASELOGHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class BaseLogHighlighter : public QSyntaxHighlighter
{
public:
    explicit BaseLogHighlighter(QTextDocument *parent);
    virtual ~BaseLogHighlighter();

    virtual void highlightBlock(const QString &text);

    void addMarkItem(const QString &pattern);
    void addErrorItem(const QString &pattern);

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QList<HighlightingRule> highlightingRules;
    QTextCharFormat succeededFormat, markFormat, errorFormat;
};

#endif // BASELOGHIGHLIGHTER_H
