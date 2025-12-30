#ifndef MDISUBINTERFACE_H
#define MDISUBINTERFACE_H

#include "fmtlib_global.h"
#include <QtWidgets>
#include "fmttable.h"

class SARibbonBar;
class ConnectionInfo;
class SARibbonContextCategory;
class FMTLIBSHARED_EXPORT MdiSubInterface : public QDialog
{
    Q_OBJECT
public:
    MdiSubInterface(QWidget *parent = Q_NULLPTR);
    virtual ~MdiSubInterface();

    void setParentWnd(QMdiSubWindow *wnd);
    QMdiSubWindow *mdiWindow() { return pParentWnd; }

    virtual QString makeWindowTitle() = 0;

    ConnectionInfo *connection() const;
    void setConnection(ConnectionInfo *Connection);

    void setRibbonBar(SARibbonBar *ribbon);
    SARibbonBar *ribbon();

    SARibbonContextCategory *findCategoryByName(const QString &name);

    virtual void updateRibbonTabs();
    virtual void clearRibbonTabs();
    virtual QList<QWidget*> statusBarSections();

    void setupAction(QAction *act, const QString& text, const QString& iconname);

    template<class T = QAction>
    T* createAction(const QString& text, const QString& iconname)
    {
        T *action = new T(this);
        setupAction(action, text, iconname);
        return action;
    }

signals:
    void needUpdateTableList();

protected:
    virtual void initRibbonPanels();

    QMdiSubWindow *pParentWnd;
    ConnectionInfo *pConnection;

private:
    SARibbonBar *m_pRibbon;
};

#endif // MDISUBINTERFACE_H
