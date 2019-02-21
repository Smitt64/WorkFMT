#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtSql>
#include <connectioninfo.h>
#include <fmttable.h>
#include <oracletnslistmodel.h>
#include <oracleauthdlg.h>

QScriptValue myPrintFunction(QScriptContext *context, QScriptEngine *engine)
{
    QScriptValue calleeData = context->callee().data();
    QTextBrowser *edit = qobject_cast<QTextBrowser*>(calleeData.toQObject());
    QString result;
    for (int i = 0; i < context->argumentCount(); ++i) {
        result.append(context->argument(i).toString());
    }
    edit->append(result);

    return engine->undefinedValue();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qApp->addLibraryPath("D:/Work/WorkFMT/bin/");
    ui->setupUi(this);
    pEngine = new QScriptEngine(this);
    QStringList ext = pEngine->availableExtensions();
    qDebug() << ext;
    QScriptValue result = pEngine->importExtension("fmt");
    if (!result.isUndefined())
        qDebug() << pEngine->uncaughtException().toString();
    dbg.attachTo(pEngine);

    QScriptValue fun = pEngine->newFunction(myPrintFunction);
    fun.setData(pEngine->newQObject(ui->textBrowser));
    pEngine->globalObject().setProperty("print", fun);

    pCodeEditor = new CodeEditor(this);
    ui->verticalLayout->insertWidget(0, pCodeEditor);

    pCodeEditor->setPlainText("var pConnection = CreateConnection(\"SERP_PDN\", \"SERP_PDN\", \"DB114DEV\"); \n"
                              "var table = new CJsFmtTable(pConnection); \n"
                              "if (table.load(\"dparty_dbt\")) \n"
                              "{ \n"
                              "    for (var i = 0; i < table.fieldsCount(); i++) \n"
                              "    { \n"
                              "        var fld = table.field(i); \n"
                              "        print(fld.Name, \": \", fld.Comment); \n"
                              "    } \n"
                              "} \n"
                              "table = null; \n"
                              "delete table; \n"
                              "pConnection = null; \n"
                              "delete pConnection;");

    Highlighter *pHighlighter = new Highlighter(Highlighter::HC_JS, pCodeEditor->document());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    pEngine->pushContext();
    pEngine->evaluate(pCodeEditor->toPlainText());
    pEngine->popContext();
    /*OracleTnsListModel m;
    ConnectionInfo *pInfo = Q_NULLPTR;
    OracleAuthDlg::OraCreateConnection("SERP_PDN", "SERP_PDN", "DB114DEV", &pInfo, &m);
    QSharedPointer<FmtTable> ptr(new FmtTable(pInfo, Q_NULLPTR));

    if (ptr->load("dparty_dbt"))
    {
        QScriptValue global = pEngine->globalObject();
        QScriptValue obj = global.property("FmtTablePtr").construct();
        global.setProperty("pFmtTable", pEngine->newVariant(obj, QVariant::fromValue(ptr)));

        qDebug() << pEngine->uncaughtException().toString();
        pEngine->evaluate(ui->plainTextEdit->toPlainText());
    }*/
    pEngine->collectGarbage();
}

void MainWindow::printHandle(const QString &str)
{
    ui->textBrowser->append(str);
}
