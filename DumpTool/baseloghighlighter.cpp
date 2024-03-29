#include "baseloghighlighter.h"

BaseLogHighlighter::BaseLogHighlighter(QTextDocument *parent) :
    QSyntaxHighlighter(parent)
{
    succeededFormat.setForeground(Qt::darkGreen);
    succeededFormat.setFontWeight(QFont::Bold);

    markFormat.setForeground(Qt::black);
    markFormat.setFontWeight(QFont::Bold);

    errorFormat.setForeground(Qt::darkRed);
    errorFormat.setFontWeight(QFont::Bold);
}

BaseLogHighlighter::~BaseLogHighlighter()
{

}

void BaseLogHighlighter::initStdOraPatterns()
{
    HighlightingRule rule;

    const QString succeedePatterns[] = {
        "Grant succeeded.",
        "User created.",
        "User altered.",
        "User dropped.",
        "PL/SQL procedure successfully completed."
    };

    for (const QString &pattern : succeedePatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = succeededFormat;
        highlightingRules.append(rule);
    }

    rule.pattern = QRegularExpression("((old)|(new))\\s*\\d+[:]");
    rule.format = markFormat;
    highlightingRules.append(rule);

    rule.pattern = QRegularExpression("&&\\d+");
    highlightingRules.append(rule);

    addMarkItem("Last Successful login time:");
    addMarkItem("SQL[*]Plus[^\n]*");
    addMarkItem("Connected to:[^\n]*");
    addMarkItem("Disconnected from [^\n]*");
    addMarkItem("Processing object type");
    addMarkItem(". . imported");
    addMarkItem(". . exported");
    addMarkItem("(\\bKB\\b)|(\\bMB\\b)|(\\bGB\\b)|(\\brows\\b)");

    addErrorItem("ERROR at line[^\n]*");
    addErrorItem("ORA-[^\n]*");
    addErrorItem("Failing sql is:");
}

void BaseLogHighlighter::addMarkItem(const QString &pattern)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = markFormat;
    highlightingRules.append(rule);
}

void BaseLogHighlighter::addErrorItem(const QString &pattern)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = errorFormat;
    highlightingRules.append(rule);
}

void BaseLogHighlighter::addRule(const QString &pattern, const QTextCharFormat &format)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(pattern);
    rule.format = format;
    highlightingRules.append(rule);
}

void BaseLogHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : std::as_const(highlightingRules))
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    if (text.contains("Job") && text.contains("completed"))
        setFormat(0, text.length(), succeededFormat);

    if (text.contains("successfully loaded/unloaded"))
        setFormat(0, text.length(), succeededFormat);
}
