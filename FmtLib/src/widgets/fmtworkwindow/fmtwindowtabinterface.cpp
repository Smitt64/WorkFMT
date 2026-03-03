#include "fmtwindowtabinterface.h"
#include <SARibbon.h>
#include <QPlainTextEdit>
#include <QToolTip>
#include <QApplication>
#include <QClipboard>
#include <codeeditor/codehighlighter.h>
#include <QFileDialog>

FmtWindowTabInterface::FmtWindowTabInterface(QWidget *parent)
    : QMainWindow{parent}, m_pRibbon(nullptr),
    m_pParentContextCategory(nullptr)
    , m_pRibbonCategory(nullptr)
    , m_CategoryAddedToContext(false)
{

}

FmtWindowTabInterface::~FmtWindowTabInterface()
{
    removeCategoryFromParentContext();

    QList<SARibbonCategory*> lst = categoryList();

    ribbon()->hideContextCategory(m_pParentContextCategory);
    for (auto category : qAsConst(lst))
        delete category;
}

void FmtWindowTabInterface::setRibbonBar(SARibbonBar *ribbon)
{
    m_pRibbon = ribbon;
}

SARibbonBar *FmtWindowTabInterface::ribbon() const
{
    return m_pRibbon;
}

void FmtWindowTabInterface::setParentContextCategory(SARibbonContextCategory *contextCategory)
{
    m_pParentContextCategory = contextCategory;
}

void FmtWindowTabInterface::setupRibbonActions()
{

}

SARibbonContextCategory *FmtWindowTabInterface::parentContextCategory() const
{
    return m_pParentContextCategory;
}

QString FmtWindowTabInterface::ribbonCategoryName() const
{
    return metaObject()->className();
}

void FmtWindowTabInterface::setupAction(QAction *act, const QString& text, const QString& iconname)
{
    act->setText(text);
    act->setIcon(QIcon::fromTheme(iconname));
    act->setObjectName(text);
}

QList<SARibbonCategory*> FmtWindowTabInterface::categoryList()
{
    return { m_pRibbonCategory };
}

void FmtWindowTabInterface::initRibbonPanels()
{
    if (m_pRibbon && !m_pRibbonCategory)
        m_pRibbonCategory = new SARibbonCategory(ribbonCategoryName());

    initDefaultPanel();
    setupRibbonActions();
}

void FmtWindowTabInterface::addCategoryToParentContext()
{
    if (m_pParentContextCategory && m_pRibbonCategory)
    {
        bool exists = false;
        QList<SARibbonCategory*> categories = m_pParentContextCategory->categoryList();
        for (SARibbonCategory *cat : qAsConst(categories))
        {
            if (cat == m_pRibbonCategory)
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            m_pParentContextCategory->addCategoryPage(m_pRibbonCategory);
            m_CategoryAddedToContext = true;
        }
    }
}

void FmtWindowTabInterface::removeCategoryFromParentContext()
{
    if (m_pParentContextCategory)
    {
        QList<SARibbonCategory*> lst = categoryList();

        ribbon()->hideContextCategory(m_pParentContextCategory);
        for (auto category : qAsConst(lst))
        {
            m_pParentContextCategory->takeCategory(category);
        }
        ribbon()->showContextCategory(m_pParentContextCategory);
    }
}

void FmtWindowTabInterface::activateRibbon()
{
    if (!m_pRibbon || !m_pParentContextCategory || !m_pRibbonCategory)
        return;

    addCategoryToParentContext();

    ribbon()->hideContextCategory(m_pParentContextCategory);
    m_pRibbon->showContextCategory(m_pParentContextCategory);
    m_pRibbon->raiseCategory(m_pRibbonCategory);
    ribbon()->showContextCategory(m_pParentContextCategory);
}

void FmtWindowTabInterface::deactivateRibbon()
{
    removeCategoryFromParentContext();

    if (m_pRibbon && m_pParentContextCategory)
    {
        if (m_pParentContextCategory->categoryCount() == 0)
            m_pRibbon->hideContextCategory(m_pParentContextCategory);
    }
}

void FmtWindowTabInterface::initDefaultPanel()
{

}

void FmtWindowTabInterface::setWordWrapToEdit(QPlainTextEdit *pEdit, bool flag)
{
    pEdit->setWordWrapMode(flag ? QTextOption::WordWrap : QTextOption::NoWrap);
}

void FmtWindowTabInterface::setAllCharsModeToEdit(QPlainTextEdit *pEdit, bool flag)
{
    if (!pEdit)
        return;

    QTextOption option = pEdit->document()->defaultTextOption();

    if (flag)
    {
        option.setFlags(option.flags() | QTextOption::ShowTabsAndSpaces
                        | QTextOption::ShowLineAndParagraphSeparators);
    } else
    {
        option.setFlags(option.flags() & ~QTextOption::ShowTabsAndSpaces);
        option.setFlags(option.flags() & ~QTextOption::ShowLineAndParagraphSeparators);
    }

    pEdit->document()->setDefaultTextOption(option);
    pEdit->viewport()->update();
}

void FmtWindowTabInterface::showCopyNotification(QPlainTextEdit *pEdit, const QString &text, bool hasSelection)
{
    if (!pEdit)
        return;

    QString message;
    int textLength = text.length();

    if (hasSelection)
        message = tr("Скопировано: %1 символов").arg(textLength);
    else
        message = tr("Скопирован весь текст: %1 символов").arg(textLength);

    QToolTip::showText(pEdit->mapToGlobal(QPoint(pEdit->width() / 2, 0)), message, pEdit, QRect(), 2000);
}

void FmtWindowTabInterface::copyFromEdit(QPlainTextEdit *pEdit)
{
    if (!pEdit)
        return;

    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard)
        return;

    QString textToCopy;
    QTextCursor cursor = pEdit->textCursor();

    if (cursor.hasSelection())
    {
        textToCopy = cursor.selectedText();
        textToCopy.replace(QChar::ParagraphSeparator, QLatin1Char('\n'));
    }
    else
        textToCopy = pEdit->toPlainText();

    if (!textToCopy.isEmpty())
    {
        clipboard->setText(textToCopy);
        showCopyNotification(pEdit, textToCopy);
    }
}

QString FmtWindowTabInterface::saveDialog(const int &ContentType, const QString &text, const QString &filename)
{
    QString filter;
    switch(ContentType)
    {
    case HighlighterSql:
        filter = "Sql files(*.sql)";
        break;
    case HighlighterCpp:
        filter = "C++ files(*.c *.cpp *.h *.hpp)";
        break;
    case HighlighterRsl:
        filter = "C++ files(*.mac)";
        break;
    default:
        filter = "Text files(*.txt)";
    }

    QString fname = filename;
    if (filename.isEmpty())
        fname = QFileDialog::getSaveFileName(this, QString(), QString(), filter);

    if (fname.isEmpty())
        return QString();

    QFile f(fname);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream.setCodec("IBM 866");
        stream << text;
        f.close();
    }

    return fname;
}
