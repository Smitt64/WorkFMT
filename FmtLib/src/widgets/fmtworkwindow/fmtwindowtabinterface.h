#ifndef FMTWINDOWTABINTERFACE_H
#define FMTWINDOWTABINTERFACE_H

#include <QMainWindow>

class SARibbonBar;
class SARibbonPannel;
class SARibbonCategory;
class SARibbonContextCategory;
class QPlainTextEdit;
class FmtWindowTabInterface : public QMainWindow
{
    Q_OBJECT
public:
    explicit FmtWindowTabInterface(QWidget *parent = nullptr);
    virtual ~FmtWindowTabInterface();

    void setRibbonBar(SARibbonBar *ribbon);
    void setParentContextCategory(SARibbonContextCategory *contextCategory);
    SARibbonBar *ribbon() const;
    SARibbonContextCategory *parentContextCategory() const;

    virtual void initRibbonPanels();
    virtual void activateRibbon();
    virtual void deactivateRibbon();

    virtual QString ribbonCategoryName() const;

    void addCategoryToParentContext();
    void removeCategoryFromParentContext();

    virtual QList<SARibbonCategory*> categoryList();

    void setupAction(QAction *act, const QString& text, const QString& iconname);

    template<class T = QAction>
    T* createAction(const QString& text, const QString& iconname)
    {
        T *action = new T(this);
        setupAction(action, text, iconname);
        return action;
    }

protected:
    virtual void setupRibbonActions();
    virtual void initDefaultPanel();
    void setWordWrapToEdit(QPlainTextEdit *pEdit, bool flag);
    void setAllCharsModeToEdit(QPlainTextEdit *pEdit, bool flag);

    void saveDialog(const int &ContentType, const QString &text);

    void copyFromEdit(QPlainTextEdit *pEdit);
    void showCopyNotification(QPlainTextEdit *pEdit, const QString &text, bool hasSelection = false);

    SARibbonBar *m_pRibbon;
    SARibbonContextCategory *m_pParentContextCategory;
    SARibbonCategory *m_pRibbonCategory;
    bool m_CategoryAddedToContext;

signals:
};

#endif // FMTWINDOWTABINTERFACE_H
