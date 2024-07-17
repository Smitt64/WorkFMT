#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "toolsruntime.h"
#include <QDebug>
#include <QtSql>
#include <connectioninfo.h>
#include <fmttable.h>
#include <oracletnslistmodel.h>
#include <oracleauthdlg.h>
#include "rslexecutor.h"
#include <QStyleFactory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qApp->addLibraryPath("D:/Work/WorkFMT/bin/");
    ui->setupUi(this);

    pCodeEditor = new CodeEditor(this);
    ui->verticalLayout->insertWidget(0, pCodeEditor);

    QString filename = toolFullFileNameFromDir("FmtLibTest.mac");
    QString data = toolReadTextFileContent(filename);
    pCodeEditor->setPlainText(data);

    pCodeEditor->setReadOnly(true);
    ui->listView->setModel(&m_Errors);

    qDebug() << QStyleFactory::keys();
    Highlighter *pHighlighter = new Highlighter(Highlighter::HC_JS, pCodeEditor->document());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->plainTextEdit->clear();

    RslExecutor exec;
    exec.setDebugMacroFlag(true);

    auto Proc = [&exec]()
    {
        QVariant result = exec.call("TestFunc", {"User Message"});

        qDebug() << result;
    };

    connect(&exec, &RslExecutor::WriteOut, [=](QString msg)
    {
        //ui->plainTextEdit->appendPlainText(msg.simplified());
        ui->plainTextEdit->moveCursor(QTextCursor::End);
        ui->plainTextEdit->insertPlainText(msg);
                //->append(msg);
    });

    exec.playRep("FmtLibTest.mac", "1.txt", Proc);

    m_Errors.setStringList(exec.errors());
}

void MainWindow::printHandle(const QString &str)
{
    ui->plainTextEdit->appendPlainText(str);
}
