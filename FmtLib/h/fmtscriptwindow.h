#ifndef FMTSCRIPTWINDOW_H
#define FMTSCRIPTWINDOW_H

#include <QtWidgets>
//#include <QtScript>
//#include <QtScriptTools>
#include "fmttable.h"
#include "src/widgets/fmtworkwindow/fmtcodetabbase.h"

#define WndTypeCode 1
#define WndTypeOutput 2

#define Prop_WndType "WndType"
#define Prop_LinkedWnd "LinkedWnd"

class CodeEditor;
class Highlighter;
class SubWindowEventFilter;
class SARibbonPannel;
class FmtScriptWindow : public FmtCodeTabBase
{
    Q_OBJECT
public:
    explicit FmtScriptWindow(QSharedPointer<FmtTable> &table, QWidget *parent = nullptr);
    virtual ~FmtScriptWindow();

    void closeLinkedWindow(QMdiSubWindow *window);
    QString ribbonCategoryName() const override;

    virtual void initRibbonPanels() override;
    virtual void activateRibbon() override;
    virtual void deactivateRibbon() override;

protected:
    virtual void preInitDefaultActions() override;
    virtual void setupRibbonActions() override;
    virtual void updateRibbonState() override;

private slots:
    void OnNew();
    void OnOpen();
    /*void OnExecute();
    void OnExecuteDebug();

    void OnSave();*/

private:
    void ExecuteEx(bool useDebug);

    QMdiSubWindow *outputWnd(QMdiSubWindow *CodeWnd);
    /*bool save();

    void CreateActions();
    void CreateMdi();
    void CreateScriptEngine();
    void SetupScriptEngineValues(CodeEditor *pEditor);
    QToolBar *m_pToolBar;
    QAction *pNew, *pOpen, *pSave, *pExecute, *pExecuteDebug, *pClear;
    QSplitter *pSplitter;
    CodeEditor *pOutput;

    CodeEditor *pEditor;*/
    //QScriptEngine *pEngine;
    //QScriptEngineDebugger dbg;

    SARibbonPannel *m_pRslPanel;
    QAction *m_pRunAction, *m_pClearOutput, *m_pCreateAction, *m_pOpenAction;
    QSharedPointer<FmtTable> pTable;
    SubWindowEventFilter *m_pEventFilter;
};

#endif // FMTSCRIPTWINDOW_H
