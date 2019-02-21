#include "loghighlighter.h"

LogHighlighter::LogHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    rsexpFormat.setForeground(Qt::darkBlue);
    rsexpFormat.setFontWeight(QFont::Bold);

    separatorFormat.setForeground(Qt::darkGreen);
    separatorFormat.setFontWeight(QFont::Bold);

    errorFormat.setForeground(Qt::darkRed);
    errorFormat.setFontWeight(QFont::Bold);

    rule.pattern = QRegExp("RSexp - [^\\n]*");
    rule.format = rsexpFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("(\\*)*\\s+(Start (un)*loading)\\s+(\\*)*");
    rule.format = separatorFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("ORA-\\d{5}[^\\n]*");
    rule.format = errorFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegExp("SP2-\\d{4}[^\\n]*");
    rule.format = errorFormat;
    highlightingRules.append(rule);
}

void LogHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
