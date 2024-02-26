#include "highlighter.h"
#include "loggingcategories.h"
#include "fmtcore.h"
#include <QtGui>
#include <QDebug>
#include <QDir>
#include "highlighter.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QFile>
#include <QRegExp>
#include <QApplication>
#include <QStyle>

#define NO_ELEMENT_TEMPLATE QString("Element \"%1\" not exists")

void CheckColorFormat(const QString &hex, const QString &field)
{
    QRegExp rx(HEX_COLOR_REGEX);

    if (!rx.exactMatch(hex))
        throw QString("Invalid \"%1\" value").arg(field);
}

HighlighterStyle::StyleItem::StyleItem()
{
    m_EditorBackground = QApplication::palette().color(QPalette::Active, QPalette::Background);
}

QTextCharFormat HighlighterStyle::StyleItem::format(const QString &alias)
{
    if (m_Formats.contains(alias))
        return m_Formats[alias];

    return QTextCharFormat();
}

bool HighlighterStyle::StyleItem::hasFormat(const QString &alias) const
{
    return m_Formats.contains(alias);
}

const QColor &HighlighterStyle::StyleItem::editorBackground() const
{
    return m_EditorBackground;
}

const QColor &HighlighterStyle::StyleItem::editorCurrentLine() const
{
    return m_EditorCurLine;
}

const QColor &HighlighterStyle::StyleItem::editorCurrentWord() const
{
    return m_EditorCurWord;
}

void HighlighterStyle::StyleItem::load(const QString &filename)
{
    QFile f(filename);
    f.open(QIODevice::ReadOnly);

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &error);

    if (error.error != QJsonParseError::NoError)
        throw error;

    QJsonObject obj = doc.object();

    if (!obj.contains("formats"))
        throw (NO_ELEMENT_TEMPLATE).arg("formats");

    if (obj.contains("editor.background"))
    {
        QString hex = obj["editor.background"].toString();
        CheckColorFormat(hex, "editor.background");
        m_EditorBackground = QColor(hex);
    }

    if (obj.contains("editor.selection.line"))
    {
        QString hex = obj["editor.selection.line"].toString();
        CheckColorFormat(hex, "editor.selection.line");
        m_EditorCurLine = QColor(hex);
    }

    if (obj.contains("editor.selection.word"))
    {
        QString hex = obj["editor.selection.word"].toString();
        CheckColorFormat(hex, "editor.selection.word");
        m_EditorCurWord = QColor(hex);
    }

    QJsonArray formats = obj["formats"].toArray();

    for (auto format : formats)
    {
        QJsonObject elem = format.toObject();

        if (!elem.contains("alias"))
            throw (NO_ELEMENT_TEMPLATE).arg("alias");

        QString Alias = elem["alias"].toString();

        QTextCharFormat textFormat;
        if (elem.contains("fontFamily"))
            textFormat.setFontFamily(elem["fontFamily"].toString());

        if (elem.contains("fontFixedPitch"))
            textFormat.setFontFixedPitch(elem["fontFixedPitch"].toBool());

        if (elem.contains("fontItalic"))
            textFormat.setFontItalic(elem["fontItalic"].toBool());

        if (elem.contains("fontKerning"))
            textFormat.setFontKerning(elem["fontKerning"].toBool());

        if (elem.contains("fontLetterSpacing"))
            textFormat.setFontLetterSpacing(elem["fontLetterSpacing"].toInt());

        if (elem.contains("fontOverline"))
            textFormat.setFontOverline(elem["fontOverline"].toBool());

        if (elem.contains("fontPointSize"))
            textFormat.setFontPointSize(elem["fontPointSize"].toInt());

        if (elem.contains("fontStretch"))
            textFormat.setFontStretch(elem["fontStretch"].toInt());

        if (elem.contains("fontStrikeOut"))
            textFormat.setFontStrikeOut(elem["fontStrikeOut"].toBool());

        if (elem.contains("fontUnderline"))
            textFormat.setFontUnderline(elem["fontUnderline"].toBool());

        if (elem.contains("fontWeight"))
        {
            int fontWeight = QFont::Normal;
            QString cons = elem["fontWeight"].toString();

            if (cons == "Thin")
                fontWeight = QFont::Thin;
            else if (cons == "ExtraLight")
                fontWeight = QFont::ExtraLight;
            else if (cons == "Light")
                fontWeight = QFont::Light;
            else if (cons == "Normal")
                fontWeight = QFont::Normal;
            else if (cons == "Medium")
                fontWeight = QFont::Medium;
            else if (cons == "DemiBold")
                fontWeight = QFont::DemiBold;
            else if (cons == "Bold")
                fontWeight = QFont::Bold;
            else if (cons == "ExtraBold")
                fontWeight = QFont::ExtraBold;
            else if (cons == "ExtraBold")
                fontWeight = QFont::Black;
            else
            {
                if (!elem["fontWeight"].isDouble())
                    throw QString("Invalid \"fontWeight\" value");
                else
                    fontWeight = elem["fontWeight"].toInt();
            }

            textFormat.setFontWeight(fontWeight);
        }

        if (elem.contains("fontWordSpacing"))
            textFormat.setFontWordSpacing(elem["fontWordSpacing"].toInt());

        if (elem.contains("foreground"))
        {
            QString hex = elem["foreground"].toString();
            CheckColorFormat(hex, "foreground");
            textFormat.setForeground(QColor(hex));
        }

        if (elem.contains("background"))
        {
            QString hex = elem["background"].toString();
            CheckColorFormat(hex, "background");
            textFormat.setBackground(QColor(hex));
        }

        if (elem.contains("underlineColor"))
        {
            QString hex = elem["underlineColor"].toString();
            CheckColorFormat(hex, "underlineColor");
            textFormat.setUnderlineColor(QColor(hex));
        }

        if (elem.contains("textOutline"))
        {
            QString hex = elem["textOutline"].toString();
            CheckColorFormat(hex, "textOutline");
            textFormat.setTextOutline(QColor(hex));
        }

        if (elem.contains("underlineStyle"))
        {
            QTextCharFormat::UnderlineStyle underlineStyle = QTextCharFormat::NoUnderline;
            QString cons = elem["underlineStyle"].toString();

            if (cons == "NoUnderline")
                underlineStyle = QTextCharFormat::NoUnderline;
            else if (cons == "SingleUnderline")
                underlineStyle = QTextCharFormat::SingleUnderline;
            else if (cons == "DashUnderline")
                underlineStyle = QTextCharFormat::DashUnderline;
            else if (cons == "DotLine")
                underlineStyle = QTextCharFormat::DotLine;
            else if (cons == "DashDotLine")
                underlineStyle = QTextCharFormat::DashDotLine;
            else if (cons == "DashDotDotLine")
                underlineStyle = QTextCharFormat::DashDotDotLine;
            else if (cons == "WaveUnderline")
                underlineStyle = QTextCharFormat::WaveUnderline;
            else if (cons == "SpellCheckUnderline")
                underlineStyle = QTextCharFormat::SpellCheckUnderline;
            else
                throw QString("Invalid \"underlineStyle\" value");

            textFormat.setUnderlineStyle(underlineStyle);
        }

        textFormat.setFontStyleHint(QFont::Courier);
        m_Formats.insert(Alias, textFormat);
    }

    f.close();
}

// ---------------------------------------------------------------

HighlighterStyle *HighlighterStyle::m_Inst = nullptr;
HighlighterStyle::HighlighterStyle()
{
    m_DefaultTheme = settings()->value("editor.theme", "Default").toString();
}

HighlighterStyle *HighlighterStyle::inst()
{
    if (!m_Inst)
    {
        m_Inst = new HighlighterStyle();
        m_Inst->loadStyles();
    }

    return m_Inst;
}

QSharedPointer<HighlighterStyle::StyleItem> HighlighterStyle::style(const QString &name) const
{
    if (name.isEmpty())
        return style(m_DefaultTheme); // Default

    if (!m_Styles.contains(name))
        return nullptr;

    return m_Styles[name];
}

void HighlighterStyle::loadStyles()
{
    bool hr = true;
    QDir syntaxhighlighter = QDir::currentPath();

    qInfo(logCore()) << "Try detect syntaxhighlighter folder";
    if (!syntaxhighlighter.cd("syntaxhighlighter"))
    {
        qWarning(logCore()) << QString("Working directory %1: false").arg(syntaxhighlighter.path());
        syntaxhighlighter = QDir(qApp->applicationDirPath());

        if (!syntaxhighlighter.cd("syntaxhighlighter"))
        {
            hr = false;
            qWarning(logCore()) << QString("Application directory %1: false").arg(syntaxhighlighter.path());
        }
        else
            qInfo(logCore()) << QString("Syntaxhighlighter folder: %1").arg(syntaxhighlighter.path());
    }
    else
        qInfo(logCore()) << QString("Syntaxhighlighter folder: %1").arg(syntaxhighlighter.path());

    if (hr)
    {
        QFileInfoList lst = syntaxhighlighter.entryInfoList(QStringList() << "*.json");

        for (auto fi : lst)
        {
            QString name = fi.baseName();

            try {
                qInfo(logCore()) << "Try load highlighter style:" << name;

                QSharedPointer<StyleItem> item(new StyleItem());
                item->load(fi.absoluteFilePath());
                m_Styles.insert(name, item);

                m_Themes.append(name);
                qInfo(logCore()) << "Highlighter style successfully loaded";
            }  catch (QJsonParseError e) {
                qWarning(logCore()) << QString("Can't load highlighter style [%1]: %2")
                                       .arg(name, e.errorString());
            }
            catch (QString e) {
                qWarning(logCore()) << QString("Can't load highlighter style [%1]: %2")
                                       .arg(name, e);
            }
        }
    }
    else
    {
        qWarning(logCore()) << QString("Syntaxhighlighter folder not found... Used default scheme...");

        QSharedPointer<StyleItem> item(new StyleItem());
        item->load(":/../syntaxhighlighter/Default.json");
        m_Styles.insert("Default", item);

        m_Themes.append("Default");
    }
}

QStringList HighlighterStyle::themes() const
{
    return m_Themes;
}

void HighlighterStyle::setDefaultTheme(const QString &theme)
{
    m_DefaultTheme = theme;
    settings()->setValue("editor.theme", theme);
    settings()->sync();
}

QString HighlighterStyle::defaultTheme() const
{
    return m_DefaultTheme;
}

// ---------------------------------------------------

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

    m_StyleName = HighlighterStyle::inst()->defaultTheme();
}

Highlighter::~Highlighter()
{

}

void Highlighter::setStyle(const QString &style)
{
    if (style.isEmpty())
        m_Style = HighlighterStyle::inst()->style(m_StyleName);
    else
    {
        m_StyleName = style;
        m_Style = HighlighterStyle::inst()->style(style);
    }
}

void Highlighter::reset()
{
    highlightingRules.clear();

    HighlightingRule rule;
    QStringList keywordPatterns, keywordPatterns2;

    if (m_Type == Highlighter::HC_CPP)
    {
        keywordPatterns2
                << "\\bbreak\\b" << "\\bcase\\b" << "\\bcatch\\b" << "\\bcontinue\\b"
                << "\\bdefault\\b"<< "\\bdo\\b"<< "\\belse\\b" << "\\bfor\\b" << "\\bgoto\\b"
                << "\\bif\\b" << "\\breturn\\b" << "\\btry\\b" << "\\bwhile\\b";

        keywordPatterns << "\\balignas\\b" << "\\balignof\\b" << "\\b__asm\\b" << "\\bauto\\b"
                        << "\\bbool\\b"<< "\\bchar\\b" << "\\bchar16_t\\b" << "\\bchar32_t\\b"
                        << "\\bclass\\b" << "\\bconst\\b"
                        << "\\bconstexpr\\b" << "\\bconst_cast\\b"  << "\\bdecltype\\b"
                        << "\\bdelete\\b"  << "\\bdouble\\b" << "\\bdynamic_cast\\b"  << "\\benum\\b"
                        << "\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b" << "\\bfloat\\b"
                        << "\\bfriend\\b"  << "\\binline\\b" << "\\bint\\b" << "\\blong\\b"
                        << "\\bmutable\\b" << "\\bnamespace\\b" << "\\bnew\\b" << "\\bnoexcept\\b" << "\\bnot\\b" << "\\bnot_eq\\b"
                        << "\\bnullptr\\b" << "\\boperator\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                        << "\\bpublic\\b" << "\\bregister\\b" << "\\breinterpret_cast\\b"  << "\\bshort\\b"
                        << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_assert\\b" << "\\bstatic_cast\\b"
                        << "\\bstruct\\b" << "\\bswitch\\b" << "\\btemplate\\b" << "\\bthis\\b" << "\\bthread_local\\b" << "\\bthrow\\b"
                        << "\\btrue\\b"  << "\\btypedef\\b" << "\\btypeid\\b" << "\\btypename\\b" << "\\bunion\\b"
                        << "\\bunsigned\\b" << "\\busing\\b" << "\\bvirtual\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b";

        QFile cpp_rstypes(":/cpp_rstypes.txt");
        cpp_rstypes.open(QIODevice::ReadOnly);

        while(!cpp_rstypes.atEnd())
        {
            QString pattern = cpp_rstypes.readLine().trimmed();

            addRsType(pattern);
        }
        cpp_rstypes.close();

        QFile cpp_rsenums(":/cpp_rsenums.txt");
        cpp_rsenums.open(QIODevice::ReadOnly);

        while(!cpp_rsenums.atEnd())
        {
            QString pattern = cpp_rsenums.readLine().trimmed();

            addRsEnums(pattern);
        }
        cpp_rsenums.close();
    }
    else if (m_Type == Highlighter::HC_JS)
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
    else if (m_Type == Highlighter::HC_SQL)
    {
        keywordPatterns << "\\bVARCHAR2\\b"
                        << "\\bNVARCHAR2\\b"
                        << "\\bVARCHAR\\b"
                        << "\\bNUMBER\\b"
                        << "\\bLONG\\b"
                        << "\\bDATE\\b"
                        << "\\bCHAR\\b";

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
                    keywordPatterns.append(QString("\\b%1\\b").arg(pattern));
            }
        } while (!line.isNull());
    }

    StyleItemPtr style = HighlighterStyle::inst()->style();
    rule.pattern = QRegularExpression("([a-zA-Z_][a-zA-Z0-9_]+)\\s*(?=\\()");
    rule.format = style->format(FormatFunction);
    highlightingRules.append(rule);

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = style->format(FormatKeyword);
        highlightingRules.append(rule);
        rule.isNotCaseInsensitive = true;
    }

    if (m_Type == Highlighter::HC_CPP)
    {
        foreach (const QString &pattern, keywordPatterns2) {
            rule.pattern = QRegularExpression(pattern);
            rule.format = style->format(FormatKeyword2);
            highlightingRules.append(rule);
            rule.isNotCaseInsensitive = true;
        }

        QFile cpp_rsdefines(":/cpp_rsdefines.txt");
        cpp_rsdefines.open(QIODevice::ReadOnly);

        while(!cpp_rsdefines.atEnd())
        {
            QString pattern = cpp_rsdefines.readLine().trimmed();

            addRsDefine(pattern);
        }
        cpp_rsdefines.close();
    }

    rule.pattern = QRegularExpression("((?<![\\\\])['\"])((?:.(?!(?<![\\\\])\\1))*.?)\\1"); // \\\".*\\\"
    rule.format = style->format(FormatStrig);
    highlightingRules.append(rule);

    if (style->hasFormat(FormatNumber))
    {
        rule.pattern = QRegularExpression("\\b[\\-\\+]?([0-9]+(\\.[0-9]+)?)\\b");
        rule.format = style->format(FormatNumber);
        highlightingRules.append(rule);
    }

    singleLineCommentFormat = style->format(FormatComment);
    multiLineCommentFormat = style->format(FormatComment);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

#define PrintParam(n) qDebug() << #n << ":" << keywordFormat.n();
void Highlighter::init(const HighlighterCode &type)
{
    m_Style = HighlighterStyle::inst()->style();

    m_Type = type;
    reset();
}

void Highlighter::highlightBlock(const QString &text)
{
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

    HighlightingRule singleLineCommentRule;
    if (m_Type != Highlighter::HC_SQL)
    {
        singleLineCommentRule.pattern = QRegularExpression("//[^\\n]*");
        singleLineCommentRule.format = m_Style->format(FormatComment);
    }
    else
    {
        singleLineCommentRule.pattern = QRegularExpression("--[^\\n]*");
        singleLineCommentRule.format = m_Style->format(FormatComment);
    }

    QRegularExpression singleLineCommentExpression(singleLineCommentRule.pattern);
    singleLineCommentExpression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch singleLineCommentMatch = singleLineCommentExpression.match(text);

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

void Highlighter::addRsType(const QString &name)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(name));
    rule.format = m_Style->format(FormatType);
    highlightingRules.append(rule);
}

void Highlighter::addRsEnums(const QString &name)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(name));
    rule.format = m_Style->format(FormatEnum);
    highlightingRules.append(rule);
}

void Highlighter::addRsDefine(const QString &name)
{
    HighlightingRule rule;
    rule.pattern = QRegularExpression(QString("\\b%1\\b").arg(name));
    rule.format = m_Style->format(FormatDefine);
    rule.isNotCaseInsensitive = true;
    highlightingRules.append(rule);
}

void Highlighter::addHighlightingRule(const Highlighter::HighlightingRule &rule)
{
    highlightingRules.prepend(rule);
}

void Highlighter::addHighlightingRules(const FmtGenHighlightingRuleList &ruleList)
{
    for (auto &rule : ruleList)
        highlightingRules.prepend({rule.pattern, m_Style->format(rule.alias)});
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

    //\'([A-Za-z0-9]|(\'{2,2})|[!@#$%^&*()-_ \[\}{}<>\?№;:"])*\'
    tableNameFormat.setForeground(Qt::darkMagenta);
    tableNameFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\bD[A-Za-z0-9_]+\\_\\w{3}\\b");
    rule.format = tableNameFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\\'([A-Za-z0-9]|(\\'{2,2})|[!@#$%^&*()-_ \\[\\}{}<>\\?\u2116;:\"])*\\'");
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
