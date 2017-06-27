#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fmterrors.h>
#include <errordlg.h>
#include <QStringListModel>
#include <QPlainTextEdit>
#include "dbfileobject.h"
#include "loghighlighter.h"

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
    void unload();

private:
    Ui::DbMainWindow *ui;
    QPlainTextEdit *pTextLog;
    LogHighlighter *logColor;

    DBFileObject *pObj;
    QStringListModel dbtModel;
    OracleTnsListModel *pTnsModel;
};

#endif // MAINWINDOW_H
