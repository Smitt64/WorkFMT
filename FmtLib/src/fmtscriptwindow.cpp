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
#include <SARibbon.h>
#include <QTemporaryFile>
#include <QTextStream>
#include <toolsruntime.h>

class SubWindowEventFilter : public QObject
{
public:
    SubWindowEventFilter(FmtScriptWindow *parent) :
        QObject(parent),
        m_parent(parent)
    {
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::Close)
        {
            QMdiSubWindow *wnd = qobject_cast<QMdiSubWindow*>(obj);

            if (wnd && wnd->property(Prop_WndType).toInt() == WndTypeCode)
                m_parent->closeLinkedWindow(wnd);
            else
            {
                QMdiSubWindow *code = reinterpret_cast<QMdiSubWindow*>(wnd->property(Prop_LinkedWnd).toInt());
                code->setProperty(Prop_LinkedWnd, 0);
            }
        }

        return QObject::eventFilter(obj, event);
    }

private:
    FmtScriptWindow *m_parent;
};

// -------------------------------------------------------------------------------------

FmtScriptWindow::FmtScriptWindow(QSharedPointer<FmtTable> &table, QWidget *parent) :
    FmtCodeTabBase(parent),
    m_pRunAction(nullptr),
    m_pClearOutput(nullptr),
    m_pCreateAction(nullptr),
    m_pOpenAction(nullptr)
{
    m_pEventFilter = new SubWindowEventFilter(this);
    pContainer->setTabsClosable(true);
    pContainer->setToolTip(tr("Редактор макросов с поддержкой отладки и вывода результатов"));
    pTable = table;
    OnNew();
}

FmtScriptWindow::~FmtScriptWindow()
{

}


void FmtScriptWindow::closeLinkedWindow(QMdiSubWindow *window)
{
    if (!window)
        return;

    // Получаем связанное окно
    int linkedWndId = window->property(Prop_LinkedWnd).toInt();
    if (linkedWndId)
    {
        QMdiSubWindow *linkedWindow = reinterpret_cast<QMdiSubWindow*>(linkedWndId);

        // Удаляем ссылку у связанного окна
        linkedWindow->setProperty(Prop_LinkedWnd, QVariant());

        // Закрываем связанное окно
        linkedWindow->close();
    }

    // Очищаем ссылку у текущего окна
    window->setProperty(Prop_LinkedWnd, QVariant());
}

QString FmtScriptWindow::ribbonCategoryName() const
{
    return tr("Макрос");
}

void FmtScriptWindow::initRibbonPanels()
{
    FmtCodeTabBase::initRibbonPanels();

    if (m_pRibbonCategory)
    {
        m_pRibbonCategory->setToolTip(tr("Инструменты для создания, редактирования и выполнения макросов"));
    }
}

void FmtScriptWindow::activateRibbon()
{
    FmtCodeTabBase::activateRibbon();
}

void FmtScriptWindow::deactivateRibbon()
{
    FmtCodeTabBase::deactivateRibbon();
}

void FmtScriptWindow::setupRibbonActions()
{
    m_pRslPanel = new SARibbonPannel(tr("Скрипт"));
    m_pRslPanel->setToolTip(tr("Панель управления выполнением скриптов"));
    m_pRibbonCategory->addPannel(m_pRslPanel);

    m_pRunAction = createAction(tr("Выполнить\nс отладкой"), "Run");
    toolAddActionWithTooltip(m_pRunAction, tr("Выполнить текущий макрос с отображением отладочной информации в окне вывода"));

    m_pRslPanel->addLargeAction(m_pRunAction);

    m_pClearOutput = createAction(tr("Очистить\nвывод"), "CleanData");
    toolAddActionWithTooltip(m_pClearOutput,
                             tr("Очистить содержимое окна вывода результатов выполнения макроса"));
    m_pRslPanel->addLargeAction(m_pClearOutput);

    m_pSave->disconnect(m_SaveConnection);
    m_pCopy->disconnect(m_CopyConnection);

    connect(m_pRunAction, &QAction::triggered, [=]()
    {
        ExecuteEx(true);
    });

    connect(m_pClearOutput, &QAction::triggered, [=]()
    {
        QMdiSubWindow *Current = getWindow();

        if (!Current)
            return;

        if (Current->property(Prop_WndType).toInt() != WndTypeOutput)
        {
            QToolTip::showText(QCursor::pos(),
                               tr("Окно вывода не активно. Выберите окно с результатами выполнения."),
                               this, QRect(), 2000);
            return;
        }

        CodeEditor *pOutput = editorFromWindow(Current);
        pOutput->clear();

        QToolTip::showText(QCursor::pos(),
                           tr("Окно вывода очищено"),
                           this, QRect(), 1500);
    });

    updateRibbonState();
}

void FmtScriptWindow::preInitDefaultActions()
{
    m_pCreateAction = createAction(tr("Новый\nмакрос"), "NewFile");
    toolAddActionWithTooltip(m_pCreateAction,
                             tr("Создать новый макрос"),
                             QKeySequence::New);
    m_pActionPannel->addLargeAction(m_pCreateAction);

    m_pOpenAction = createAction(tr("Открыть\nмакрос"), "OpenFile");
    toolAddActionWithTooltip(m_pOpenAction,
                             tr("Открыть существующий макрос из файла"),
                             QKeySequence::Open);
    m_pActionPannel->addLargeAction(m_pOpenAction);

    //
    connect(m_pCreateAction, &QAction::triggered, this, &FmtScriptWindow::OnNew);
    connect(m_pOpenAction, &QAction::triggered, this, &FmtScriptWindow::OnOpen);
}

void FmtScriptWindow::OnNew()
{
    QString sample = toolReadTextFileContent(":/txt/samplescript.mac", "IBM 866");

    QMdiSubWindow *wnd = AddTab("samplescript", sample, HighlighterRsl);
    CodeEditor *editor = editorFromWindow(wnd);
    editor->setReadOnly(false);
    editor->setToolTip(tr("Редактор макроса (RSL). Нажмите F5 для выполнения."));
    wnd->setProperty(Prop_WndType, WndTypeCode);
    wnd->setToolTip(tr("Окно редактирования макроса. Для выполнения используйте кнопку 'Выполнить' или клавишу F5"));

    wnd->installEventFilter(m_pEventFilter);

    QToolTip::showText(QCursor::pos(),
                       tr("Создан новый макрос"),
                       this, QRect(), 1500);
}

void FmtScriptWindow::OnOpen()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Открыть макрос"), QDir::currentPath(), tr("Макро файлы (*.mac)"));

    if (!file.isEmpty())
    {
        QFileInfo fi(file);
        QString sample = toolReadTextFileContent(file, "IBM 866");

        QMdiSubWindow *wnd = AddTab(fi.baseName(), sample, HighlighterRsl);
        CodeEditor *editor = editorFromWindow(wnd);
        editor->setReadOnly(false);
        editor->setToolTip(tr("Редактор макроса: %1").arg(file));
        wnd->setProperty(Prop_WndType, WndTypeCode);
        wnd->installEventFilter(m_pEventFilter);
        wnd->setWindowFilePath(file);
        wnd->setToolTip(tr("Макрос из файла: %1").arg(file));

        QToolTip::showText(QCursor::pos(),
                           tr("Макрос загружен: %1").arg(fi.fileName()),
                           this, QRect(), 2000);
    }
}

/*
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
}*/

QMdiSubWindow *FmtScriptWindow::outputWnd(QMdiSubWindow *CodeWnd)
{
    if (CodeWnd->property(Prop_WndType).toInt() != WndTypeCode)
        return nullptr;

    if (!CodeWnd->property(Prop_LinkedWnd).isValid() || !CodeWnd->property(Prop_LinkedWnd).toInt())
    {
        QString outputTitle = tr("Output: %1").arg(CodeWnd->windowTitle());
        QMdiSubWindow *wnd = AddTab(outputTitle, QString(), HighlighterPlain);
        wnd->setProperty(Prop_WndType, WndTypeOutput);
        wnd->setProperty(Prop_LinkedWnd, reinterpret_cast<int>(CodeWnd));
        wnd->setToolTip(tr("Окно вывода результатов выполнения макроса '%1'").arg(CodeWnd->windowTitle()));

        CodeEditor *outputEditor = editorFromWindow(wnd);
        outputEditor->setReadOnly(true);
        outputEditor->setToolTip(tr("Здесь отображаются результаты выполнения макроса"));

        CodeWnd->setProperty(Prop_LinkedWnd, reinterpret_cast<int>(wnd));
        wnd->installEventFilter(m_pEventFilter);

        return wnd;
    }

    QMdiSubWindow *wnd = reinterpret_cast<QMdiSubWindow*>(CodeWnd->property(Prop_LinkedWnd).toInt());
    return wnd;
}

void FmtScriptWindow::ExecuteEx(bool useDebug)
{
    QMdiSubWindow *Current = getWindow();

    if (!Current)
    {
        QToolTip::showText(QCursor::pos(),
                           tr("Нет активного окна с макросом"),
                           this, QRect(), 2000);
        return;
    }

    if (Current->property(Prop_WndType).toInt() != WndTypeCode)
    {
        QToolTip::showText(QCursor::pos(),
                           tr("Активное окно не содержит макрос. Выберите окно с макросом."),
                           this, QRect(), 2000);
        return;
    }

    QTemporaryFile tmp;
    QScopedPointer<ToolbarActionExecutor> executor(new ToolbarActionExecutor(pTable, this));

    QString fileName = Current->windowFilePath();
    if (fileName.isEmpty())
    {
        tmp.open();
        QTextStream stream(&tmp);
        stream.setCodec("IBM 866");
        stream << editorFromWindow(Current)->toPlainText();
        stream.flush();

        fileName = tmp.fileName();
    }
    else
    {
        CodeEditor *pCode = editorFromWindow(Current);

        int Syntax = pCode->property(FmtSyntaxProperty).toInt();
        QString filename = saveDialog(Syntax, pCode->toPlainText(), Current->windowFilePath());

        if (!filename.isEmpty())
            Current->setWindowFilePath(filename);
    }

    QMdiSubWindow *pOutputWnd = outputWnd(Current);
    CodeEditor *pOutput = editorFromWindow(pOutputWnd);
    pOutput->clear();

    // Добавляем информацию о начале выполнения
    QTextCursor cursor = pOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(tr("=== Выполнение макроса начато ===\n"));

    // Показываем уведомление о начале выполнения
    QToolTip::showText(QCursor::pos(),
                       tr("Выполнение макроса..."),
                       this, QRect(), 1000);

    connect(executor.data(), &ToolbarActionExecutor::WriteOut, [=](const QString &out)
    {
        QTextCursor cursor = pOutput->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.insertText(out);
        // Прокручиваем вниз для отображения новых данных
        pOutput->ensureCursorVisible();
    });

    executor->playRep(fileName);

    // Добавляем информацию о завершении выполнения
    cursor = pOutput->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(tr("\n=== Выполнение макроса завершено ===\n"));

    pContainer->setActiveSubWindow(pOutputWnd);

    // Показываем уведомление о завершении
    QToolTip::showText(QCursor::pos(),
                       tr("Выполнение макроса завершено"),
                       this, QRect(), 2000);

    updateRibbonState();
}

void FmtScriptWindow::updateRibbonState()
{
    if (!m_pRunAction)
        return;

    m_pConvertPg->setEnabled(false);
    m_pExecuteSql->setEnabled(false);

    FmtCodeTabBase::updateRibbonState();
    QMdiSubWindow *Current = pContainer->currentSubWindow();

    if (!Current)
    {
        m_pRunAction->setEnabled(false);
        m_pClearOutput->setEnabled(false);

        return;
    }

    int type = Current->property(Prop_WndType).toInt();
    bool isCodeWindow = (type == WndTypeCode);
    //bool isOutputWindow = (type == WndTypeOutput);

    m_pClearOutput->setEnabled(type == WndTypeOutput);
    m_pRunAction->setEnabled(type == WndTypeCode);

    CodeEditor *pCode = editorFromWindow(Current);
    if (!pCode)
    {
        m_pCopy->setEnabled(false);
        m_pCut->setEnabled(false);
        m_pPaste->setEnabled(false);
        m_pUndoAction->setEnabled(false);
        m_pRedoAction->setEnabled(false);

        return;
    }

    m_pCopy->setEnabled(pCode->textCursor().hasSelection());
    m_pCut->setEnabled(isCodeWindow && !pCode->isReadOnly() && pCode->textCursor().hasSelection());
    m_pPaste->setEnabled(isCodeWindow && !pCode->isReadOnly());

    FmtCodeTabBase::updateRibbonState();
}
