#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <codeeditor.h>
#include <QStringListModel>

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
    CodeEditor *pCodeEditor;

    QStringListModel m_Errors;
};

#endif // MAINWINDOW_H
