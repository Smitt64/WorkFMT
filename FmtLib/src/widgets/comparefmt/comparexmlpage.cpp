#include "comparexmlpage.h"
#include "toolsruntime.h"
#include "ui_comparexmlpage.h"
#include "codeeditor/codeeditor.h"
#include "codeeditor/codehighlighter.h"
#include "fmtcore.h"
#include <QDebug>

CompareXmlPage::CompareXmlPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::CompareXmlPage)
{
    ui->setupUi(this);

    m_pXmlCodeEditor = new CodeEditor(this);
    m_pOraCodeEditor = new CodeEditor(this);
    m_pPgCodeEditor = new CodeEditor(this);

    ui->tabWidget->addTab(m_pXmlCodeEditor, tr("Xml структура с полями"));
    ui->tabWidget->addTab(m_pOraCodeEditor, tr("Скрипт дампа для Oracle"));
    ui->tabWidget->addTab(m_pPgCodeEditor, tr("Скрипт дампа для Postgres"));

    m_pOraCodeEditor->setReadOnly(true);
    m_pPgCodeEditor->setReadOnly(true);

    m_pOraCodeEditor->setPlainText(toolReadTextFileContent("://dump_fmt_to_xml_ora.sql"));
    m_pPgCodeEditor->setPlainText(toolReadTextFileContent("://dump_fmt_to_xml_pg.sql"));

    setTitle(tr("Дамп из xml"));
    ToolApplyHighlighter(m_pXmlCodeEditor, HighlighterXml);
    ToolApplyHighlighter(m_pOraCodeEditor, HighlighterSql, FmtCodeTabStyle);
    ToolApplyHighlighter(m_pPgCodeEditor, HighlighterSql, FmtCodeTabStyle);

    registerField("XmlDump", m_pXmlCodeEditor, "plainText");
}

CompareXmlPage::~CompareXmlPage()
{
    delete ui;
}
