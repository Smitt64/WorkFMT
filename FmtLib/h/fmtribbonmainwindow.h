#ifndef FMTRIBBONMAINWINDOW_H
#define FMTRIBBONMAINWINDOW_H

#include <QObject>
#include <SARibbon.h>
#include <QKeySequence>
#include <VarLocker.hpp>
#include "fmtlib_global.h"

class QMdiArea;
class TablesDock;
class QStatusBar;
class QAction;
class FmtTableListDelegate;
class ConnectionInfo;
class TreeComboBox;
class SubWindowsModel;
class QMdiSubWindow;
class FmtWorkWindow;
class FmtTable;
class MDIProxyStyle;
class MdiSubInterface;
class FMTLIBSHARED_EXPORT FmtRibbonMainWindow : public SARibbonMainWindow
{
    using WorkWindowList = QList<QWidget*>;
public:
    FmtRibbonMainWindow(QWidget *parent = nullptr);
    virtual ~FmtRibbonMainWindow();

    void OpenConnection(const QString &connectionString);

    QMdiSubWindow *CreateMdiWindow(MdiSubInterface *window, ConnectionInfo *pConnection);
    QMdiSubWindow *CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow = Q_NULLPTR);

    void UpdateActions();

    ConnectionInfo *CurrentConnection();

    void DisconnectCurrent();

private slots:
    void ActionConnectTriggered();
    void OpenConnectionFile();
    void StartGuiConverter();
    void TableClicked(const quint32 &id);
    void UnloadSqlite();

protected:
    //virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;
    virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    virtual void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    void ConnectionActionSelected(QAction *action);
    ConnectionInfo* openConnection();
    QAction *CreateConnectionActio(ConnectionInfo *info);

    void SetActiveFmtWindow(QMdiSubWindow *wnd);
    QMdiSubWindow *HasTableWindow(const QString &tableName);
    QMdiSubWindow *HasTableWindow(const quint64 &tableID);

    void setupAction(QAction *act, const QString& text, const QString& iconname, const QKeySequence &key = QKeySequence());

    template<class T = QAction>
    T* createAction(const QString& text, const QString& iconname = QString(), const QKeySequence &key = QKeySequence())
    {
        T *action = new T(this);
        setupAction(action, text, iconname, key);
        return action;
    }

    void InitWindowsCombo();

    void InitQuickAccessBar();
    void InitMainRibbonTab();
    void InitContextCategoryes();

    QMdiArea *pMdi;
    QMdiSubWindow *m_LastActiveWindow;
    VarLocker<QString> m_LastRibbonTabName;

    TablesDock *pTablesDock;
    FmtTableListDelegate *pTableListDelegate;
    QStatusBar *m_pStatusBar;

    SubWindowsModel *pWindowsModel;
    TreeComboBox *pWindowsComboBox;

    SARibbonLineEdit *pSearchLine;

    QMap<ConnectionInfo*, WorkWindowList> m_Windows;
    QList<ConnectionInfo*> m_pConnections;

    QAction *m_pActionConnect, *m_pActionDisconnect, *m_pOpenConnection, *m_pSqliteAction;
    QAction *m_pActionImport, *m_pActionImportDir, *m_pActionExport;

    QMenu *m_pMenuCreate;
    QAction *m_pActionCreate, *m_pActionCreateGroup, *m_pActionCreateText, *m_pActionCreateXml;
    QAction *m_pActionCopyTable, *m_pActionCopyTableTmp, *m_pActionCopyTableTo;

    QAction *m_pActionDumpTool, *m_pActionDiffTool, *m_pActionGuiConverter, *m_pActionDebug, *m_pActionConvertScript;

    SARibbonGallery* m_pConnectionsGallery;
    SARibbonGalleryGroup *m_pConnectionsGalleryGroup;
};

#endif // FMTRIBBONMAINWINDOW_H
