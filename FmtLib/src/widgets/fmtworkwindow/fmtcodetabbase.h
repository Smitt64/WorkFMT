#ifndef FMTCODETABBASE_H
#define FMTCODETABBASE_H

#include <QMap>
#include <QPointer>
#include "fmtcore.h"
#include "fmtwindowtabinterface.h"

#define FmtSyntaxProperty "Syntax"

class QMdiArea;
class GenInterfaceFactoryModel;
class CodeEditor;
class Highlighter;
class GeneratorsProxyModel;
class QMdiSubWindow;
class SARibbonCategory;
class SARibbonPannel;
class FmtCodeTabBase : public FmtWindowTabInterface
{
    Q_OBJECT
public:
    FmtCodeTabBase(QWidget *parent = nullptr);
    virtual ~FmtCodeTabBase();

    QMdiSubWindow *AddTab(const QString &tabname, const QString &icon = "Code");
    QMdiSubWindow *AddTab(const QString &tabname, const QString &code, const qint16 &Syntax, const QString &icon = "Code");

    CodeEditor *editorFromWindow(QMdiSubWindow *wnd);

    QMdiSubWindow *getWindow();

    virtual QString ribbonCategoryName() const override;

    virtual void initRibbonPanels() override;
    virtual void activateRibbon() override;
    virtual void deactivateRibbon() override;

    void setWordWrap(const bool &val);

private slots:
    void undoUpdate(bool available);
    void redoUpdate(bool available);
    void executeSqlAction();

protected:
    virtual void preInitDefaultActions();
    virtual void updateRibbonState();
    void setHighlighter(CodeEditor *edidor, const qint16 &Syntax);

    bool saveSqlScriptToFile(const QString& scriptPath, const QString& sqlScript);
    bool createBatFileForOracle(const QString& batPath, const QString& sqlPath, const QString& logPath, ConnectionInfo* connInfo);
    bool createBatFileForPostgres(const QString& batPath, const QString& sqlPath, const QString& logPath, ConnectionInfo* connInfo);
    QString executeBatFileAndGetLog(const QString& batPath, const QString& logPath, ConnectionInfo* connInfo, const QString& tempPath);
    void showExecutionResult(bool hasError, const QString& logContent,
                             const QString& tempPath, const QString& logFilePath,
                             ConnectionInfo* connInfo, int dbType);
    void executeSqlScript(const QString& sqlScript, ConnectionInfo* connInfo);

    QMdiArea *pContainer;

    SARibbonPannel* m_pActionPannel, *m_pViewPannel, *m_pTabsPannel;
    QAction *m_pSave, *m_pCopy, *m_pCut, *m_pPaste, *m_pWordWrap, *m_pShowChars;
    QAction *m_pUndoAction, *m_pRedoAction;

    QMdiSubWindow *pLastActiveWindow;
    QMetaObject::Connection m_SaveConnection, m_CopyConnection;

    QAction *m_pConvertPg, *m_pExecuteSql;
    QAction *m_pPrevTab, *m_pNextTab;

    QMap<QString, QMdiSubWindow*> m_pWindows;
    QList<QMdiSubWindow*> m_pWindowsList;

private:
    void initDefaultPanel() override;
};

#endif // FMTCODETABBASE_H
