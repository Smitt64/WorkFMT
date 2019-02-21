#include "highlighter.h"

#include <QtGui>
#include <QDebug>
#include "highlighter.h"

typedef QPair<qint32, qint32> Range;
typedef QList<Range> RangeList;

bool isInRange(const Range &range, const qint32 &value)
{
    return value >= range.first && value <= range.second;
}

bool IsInOffsets(const QList<qint32> &offsets, const qint32 &value)
{
    bool result = false;

    foreach(const qint32 &offset, offsets) {
        if (value > offset)
            result = true;
    }

    return result;
}

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    init(Highlighter::HC_CPP);
}

Highlighter::Highlighter(const HighlighterCode &type, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    init(type);
}

void Highlighter::init(const HighlighterCode &type)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

    if (type == Highlighter::HC_CPP)
    {
        keywordPatterns << "\\balignas\\b" << "\\balignof\\b" << "\\band\\b" << "\\band_eq\\b" << "\\basm\\b" << "\\bauto\\b"
                        << "\\bbitand\\b" << "\\bbitor\\b" << "\\bbool\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bcatch\\b"
                        << "\\bchar\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b" << "\\bclass\\b" << "\\bcompl\\b" << "\\bconst\\b"
                        << "\\bconstexpr\\b" << "\\bconst_cast\\b" << "\\bcontinue\\b" << "\\bdecltype\\b" << "\\bdefault\\b"
                        << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\bdynamic_cast\\b" << "\\belse\\b" << "\\benum\\b"
                        << "\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b" << "\\bfloat\\b" << "\\bfor\\b"
                        << "\\bfriend\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\binline\\b" << "\\bint\\b" << "\\blong\\b"
                        << "\\bmutable\\b" << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnoexcept\\b" << "\\bnot\\b" << "\\bnot_eq\\b"
                        << "\\bnullptr\\b" << "\\boperator\\b" << "\\bor\\b" << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                        << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b" << "\\breturn\\b" << "\\bshort\\b"
                        << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_assert\\b" << "\\bstatic_cast\\b"
                        << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b" << "\\bthis\\b" << "\\bthread_local\\b" << "\\bthrow\\b"
                        << "\\btrue\\b" << "\\btry\\b" << "\\btypedef\\b" << "\\btypeid\\b" << "\\btypename\\b" << "\\bunion\\b"
                        << "\\bunsigned\\b" << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b"
                        << "\\bwhile\\b" << "\\bxor\\b" << "\\bxor_eq\\b";
    }
    else if (type == Highlighter::HC_JS)
    {
        keywordPatterns << "\\babstract\\b"
        << "\\barguments\\b" << "\\bawait\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b" << "\\bcase\\b"
        << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b" << "\\bdebugger\\b"
        << "\\bdefault\\b" << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\beval\\b"
        << "\\bexport\\b" << "\\bextends\\b" << "\\bfalse\\b" << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b"
        << "\\bfor\\b" << "\\bfunction\\b" << "\\bgoto\\b" << "\\bif\\b" << "\\bimplements\\b" << "\\bimport\\b" << "\\bin\\b"
        << "\\binstanceof\\b" << "\\bint\\b" << "\\binterface\\b" << "\\blet\\b" << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b"
        << "\\bnull\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b" << "\\breturn\\b"
        << "\\bshort\\b" << "\\bstatic\\b" << "\\bsuper\\b" << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b"
        << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b" << "\\btrue\\b" << "\\btry\\b" << "\\btypeof\\b"
        << "\\bvar\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";
    }

    singleLineCommentFormat.setForeground(Qt::darkGreen);

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
        rule.isNotCaseInsensitive = true;
    }

    keywordFormat2.setForeground(QColor(160, 0, 160));
    keywordFormat2.setFontWeight(QFont::Bold);
    QStringList keywordPatterns2;
    keywordPatterns2 << "\\bNULL\\b";
    foreach (const QString &pattern, keywordPatterns2) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat2;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);


    multiLineCommentFormat.setForeground(Qt::darkGreen);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("((?<![\\\\])['\"])((?:.(?!(?<![\\\\])\\1))*.?)\\1"); // \\\".*\\\"
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\s*\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    /*numbersFormat.setForeground(QColor(255, 128, 0));
    rule.pattern = QRegExp("-?\\d*(\\.\\d+)?$");
    rule.format = numbersFormat;
    highlightingRules.append(rule);*/

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    HighlightingRule singleLineCommentRule;
    singleLineCommentRule.pattern = QRegularExpression("//[^\\n]*");
    singleLineCommentRule.format = singleLineCommentFormat;

    QRegularExpression singleLineCommentExpression(singleLineCommentRule.pattern);
    singleLineCommentExpression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch singleLineCommentMatch = singleLineCommentExpression.match(text);

    QList<qint32> offsets;
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpression expression(rule.pattern);

        if (!rule.isNotCaseInsensitive)
            expression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

        QRegularExpressionMatch match = expression.match(text);
        while (match.hasMatch())
        {
            int offset = match.capturedLength() + match.capturedStart();

            if (!IsInOffsets(offsets, offset))
            {
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
                match = expression.match(text, offset);
            }
        }
    }
    while (singleLineCommentMatch.hasMatch())
    {
        int offset = singleLineCommentMatch.capturedLength() + singleLineCommentMatch.capturedStart();
        setFormat(singleLineCommentMatch.capturedStart(), singleLineCommentMatch.capturedLength(), singleLineCommentRule.format);
        singleLineCommentMatch = singleLineCommentExpression.match(text, offset);
        offsets.append(offset);
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}


// ==============================================================
SqlHighlighter::SqlHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    typeFormat.setForeground(Qt::darkRed);
    typeFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bVARCHAR2\\b"
                    << "\\bNVARCHAR2\\b"
                    << "\\bVARCHAR\\b"
                    << "\\bNUMBER\\b"
                    << "\\bLONG\\b"
                    << "\\bDATE\\b"
                    << "\\bCHAR\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = typeFormat;
        highlightingRules.append(rule);
    }

    QFile f(":/sql_keys.txt");
    f.open(QIODevice::ReadOnly);
    QTextStream stream(&f);

    QString line;
    do
    {
        line = stream.readLine();
        QStringList l = line.remove(" ").split(",");

        foreach (const QString &pattern, l)
        {
            if (!pattern.isEmpty())
            {
                rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(pattern));
                rule.format = keywordFormat;
                highlightingRules.append(rule);
            }
        }
    } while (!line.isNull());

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\'.*\'");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void SqlHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpression expression(rule.pattern);
        expression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
        QRegularExpressionMatch match = expression.match(text);
        while (match.hasMatch())
        {
            int offset = match.capturedLength() + match.capturedStart();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            match = expression.match(text, offset);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
