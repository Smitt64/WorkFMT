#ifndef SELSERVERDIRDLG_H
#define SELSERVERDIRDLG_H

#include <QDialog>

namespace Ui {
class SelServerDirDlg;
}

class OdbcTableModel;
class OdbcTableModelDelegate;
class SelServerDirDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SelServerDirDlg(QWidget *parent = nullptr);
    ~SelServerDirDlg();

    QString path() const;
    QString service() const;

private slots:
    void on_toolButton_clicked();

    void on_comboBox_2_currentTextChanged(const QString &arg1);

private:
    Ui::SelServerDirDlg *ui;

    OdbcTableModel *m_pDsnModel;
    OdbcTableModelDelegate *m_DataSourceDelegate;
};

#endif // SELSERVERDIRDLG_H
