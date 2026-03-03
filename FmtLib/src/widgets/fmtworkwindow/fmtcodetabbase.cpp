#include "fmtcodetabbase.h"
#include <codeeditor/codeeditor.h>
#include <codeeditor/codehighlighter.h>
#include "toolsqlconverter.h"
#include <QMdiArea>
#include <QMdiSubWindow>
#include <SARibbon.h>

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
    m_pConvertPg(nullptr),
    m_pPrevTab(nullptr),
    m_pNextTab(nullptr)
{
    pContainer = new QMdiArea(this);
    pContainer->setDocumentMode(true);
    pContainer->setViewMode(QMdiArea::TabbedView);
    pContainer->setTabsClosable(false);
    setCentralWidget(pContainer);

    connect(pContainer, &QMdiArea::subWindowActivated, [=](QMdiSubWindow *window)
    {
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

void FmtCodeTabBase::setHighlighter(CodeEditor *edidor, const qint16 &Syntax)
{
    edidor->setProperty(FmtSyntaxProperty, Syntax);
    ToolApplyHighlighter(edidor, Syntax, FmtCodeTabStyle);
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
    setWordWrapToEdit(pCode, m_pWordWrap ? m_pWordWrap->isChecked() : true);
    setAllCharsModeToEdit(pCode, m_pShowChars ? m_pShowChars->isChecked() : false);

    QMdiSubWindow *mdi = pContainer->addSubWindow(pCode);
    mdi->setWindowTitle(tabname);
    mdi->setAttribute(Qt::WA_DeleteOnClose);
    mdi->setWindowIcon(QIcon::fromTheme(icon));
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
    setWordWrapToEdit(pCode, m_pWordWrap ? m_pWordWrap->isChecked() : true);
    setAllCharsModeToEdit(pCode, m_pShowChars ? m_pShowChars->isChecked() : false);
    setHighlighter(pCode, Syntax);
    pCode->rehighlight();

    QMdiSubWindow *mdi = pContainer->addSubWindow(pCode);
    mdi->setWindowTitle(tabname);
    mdi->setAttribute(Qt::WA_DeleteOnClose);
    mdi->setWindowIcon(QIcon::fromTheme(icon));
    mdi->showMaximized();

    m_pWindows.insert(tabname, mdi);
    m_pWindowsList.append(mdi);

    return mdi;
}

void FmtCodeTabBase::updateRibbonState()
{
    m_pCut->setEnabled(false);
    m_pPaste->setEnabled(false);

    QMdiSubWindow *Current = pContainer->currentSubWindow();
    if (!Current)
    {
        m_pSave->setEnabled(false);
        m_pCopy->setEnabled(false);
        m_pConvertPg->setEnabled(false);
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
    QMdiSubWindow *Current = pContainer->currentSubWindow();

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
    QMdiSubWindow *Current = pContainer->currentSubWindow();

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
    m_pActionPannel->addLargeAction(m_pSave);

    m_pCopy = createAction(tr("Копировать"), "Copy");
    m_pActionPannel->addSmallAction(m_pCopy);

    m_pCut = createAction(tr("Вырезать"), "Cut");
    m_pActionPannel->addSmallAction(m_pCut);

    m_pPaste = createAction(tr("Вставить"), "Paste");
    m_pActionPannel->addSmallAction(m_pPaste);

    m_pActionPannel->addSeparator();

    m_pUndoAction = createAction(tr("Отменить"), "Undo");
    m_pUndoAction->setShortcut(QKeySequence::Undo);
    m_pUndoAction->setEnabled(false);
    m_pActionPannel->addMediumAction(m_pUndoAction);

    m_pRedoAction = createAction(tr("Повторить"), "Redo");
    m_pRedoAction->setShortcut(QKeySequence::Redo);
    m_pRedoAction->setEnabled(false);
    m_pActionPannel->addMediumAction(m_pRedoAction);

    m_pActionPannel->addSeparator();

    m_pConvertPg = createAction(tr("Конвертировать в PostgreSQL"), "DataSourceTarget");
    m_pActionPannel->addLargeAction(m_pConvertPg);

    m_pWordWrap = createAction(tr("Перенос строк"), "WordWrap");
    m_pWordWrap->setCheckable(true);
    //m_pWordWrap->setChecked(true);
    m_pViewPannel->addLargeAction(m_pWordWrap);

    m_pShowChars = createAction(tr("Все символы"), "Paragraph");
    m_pShowChars->setCheckable(true);
    m_pViewPannel->addLargeAction(m_pShowChars);

    m_pWordWrap->setChecked(true);

    m_pPrevTab = createAction(tr("Предыдущее окно"), "PreviousBookmark");
    m_pTabsPannel->addLargeAction(m_pPrevTab);

    m_pNextTab = createAction(tr("Следующее окно"), "NextBookmark");
    m_pTabsPannel->addLargeAction(m_pNextTab);

    m_SaveConnection = connect(m_pSave, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = pContainer->currentSubWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
        if (pCode)
        {
            int Syntax = pCode->property(FmtSyntaxProperty).toInt();
            QString filename = saveDialog(Syntax, pCode->toPlainText(), window->windowFilePath());

            if (!filename.isEmpty())
                window->setWindowFilePath(filename);
        }
    });

    m_CopyConnection = connect(m_pCopy, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = pContainer->currentSubWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());
        if (pCode)
            copyFromEdit(pCode);
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
    });

    connect(m_pConvertPg, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = pContainer->currentSubWindow();

        if (!window)
            return;

        CodeEditor *pCode = qobject_cast<CodeEditor*>(window->widget());

        if (pCode)
        {
            int Syntax = pCode->property(FmtSyntaxProperty).toInt();

            if (Syntax == HighlighterSql)
            {
                SqlConversionResult result = convertSql(pCode->toPlainText()/*,pTable->connection()->user()*/);
                QString sql = result.result;

                if (!result.error.isEmpty())
                    sql = result.error;

                AddTab(QString("%1 (PostgreSQL)").arg(window->windowTitle()), sql);
            }
        }
    });

    connect(m_pUndoAction, &QAction::triggered, [=]()
    {
        QMdiSubWindow *window = pContainer->currentSubWindow();
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
        QMdiSubWindow *window = pContainer->currentSubWindow();
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

    updateRibbonState();
}

void FmtCodeTabBase::initRibbonPanels()
{
    FmtWindowTabInterface::initRibbonPanels();

    if (!m_pRibbonCategory)
        return;

    // Настраиваем категорию
    m_pRibbonCategory->setObjectName(ribbonCategoryName());

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
