#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtScript>
#include <QtScriptTools>
#include <codeeditor.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void printHandle(const QString &str);

private:
    Ui::MainWindow *ui;
    QScriptEngine *pEngine;
    QScriptEngineDebugger dbg;
    CodeEditor *pCodeEditor;
};

#endif // MAINWINDOW_H
