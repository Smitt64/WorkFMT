#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "fmtlib_global.h"
#include "updatechecker.h"

namespace Ui {
class MainWindow;
}

class TablesDock;
class FmtTable;
class ConnectionInfo;
class FmtWorkWindow;
class TreeComboBox;
class SubWindowsModel;
class WindowsComboAction;
class FmtTableListDelegate;
class MdiSubInterface;
class SearchActionWidget;
class ToolbarActionExecutor;
class FMTLIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QList<QWidget*> WorkWindowList;
public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    ConnectionInfo *currentConnection();
    void OpenConnection(const QString &connectionString);

    QMdiSubWindow *hasTableWindow(const QString &tableName);
    QMdiSubWindow *hasTableWindow(const quint64 &tableID);

    FmtWorkWindow *currentWorkWindow();

    QMdiSubWindow *CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow = Q_NULLPTR);
    QMdiSubWindow *CreateMdiWindow(MdiSubInterface *window, ConnectionInfo *pConnection);

    const QList<ConnectionInfo*> &connections() const;
    const QMap<ConnectionInfo*, WorkWindowList> &windows() const;
    WorkWindowList windows(ConnectionInfo* info) const;

    ConnectionInfo* openConnection();
    bool isExistsConnection(ConnectionInfo *connection);
    bool addConnection(ConnectionInfo *connection);

    void SetActiveFmtWindow(QMdiSubWindow *wnd);
    QMdiSubWindow *currentMdiWindow();

public slots:
    void actionConnectTriggered();

private slots:
    void actionDisconnectTriggered();
    void actionCreate();
    void tableClicked(const quint32 &id);
    void conActionTriggered(QAction *action);
    void rebuidOffsets();
    void createFromText();
    void actionEditFmt();
    void actionExportTableXml();
    void actionInit();
    void WorkWindowDestroyed(QObject *wnd);
    void subWindowActivated(QMdiSubWindow *window);
    void subWindowIndexChanged(const QModelIndex &index);
    void showWindowList();
    //void setSubWindowIcon(const QIcon &icon);
    void about();

    void ImpDirAction();
    void ImportAction();
    void CreateTableSql();
    void HotFixCreate();
    void OpenRecentConnection();
    void CopyTable();
    void CopyTableTo();
    void CopyTableToTmp();
    void UnloadDbf();
    void LoadDbf();
    void UnloadSqlite();
    void OpenConnection();
    void RemoveFmtTable();
    void EditContent();
    void GenCreateTableScript();
    void GenModifyTableFields();
    void GenAddFiledsScript();
    void GenDeleteFiledsScript();
    void GenDiffToScriptScript();
    void OnTableChangeUpdtList();
    void OnMassOpAction();
    void OnConfluence();
    void CreateFromXml();
    void UpdateCheckFinished(bool hasUpdates, const CheckDataList &updatedata);
    void UpdateCheckStarted();
    void OptionsAction();
    void StartGuiConverter();
    void onUserActionTriggered();
    void on_actionDebug_triggered();

protected:
    virtual void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void CreateMainToolBar();
    void CreateWindowFunctional();
    void UpdateActions();
    void CreateWindowsCombo();
    void CreateViewMenu();
    void CreateSearchToolBar();

    void CreateCheckUpdateRunnable();

    QAction *CreateConnectionActio(ConnectionInfo *info);
    void tablesContextMenu(QContextMenuEvent *event, QListView *view);
    Ui::MainWindow *ui;
    TablesDock *pTablesDock;
    QMdiArea *pMdi;

    QLineEdit *pSearchLine;
    //QToolBar *pSearch;
    QMenu *pSearch;
    SearchActionWidget *pSearchWidget;

    FmtTableListDelegate *pTableListDelegate;
    QShortcut *pFindShortcut;

    TreeComboBox *pWindowsComboBox;

    QMap<ConnectionInfo*, WorkWindowList> m_Windows;
    QList<ConnectionInfo*> m_pConnections;

    QAction *actionEdit, *actionExport, *actionDeleteTable;
    QToolButton *toolConnect;
    QMenu *toolConnectMenu, *pUserActionsMenu;
    QActionGroup *m_ConnectionsGroup;

    QPushButton *pUpdateButton;

    SubWindowsModel *pWindowsModel;
    UpdateChecker *pUpdateChecker;

    ToolbarActionExecutor *pActionExecutor;
};

#endif // MAINWINDOW_H
