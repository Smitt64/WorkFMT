#ifndef LOGHIGHLIGHTER_H
#define LOGHIGHLIGHTER_H

#include <QObject>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QSyntaxHighlighter>

class LogHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    LogHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat rsexpFormat, separatorFormat, errorFormat;
};

#endif // LOGHIGHLIGHTER_H
