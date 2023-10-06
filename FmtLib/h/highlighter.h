#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "fmtcore.h"
#include <QHash>
#include <QTextCharFormat>

#define FormatKeyword "keyword"
#define FormatKeyword2 "keyword2"
#define FormatComment "comment"
#define FormatDefault "plain"
#define FormatFunction "function"
#define FormatType "type"
#define FormatNumber "number"
#define FormatStrig "string"
#define FormatEnum "enum"
#define FormatDefine "define"

class QTextDocument;

typedef struct FmtGenHighlightingRule
{
    QRegularExpression pattern;
    QString alias;
}FmtGenHighlightingRule;
typedef QList<FmtGenHighlightingRule> FmtGenHighlightingRuleList;

class FMTLIBSHARED_EXPORT HighlighterStyle
{
public:
    class StyleItem
    {
    public:
        StyleItem();
        void load(const QString &filename);

        QTextCharFormat format(const QString &alias);
        bool hasFormat(const QString &alias) const;

        const QColor &editorBackground() const;
        const QColor &editorCurrentLine() const;
        const QColor &editorCurrentWord() const;

    private:
        QMap<QString, QTextCharFormat> m_Formats;

        QColor m_EditorBackground, m_EditorCurLine, m_EditorCurWord;
    };

    static HighlighterStyle *inst();
    QSharedPointer<StyleItem> style(const QString &name = QString()) const;

    QStringList themes() const;
    void setDefaultTheme(const QString &theme);
    QString defaultTheme() const;

private:

    HighlighterStyle();
    static HighlighterStyle *m_Inst;

    void loadStyles();
    QMap<QString, QSharedPointer<StyleItem>> m_Styles;
    QStringList m_Themes;
    QString m_DefaultTheme;
};

typedef QSharedPointer<HighlighterStyle::StyleItem> StyleItemPtr;

class FMTLIBSHARED_EXPORT Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    typedef struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
        bool isNotCaseInsensitive;
    }HighlightingRule;

    enum HighlighterCode
    {
        HC_CPP = 1,
        HC_JS,
        HC_SQL
    };
    Highlighter(QTextDocument *parent = Q_NULLPTR);
    Highlighter(const HighlighterCode &type, QTextDocument *parent = Q_NULLPTR);
    virtual ~Highlighter();

    void addRsType(const QString &name);
    void addRsEnums(const QString &name);
    void addRsDefine(const QString &name);
    void addHighlightingRule(const HighlightingRule &name);
    void addHighlightingRules(const FmtGenHighlightingRuleList &ruleList);

    void setStyle(const QString &style);

    void reset();

protected:
    void highlightBlock(const QString &text);

private:
    void init(const HighlighterCode &type);
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;

    HighlighterCode m_Type;
    QSharedPointer<HighlighterStyle::StyleItem> m_Style;
    QString m_StyleName;
};

typedef QList<Highlighter::HighlightingRule> HighlightingRuleList;

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

    QTextCharFormat tableNameFormat;
};

#endif // HIGHLIGHTER_H
