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
class FMTLIBSHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT
    typedef QList<QWidget*> WorkWindowList;
public:
    explicit MainWindow(QWidget *parent = 0);
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
    void ExportXmlAction();
    void CreateTableSql();
    void HotFixCreate();
    void EditGroups();
    void OpenRecentConnection();
    void CopyTable();
    void RsExpExportDir();
    void UnloadDbf();

protected:
    virtual void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

private:
    void CreateMainToolBar();
    void CreateWindowFunctional();
    void UpdateActions();
    void CreateWindowsCombo();
    void CreateViewMenu();
    QAction *CreateConnectionActio(const QString &ShemeName, ConnectionInfo *info);
    void tablesContextMenu(QContextMenuEvent *event, QListView *view);
    QMdiSubWindow *CreateDocument(QSharedPointer<FmtTable> &table);
    Ui::MainWindow *ui;
    TablesDock *pTablesDock;
    QMdiArea *pMdi;

    //QUndoView *pUndoView;

    TreeComboBox *pWindowsComboBox;

    QMap<ConnectionInfo*, WorkWindowList> m_Windows;

    QAction *actionEdit, *actionExport;
    QToolButton *toolConnect;
    QMenu *toolConnectMenu;
    QActionGroup *m_ConnectionsGroup;

    SubWindowsModel *pWindowsModel;
};

#endif // MAINWINDOW_H
