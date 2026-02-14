#ifndef FMTCODETABBASE_H
#define FMTCODETABBASE_H

#include <QMap>
#include "fmtwindowtabinterface.h"

#define FmtSyntaxProperty "Syntax"
#define FmtCodeTabStyle "office2013_highlighter"

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

    virtual QString ribbonCategoryName() const override;

    virtual void initRibbonPanels() override;
    virtual void activateRibbon() override;
    virtual void deactivateRibbon() override;

    void setWordWrap(const bool &val);

protected:
    virtual void preInitDefaultActions();
    virtual void updateRibbonState();
    void setHighlighter(CodeEditor *edidor, const qint16 &Syntax);
    QMdiArea *pContainer;

    SARibbonPannel* m_pActionPannel, *m_pViewPannel, *m_pTabsPannel;
    QAction *m_pSave, *m_pCopy, *m_pCut, *m_pPaste, *m_pWordWrap, *m_pShowChars;

    QAction *m_pConvertPg;
    QAction *m_pPrevTab, *m_pNextTab;

    QMap<QString, QMdiSubWindow*> m_pWindows;
    QList<QMdiSubWindow*> m_pWindowsList;

private:
    void initDefaultPanel() override;
};

#endif // FMTCODETABBASE_H
