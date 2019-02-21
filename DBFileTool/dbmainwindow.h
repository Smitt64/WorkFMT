#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dbfileobject.h"
#include "loghighlighter.h"
#include <QMainWindow>
#include <fmterrors.h>
#include <errordlg.h>
#include <connectioninfo.h>
#include <QStringListModel>
#include <QPlainTextEdit>
#include <QTranslator>
#include <QActionGroup>
#include <QCompleter>

namespace Ui {
class DbMainWindow;
}

class OracleTnsListModel;
class DbMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DbMainWindow(QWidget *parent = 0);
    ~DbMainWindow();

private slots:
    void addTable();
    void exportDir();
    void importTables();
    void unload();
    void clear();
    void remove();
    void listContextMenu(const QPoint &pos);
    void addTablesGroup();
    void openConnection();

    void saveList();
    void loadList();

private:
    void loadTranslators();
    QAction *CreateConnectionAction(const QString &ShemeName, ConnectionInfo *info);
    Ui::DbMainWindow *ui;
    QPlainTextEdit *pTextLog;
    LogHighlighter *logColor;
    QTranslator qt_translator;

    DBFileObject *pObj;
    QStringListModel dbtModel;
    OracleTnsListModel *pTnsModel;

    QMenu *pAddMenu;
    QAction *pAddTablesGroup;
    QToolBar *pConnectionsToolBar;
    QList<QAction*> pConActionList;
    QActionGroup *m_ConnectionsGroup;
    QList<ConnectionInfo*> m_pConnections;

    QCompleter *pCompleter;
};

#endif // MAINWINDOW_H
