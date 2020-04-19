#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "errordlg.h"
#include "fmtlib_global.h"

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
class FMTLIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QList<QWidget*> WorkWindowList;
public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    ConnectionInfo *currentConnection();
    void OpenConnection(const QString &connectionString);

private slots:
    void actionConnectTriggered();
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

    void ImpExpSettings();
    void ImpDirAction();
    void ImportAction();
    void CreateTableSql();
    void HotFixCreate();
    void EditGroups();
    void OpenRecentConnection();
    void CopyTable();
    void CopyTableTo();
    void RsExpExportDir();
    void UnloadDbf();
    void LoadDbf();
    void UnloadSqlite();
    void OpenConnection();
    void RemoveFmtTable();
    void LoggingSettings();
    void EditContent();
    void GenCreateTableScript();
    void GenModifyTableFields();
    void GenAddFiledsScript();
    void GenDeleteFiledsScript();
    void OnTableChangeUpdtList();
    void OnMassOpAction();
    void OnConfluence();

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
    void SetActiveFmtWindow(QMdiSubWindow *wnd);
    QMdiSubWindow *hasTableWindow(const QString &tableName);
    QMdiSubWindow *hasTableWindow(const FmtRecId &tableID);
    QAction *CreateConnectionActio(const QString &ShemeName, ConnectionInfo *info);
    void tablesContextMenu(QContextMenuEvent *event, QListView *view);
    QMdiSubWindow *CreateDocument(QSharedPointer<FmtTable> &table, FmtWorkWindow **pWindow = Q_NULLPTR);
    QMdiSubWindow *CreateMdiWindow(MdiSubInterface *window, ConnectionInfo *pConnection);
    Ui::MainWindow *ui;
    TablesDock *pTablesDock;
    QMdiArea *pMdi;

    QLineEdit *pSearchLine;
    QToolBar *pSearch;
    FmtTableListDelegate *pTableListDelegate;
    QShortcut *pFindShortcut;

    TreeComboBox *pWindowsComboBox;

    QMap<ConnectionInfo*, WorkWindowList> m_Windows;
    QList<ConnectionInfo*> m_pConnections;

    QAction *actionEdit, *actionExport, *actionDeleteTable;
    QToolButton *toolConnect;
    QMenu *toolConnectMenu;
    QActionGroup *m_ConnectionsGroup;

    QPushButton *pLogButton;

    SubWindowsModel *pWindowsModel;
};

#endif // MAINWINDOW_H
