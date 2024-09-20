#include "fmtscriptwindow.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include "fmtcore.h"
#include "toolsruntime.h"
#include "rslexecutors/toolbaractionexecutor.h"
#include "mainwindow.h"
#include <QFileDialog>
#include <errorsmodel.h>
#include <errordlg.h>

FmtScriptWindow::FmtScriptWindow(QSharedPointer<FmtTable> &table, QWidget *parent) :
    QMainWindow(parent)
{
    CreateMdi();
    CreateActions();
    //CreateScriptEngine();

    pTable = table;
    OnNew();
}

FmtScriptWindow::~FmtScriptWindow()
{

}

void FmtScriptWindow::CreateActions()
{
    m_pToolBar = addToolBar(tr("Main"));
    m_pToolBar->setFloatable(false);
    m_pToolBar->setMovable(false);
    m_pToolBar->setIconSize(QSize(16, 16));

    pNew = m_pToolBar->addAction(QIcon(":/img/NewDocumentHS.png"), tr("Новый"));
    pOpen = m_pToolBar->addAction(QIcon(":/img/openfolderHS.png"), tr("Открыть"));
    pSave = m_pToolBar->addAction(QIcon(":/save"), tr("Сохранить"));
    m_pToolBar->addSeparator();
    pExecute = m_pToolBar->addAction(QIcon(":/img/FormRunHS.png"), tr("Выполнить"));
    pExecuteDebug = m_pToolBar->addAction(QIcon(":/img/rundebug.png"), tr("Выполнить c отладкой"));
    m_pToolBar->addSeparator();
    pClear = m_pToolBar->addAction(tr("Очистить вывод"));

    connect(pNew, SIGNAL(triggered(bool)), SLOT(OnNew()));
    connect(pExecute, SIGNAL(triggered(bool)), SLOT(OnExecute()));
    connect(pExecuteDebug, SIGNAL(triggered(bool)), SLOT(OnExecuteDebug()));
    connect(pClear, SIGNAL(triggered(bool)), pOutput, SLOT(clear()));
    connect(pOpen, SIGNAL(triggered(bool)), SLOT(OnOpen()));
    connect(pSave, SIGNAL(triggered(bool)), SLOT(OnSave()));
}

void FmtScriptWindow::CreateMdi()
{
    pOutput = new CodeEditor(this);
    pSplitter = new QSplitter(Qt::Vertical, this);

    pEditor = new CodeEditor(this);
    ToolApplyHighlighter(pEditor, HighlighterRsl);
    pSplitter->addWidget(pEditor);
    pSplitter->addWidget(pOutput);
    setCentralWidget(pSplitter);

    pOutput->setReadOnly(true);
    ToolApplyHighlighter(pOutput, HighlighterPlain);
}

void FmtScriptWindow::CreateScriptEngine()
{
    /*pEngine = new QScriptEngine(this);
    QScriptValue result = pEngine->importExtension("fmt");
    if (!result.isUndefined())
        qDebug() << "Uncaught exception: " << pEngine->uncaughtException().toString();

    QScriptValue fun = pEngine->newFunction(myPrintFunction);
    fun.setData(pEngine->newQObject(pOutput));
    pEngine->globalObject().setProperty("Print", fun);

    QScriptValue impfun = pEngine->newFunction(myImportFunction);
    pEngine->globalObject().setProperty("ImportExt", impfun);*/
}

void FmtScriptWindow::OnNew()
{
    QString sample = toolReadTextFileContent(":/txt/samplescript.mac", "IBM 866");

    pEditor->setPlainText(sample);
    pEditor->setWindowFilePath(QString());
}

void FmtScriptWindow::OnOpen()
{
    QString file = QFileDialog::getOpenFileName(this, QString(), QDir::currentPath(), tr("Макро файлы (*.mac)"));

    if (!file.isEmpty())
    {
        QFileInfo fi(file);
        QString sample = toolReadTextFileContent(file, "IBM 866");
        pEditor->setPlainText(sample);
        pEditor->setWindowFilePath(file);
    }
}

void FmtScriptWindow::OnExecute()
{
    ExecuteEx(false);
}

void FmtScriptWindow::OnExecuteDebug()
{
    ExecuteEx(true);
}

bool FmtScriptWindow::save()
{
    QString filename;
    QString macroFile = pEditor->windowFilePath();

    if (macroFile.isEmpty())
    {
        QDir d = qApp->applicationDirPath();
        d.cd("mac");
        filename = QFileDialog::getSaveFileName(this, QString(), d.absolutePath(), tr("Макро файлы (*.mac)"));
    }
    else
        filename = macroFile;

    if (filename.isEmpty())
        return false;

    QFile f(filename);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream.setCodec("IBM 866");
        stream << pEditor->toPlainText();
        pEditor->setWindowFilePath(filename);
        f.close();
    }

    return true;
}

void FmtScriptWindow::OnSave()
{
    save();
}

void FmtScriptWindow::SetupScriptEngineValues(CodeEditor *pEditor)
{
    /*QScriptValue global = pEngine->globalObject();
    QScriptValue obj = global.property("FmtTablePtr").construct();
    global.setProperty("pTable", pEngine->newVariant(obj, QVariant::fromValue(pTable)));

    QString macroFile = pEditor->windowFilePath();
    if (macroFile.isEmpty())
    {
        QDir d = qApp->applicationDirPath();
        if (!d.cd("mac"))
        {
            d = QDir::current();
            d.cd("mac");
        }
        macroFile = d.absolutePath();
    }
    else
    {
        QFileInfo fi(macroFile);
        macroFile = fi.path();
    }

    global.setProperty("$macroDirPath", macroFile, QScriptValue::ReadOnly | QScriptValue::Undeletable);*/
}

void FmtScriptWindow::ExecuteEx(bool useDebug)
{
    MainWindow *pMainWindow = qobject_cast<MainWindow*>(qApp->activeWindow());

    if (!pMainWindow)
        return;

    QMdiSubWindow *old = pMainWindow->currentMdiWindow();
    QString macroFile = pEditor->windowFilePath();
    QScopedPointer<ToolbarActionExecutor> executor(new ToolbarActionExecutor(pMainWindow));

    if (!save())
        return;

    pMainWindow->SetActiveFmtWindow(old);
    macroFile = pEditor->windowFilePath();
    connect(executor.data(), &ToolbarActionExecutor::WriteOut, [=](const QString &out)
    {
        QTextCursor cursor = pOutput->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(out);
        //pOutput->ad
    });
    executor->playRep(macroFile);
}

