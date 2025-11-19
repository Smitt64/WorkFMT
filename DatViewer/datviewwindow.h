#ifndef DATVIEWWINDOW_H
#define DATVIEWWINDOW_H

#include <QMdiSubWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>

class DatViewWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit DatViewWindow(const QString &filePath, QWidget *parent = nullptr);
    ~DatViewWindow();

private slots:
    void refreshData();
    void onTableChanged(int index);

private:
    void setupUI();
    void loadDatToDatabase();
    void setupDatabase();
    QString findFmtFile(const QString &datFilePath);

    QString m_filePath;
    QSqlDatabase m_database;
    QSqlTableModel *m_model;
    QTableView *m_tableView;
    QComboBox *m_tableComboBox;
    QPushButton *m_refreshButton;
};

#endif // DATVIEWWINDOW_H
