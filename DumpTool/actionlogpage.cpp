#include "actionlogpage.h"
#include "ui_actionlogpage.h"
#include "importoperation.h"
#include "baseloghighlighter.h"
#include "selectactionpage.h"
#include "exportoperation.h"
#include <QThreadPool>
#include <QPushButton>
#include <QProgressDialog>
#include <QEventLoop>
#include <QVBoxLayout>

TextBrowser::TextBrowser(QWidget *parent) :
    QPlainTextEdit(parent)
{
    setReadOnly(true);

    QFont font("Consolas");
    font.setPointSize(10);
    setFont(font);
}

TextBrowser::~TextBrowser()
{

}

void TextBrowser::appendHtml(const QString &msg)
{
    moveCursor(QTextCursor::End);
    appendPlainText(msg);
    moveCursor (QTextCursor::End);
}

void TextBrowser::message(const QString &msg)
{
    appendHtml(msg);
}

void TextBrowser::errormsg(const QString &msg)
{
    appendHtml(msg);
}

void TextBrowser::succeeded(const QString &msg)
{
    appendHtml(msg);
    /*appendHtml(QString("<b><font color='%2'>%1</font></b>")
               .arg(msg)
               .arg(QColor(Qt::darkGreen).name()));*/
}

// -----------------------------------------------------------

ActionLogPage::ActionLogPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::ActionLogPage)
{
    ui->setupUi(this);
    setTitle(tr("Выполнение операции"));

    m_TextBrowser.reset(new TextBrowser());
    //m_TextBrowser->moveToThread(m_Thread.data());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_TextBrowser.data());

    setLayout(layout);
    //m_Thread.reset(new QThread());
    setFinalPage(true);
}

ActionLogPage::~ActionLogPage()
{
    delete ui;
}

int ActionLogPage::nextId() const
{
    return -1;
}

void ActionLogPage::finished()
{
    m_IsFinished = true;
    QApplication::beep();
    emit completeChanged();
}

void ActionLogPage::message(const QString &msg)
{
    //QApplication::processEvents();
    /*ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(msg);
    ui->textBrowser->moveCursor (QTextCursor::End);*/
    update();
    //QApplication::processEvents();
}

void ActionLogPage::errormsg(const QString &msg)
{
    /*ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(QString("<b><font color='#880808'>%1</font></b>").arg(msg));
    ui->textBrowser->moveCursor (QTextCursor::End);*/
    update();
    //QApplication::processEvents();
}

void ActionLogPage::succeeded(const QString &msg)
{
    /*ui->textBrowser->moveCursor(QTextCursor::End);
    ui->textBrowser->insertHtml(QString("<b><font color='%2'>%1</font></b>")
                                .arg(msg)
                                .arg(QColor(Qt::darkGreen).name()));
    ui->textBrowser->moveCursor (QTextCursor::End);*/
    update();
    //QApplication::processEvents();
}

void ActionLogPage::stageChanged(const QString &msg)
{
    setTitle(tr("Выполнение операции: %1").arg(msg));
}

void ActionLogPage::initializePage()
{
    m_IsFinished = false;
    BaseOperation *op = nullptr;

    if (field("Action").toInt() == SelectActionPage::ActionImport)
        op = new ImportOperation(qobject_cast<DumpToolWizard*>(wizard()));
    else
        op = new ExportOperation(qobject_cast<DumpToolWizard*>(wizard()));

    connect(op, &ImportOperation::finished, this, &ActionLogPage::finished);
    connect(op, &ImportOperation::message, m_TextBrowser.data(), &TextBrowser::message);
    connect(op, &ImportOperation::errormsg, m_TextBrowser.data(), &TextBrowser::errormsg);
    connect(op, &ImportOperation::succeeded, m_TextBrowser.data(), &TextBrowser::succeeded);
    connect(op, &ImportOperation::progressDialog, this, &ActionLogPage::progressDialog);
    connect(op, &ImportOperation::stageChanged, this, &ActionLogPage::stageChanged);

    BaseLogHighlighter *highlighter = nullptr;
    op->highlighter(&highlighter, m_TextBrowser->document());

    wizard()->button(QWizard::BackButton)->setEnabled(false);
    QThreadPool::globalInstance()->start(op);
}

bool ActionLogPage::isComplete() const
{
    return m_IsFinished;
}

void ActionLogPage::progressDialog(const QString &msg, qint64 max)
{
    BaseOperation *op = qobject_cast<BaseOperation*>(sender());

    QProgressDialog dlg(this);
    dlg.setLabelText(msg);
    dlg.setRange(0, max);
    dlg.setCancelButton(nullptr);
    dlg.setAutoClose(true);
    dlg.setAutoReset(false);
    dlg.setWindowFlags(Qt::WindowTitleHint);

    QEventLoop loop;
    connect(op, &ImportOperation::progressValue, &dlg, &QProgressDialog::setValue);
    connect(op, &ImportOperation::progressDialogEnd, &dlg, &QProgressDialog::close);
    connect(op, &ImportOperation::progressDialogEnd, &loop, &QEventLoop::quit);
    connect(op, &ImportOperation::finished, &loop, &QEventLoop::quit);

    loop.exec();
}

void ActionLogPage::progressDialogEnd()
{
    //m_ProgressDialog.reset();
}
