#include "fmtscriptwindow.h"
#include "codeeditor.h"
#include "fmtcore.h"
#include "highlighter.h"
#include <QFileDialog>

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

QScriptValue myImportFunction(QScriptContext *context, QScriptEngine *engine)
{
    //QScriptValue calleeData = context->callee().data();
    //QTextBrowser *edit = qobject_cast<QTextBrowser*>(calleeData.toQObject());
    QString result;
    for (int i = 0; i < context->argumentCount(); ++i) {
        engine->importExtension(context->argument(i).toString());
    }

    return engine->undefinedValue();
}

// =================================================================================

FmtScriptWindow::FmtScriptWindow(QSharedPointer<FmtTable> &table, QWidget *parent) :
    QMainWindow(parent)
{
    CreateMdi();
    CreateActions();
    CreateScriptEngine();

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
    pMdi = new QMdiArea(this);
    pOutput = new QTextBrowser(this);
    pSplitter = new QSplitter(Qt::Vertical, this);
    pSplitter->addWidget(pMdi);
    pSplitter->addWidget(pOutput);
    setCentralWidget(pSplitter);

    pMdi->setDocumentMode(true);
    pMdi->setViewMode(QMdiArea::TabbedView);
    pMdi->setTabsClosable(true);
    pMdi->setTabShape(QTabWidget::Triangular);
}

void FmtScriptWindow::CreateScriptEngine()
{
    pEngine = new QScriptEngine(this);
    QScriptValue result = pEngine->importExtension("fmt");
    if (!result.isUndefined())
        qDebug() << "Uncaught exception: " << pEngine->uncaughtException().toString();

    QScriptValue fun = pEngine->newFunction(myPrintFunction);
    fun.setData(pEngine->newQObject(pOutput));
    pEngine->globalObject().setProperty("Print", fun);

    QScriptValue impfun = pEngine->newFunction(myImportFunction);
    pEngine->globalObject().setProperty("ImportExt", impfun);
}

void FmtScriptWindow::OnNew()
{
    QString sample = ReadTextFileContent(":/txt/samplescript.js");
    CodeEditor *pEditor = new CodeEditor(this);
    QMdiSubWindow *wnd = pMdi->addSubWindow(pEditor);
    wnd->showMaximized();

    Highlighter *pHighlighter = new Highlighter(Highlighter::HC_JS, pEditor->document());
    Q_UNUSED(pHighlighter)
    pEditor->setPlainText(sample);
}

void FmtScriptWindow::OnOpen()
{
    QString file = QFileDialog::getOpenFileName(this, QString(), QDir::currentPath(), tr("JavaScript files (*.js)"));

    if (!file.isEmpty())
    {
        QFileInfo fi(file);
        QString sample = ReadTextFileContent(file);
        CodeEditor *pEditor = new CodeEditor(this);
        pEditor->setWindowFilePath(file);
        QMdiSubWindow *wnd = pMdi->addSubWindow(pEditor);
        wnd->showMaximized();

        Highlighter *pHighlighter = new Highlighter(Highlighter::HC_JS, pEditor->document());
        Q_UNUSED(pHighlighter)
        pEditor->setPlainText(sample);
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

void FmtScriptWindow::OnSave()
{
    QWidget *fw = qApp->focusWidget();
    CodeEditor *pEditor = NULL;

    if (!fw)
        return;

    pEditor = qobject_cast<CodeEditor*>(fw);
    if (!pEditor)
        return;

    QString filename;
    QString macroFile = pEditor->windowFilePath();

    if (macroFile.isEmpty())
    {
        QDir d = qApp->applicationDirPath();
        d.cd("mac");
        filename = QFileDialog::getSaveFileName(this, QString(), d.absolutePath(), tr("JavaScript files (*.js)"));
    }
    else
        filename = macroFile;

    if (filename.isEmpty())
        return;

    QFile f(filename);
    if (f.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&f);
        stream.setCodec("UTF-8");
        stream << pEditor->toPlainText();
        f.close();
    }
}

void FmtScriptWindow::SetupScriptEngineValues(CodeEditor *pEditor)
{
    QScriptValue global = pEngine->globalObject();
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

    global.setProperty("$macroDirPath", macroFile, QScriptValue::ReadOnly | QScriptValue::Undeletable);
}

void FmtScriptWindow::ExecuteEx(bool useDebug)
{
    QWidget *fw = qApp->focusWidget();

    if (fw)
    {
        CodeEditor *pEditor = qobject_cast<CodeEditor*>(fw);
        if (pEditor)
        {
            if (useDebug)
                dbg.attachTo(pEngine);

            pEngine->pushContext();
            SetupScriptEngineValues(pEditor);
            pEngine->evaluate(pEditor->toPlainText());

            if (pEngine->hasUncaughtException())
            {
                QScriptValue error = pEngine->uncaughtException();
                QMessageBox::critical(this, tr("Ошибка выполнения скрипта"), tr("Строка %0: %1")
                                      .arg(error.property("lineNumber").toInt32())
                                      .arg(error.toString()));
            }
            pEngine->popContext();

            if (useDebug)
                dbg.detach();
        }
    }
}

