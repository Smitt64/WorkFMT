#ifndef FMTSCRIPTWINDOW_H
#define FMTSCRIPTWINDOW_H

#include <QtWidgets>
//#include <QtScript>
//#include <QtScriptTools>
#include "fmttable.h"

class CodeEditor;
class Highlighter;
class FmtScriptWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit FmtScriptWindow(QSharedPointer<FmtTable> &table, QWidget *parent = nullptr);
    virtual ~FmtScriptWindow();

signals:

private slots:
    void OnNew();
    void OnExecute();
    void OnExecuteDebug();
    void OnOpen();
    void OnSave();

private:
    bool save();
    void ExecuteEx(bool useDebug);
    void CreateActions();
    void CreateMdi();
    void CreateScriptEngine();
    void SetupScriptEngineValues(CodeEditor *pEditor);
    QToolBar *m_pToolBar;
    QAction *pNew, *pOpen, *pSave, *pExecute, *pExecuteDebug, *pClear;
    QSplitter *pSplitter;
    CodeEditor *pOutput;

    CodeEditor *pEditor;
    //QScriptEngine *pEngine;
    //QScriptEngineDebugger dbg;
    QSharedPointer<FmtTable> pTable;
};

#endif // FMTSCRIPTWINDOW_H
