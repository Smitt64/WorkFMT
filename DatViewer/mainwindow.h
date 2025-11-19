#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QSettings>
#include <QListWidget>
#include <QLineEdit>
#include <QDir>
#include <QFileSystemWatcher>
#include "SARibbon.h"

class QMdiArea;
class QMdiSubWindow;

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openRepository();
    void createNewDocument();
    void onFileItemClicked(QListWidgetItem *item);
    void onFileItemDoubleClicked(QListWidgetItem *item);
    void onFilterTextChanged(const QString &text);
    void onDirectoryChanged(const QString &path);

private:
    void setupRibbon();
    void setupMdiArea();
    void setupDockWidget();
    void readSettings();
    void writeSettings();
    void refreshFileList();
    void openDatFile(const QString &filePath);

    QMdiArea *m_mdiArea;
    QString m_lastRepositoryPath;
    QSettings *m_settings;

    // DockWidget элементы
    QDockWidget *m_dockWidget;
    QListWidget *m_fileListWidget;
    QLineEdit *m_filterEdit;
    QFileSystemWatcher *m_fileWatcher;
    QString m_currentDataPath;
};

#endif // MAINWINDOW_H
