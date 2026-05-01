#include "fmtcodetabbase.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include "toolsqlconverter.h"
#include <connectioninfo.h>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <SARibbon.h>
#include <QToolTip>
#include <toolsruntime.h>
#include <QApplication>
#include <QTimer>
#include <QMessageBox>
#include <QDesktopServices>
#include <QTemporaryDir>
#include <QProgressDialog>
#include <QProcess>

FmtCodeTabBase::FmtCodeTabBase(QWidget *parent) :
    FmtWindowTabInterface(parent),
    pContainer(nullptr),
    m_pActionPannel(nullptr),
    m_pViewPannel(nullptr),
    m_pTabsPannel(nullptr),
    m_pSave(nullptr),
    m_pCopy(nullptr),
    m_pCut(nullptr),
    m_pPaste(nullptr),
    m_pWordWrap(nullptr),
    m_pShowChars(nullptr),
    pLastActiveWindow(nullptr),
    m_pConvertPg(nullptr),
    m_pExecuteSql(nullptr),
    m_pPrevTab(nullptr),
    m_pNextTab(nullptr)
{
    pContainer = new QMdiArea(this);
    //pContainer->setDocumentMode(true);
    pContainer->setViewMode(QMdiArea::TabbedView);
    pContainer->setTabsClosable(false);
    pContainer->setToolTip(tr("Область отображения кода с поддержкой нескольких вкладок"));
    setCentralWidget(pContainer);

    connect(pContainer, &QMdiArea::subWindowActivated, [=](QMdiSubWindow *window)
    {
        pLastActiveWindow = window;
        updateRibbonState();
    });
}

FmtCodeTabBase::~FmtCodeTabBase()
{

}

QString FmtCodeTabBase::ribbonCategoryName() const
{
    return tr("Код");
}

void FmtCodeTabBase::setHighlighter(CodeEditor *editor, const qint16 &Syntax)
{
    editor->setProperty(FmtSyntaxProperty, Syntax);
    ToolApplyHighlighter(editor, Syntax, FmtCodeTabStyle);

    QString syntaxName;
    switch(Syntax)
    {
    case HighlighterSql: syntaxName = tr("SQL"); break;
    case HighlighterCpp: syntaxName = tr("C++"); break;
    case HighlighterXml: syntaxName = tr("XML"); break;
    default: syntaxName = tr("Текст"); break;
    }
    editor->setToolTip(tr("Редактор кода (%1)").arg(syntaxName));
}

CodeEditor *FmtCodeTabBase::editorFromWindow(QMdiSubWindow *wnd)
{
    CodeEditor *pCode = qobject_cast<CodeEditor*>(wnd->widget());
    return pCode;
}

QMdiSubWindow* FmtCodeTabBase::AddTab(const QString &tabname, const QString &icon)
{
    CodeEditor *pCode = new CodeEditor(this);
    pCode->setReadOnly(true);
    pCode->setToolTip(tr("Редактор кода: %1").arg(tabname));
    setWordWrapToEdit(pCode, m_pWordWrap ? m_pWordWrap->isChecked() : true);
    setAllCharsModeToEdit(pCode, m_pShowChars ? m_pShowChars->isChecked() : false);

    QMdiSubWindow *mdi = pContainer->addSubWindow(pCode);
    mdi->setWindowTitle(tabname);
    mdi->setAttribute(Qt::WA_DeleteOnClose);
    mdi->setWindowIcon(QIcon::fromTheme(icon));
    mdi->setToolTip(tr("Вкладка: %1").arg(tabname));
    mdi->showMaximized();

    m_pWindows.insert(tabname, mdi);
    m_pWindowsList.append(mdi);

    return mdi;
}

QMdiSubWindow *FmtCodeTabBase::AddTab(const QString &tabname, const QString &code, const qint16 &Syntax, const QString &icon)
{
    CodeEditor *pCode = new CodeEditor(this);
    pCode->setReadOnly(true);
    pCode->setPlainText(code);
    pCode->setToolTip(tr("Редактор кода: %1").arg(tabname));
    setWordWrapToEdit(pCode, m_pWordWrap ? m_pWordWrap->isChecked() : true);
    setAllCharsModeToEdit(pCode, m_pShowChars ? m_pShowChars->isChecked() : false);
    setHighlighter(pCode, Syntax);
    pCode->rehighlight();

    QMdiSubWindow *mdi = pContainer->addSubWindow(pCode);
    mdi->setWindowTitle(tabname);
    mdi->setAttribute(Qt::WA_DeleteOnClose);
    mdi->setWindowIcon(QIcon::fromTheme(icon));
    mdi->setToolTip(tr("Вкладка: %1").arg(tabname));
    mdi->showMaximized();

    m_pWindows.insert(tabname, mdi);
    m_pWindowsList.append(mdi);

    return mdi;
}

void FmtCodeTabBase::updateRibbonState()
{
    m_pCut->setEnabled(false);
    m_pPaste->setEnabled(false);

    QMdiSubWindow *Current = pContainer->currentSubWindow() ? pContainer->currentSubWindow() : pLastActiveWindow;
    if (!Current)
    {
        m_pSave->setEnabled(false);
        m_pCopy->setEnabled(false);
        m_pConvertPg->setEnabled(false);
        m_pExecuteSql->setEnabled(false);
        m_pUndoAction->setEnabled(false);
        m_pRedoAction->setEnabled(false);

        return;
    }

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

    m_pUndoAction->setEnabled(pCode->document()->isUndoAvailable());
    m_pRedoAction->setEnabled(pCode->document()->isRedoAvailable());

    // Обновляем статус Undo/Redo при изменениях в документе
    if (pCode)
    {
        QList<QMdiSubWindow*> windows = pContainer->subWindowList();

        for (QMdiSubWindow *wnd : qAsConst(windows))
        {
            CodeEditor *Code = editorFromWindow(wnd);
            QObject::disconnect(Code->document(), &QTextDocument::undoAvailable, this, &FmtCodeTabBase::undoUpdate);
            QObject::disconnect(Code->document(), &QTextDocument::undoAvailable, this, &FmtCodeTabBase::redoUpdate);
        }

        connect(pCode->document(), &QTextDocument::undoAvailable, this, &FmtCodeTabBase::undoUpdate);
        connect(pCode->document(), &QTextDocument::redoAvailable, this, &FmtCodeTabBase::redoUpdate);
    }
}

void FmtCodeTabBase::undoUpdate(bool available)
{
    QMdiSubWindow *Current = pContainer->currentSubWindow() ? pContainer->currentSubWindow() : pLastActiveWindow;

    if (!Current)
    {
        m_pUndoAction->setEnabled(false);
        return;
    }

    if (pContainer->currentSubWindow() == Current)
        m_pUndoAction->setEnabled(available);
}

void FmtCodeTabBase::redoUpdate(bool available)
{
    QMdiSubWindow *Current = getWindow();

    if (!Current)
    {
        m_pRedoAction->setEnabled(false);
        return;
    }

    if (pContainer->currentSubWindow() == Current)
        m_pRedoAction->setEnabled(available);
}


void FmtCodeTabBase::preInitDefaultActions()
{

}

void FmtCodeTabBase::initDefaultPanel()
{
    m_pActionPannel = new SARibbonPannel(tr("Действия"));
    m_pRibbonCategory->addPannel(m_pActionPannel);

    m_pViewPannel = new SARibbonPannel(tr("Вид"));
    m_pRibbonCategory->addPannel(m_pViewPannel);

    m_pTabsPannel = new SARibbonPannel(tr("Вкладки"));
    m_pRibbonCategory->addPannel(m_pTabsPannel);

    preInitDefaultActions();

    m_pSave = createAction(tr("Сохранить"), "Save");
    toolAddActionWithTooltip(m_pSave,
                             tr("Сохранить содержимое текущей вкладки в файл"),
                             QKeySequence::Save);
    m_pActionPannel->addLargeAction(m_pSave);

    m_pCopy = createAction(tr("Копировать"), "Copy");
    toolAddActionWithTooltip(m_pCopy,
                             tr("Копировать выделенный текст в буфер обмена"),
                             QKeySequence::Copy);
    m_pActionPannel->addSmallAction(m_pCopy);

    m_pCut = createAction(tr("Вырезать"), "Cut");
    toolAddActionWithTooltip(m_pCut,
                             tr("Вырезать выделенный текст в буфер обмена"),
                             QKeySequence::Cut);
    m_pActionPannel->addSmallAction(m_pCut);

    m_pPaste = createAction(tr("Вставить"), "Paste");
    toolAddActionWithTooltip(m_pPaste,
                             tr("Вставить текст из буфера обмена"),
                             QKeySequence::Paste);
    m_pActionPannel->addSmallAction(m_pPaste);

    m_pActionPannel->addSeparator();

    m_pUndoAction = createAction(tr("Отменить"), "Undo");
    m_pUndoAction->setShortcut(QKeySequence::Undo);
    m_pUndoAction->setEnabled(false);
    toolAddActionWithTooltip(m_pUndoAction,
                             tr("Отменить последнее действие"),
                             QKeySequence::Undo);
    m_pActionPannel->addMediumAction(m_pUndoAction);

    m_pRedoAction = createAction(tr("Повторить"), "Redo");
    m_pRedoAction->setShortcut(QKeySequence::Redo);
    m_pRedoAction->setEnabled(false);
    toolAddActionWithTooltip(m_pRedoAction,
                             tr("Повторить отмененное действие"),
                             QKeySequence::Redo);
    m_pActionPannel->addMediumAction(m_pRedoAction);

    m_pActionPannel->addSeparator();

    m_pConvertPg = createAction(tr("Конвертировать в PostgreSQL"), "DataSourceTarget");
    toolAddActionWithTooltip(m_pConvertPg, tr("Конвертировать SQL скрипт из Oracle синтаксиса в PostgreSQL"));
    m_pActionPannel->addLargeAction(m_pConvertPg);

    m_pExecuteSql = createAction(tr("Выполнить\n SQL скрипт"), "Execute");
    toolAddActionWithTooltip(m_pExecuteSql, tr("Выполнить SQL скрипт в текущей базе данных"));
    m_pActionPannel->addLargeAction(m_pExecuteSql);

    m_pWordWrap = createAction(tr("Перенос строк"), "WordWrap");
    m_pWordWrap->setCheckable(true);
    toolAddActionWithTooltip(m_pWordWrap,
                             tr("Включить/выключить перенос длинных строк по словам"));
    m_pViewPannel->addLargeAction(m_pWordWrap);

    m_pShowChars = createAction(tr("Все символы"), "Paragraph");
    m_pShowChars->setCheckable(true);
    toolAddActionWithTooltip(m_pShowChars,
                             tr("Показывать непечатаемые символы (пробелы, табуляцию, концы строк)"));
    m_pViewPannel->addLargeAction(m_pShowChars);

    m_pWordWrap->setChecked(true);

    m_pPrevTab = createAction(tr("Предыдущее окно"), "PreviousBookmark");
    toolAddActionWithTooltip(m_pPrevTab,
                             tr("Переключиться на предыдущую вкладку"),
                             QKeySequence::PreviousChild);
    m_pTabsPannel->addLargeAction(m_pPrevTab);

    m_pNextTab = createAction(tr("Следующее окно"), "NextBookmark");
    toolAddActionWithTooltip(m_pNextTab,
                             tr("Переключиться на следующую вкладку"),
                             QKeySequence::NextChild);
    m_pTabsPannel->addLargeAction(m_pNextTab);

    m_SaveConnection = connect(m_pSave, &QAction::triggered, [=]()
   {
       QMdiSubWindow *window = getWindow();

       if (!window)
           return;

       CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
       if (pCode)
       {
           int Syntax = pCode->property(FmtSyntaxProperty).toInt();
           QString filename = saveDialog(Syntax, pCode->toPlainText(), window->windowFilePath());

           if (!filename.isEmpty())
           {
               window->setWindowFilePath(filename);
               QToolTip::showText(QCursor::pos(),
                                  tr("Файл сохранен: %1").arg(filename),
                                  this, QRect(), 2000);
           }
       }
   });

    m_CopyConnection = connect(m_pCopy, &QAction::triggered, [=]()
   {
       QMdiSubWindow *window = getWindow();

       if (!window)
           return;

       CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
       if (pCode)
       {
           copyFromEdit(pCode);
           QToolTip::showText(QCursor::pos(),
                              tr("Текст скопирован в буфер обмена"),
                              this, QRect(), 1500);
       }
   });

    connect(m_pCut, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = getWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
        if (pCode && !pCode->isReadOnly())
        {
            pCode->cut();
            QToolTip::showText(QCursor::pos(),
                               tr("Текст вырезан в буфер обмена"),
                               this, QRect(), 1500);
        }
    });

    connect(m_pPaste, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = getWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
        if (pCode && !pCode->isReadOnly())
        {
            pCode->paste();
        }
    });

    connect(m_pWordWrap, &QAction::toggled, [=](bool value)
    {
        QList<QMdiSubWindow*> windows = pContainer->subWindowList();
        for (auto wnd : qAsConst(windows))
        {
            CodeEditor *pCode = qobject_cast<CodeEditor*>(wnd->widget());

            if (pCode)
                setWordWrapToEdit(pCode, value);
        }

        QToolTip::showText(QCursor::pos(),
                           value ? tr("Перенос строк включен") : tr("Перенос строк выключен"),
                           this, QRect(), 1500);
    });

    connect(m_pShowChars, &QAction::toggled, [=](bool value)
    {
        QList<QMdiSubWindow*> windows = pContainer->subWindowList();
        for (auto wnd : qAsConst(windows))
        {
            CodeEditor *pCode = qobject_cast<CodeEditor*>(wnd->widget());

            if (pCode)
                setAllCharsModeToEdit(pCode, value);
        }

        QToolTip::showText(QCursor::pos(),
                           value ? tr("Отображение непечатаемых символов включено") : tr("Отображение непечатаемых символов выключено"),
                           this, QRect(), 1500);
    });

    connect(m_pConvertPg, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = getWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());

        if (pCode)
        {
            int Syntax = pCode->property(FmtSyntaxProperty).toInt();

            if (Syntax == HighlighterSql)
            {
                SqlConversionResult result = convertSql(pCode->toPlainText());
                QString sql = result.result;

                if (!result.error.isEmpty())
                    sql = result.error;

                QString newTabName = QString("%1 (PostgreSQL)").arg(window->windowTitle());
                QMdiSubWindow *newWindow = AddTab(newTabName, sql, HighlighterSql, "DataSourceTarget");

                if (result.error.isEmpty())
                {
                    newWindow->setToolTip(tr("Результат конвертации SQL в PostgreSQL"));
                    QToolTip::showText(QCursor::pos(),
                                       tr("Конвертация завершена успешно"),
                                       this, QRect(), 2000);
                }
                else
                {
                    newWindow->setToolTip(tr("Ошибка конвертации: %1").arg(result.error));
                    QToolTip::showText(QCursor::pos(),
                                       tr("Ошибка конвертации"),
                                       this, QRect(), 2000);
                }
            }
            else
            {
                QToolTip::showText(QCursor::pos(),
                                   tr("Конвертация доступна только для SQL скриптов"),
                                   this, QRect(), 2000);
            }
        }
    });

    connect(m_pUndoAction, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = getWindow();

        if (!window) return;

        CodeEditor *pCode = editorFromWindow(window);
        if (pCode && !pCode->isReadOnly())
        {
            pCode->undo();
            updateRibbonState();
        }
    });

    connect(m_pRedoAction, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = getWindow();

        if (!window) return;

        CodeEditor *pCode = editorFromWindow(window);
        if (pCode && !pCode->isReadOnly())
        {
            pCode->redo();
            updateRibbonState();
        }
    });

    connect(m_pPrevTab, &QAction::triggered, pContainer, &QMdiArea::activatePreviousSubWindow);
    connect(m_pNextTab, &QAction::triggered, pContainer, &QMdiArea::activateNextSubWindow);
    connect(m_pNextTab, &QAction::triggered, pContainer, &QMdiArea::activateNextSubWindow);
    connect(m_pExecuteSql, &QAction::triggered, this, &FmtCodeTabBase::executeSqlAction);
}

void FmtCodeTabBase::initRibbonPanels()
{
    FmtWindowTabInterface::initRibbonPanels();

    if (!m_pRibbonCategory)
        return;

    // Настраиваем категорию
    m_pRibbonCategory->setObjectName(ribbonCategoryName());
    m_pRibbonCategory->setToolTip(tr("Инструменты для работы с кодом"));

    // Обновляем состояние
    updateRibbonState();

    parentContextCategory()->addCategoryPage(m_pRibbonCategory);

    ribbon()->hideContextCategory(parentContextCategory());
    ribbon()->showCategory(m_pRibbonCategory);
    ribbon()->showContextCategory(parentContextCategory());
}

void FmtCodeTabBase::activateRibbon()
{
    FmtWindowTabInterface::activateRibbon();
}

void FmtCodeTabBase::deactivateRibbon()
{
    FmtWindowTabInterface::deactivateRibbon();
}

void FmtCodeTabBase::setWordWrap(const bool &val)
{
    m_pWordWrap->setChecked(val);
}

QMdiSubWindow *FmtCodeTabBase::getWindow()
{
    QTabBar *tabBar = pContainer->findChild<QTabBar*>();
    if (tabBar && tabBar->count())
    {
        QList<QMdiSubWindow*> list = pContainer->subWindowList();
        return list[tabBar->currentIndex()];
    }

    return nullptr;
}

bool FmtCodeTabBase::saveSqlScriptToFile(const QString& scriptPath, const QString& sqlScript)
{
    QFile sqlFile(scriptPath);
    if (!sqlFile.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось создать SQL файл: %1").arg(sqlFile.errorString()));
        return false;
    }

    QTextStream sqlStream(&sqlFile);
    sqlStream.setCodec("IBM 866");
    sqlStream << sqlScript;
    sqlFile.close();

    return true;
}

bool FmtCodeTabBase::createBatFileForOracle(const QString& batPath, const QString& sqlPath,
                                            const QString& logPath, ConnectionInfo* connInfo)
{
    QFile batFile(batPath);
    if (!batFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось создать bat файл для Oracle: %1").arg(batFile.errorString()));
        return false;
    }

    QString user = connInfo->user();
    QString password = connInfo->password();
    QString tns = connInfo->dsn();

    QTextStream batStream(&batFile);
    batStream.setCodec("IBM 866");

    batStream << "@echo off\n";
    batStream << "chcp 866 > nul\n\n";
    batStream << "set LOG_FILE=" << logPath << "\n";
    batStream << "echo Execution started at %DATE% %TIME% > %LOG_FILE%\n";
    batStream << "echo ---------------------------------------- >> %LOG_FILE%\n\n";
    batStream << "set NLS_LANG=AMERICAN_AMERICA.RU8PC866\n";
    batStream << "echo Connecting to Oracle: " << user << "@" << tns << " >> %LOG_FILE%\n";
    batStream << "sqlplus -S " << user << "/" << password << "@" << tns
              << " @" << sqlPath << " >> %LOG_FILE% 2>&1\n\n";
    batStream << "echo ---------------------------------------- >> %LOG_FILE%\n";
    batStream << "echo Execution finished at %DATE% %TIME% >> %LOG_FILE%\n";

    batFile.close();

    return true;
}

bool FmtCodeTabBase::createBatFileForPostgres(const QString& batPath, const QString& sqlPath,
                                              const QString& logPath, ConnectionInfo* connInfo)
{
    QFile batFile(batPath);
    if (!batFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось создать bat файл для PostgreSQL: %1").arg(batFile.errorString()));
        return false;
    }

    QString user = connInfo->user();
    QString password = connInfo->password();
    QString host = connInfo->host();
    QString service = connInfo->service();
    int port = connInfo->property("port").toInt();
    if (port == 0) port = 5432; // Стандартный порт PostgreSQL

    QTextStream batStream(&batFile);
    batStream.setCodec("IBM 866");

    batStream << "@echo off\n";
    batStream << "chcp 866 > nul\n\n";
    batStream << "set LOG_FILE=" << logPath << "\n";
    batStream << "echo Execution started at %DATE% %TIME% > %LOG_FILE%\n";
    batStream << "echo ---------------------------------------- >> %LOG_FILE%\n\n";
    batStream << "set PGPASSWORD=" << password << "\n";
    batStream << "echo Connecting to PostgreSQL: " << user << "@" << host << ":" << port << "/" << service << " >> %LOG_FILE%\n";
    batStream << "psql -h " << host << " -p " << port << " -U " << user << " -d " << service
              << " -f " << sqlPath << " >> %LOG_FILE% 2>&1\n";
    batStream << "set PGPASSWORD=\n\n";
    batStream << "echo ---------------------------------------- >> %LOG_FILE%\n";
    batStream << "echo Execution finished at %DATE% %TIME% >> %LOG_FILE%\n";

    batFile.close();
    return true;
}

QString FmtCodeTabBase::executeBatFileAndGetLog(const QString& batPath, const QString& logPath,
                                                ConnectionInfo* connInfo, const QString& tempPath)
{
    QProcess process;
    QProgressDialog *progressDlg = new QProgressDialog(
        tr("Выполнение SQL скрипта...\nПодключение: %1\nПожалуйста, подождите...")
            .arg(connInfo->typeName()),
        tr("Отмена"),
        0, 0, this);
    progressDlg->setWindowTitle(tr("Выполнение SQL"));
    progressDlg->setModal(true);
    progressDlg->setMinimumDuration(0);
    progressDlg->setAutoReset(false);
    progressDlg->setAutoClose(false);

    QString logContent;
    bool hasError = false;
    QString errorDetails;

    // Запускаем процесс
    process.start(batPath);

    if (!process.waitForStarted(5000))
    {
        delete progressDlg;
        QMessageBox::critical(this, tr("Ошибка"), tr("Не удалось выполнить скрипт"));
        return QString();
    }

    progressDlg->show();

    // Обработка отмены
    connect(progressDlg, &QProgressDialog::canceled, [&]() {
        process.kill();
        process.waitForFinished(3000);
        QMessageBox::information(this, tr("Отмена"), tr("Выполнение SQL скрипта отменено"));
    });

    // Ждем завершения (таймаут 10 минут)
    if (!process.waitForFinished(10 * 60 * 1000))
    {
        process.terminate();
        progressDlg->close();
        QMessageBox::warning(this, tr("Таймаут"),
                             tr("Превышено время ожидания выполнения скрипта (10 минут)"));
        return QString();
    }

    progressDlg->close();

    // Читаем лог файл
    QFile logFile(logPath);
    if (logFile.open(QIODevice::ReadOnly))
    {
        QTextStream logStream(&logFile);
        logStream.setCodec("IBM 866");
        logContent = logStream.readAll();
        logFile.close();
    }
    else
    {
        logContent = tr("Не удалось прочитать лог файл: %1").arg(logFile.errorString());
        hasError = true;
    }

    // Анализируем результат выполнения
    if (process.exitStatus() == QProcess::CrashExit)
    {
        hasError = true;
        errorDetails = tr("Процесс аварийно завершился");
    }
    else if (process.exitCode() != 0)
    {
        hasError = true;
        errorDetails = tr("Процесс завершился с кодом ошибки: %1").arg(process.exitCode());
    }

    // Анализируем лог на наличие ошибок SQL
    if (logContent.contains("ERROR", Qt::CaseInsensitive) ||
        logContent.contains("ORA-", Qt::CaseInsensitive) ||
        logContent.contains("SQL error", Qt::CaseInsensitive) ||
        logContent.contains("syntax error", Qt::CaseInsensitive) ||
        logContent.contains("ERROR:", Qt::CaseInsensitive))
    {
        hasError = true;
        if (errorDetails.isEmpty())
            errorDetails = tr("Обнаружены ошибки в SQL скрипте");
    }

    if (hasError && !errorDetails.isEmpty())
    {
        logContent = tr("=== ОШИБКА: %1 ===\n\n").arg(errorDetails) + logContent;
    }

    delete progressDlg;

    return logContent;
}

void FmtCodeTabBase::showExecutionResult(bool hasError, const QString& logContent,
                                         const QString& tempPath, const QString& logFilePath,
                                         ConnectionInfo* connInfo, int dbType)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(hasError ? tr("Выполнение SQL завершено с ошибками")
                                   : tr("Выполнение SQL завершено успешно"));

    if (hasError)
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("SQL скрипт выполнен с ошибками"));
        msgBox.setInformativeText(tr("Подробности в детальной информации"));
    }
    else
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("SQL скрипт выполнен успешно"));

        // Пытаемся подсчитать количество выполненных операторов
        int successCount = logContent.count("rows affected");
        if (successCount == 0)
            successCount = logContent.count("INSERT");
        if (successCount == 0)
            successCount = logContent.count("UPDATE");
        if (successCount == 0)
            successCount = logContent.count("DELETE");
        if (successCount == 0)
            successCount = logContent.count("CREATE");

        if (successCount > 0)
        {
            msgBox.setInformativeText(tr("Выполнено операторов: %1").arg(successCount));
        }
    }

    // Формируем детальную информацию
    QString detailedText = tr("Тип БД: %1\n").arg(connInfo->typeName());
    detailedText += tr("Пользователь: %1\n").arg(connInfo->user());

    if (dbType == ConnectionInfo::CON_ORA)
        detailedText += tr("TNS: %1\n").arg(connInfo->dsn());
    else if (dbType == ConnectionInfo::CON_POSTGRESQL)
        detailedText += tr("База данных: %1\n").arg(connInfo->service());
    else if (dbType == ConnectionInfo::CON_SQLITE)
        detailedText += tr("Файл БД: %1\n").arg(connInfo->dsn());

    detailedText += tr("\nВременная директория: %1\n").arg(tempPath);
    detailedText += tr("Лог файл: %1\n\n").arg(logFilePath);
    detailedText += tr("========== СОДЕРЖИМОЕ ЛОГА ==========\n");
    detailedText += logContent;

    // Ограничиваем размер детального текста (макс 1MB)
    if (detailedText.size() > 1024 * 1024)
    {
        detailedText = detailedText.left(1024 * 1024 - 1000);
        detailedText += "\n\n... (лог урезан до 1MB) ...\n";
        detailedText += tr("Полный лог доступен по пути: %1").arg(logFilePath);
    }

    msgBox.setDetailedText(detailedText);

    // Добавляем кнопку для открытия папки с логами
    QPushButton *openFolderButton = msgBox.addButton(tr("Открыть папку с логами"),
                                                     QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Ok);

    msgBox.exec();

    if (msgBox.clickedButton() == openFolderButton)
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(tempPath));
    }
}

void FmtCodeTabBase::executeSqlScript(const QString& sqlScript, ConnectionInfo* connInfo)
{
    if (!connInfo || !connInfo->isOpen())
    {
        QMessageBox::warning(this, tr("Ошибка"),
                             tr("Нет активного подключения к базе данных"));
        return;
    }

    // Создаем временную директорию
    QTemporaryDir tempDir;
    if (!tempDir.isValid())
    {
        QMessageBox::critical(this, tr("Ошибка"),
                              tr("Не удалось создать временную директорию"));
        return;
    }

    QString tempPath = tempDir.path();
    QString sqlFileName = tempPath + "/script.sql";
    QString batFileName = tempPath + "/run_sql.bat";
    QString logDirPath = tempPath + "/log";
    QString logFilePath = logDirPath + "/script.log";

    // Создаем директорию для логов
    QDir().mkpath(logDirPath);

    // Сохраняем SQL скрипт
    if (!saveSqlScriptToFile(sqlFileName, sqlScript))
        return;

    // Создаем bat файл в зависимости от типа БД
    int dbType = connInfo->type();
    bool batCreated = false;

    if (dbType == ConnectionInfo::CON_ORA)
    {
        batCreated = createBatFileForOracle(batFileName, sqlFileName, logFilePath, connInfo);
    }
    else if (dbType == ConnectionInfo::CON_POSTGRESQL)
    {
        batCreated = createBatFileForPostgres(batFileName, sqlFileName, logFilePath, connInfo);
    }
    /*else if (dbType == ConnectionInfo::CON_SQLITE)
    {
        batCreated = createBatFileForSqlite(batFileName, sqlFileName, logFilePath, connInfo);
    }*/
    else
    {
        QMessageBox::warning(this, tr("Предупреждение"),
                             tr("Тип базы данных '%1' не поддерживается для автоматического выполнения")
                                 .arg(connInfo->typeName()));
        return;
    }

    if (!batCreated)
        return;

    // Выполняем bat файл
    QString logContent = executeBatFileAndGetLog(batFileName, logFilePath, connInfo, tempPath);
    if (logContent.isEmpty())
        return;

    // Определяем наличие ошибок
    bool hasError = logContent.contains("ERROR", Qt::CaseInsensitive) ||
                    logContent.contains("ORA-", Qt::CaseInsensitive) ||
                    logContent.contains("SQL error", Qt::CaseInsensitive) ||
                    logContent.contains("syntax error", Qt::CaseInsensitive) ||
                    logContent.contains("ERROR:", Qt::CaseInsensitive) ||
                    logContent.contains("failed", Qt::CaseInsensitive);

    // Показываем результат
    showExecutionResult(hasError, logContent, tempPath, logFilePath, connInfo, dbType);
}

void FmtCodeTabBase::executeSqlAction()
{
    QMdiSubWindow *window = getWindow();

    if (!window)
    {
        QToolTip::showText(QCursor::pos(),
                           tr("Нет активного окна с кодом"),
                           this, QRect(), 2000);
        return;
    }

    CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());

    if (!pCode)
    {
        QToolTip::showText(QCursor::pos(),
                           tr("Активное окно не является редактором кода"),
                           this, QRect(), 2000);
        return;
    }

    int Syntax = pCode->property(FmtSyntaxProperty).toInt();

    if (Syntax != HighlighterSql)
    {
        QToolTip::showText(QCursor::pos(),
                           tr("Выполнение SQL доступно только для SQL скриптов"),
                           this, QRect(), 2000);
        return;
    }

    QString sqlScript = pCode->toPlainText();

    if (sqlScript.trimmed().isEmpty())
    {
        QToolTip::showText(QCursor::pos(),
                           tr("SQL скрипт пуст"),
                           this, QRect(), 2000);
        return;
    }

    // Получаем текущее подключение к БД
    ConnectionInfo *connInfo = connection();

    // Выполняем скрипт
    executeSqlScript(sqlScript, connInfo);
}